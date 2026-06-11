/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:57:59 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/12 17:52:29 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "Server.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 3 || !isValidInput(argv[1], argv[2])) { // still need to ask for this function
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server	server("127.0.0.1", std::string(argv[1]), std::string(argv[2]));
	server.run();
	return server.getErrorCode();
}
