*This project has been created as part of the 42 curriculum by joao-vri, mcarvalh, ppassos.*

## Description

**ft_irc** is a project from the 42 curriculum where the main goal is to build a fully functional Internet Relay Chat (IRC) server from scratch using C++98. The server handles multiple concurrent client connections, allowing users to authenticate, join channels, send private messages, and communicate in real-time.

To build this, we had to dive deep into socket programming and multiplexing (non-blocking I/O). [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) served as an invaluable resource for understanding the networking side of the code.

For the actual IRC protocol implementation, command parsing, and server replies, we took reference from the official IRC RFCs (primarily RFC 1459 and RFC 2812) to ensure our server behaves exactly like a real, standard-compliant IRC server.

Here is the Instructions section for your README, structured around your codebase requirements and HexChat setup:



Markdown
## Instructions

### Compilation

The project is built entirely in C++98. To compile the server, simply run the following command in your terminal:

```bash
make
```

This will produce the executable binary named ./ircserv.
Execution
To launch the IRC server, run the executable with a specified port and password:

```bash
./ircserv <port> <password>
```

\<port>: The port number the server will listen on. It must be a numeric value within the range of `1024` to `65535` (ports below `1024` are reserved for system services).

\<password>: The password required for clients to authenticate and connect to your server.

Example:
```bash
./ircserv 6667 my_secret_password
```

Connecting with HexChat
Once the server is running locally, you can connect to it using HexChat by following these steps:
Open HexChat and open the Network List (Ctrl + S or via the top menu HexChat -> Network List).
Click Add to create a new network and name it something like ft_irc.
Select your new network and click Edit....
In the servers list at the top, click on the existing entry (or click Add) and change it to:

```bash
localhost/6667
```



(Replace `6667` with the port you chose when executing the server).
Look for the Character set option and change it to IRC (IRCX) / ISO-8859-1 or UTF-8.
Under the Password field, enter the password you passed as an argument to the server.
Fill in your preferred Nick name and User name.
Click Close, select your network from the list, and click Connect.

## Resources

Building an IRC server requires a deep understanding of network programming and the strict implementation of internet standards. The following references were foundational to this project:

### Official Specifications (RFCs)
*   **[RFC 2812 - Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812):** The primary protocol specification used for 90% of the project. It dictates the exact text-based messaging structure, command formats (e.g., `JOIN`, `PART`, `PRIVMSG`), and numeric replies sent between the client and server.
*   **[RFC 2811 - Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811):** Defines the structural rules for channel creation, user membership limits, and channel modes (such as operators `+o`, passwords `+k`, limits `+l`, and invite-only status `+i`).
*   **[RFC 2810 - Internet Relay Chat: Architecture](https://datatracker.ietf.org/doc/html/rfc2810):** Outlines the global design principles, core requirements, and network topology models of an IRC network.

### Networking & Systems Programming
*   **[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/index-wide.html):** The definitive guide for Unix socket programming. Used to implement non-blocking I/O multiplexing (`poll()`/`epoll()`), handle file descriptors, and establish low-level socket connections.
*   **[GeeksforGeeks - The TCP/IP Model](https://www.geeksforgeeks.org/computer-networks/tcp-ip-model/):** A comprehensive reference guide for understanding data encapsulation, packet transmission, and stream-oriented socket communication across layers.

---

## AI Usage Disclosure

Artificial Intelligence was leveraged as an adaptive engineering collaborator throughout the development of `ft_irc`. AI assistance was strictly isolated to logical diagnostics, specification parsing, and architectural optimization rather than boilerplate generation.

### Utilized Tasks & Project Components

1. **Protocol Validation & RFC Translation**
   * **Component:** Command Parsing.
   * **Task:** Assisted in breaking down edge-case requirements outlined in RFC 2812—specifically clarifying user prefix behavior and ensuring that error validation for silent mechanisms (such as the `NOTICE` command) aligned perfectly with the official internet standards.

2. **Architectural Pair-Programming & Design Validation**
   * **Component:** System Architecture Design.
   * **Task:** Acted as a co-pilot to review, challenge, and validate high-level structural decisions. This included analyzing data ownership patterns, evaluating synchronous vs. asynchronous tracking relationships, and offering objective technical opinions on how to maintain data integrity across reciprocal object collections without overcomplicating the engine.