/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:06 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/17 20:13:48 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Client.hpp"

Client::Client(std::string ip, int port, int fd, std::string buffer) : _ip(ip), _port(port),
		_fd(fd), _authenticated(false), _registered(false), _buffer(buffer)
{
}

Client::~Client()
{
}