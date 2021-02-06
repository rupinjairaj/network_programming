#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT "50002"
#define BACKLOG 10

int main(int argc, char *argv[])
{
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int client_fd, status, bytes_recv;
    size_t recv_msg_len = 1024;
    void *recv_buf;

    if (argc != 2)
    {
        fprintf(stderr, "usage: bind_setup hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    if ((status = getaddrinfo("127.0.0.1", PORT, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // create a socket to the server based on the addrinfo
    // obtained from the response of getaddrinfo.
    if ((client_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "socket: failed to create a socket\n");
        return 3;
    }

    if ((connect(client_fd, res->ai_addr, res->ai_addrlen)) == -1)
    {
        fprintf(stderr, "connect: failed to connect to %s on port %s\n", argv[1], PORT);
        return 4;
    }

    if ((bytes_recv = recv(client_fd, &recv_buf, recv_msg_len, 0)) == -1)
    {
        fprintf(stderr, "recv: failed to receive response from server\n");
        return 5;
    }

    printf("Received %d bytes from the server.\n", bytes_recv);

    return 0;
}