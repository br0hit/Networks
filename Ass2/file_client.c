
/*    THE CLIENT PROCESS */

/*
	Name: Bannuru Rohit Kumar Reddy
	Roll No: 21CS30011
	Assignment 2 Client Code
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

#define MAX_BUFFER_SIZE 100


int main()
{
	int	sockfd ;
	struct sockaddr_in	serv_addr;
	int file_descriptor;
	int new_file_descriptor;
	int rec_val;

	int i;
	char buf[MAX_BUFFER_SIZE];
	char rec_buf[MAX_BUFFER_SIZE];
                                         

	// Reading the filename from the user
	char input_file[100];
    char encrypted_file[100];  // 5 is the length of ".enc" + null terminator

	while(1)
	{
		while (1) 
		{
				if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				perror("Unable to create socket\n");
				exit(0);
			}

			// Reading the filename from the user
			printf("\nEnter the file name: ");
			scanf("%s", input_file);

			// Checking if the file exists or not
			file_descriptor = open(input_file, O_RDONLY);

			if (file_descriptor == -1) {
				perror("Error opening file");
				printf("File doesn't exist. Include .txt in the file name\n");
				// Continue to the next iteration of the loop to ask for the filename again
				continue;
			} else 
			{
				close(file_descriptor);
				// Break out of the loop since a valid file is obtained
				break;
			}
		}

		int key_num;
		char key[10];
		printf("Enter the key: ");
		scanf("%d",&key_num);
		sprintf(key,"%d", key_num);

		serv_addr.sin_family	= AF_INET;
		inet_aton("127.0.0.1", &serv_addr.sin_addr);
		serv_addr.sin_port	= htons(8181);

		if ((connect(sockfd, (struct sockaddr *) &serv_addr,
							sizeof(serv_addr))) < 0) {
			perror("Unable to connect to server\n");
			exit(0);
		}

		// Start connecting with server after the key has been given
		
		send(sockfd, key, strlen(key) + 1, 0);
		for(i=0; i < 100; i++) buf[i] = '\0';
		recv(sockfd, buf, 100, 0);
		printf("\n%s\n", buf);


		// Open the file for reading
		file_descriptor = open(input_file, O_RDONLY);
		if (file_descriptor == -1) {
			perror("Error opening file");
			exit(EXIT_FAILURE);
		}

		// Read and send the file in chunks
		char buffer[MAX_BUFFER_SIZE];
		ssize_t bytesRead;

		while ((bytesRead = read(file_descriptor, buffer, sizeof(buffer))) > 0) {
			// Send the chunk to the server
			send(sockfd, buffer, bytesRead, 0);
		}

		// Temporarily disconnect the sending side
		if (shutdown(sockfd, SHUT_WR) == -1) {
			perror("Error shutting down the sending side");
			// Handle error as needed
		}

		// Close the file
		close(file_descriptor);
		printf("Compelte file sent\n");

		for(i=0; i < 100; i++) rec_buf[i] = '\0';
		recv(sockfd, rec_buf, 100, 0);
		printf("\n%s", rec_buf);

		sprintf(encrypted_file, "%s.enc", input_file);

		// Create a new file (or open it if it already exists) with read and write permissions
		new_file_descriptor = open(encrypted_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

		while(1)
		{
			for(i=0; i < 100; i++) buf[i] = '\0';
			rec_val= recv(sockfd, buf, 100, 0);

			if(rec_val>0)
			{
				// Write the received data to the file
				write(new_file_descriptor, buf, rec_val);
			}

			if(rec_val<=0)
			{
				printf("\nEncrypted File received Sucessfully\n\n");
				printf("Original File name: %s\n",input_file);
				printf("Encrypted File name: %s\n",encrypted_file);
				printf("Process Done, Client waiting for new file name, Press ctrl^c to end the process\n");
				break;
			}
		}

		close(sockfd);

	}

	return 0;

}
