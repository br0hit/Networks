// Writing the functions for the actual socket communication

#include "msocket.h"

// Function to create a socket

/*
m_socket – This function opens an UDP socket with the socket call. The parameters
to these are the same as the normal socket() call, except that it will take only
SOCK_MTP as the socket type. m_socket() checks whether a free entry is available
in the SM, creates the corresponding UDP socket if a free entry is available, and
initializes SM with corresponding entries. If no free entry is available, it returns -1 with
the global error variable set to ENOBUFS. If any error is received while creating the
UDP socket, errno is set to the corresponding error.
*/

int m_socket(int domain, int type, int protocol) {
    int i;
    int sockfd;
    struct sockaddr_in servaddr;
    int len = sizeof(servaddr);

    // Check if the socket type is SOCK_MTP
    if (type != SOCK_MTP) {
        errno = EPROTOTYPE;
        return -1;
    }

    // Check if there is a free entry aviailable in the SM
    for (i = 0; i < MAX_SOCKETS; i++) {
        if (socket_map[i].status == 0) {
            break;
        }
    }

    // If no free entry is available, return -1 with the global error variable set to ENOBUFS
    if (i == MAX_SOCKETS) {
        errno = ENOBUFS;
        return -1;
    }

    // Create the UDP socket
    sockfd = socket(domain, SOCK_DGRAM, protocol);
    if (sockfd < 0) {
        errno = ECONNREFUSED;
        return -1;
    }

    // Initialize the SM with corresponding entries
    socket_map[i].status = 1;
    socket_map[i].sockfd = sockfd;
    socket_map[i].addr = servaddr;
    socket_map[i].len = len;

}

// Function to bind a socket

/*
m_bind – binds the socket with some address-port using the bind call. Bind is
necessary for each MTP socket irrespective of whether it is used as a server or a client.
This function takes the source IP, the source port, the destination IP and the
destination port. It binds the UDP socket with the source IP and source port, and
updates the corresponding SM with the destination IP and destination port. If any error
is received while binding the UDP socket, errno is set to the corresponding error, and
SM is not updated. The idea here is that a single MTP socket is used to communicate
with another pre-specified MTP socket only (different from how an UDPP socket
behaves, even though MTP uses UDP underneath).
*/

