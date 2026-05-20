/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/18 21:10:08 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Client.hpp"

Client::Client(std::string ip, int port, int fd, std::string buffer) : _ip(ip), _port(port),
		_fd(fd), _authenticated(false), _registered(false), _buffer(buffer)
{
	// add here new_client_handler function
}

Client::~Client()
{
	// add here destroy_client function (it has to delete also from Channel's)
}

int	Client::get_fd()
{
	return _fd;
}