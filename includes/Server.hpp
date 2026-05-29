/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:25 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/27 19:08:02 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <netinet/in.h>
# include <poll.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <cstdlib>
# include "Channel.hpp"
# include "Client.hpp"
# include "Commands.hpp"
# include "Message.hpp"
# include "Utils.hpp"
# include <cerrno>
# include <cstring>

class Server
{
private:
	const std::string			_ip;				// usually 127.0.0.1
	std::string					_port;
	std::string					_password;
	int							_socket_fd;			// server's socket fd

	Commands						command_handler;
	std::map<std::string, Channel>	_channel_map;		// each channel is mapped by their name
	std::map<int, Client>			_client_map;		// each client is mapped by it's socket fd

	const int _BACKLOG;
public:
	const std::string NAME;

	Server(std::string ip, std::string port, std::string password);
	~Server();

	void	initServer();
	void	acceptClient();
	void	addClient(const std::string& ip, std::string port, int client_fd);
	// void		deleteClient(Client *user);
};

#endif
