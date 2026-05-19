/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:25 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/20 17:12:15 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"

class Server
{
private:
	static const std::string	_ip; // usually 127.0.0.1
	int	_port;
	std::string _password;
	int	_socket_fd; // server's socket fd

	Commands	command_handler;
	std::map<std::string, Channel>	_channels; // each channel is mapped by their name
	std::map<int, Client>	_clients; // each client is mapped by it's socket fd
public:
	Server(std::string ip, int port, std::string password);
	~Server();
	void		init_server(std::string ip, int port, std::string password, int socket_fd);
	Client*		create_user(int fd, const std::string& ip, int port);
	void		delete_user(Client *user);
};

struct addrinfo {
	int					ai_flags;			// AI_PASSIVE, AI_CANONNAME, etc.
	int					ai_family;			// AF_INET, AF_INET6, AF_UNSPEC
	int					ai_socktype;		// SOCK_STREAM, SOCK_DGRAM
	int					ai_protocol;		// use 0 for "any"
	size_t				ai_addrlen;			// size of ai_addr in bytes
	struct sockaddr		*ai_addr;			// struct sockaddr_in or _in6
	char				*ai_canonname;		// full canonical hostname

	struct addrinfo		*ai_next;			// linked list, next node
};

#endif
