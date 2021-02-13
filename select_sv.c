#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "50002"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    fd_set master;
    fd_set read_fds;

    int fd_max;

    int listener;
    int new_fd;

    struct sockaddr_storage remote_addr;
    socklen_t addr_len;

    char buf[256];
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes = 1;
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        fprintf(stderr, "select_sv: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (listener < 0)
        {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "select_sv: failed to bind.\n");
        exit(2);
    }

    freeaddrinfo(ai);

    if (listen(listener, BACKLOG) == -1)
    {
        perror("listen");
        exit(3);
    }

    FD_SET(listener, &master);

    fd_max = listener;

    for (;;)
    {
        read_fds = master;
        if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for (i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == listener)
                {
                    addr_len = sizeof remote_addr;
                    new_fd = accept(listener,
                                    (struct sockaddr *)&remote_addr,
                                    &addr_len);
                    if (new_fd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(new_fd, &master);
                        if (new_fd > fd_max)
                        {
                            fd_max = new_fd;
                        }
                        printf("select_sv: new connection from %s on socket %d\n",
                               inet_ntop(remote_addr.ss_family,
                                         get_in_addr((struct sockaddr *)&remote_addr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               new_fd);
                    }
                }
                else
                {
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        if (nbytes == 0)
                        {
                            // connection closed
                            printf("select_sv: socket %d hung up.\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        for (j = 0; j <= fd_max; j++)
                        {
                            if (FD_ISSET(j, &master))
                            {
                                if (j != listener && j != i)
                                {
                                    if (send(j, buf, nbytes, 0) == -1)
                                    {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}