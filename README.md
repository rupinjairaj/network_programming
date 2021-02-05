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