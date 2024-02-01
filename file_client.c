/*
	Name: Pasupulety Chethan Krishna Venkat
	Roll No: 21CS30036
	Assignment 2 Client Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 100

int main()
{
	int	sockfd ;
	struct sockaddr_in	serv_addr;
	int fd;

	int i;
	char buf[MAX];	/* Opening a socket is exactly similar to the server process */

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(8181);

	char inputfilename[MAX];
	int k;

	// while(1){

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}


	// connecting to the server
		if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}

	printf("Connected to server\n");

	// Taking filename, k as inputs from the user
	while(1){
	printf("Enter the name of the file to be transferred: ");
	scanf("%s", inputfilename);

	 fd=open(inputfilename, O_RDONLY);
	if(fd==-1){
		printf("Error! File not found\n");
		continue;
	}
	else{
		break;
	}
	
}
	printf("Enter k:");
	scanf("%d", &k);

	// // sending k first (assuming k<10)
	// char num[2];
	// sprintf(num, "%d", k);

	char msg[MAX]="<client connects to SMTP port>";
	send(sockfd, msg, strlen(msg), 0);
    send(sockfd, "\r\n", 2, 0);
	printf("%s sent to server\n",msg);
	



	// // sending the value of k to the server
	// send(sockfd, num, strlen(num)+1, 0);
	// printf("k %s sent to server\n",num);



	// for(int i=0;i<MAX;i++)
	// 	buf[i]='\0';
	// int byte_read=0;

	// // sending contents of the file to the server
	//     do{
    //     char read_buffer[MAX];
    //     byte_read = read(fd, read_buffer,MAX-1);
    //     read_buffer[byte_read]='\0';
	// 	if(send(sockfd,read_buffer,strlen(read_buffer)+1,0)<0){	
	// 		perror("Error in sending file\n");
	// 		exit(0);
	// 	}
	// 	else {
	// 		// printf("%s sent to server\n",read_buffer);
	// 	}
    // }while(byte_read==MAX-1);

	// printf("Original File sent to server\n");
	// close(fd);

	// // Temporarily disconnect the sending side
    //     if (shutdown(sockfd, SHUT_WR) == -1) {
    //         perror("Error shutting down the sending side");
    //         // Handle error as needed
    //     }

	// // receiving the encrypted file
	// // new file name
	// char newfilename[MAX];
	// strcpy(newfilename, inputfilename);
	// strcat(newfilename, ".enc");


	// // creating the new encrypted file
	// int fd2=open(newfilename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// 		if(fd==-1){
	// 			printf("Error! File not created\n");
	// 			exit(0);
	// 		}
	

	// byte_read = 0;

	// while ((byte_read = recv(sockfd, buf,MAX, 0)) > 0)
	// {
	// 	// printf("%s received from server\n", buf);
	// 	// writing data to the file
	// 	write(fd2, buf, strlen(buf));

	// 	for (int i = 0; i < MAX; i++)
	// 		buf[i] = '\0';
	// }

	// printf("\nEncrypted File received Sucessfully\n\n");
	// printf("Original File name: %s\n", inputfilename);
	// printf("Encrypted File name: %s\n", newfilename);
	// printf("Process Done, Client waiting for new file name, Press ctrl^c to end the process\n");

	// close(fd2);
	close(sockfd);

// }
	return 0;

}

