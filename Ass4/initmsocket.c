#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/select.h> // Include the select header file
#include "msocket.h"

// Define the maximum number of active MTP sockets
#define MAX_MTP_SOCKETS 25

// Function prototypes
void *receiver_thread(void *arg);
void *sender_thread(void *arg);
void garbage_collector();

int main()
{
    pthread_t receiver_thread_id, sender_thread_id;
    pid_t garbage_collector_pid;

    // Start the receiver thread
    if (pthread_create(&receiver_thread_id, NULL, receiver_thread, NULL) != 0)
    {
        perror("Error creating receiver thread");
        exit(EXIT_FAILURE);
    }

    // Start the sender thread
    if (pthread_create(&sender_thread_id, NULL, sender_thread, NULL) != 0)
    {
        perror("Error creating sender thread");
        exit(EXIT_FAILURE);
    }

    // Start the garbage collector process
    garbage_collector_pid = fork();
    if (garbage_collector_pid == -1)
    {
        perror("Error creating garbage collector process");
        exit(EXIT_FAILURE);
    }
    else if (garbage_collector_pid == 0)
    {
        // Child process (garbage collector)
        garbage_collector();
    }
    else
    {
        // Parent process
        // Wait for the garbage collector process to finish
        waitpid(garbage_collector_pid, NULL, 0);
    }

    // Cleanup and exit
    pthread_join(receiver_thread_id, NULL);
    pthread_join(sender_thread_id, NULL);

    return 0;
}

void *receiver_thread(void *arg)
{
    // Implementation of receiver thread (R)
    printf("Receiver thread started.\n");

    // It waits for a message to come in a recvfrom() call from any of the UDP sockets
    // (you need to use select() to keep on checking whether there is any incoming message
    // on any of the UDP sockets, on timeout check whether a new MTP socket has been created
    // and include it in the read/write set accordingly)

    // Declare the file descriptor set for select()
    fd_set readfds;
    int max_fd; // Maximum file descriptor value

    while (1)
    {
        // Clear the file descriptor set
        FD_ZERO(&readfds);

        // Add the UDP sockets to the file descriptor set
        // (Assuming you have an array of UDP socket file descriptors called 'udp_sockets')
        for (int i = 0; i < MAX_MTP_SOCKETS; i++)
        {
            if (udp_sockets[i] != -1)
            {
                FD_SET(udp_sockets[i], &readfds);
                if (udp_sockets[i] > max_fd)
                {
                    max_fd = udp_sockets[i];
                }
            }
        }

        // Set the timeout value for select()
        struct timeval timeout;
        timeout.tv_sec = 5; // Example: 5 seconds
        timeout.tv_usec = 0;

        // Call select() to check for incoming messages or timeout
        int activity = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        if (activity == -1)
        {
            perror("Error in select");
            exit(EXIT_FAILURE);
        }
        else if (activity == 0)
        {
            // Timeout occurred, check for new MTP socket creation
            // and include it in the read/write set accordingly
            // Your implementation here
        }
        else
        {

            // Check for incoming messages on the UDP sockets
            for (int i = 0; i < MAX_MTP_SOCKETS; i++)
            {
                if (FD_ISSET(udp_sockets[i], &readfds))
                {
                    // Receive the message
                    char buffer[MAX_BUFFER_SIZE];
                    ssize_t num_bytes = recvfrom(udp_sockets[i], buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
                    if (num_bytes == -1)
                    {
                        perror("Error in recvfrom");
                        exit(EXIT_FAILURE);
                    }

                    // Process the received message
                    if (is_data_message(buffer))
                    {
                        // Store the data message in the receiver-side message buffer
                        store_data_message(buffer, num_bytes, mtp_socket[i]);

                        // Send an ACK message to the sender
                        send_ack_message(udp_sockets[i], buffer, num_bytes);
                    }
                    else if (is_ack_message(buffer))
                    {
                        // Process the ACK message
                        process_ack_message(buffer, num_bytes);
                    }
                }
            }
        }

        void *sender_thread(void *arg)
        {
            // Implementation of sender thread (S)
            printf("Sender thread started.\n");

            // Your implementation here

            return NULL;
        }

        void garbage_collector()
        {
            // Implementation of garbage collector process (G)
            printf("Garbage collector process started.\n");

            // Your implementation here

            while (1)
            {
                // Implement garbage collection logic
                // Check for unused MTP socket entries and clean them up
                // Sleep for a certain period before checking again
                sleep(60); // Example: check every 60 seconds
            }
        }
