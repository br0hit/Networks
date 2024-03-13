/*
    While the described file transfer protocol may work, it has several drawbacks and is not considered a good practice for various reasons:

    Inefficiency: The protocol's word-by-word exchange may lead to high message overhead and increased latency, especially for large files.
    Error Handling: The protocol lacks detailed error handling, offering only a generic "File Not Found" message without specific information about transfer failures.
    Limited Security: The absence of encryption or authentication mechanisms exposes the file transfer process to security vulnerabilities and unauthorized access.
*/



// A Simple Client Implementation

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAXLINE 1024 
  
int main() 
{ 
    char *file_name = "sample.txt";
    char *newline = "\n";

    int sockfd, err; 
    struct sockaddr_in servaddr; 
    int n;
    socklen_t len; 

    char *hello = "CLIENT:HELLO"; 
    char buffer[MAXLINE]; 
    char send_word[100] = "WORD";
    char temp_number[100];

    int word_count=0;
    int i;

  
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(8181); 
    err = inet_aton("127.0.0.1", &servaddr.sin_addr);
    if (err == 0) {
	   printf("Error in ip-conversion\n");
	   exit(EXIT_FAILURE);
    }

    sendto(sockfd, (const char *)file_name, strlen(file_name), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("File name sent to the server\n"); 
   // getchar(); 


while(1)
    {
        int length = sizeof(servaddr);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &servaddr, &length); 
        buffer[n] = '\0';

        // char *check = "NOTFOUND";
        // char check_val[9];
        // for(i=0;i<8;i++) buffer[i] = check_val[i];
        // check_val[8] = '\0';

        // Check if "NOTFOUND" is present in the buffer
        if (strstr(buffer, "NOTFOUND") != NULL) 
        {
        // Print the desired message
        printf("File %s Not Found\n", buffer + 9); // Assuming "NOTFOUND " is fixed and always 9 characters long
        exit(EXIT_FAILURE);
        }

        if(strcmp(buffer,"END")==0)
        {
            printf("End of reading the file");
            close(sockfd); 
            return 0; 
        } 


        if(word_count==0)
        {
            // Open or create the file "receive.txt" for writing
            FILE *file = fopen("receive.txt", "w");
            fclose(file);

            word_count++;
            sprintf(temp_number, "%d", word_count);
            strcat(send_word, temp_number);

            sendto(sockfd, (const char *)send_word, strlen(send_word), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        }

        else
        {
            FILE *file = fopen("receive.txt", "a");

            // printf("Writing stuff here %s\n", buffer);
            fprintf(file,"%s", buffer);
            fputc('\n', file);
            fclose(file);

            word_count++;
            char send_word[100] = "WORD";
            sprintf(temp_number, "%d", word_count);
            strcat(send_word, temp_number);

            // printf("%s",send_word);

            sendto(sockfd, (const char *)send_word, strlen(send_word), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        }

        // printf("%s\n", buffer); 
    }    

    close(sockfd); 
    return 0; 
}