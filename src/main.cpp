/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:57:59 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/19 13:10:54 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "Server.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 3 || std::strcmp(argv[1], "")) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	std::string	port = argv[1];

	for (unsigned char c : port) {
		if (!std::isdigit(c)) {
			std::cerr << "Error: Invalid port '" << port << "'. Port must be a numeric value." << std::endl;
			return 1;
		}
	}

	std::string	password = argv[2];
	Server	server("127.0.0.1", std::string(argv[1]), std::string(argv[2]));
	server.run();
	return server.getErrorCode();
}
