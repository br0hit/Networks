/*
	Name: Pasupulety Chethan Krishna Venkat
	Roll No: 21CS30036
	Assignment 2 Server Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAX 100

int main()
{
	int sockfd, newsockfd; /* Socket descriptors */
	int clilen;
	struct sockaddr_in cli_addr, serv_addr;

	int i;
	char buf[MAX];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Cannot create socket\n");
		exit(0);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(9000);

	if (bind(sockfd, (struct sockaddr *)&serv_addr,
			 sizeof(serv_addr)) < 0)
	{
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5);
	printf("Server is running\n");

	while (1)
	{

		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
						   &clilen);

		if (newsockfd < 0)
		{
			printf("Accept error\n");
			exit(0);
		}

		if (fork() == 0)
		{

			close(sockfd);

			// receive until "\r\n"
			char message[MAX];
			for (int i = 0; i < MAX; i++)
			{
				message[i] = '\0';
			}

			while (1)
			{
				ssize_t bytes = recv(newsockfd, buf, MAX, 0);
				if (bytes < 0)
				{
					printf("Error in receiving\n");
					exit(0);
				}

				if (strstr(buf, "\r\n"))
				{
					// sz=write(fd,buf,bytes-5);
					buf[bytes - 2] = '\0';
					strcat(message, buf);
					break;
				}
				else
				{
					strcat(message, buf);
				}
			}

			// Checkin if the message received is "client connects to SMTP port" by string matching 

			char *first_msg = "<client connects to SMTP port>";

			if(strcmp(first_msg,buf)==0) printf("220\n");

			printf("Message received from client: %s\n", message);

			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
	}
	return 0;
}