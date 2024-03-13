
/*
	Name: Bannuru Rohit Kumar Reddy
	Roll No: 21CS30011
	Assignment 2 Server Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

void encryptFile(const char *inputFileName, const char *outputFileName, int key) 
{
    int inputFile, outputFile;
    char buffer[4096];
    ssize_t bytesRead, bytesWritten;

    // Open the input file in read-only mode
    inputFile = open(inputFileName, O_RDONLY);
    if (inputFile == -1) {
        perror("Error opening input file");
        return;
    }

    // Open or create the output file in write-only mode with permissions 0644
    outputFile = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outputFile == -1) {
        perror("Error opening or creating output file");
        close(inputFile);
        return;
    }

    // Read from the input file, encrypt, and write to the output file
    while ((bytesRead = read(inputFile, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            // Encrypt only alphabetical characters
            if ('A' <= buffer[i] && buffer[i] <= 'Z') {
                buffer[i] = 'A' + (buffer[i] - 'A' + key) % 26;
            } else if ('a' <= buffer[i] && buffer[i] <= 'z') {
                buffer[i] = 'a' + (buffer[i] - 'a' + key) % 26;
            }
        }

        // Write the encrypted data to the output file
        bytesWritten = write(outputFile, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Error writing to output file");
            close(inputFile);
            close(outputFile);
            return;
        }
    }

    // Close the files
    close(inputFile);
    close(outputFile);
}


/* THE SERVER PROCESS */

int main()
{
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;
            
	int i;
	char buf[100];		/* We will use this buffer for communication */
	char msg_buf[100];
	char buffer[100];
	int temp=0;
	char key_num;
	int key;
	int rec_val;
	int new_file_descriptor;
	int file_descriptor;
	ssize_t bytesRead;


	// Create the filename based on the client's IP address and port
	char filename[50];  // Adjust the size accordingly
	char outputFileName[50];  // Adjust the size accordingly

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket\n");
		exit(0);
	}

	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(8181);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); 

	while (1) 
	{

		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen) ;

		if (newsockfd < 0) {
			printf("Accept error\n");
			exit(0);
		}

		if (fork() == 0) 
		{
			close(sockfd);	

			/* We again initialize the buffer, and receive a 
			message from the client. 
			*/
			for(i=0; i < 100; i++) buf[i] = '\0';
			rec_val= recv(newsockfd, buf, 2, 0);

			if(rec_val>=2)
			{	
				strcpy(msg_buf,"Server received key, Creating a temporary file on the server");
				send(newsockfd, msg_buf, strlen(msg_buf) + 1, 0);

				// receiving the key
				temp++;

				char client_ip[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
				int client_port = ntohs(cli_addr.sin_port);

				snprintf(filename, sizeof(filename), "%s.%d.txt", client_ip, client_port);
				snprintf(outputFileName, sizeof(outputFileName), "%s.%d.txt.enc", client_ip, client_port);

				// Create a new file (or open it if it already exists) with read and write permissions
				new_file_descriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

				if (new_file_descriptor == -1) {
					perror("Error creating/opening file");
					// Handle the case where the file cannot be created or opened
				} else {
					printf("\nFile %s created successfully.\n", filename);
				}

				key_num = buf[0];
				key = (int)key_num-'0';
			}

			while(1)
			{
				for(i=0; i < 100; i++) buf[i] = '\0';
				rec_val= recv(newsockfd, buf, 100, 0);

				// printf("%d\n", rec_val);
				if(rec_val>0)
				{
					// Write the received data to the file
					write(new_file_descriptor, buf, rec_val);
				}
				else 
				{
					printf("Complete File received\n");
					// Close the file when done
					close(new_file_descriptor);

					printf("\nEncryptying file\n");
					encryptFile(filename, outputFileName, key);

					printf("Sending encrypted file to client\n");
					strcpy(msg_buf,"Receiving encrypted message from the server");
					send(newsockfd, msg_buf, strlen(msg_buf) + 1, 0);

					file_descriptor = open(outputFileName, O_RDONLY);
					if (file_descriptor == -1) {
						perror("Error opening file");
						exit(EXIT_FAILURE);
					}

					while ((bytesRead = read(file_descriptor, buffer, sizeof(buffer))) > 0) {
					// Send the chunk to the server
					send(newsockfd, buffer, bytesRead, 0);
					}		
					printf("Encrypted file sent to client\n");
					printf(".... Closing connection\n");

					close(newsockfd);


					exit(0);
					break;
				}
			}		
		}
		close(newsockfd);		
	}
	return 0;
}
