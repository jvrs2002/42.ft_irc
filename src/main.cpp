/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:57:59 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/09 20:34:53 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Message.hpp"
#include "Server.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 3 || !argv)
		return -1;
	// call input checking function first
	Server	server("127.0.0.1", argv[1], argv[2]);
	while (server.isRunning())
		// server.run(); still need to create function
	return server.getErrorCode();
}
