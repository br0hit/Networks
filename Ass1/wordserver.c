// A Simple UDP Server that sends a HELLO message

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define MAXLINE 1024 
#define MAX_FILESIZE 100
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    int n; 
    int m;
    socklen_t len;

    char buffer[MAXLINE]; 
    char* filename;
    int i;
    int word_count=0;
      
    // Create socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(8181); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    printf("\nServer Running....\n\n");

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0';

    filename = buffer;

    // Try to open the file for reading
    FILE *file = fopen(filename, "r");

    if(file==NULL)
    {
           // File not found, print an error message
            fprintf(stderr, "NOTFOUND %s\n", filename);

            // Define the error message string
            char errorMessage[50]; // Adjust the size accordingly

            // Format the error message with the filename from the buffer
            sprintf(errorMessage, "NOTFOUND %s", buffer);

            m = sendto(sockfd, (char *)errorMessage, strlen(errorMessage), 0, ( struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
    }
    else
    {
        // Try to open the file for reading
        FILE *file = fopen(filename, "r");

        // Check if the file exists
        if (file != NULL) 
        {
            // File found, close it and print a message
            printf("File found: %s\n", filename);

            i =0;
            char temp_word[100];

            while(1)
            {
                fscanf(file, "%s", temp_word);

                if(word_count==0)
                {
                    m = sendto(sockfd, (char *)temp_word, strlen(temp_word), 0, ( struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
                    word_count++;
                    continue;
                }

                len = sizeof(cliaddr);
                n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
                buffer[n] = '\0';

                printf("%s Being sent\n",buffer);

                m = sendto(sockfd, (char *)temp_word, strlen(temp_word), 0, ( struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
                word_count++;
                if(strcmp(temp_word,"END")==0) break;
            }

            fclose(file);

            char *test = "END";
            m = sendto(sockfd, (char *)test, strlen(test), 0, ( struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
            printf("%s Encountered, End of word by word file transfer",test);

        }     

    }
    
    printf("\n\nClosing connection..");
    
    close(sockfd);
    return 0; 
} 
