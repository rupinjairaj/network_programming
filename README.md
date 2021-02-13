# How to read this project.

### Order of code files
1. showip.c
    - Learn about how *getaddrinfo* can be used to fetch details about an address (IP or domain).
    - It can be fed with details (hints) to nudge the kind of response you need. (eg: In case you need either IPv4 or IPv6 address info).

2. socket_setup.c
    - Builds on showipc.
    - Uses the res from *getaddrinfo* to create a socket to the IP/domain supplied to *getaddrinfo*.

3. bind_setup.c
    - Builds on socket_setup.c
    - Use *setsockopt* to handle "address already in use errors".
    - Bind the socket to the address received as a response from *getaddrinfo*.

4. connect_setup.c
    - Builds on bind_setup.c
    - We don't call *bind* here because we don't need to know about the local port that we are communicating through. The server can get the random port the kernel assigns to this connection.
    - This is what a typical client code will do. Client normally don't need to *bind* to a specific port because they don't normally need to handle any incoming connections and hence don't need to be listening on some well known port. 

5. listen_accept.c
    - Shows how to *listen* for incoming connections and *accept* them.
    - *listen* also takes as an arg the number of connections that can be held in the backlog queue.
    - *getaddrinfo* -> socket -> *bind* -> *listen* -> accept
    - *bind* is necessary here because we want to fix the port through which we will handle incoming connections.
    - A client *connect*s to a specified port that the server is listening on(the port the server socket is bound to with *bind*). The server socket will be listening on this port. The incoming client connection will be queued to be accepted. When we call *accept* to fetch this pending connection. *accept* returns a brand new socket file descriptor to use for this connection. 
    - *accept* also populates a value-result parameter with the client address info.

6. send_setup.c & recv_setup.c
    - Builds on top of listen_accept.c
    - Shows the use of *send* and *recv* between an extremely simple client/server program.

7. DGRAM *sendto* and *recvfrom* [No code files for this]
    - *sendto* and *recvfrom* work the same way as *send* and *recv* but for DGRAM sockets (UDP). 
    - The only pieces of additional info are the destination, from addr and their lengths that we specify or receive as a result of value-result params.
    - If we use *connect* on DGRAM sockets then we can simply use *read* and *send*. They still work on UDP but the address is filled by the socket interface automatically.  

8. *close* and *shutdown* [No code files for this]
    - Simple file descriptor *close* works on closing sockets. Any attempt to read and write to a closed socket will lead to an error.
    - *shutdown* allows you to cut off communication in a certain direction or both directions like *close*.
    - *shutdown* doesn't close a socket fd. It only changes it usability. Use *close* to free the socket file descriptor.

9. *getpeername* [No code files for this]
    - Populates the value-result param addr with the address of who is connected on the other end of the socket.

10. *gethostname* [No code files for this]
    - Returns the hostname of the system it is called on.
    - Can be used with *gethostbyname* to fetch the system's IP address.

11. client.c & server.c
    - Simple client-server example code to demonstrate socket communication using the concepts mentioned above.
    - Work on TCP (STREAM).

12. poll_stdin.c
    - A simple example to show how to use the *poll* syscall, its parameters and how to handle it's result to check for which event occurred in which file descriptor.

13. poll_sv.c
    - A server that uses *poll* to display non-blocking IO for a simple broadcast chat application.
    - Scales(memory wise, not performance wise) for large number of clients by *realloc*ating the *poll* array of structs that house the file descriptors.
    - Obvious issue is that when a new client joins the fd_count is updated (increased) forcing the loop to run one more time to check for events.  

14. select_stdin.c
   - A simple example to show how to use the *select* syscall, its parameters and how to handle it's result to check for which event occurred.

15. select_sv.c
    - A server that uses *select* to demonstrate non-blocking IO for a simple broadcast chat application.

### Further reading and references
- man pages for each function highlighted.
- [Beej's network programming](http://beej.us/guide/bgnet/html/)
- [The Linux Programming Interface](https://man7.org/tlpi/)