/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:57:59 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/22 14:27:28 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <csignal>

Server	*g_server_ptr = NULL;

void sigintHandler(int sig) {
	if (g_server_ptr == NULL)
		exit(sig);
	else
		g_server_ptr->shutdownServer(sig);
}

static bool	checkPort(const std::string& port)
{
	int	size = port.size();

	for (int i = 0; i < size; i++) {
		unsigned char c = port[i];
		if (!std::isdigit(c)) {
			std::cerr << "Error: Invalid port '" << port << "'. Port must be a numeric value." << std::endl;
			return false;
		}
	}

	long long check_port = std::atoll(port.c_str()); 

	if (check_port < 1024 || check_port > 65535) {
		std::cerr << "Error: Port " << check_port << " out of range. Valid range is 1024-65535." << std::endl;
		return false;
	}

	return true;
}

int	main(int argc, char *argv[])
{
	if (argc != 3 || argv[1][0] == '\0') {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	std::string	port = argv[1];

	if (checkPort(port) == false)
		return 1;
	
	std::string	password = argv[2];
	Server	server("127.0.0.1", std::string(argv[1]), std::string(argv[2]));
	g_server_ptr = &server;
	std::signal(SIGINT, sigintHandler);
	server.run();
	return server.getErrorCode();
}
