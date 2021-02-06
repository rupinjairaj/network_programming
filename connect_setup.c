#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status, sfd;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: connect_setup hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // specifying the port on which we wish to connect to
    char *port = "50002";
    if ((status = getaddrinfo(argv[1], port, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // create a socket to the server based on the addrinfo
    // obtained from the response of getaddrinfo.
    if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        fprintf(stderr, "socket: failed to create a socket\n");
        return 3;
    }

    // prevent "address already in use" error message
    int yes = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
        fprintf(stderr, "setsockopt: failed to reuse addr for socket\n");
        return 3;
    }

    if ((connect(sfd, res->ai_addr, res->ai_addrlen)) == -1)
    {
        fprintf(stderr, "connect: failed to connect to %s on port %s\n", argv[1], port);
        return 4;
    }

    freeaddrinfo(res);

    return 0;
}