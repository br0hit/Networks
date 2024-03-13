// Purpose: Header file for the new socket that we are going to define


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>

#define SOCK_MTP 3
#define MAX_SOCKETS 10
#define MAX_BUFFER_SIZE 1024
#define MAX_MESSAGE_SIZE 1024
#define MAX_IP_SIZE 16
#define MAX_PORT_SIZE 6

// Structure for the socket entry
typedef struct {
    int socket;
    struct sockaddr_in address;
    int address_len;
    int port;
    char ip[MAX_IP_SIZE];
} msocket_t;

// Structure for the message
typedef struct {
    char message[MAX_MESSAGE_SIZE];
    int message_len;
    int socket;
} msocket_message_t;

// Structure for the socket set
typedef struct {
    int sockets[MAX_SOCKETS];
    int sockets_len;
} msocket_set_t;

// Utility functions for the new socket that we are going to define
// Note: The parameters and return values to these functions are exactly the same as the corresponding functions of the UDP socket, except for


// Function to create a new socket
int m_socket(int domain, int type, int protocol);

// Function to bind the socket to the address, This function takes the source IP, the source port, the destination IP and the destination port
int m_bind(int socket, const char *src_ip, int src_port, const char *dest_ip, int dest_port);

// Function to send the message to the socket
int m_sendto(int socket, const void *message, int message_len, int flags, const struct sockaddr *dest_addr, int dest_len);

// Function to receive the message from the socket
int m_recvfrom(int socket, void *message, int message_len, int flags, struct sockaddr *src_addr, int *src_len);

// Function to close the socket
int m_close(int socket);


