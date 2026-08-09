/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:25 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/27 19:32:05 by joao-vri         ###   ########.fr       */
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
	const std::string				_ip;				// usually 127.0.0.1
	std::string						_port;
	std::string						_password;
	int								_socket_fd;			// server's socket fd

	Commands						_command_handler;
	std::map<std::string, Channel>	_channel_map;		// each channel is mapped by their name
	std::map<int, Client>			_client_map;		// each client is mapped by it's socket fd
	std::vector<struct pollfd>		_pollfd_vector;

	const int						_BACKLOG;
	bool							_running;			// condition for while loop
	int								_error_code;
public:
	const std::string NAME;

	Server(const std::string& ip, const std::string& port, const std::string& password);
	~Server();

	void		initServer();
	int			acceptClient();
	void		addClient(const std::string& ip, const std::string& port, int client_fd);
	bool		createChannel(const std::string& channel_name, Client *creator);
	int			getClientFd(const std::string& nickname) const;
	Client*		getClientInstance(int client_fd);
	Channel*	getChannel(const std::string& channel_name);
	const std::map<std::string, Channel>& getChannelMap() const;
	bool		deleteChannel(const std::string& channel_name);
	void		disconnectClient(Client *user, const std::string& reason = "Client Quit");
	void		shutdownServer(int error_code);
	int			getErrorCode();
	bool		userExists(const std::string& nickname) const;
	void		run();
	void		processEvents();
	bool		authenticate(const std::string& user_pass, Client *user);
};

#endif
