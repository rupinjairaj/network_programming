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
    int new_fd, listener_fd, status, msg_len, bytes_sent;
    char *msg = "Server has accepted your connection";

    if (argc != 2)
    {
        fprintf(stderr, "usage: bind_setup hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    if ((status = getaddrinfo(argv[1], PORT, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // create a socket to the server based on the addrinfo
    // obtained from the response of getaddrinfo.
    if ((listener_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "socket: failed to create a socket\n");
        return 3;
    }

    if (bind(listener_fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        fprintf(stderr, "bind: failed to bind\n");
        return 4;
    }

    if (listen(listener_fd, BACKLOG) == -1)
    {
        fprintf(stderr, "listen: failed to listen\n");
        return 5;
    }

    // accept incoming connections
    addr_size = sizeof client_addr;
    if ((new_fd = accept(listener_fd, (struct sockaddr *)&client_addr, &addr_size)) == -1)
    {
        fprintf(stderr, "accept: failed to accept new connection\n");
        return 6;
    }

    printf("New client connection socket fd: %d\n", new_fd);
    msg_len = strlen(msg);
    bytes_sent = send(new_fd, msg, msg_len, 0);
    if (bytes_sent == -1 || bytes_sent != msg_len)
    {
        fprintf(stderr, "send: failed to send reply to new connection\n");
        return 7;
    }
    freeaddrinfo(res);

    return 0;
}