# How to read this project.

### Order of code files
1. showip.c
    - Learn about how getaddrinfo() can be used to fetch details about an address (IP or domain).
    - It can be fed with details (hints) to nudge the kind of response you need. (eg: In case you need either IPv4 or IPv6 address info).

2. socket_setup.c
    - Builds on showipc.
    - Uses the res from getaddrinfo to create a socket to the IP/domain supplied to getaddrinfo.

3. bind_setup.c
    - Builds on socket_setup.c
    - Use setsockopt to handle "address already in use errors".
    - Bind the socket to the address received as a response from getaddrinfo.

4. connect_setup.c
    - Build on bind_setup.c
    - We don't call bind here because we don't need to know about the local port that we are communicating through. The server can get the random port the kernel assigns to this connection.
    - This is what a typical client code will do. Client normally don't need to bind to a specific port because they don't normally need to handle any incoming connections and hence don't need to be listening on some well known port. 

5. listen_accept.c
    - Shows how to listen for incoming connections and accept them.
    - listen also takes as an arg the number of connections that can be held in the backlog queue.
    - getaddrinfo -> socket -> bind -> listen -> accept
    - bind is necessary here because we want to fix the port through which we will handle incoming connections.
    - A client 'connect's to a specified port that the server is listening on(the port the server socket is bound to with bind). The server socket will be listening on this port. The incoming client connection will be queued to be accepted. When we call accept to fetch this pending connection. accept returns a brand new socket file descriptor to use for this connection. 
    - accept also populates a value-result parameter with the client address info.