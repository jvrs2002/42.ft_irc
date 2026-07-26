/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:18:29 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/26 22:59:42 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

Server::Server(const std::string& ip, const std::string& port, const std::string& password) :
	_ip(ip),
	_port(port), 
	_password(password),
	_socket_fd(-1),
	_BACKLOG(10),
	_running(true),
	_error_code(0),
	NAME("irc.ft_irc.net")
{
	Server::initServer();
}
Server::~Server()
{
	_client_map.clear();
	_channel_map.clear();

	if (_socket_fd != -1)
		close(_socket_fd);
}

void	Server::initServer()
{
	int	status;
	struct addrinfo	hints;
	struct addrinfo	*serv_info;

	std::fill(reinterpret_cast<char*>(&hints), reinterpret_cast<char*>(&hints) + sizeof(hints), 0);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	status = getaddrinfo(NULL, _port.c_str(), &hints, &serv_info);

	if (status != 0) {
		std::cerr << "getaddrinfo() error." << std::endl;
		exit(EXIT_FAILURE);
	}

	_socket_fd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);

	if (_socket_fd == -1) {
		std::cerr << "socket() error." << std::endl;
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}

	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl() error setting O_NONBLOCK." << std::endl;
		close(_socket_fd);
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}

	int	yes = 1;
	status = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

	if (status == -1) {
		std::cerr << "setsockopt() error." << std::endl;
		close(_socket_fd);
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}
	
	status = bind(_socket_fd, serv_info->ai_addr, serv_info->ai_addrlen);

	if (status == -1) {
		std::cerr << "bind() error." << std::endl;
		close(_socket_fd);
		freeaddrinfo(serv_info);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(serv_info);
	status = listen(_socket_fd, _BACKLOG);

	if (status == -1) {
		close(_socket_fd);
		std::cerr << "listen() error." << std::endl;
		exit(EXIT_FAILURE);
	}

	struct pollfd pfd;
	pfd.fd = _socket_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_pollfd_vector.push_back(pfd);
}

void	Server::acceptClient()
{
	struct sockaddr_storage	client_addr;
	std::string				client_port;
	socklen_t				sin_size = sizeof client_addr;
	char					client_ip[INET6_ADDRSTRLEN];

	int	client_fd = accept(_socket_fd, (struct sockaddr *)&client_addr, &sin_size);

	if (client_fd == -1)
	{
		std::cerr << "new client error" << std::endl;
		return ;
	}

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl() error on incoming client." << std::endl;
		close(client_fd);
		return ;
	}

	inet_ntop(client_addr.ss_family, utils_get_in_addr((struct sockaddr *)&client_addr), client_ip, sizeof client_ip);
	client_port = utils_get_port_str((struct sockaddr *)&client_addr);

	std::cout << "server: got connection from IP " << client_ip << " using PORT "<< client_port << std::endl; // testing

	addClient(client_ip, client_port, client_fd);
}

void	Server::addClient(const std::string& ip, const std::string& port, int client_fd)
{	
	if (ip.empty() || port.empty() || client_fd == -1)
		return ;

	Client&	new_client = _client_map[client_fd];
	
	if (new_client.initClient(ip, port, client_fd) == false)
		disconnectClient(&new_client);

	struct pollfd	pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfd_vector.push_back(pfd);
}

int	Server::getClientFd(const std::string& nickname) const
{
	if (nickname.empty())
		return (-1);
	
	for (std::map<int, Client>::const_iterator it = _client_map.begin(); it != _client_map.end(); ++it) {
		if (it->second.getNickname() == nickname)
			return it->first;
	}

	return (-1);
}

Channel*	Server::getChannel(const std::string& channel_name)
{
	if (channel_name.empty() || !_channel_map.count(channel_name))
		return NULL;

	return &_channel_map[channel_name];
}

bool	Server::deleteChannel(const std::string& channel_name)
{
	if (channel_name.empty() || !_channel_map.count(channel_name))
		return false;
	
	if (!_channel_map[channel_name].emptyChannel())
		return false;
	
	_channel_map.erase(channel_name);
	return true;
}

void	Server::disconnectClient(Client *user)
{
	if (!user)
		return ;

	int	client_fd = user->getClientFd();
	std::vector<struct pollfd>::iterator it = _pollfd_vector.begin();

	std::cout << "server: disconnecting from IP " << user->getClientIP() << " using FD " << user->getClientFd() << std::endl; // testing

	while (it != _pollfd_vector.end() && it->fd != client_fd)
		++it;

	if (it != _pollfd_vector.end())
		_pollfd_vector.erase(it);

	_client_map.erase(client_fd);
}

/*	This function doesn't add the new channel into the creator's map.
	Remember to always call creator's addToChannel() after this call. */
bool	Server::createChannel(const std::string& channel_name, Client *creator)
{
	if (channel_name.empty() || !creator || !creator->isRegistered())
		return false;

	if (_channel_map.find(channel_name) != _channel_map.end())
		return false;

	_channel_map[channel_name] = Channel(channel_name, creator);
	creator->addToChannel(&_channel_map[channel_name]);
	return true;
}

// the main's while loop depends on the _running as a condition
void	Server::shutdownServer(int error_code)
{
	_running = false;
	_error_code = error_code;
}

int	Server::getErrorCode()
{
	return _error_code;
}

bool	Server::userExists(const std::string& nickname) const
{
	if (nickname.empty())
		return false;
	
	for (std::map<int, Client>::const_iterator it = _client_map.begin(); it != _client_map.end(); ++it) {
		if (it->second.getNickname() == nickname)
			return true;
	}

	return false;
}

// will run as long as _running is true. will stop with shutdownServer() function
void	Server::run()
{
	int	events_count;

	while (_running == true && _socket_fd != -1 && !_pollfd_vector.empty() &&
	_pollfd_vector[0].fd == _socket_fd) {
		events_count = poll(&_pollfd_vector[0], _pollfd_vector.size(), -1);
		if (events_count == -1) {
			if (errno == EINTR)
				continue ;
			shutdownServer(EXIT_FAILURE);
			return ;
		}
		if (events_count == 0)
			continue ;
		processEvents(events_count);
	}
}

void	Server::processEvents(int events_count)
{
	size_t	i = 0;
	int	status = 0;
	Client	*active_client;
	std::string	command;


	if (events_count == 0)
		return ;
	
	while (events_count > 0 && i < _pollfd_vector.size()) {
		if (_pollfd_vector[i].revents & (POLLIN | POLLHUP | POLLERR)) {
			active_client = getClientInstance(_pollfd_vector[i].fd);
			if (!active_client) // new client to be added
				acceptClient();
			else {
				status = active_client->receiveBuffer();
				if (status == Client::RECV_EOF) {
					disconnectClient(active_client);
					events_count--;
					continue ;
				}
				else {
					command = active_client->handlePartialBuffer();
					while (!command.empty()) { // if empty it's still not ready to be read
						std::cout << command << std::endl;
						Message	msg(active_client, command);
						_command_handler.Commandhandler(msg, active_client, this);
						command = active_client->handlePartialBuffer();
					}
				}
			}
			events_count--;
		}
		i++;
	}

	if (events_count != 0) {
		std::cerr << "events_count error: " << std::endl;
		shutdownServer(42);
	}
}

Client* Server::getClientInstance(int client_fd)
{
	std::map<int, Client>::iterator it = _client_map.find(client_fd);

	if (it == _client_map.end()) {
		return NULL;
	}

	return &(it->second);
}

bool	Server::authenticate(const std::string& user_pass, Client *user)
{
	if (!user)
		return false;

	if (user_pass == _password) {
		user->setAuthenticated();
		std::cout << "server: user with FD " << user->getClientFd() << " is authenticated" << std::endl;
		return true;
	}

	return false;
}

const std::map<std::string, Channel>& Server::getChannelMap() const
{
	return _channel_map;
}

