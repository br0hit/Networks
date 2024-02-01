#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_LENGTH 1000
#define MAX 100


// Function to send commands to SMTP server and receive responses
void send_command(int sockfd, char *command, char *response)
{
    send(sockfd, command, strlen(command), 0);
    recv(sockfd, response, BUFFER_SIZE, 0);
}

int main(int argc, char *argv[])
{
    // if (argc != 4)
    // {
    //     printf("Usage: %s <server_IP> <smtp_port> <pop3_port>\n", argv[0]);
    //     exit(EXIT_FAILURE);
    // }

    char command[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Read username and password
    char username[100];
    char password[100];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Implement options
    int option;
    do
    {
        printf("1. Manage Mail\n");
        printf("2. Send Mail\n");
        printf("3. Quit\n");
        printf("Enter option: ");
        scanf("%d\n", &option);

        switch (option)
        {
        case 1:
            // Manage Mail
            // Implement this as per your requirements
            break;
        case 2:

            char from[MAX_LENGTH], to[MAX_LENGTH], subject[MAX_LENGTH], input[MAX_LENGTH], message[MAX_LENGTH * 50];
            int from_flag = 0, to_flag = 0, subject_flag = 0;

            // initilazing message to NULL so the previous content of the message is not concatenated with the new message
            message[0] = '\0';

            int count = 0;
            while (1)
            {
                fgets(input, sizeof(input), stdin);

                if (strcmp(input, ".\n") == 0)
                    break;

                if (count == 0)
                {
                    strcpy(from, input);
                    printf("From message received: %s", from);

                    // Cheching whether the from message is of the correct format or not :
                    if (strstr(from, "From: ") != from || strchr(from, '@') == NULL)
                    {
                        from_flag = 1;
                    }

                    // Find the position of '@' symbol
                    char *at_position = strchr(from, '@');

                    if (at_position == NULL)
                    {
                        printf("Invalid format: '@' symbol not found.\n");
                        break;
                    }

                    // Extract username
                    char *username_start = strstr(from, ": ") + 2; // Skip "From: "
                    char *username_end = at_position;
                    int username_length = username_end - username_start;
                    char username[username_length + 1];
                    strncpy(username, username_start, username_length);
                    username[username_length] = '\0';

                    // Extract domain name
                    char *domain_start = at_position + 1;
                    char *domain_end = strchr(domain_start, '\n');
                    int domain_length = domain_end - domain_start;
                    char domain[domain_length + 1];
                    strncpy(domain, domain_start, domain_length);
                    domain[domain_length] = '\0';

                    printf("Username: %s\n", username);
                    printf("Domain (as string): %s\n", domain);

                    count++;
                }
                else if (count == 1)
                {
                    strcpy(to, input);
                    printf("To message received: %s", to);

                    // Cheching whether the to message is of the correct format or not :
                    if (strstr(to, "To: ") != to || strchr(to, '@') == NULL)
                    {
                        to_flag = 1;
                    }

                    count++;
                }
                else if (count == 2)
                {
                    strcpy(subject, input);
                    printf("Subject message received: %s", subject);

                    // Cheching whether the subject message is of the correct format or not :
                    if (strstr(subject, "Subject: ") != subject)
                    {
                        subject_flag = 1;
                    }
                    count++;
                }
                else
                {
                    strcat(message, input);
                }
            }

            // Checking whether the from and to messages are of the correct format or not :
            if (from_flag == 1 || to_flag == 1 || subject_flag == 1)
            {
                printf("Incorrect format\n");
                break;
            }

            printf("%s%s%s", from, to, message);

            int sockfd;
            struct sockaddr_in serv_addr;
            int fd;

            int i;
            char buf[MAX]; /* Opening a socket is exactly similar to the server process */

            serv_addr.sin_family = AF_INET;
            inet_aton("127.0.0.1", &serv_addr.sin_addr);
            serv_addr.sin_port = htons(9000);

            char inputfilename[MAX];
            int k;

            // while(1){

            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror("Unable to create socket\n");
                exit(0);
            }

            // connecting to the server
            if ((connect(sockfd, (struct sockaddr *)&serv_addr,
                         sizeof(serv_addr))) < 0)
            {
                perror("Unable to connect to server\n");
                exit(0);
            }

            printf("Connected to server\n");

            char msg[MAX] = "<client connects to SMTP port>";
            send(sockfd, msg, strlen(msg), 0);
            send(sockfd, "\r\n", 2, 0);
            printf("%s sent to server\n", msg);

        case 3:
            printf("Quitting program\n");
            break;
        default:
            printf("Invalid option\n");
        }
    } while (option != 3);

    // Close socket
    // close(sockfd);

    return 0;
}
