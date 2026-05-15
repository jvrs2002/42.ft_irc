/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:15 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/15 16:55:07 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Message.hpp"
#include "Client.hpp"

class Commands
{
private:
		static void join_handler(Message msg, Client user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map);		
		static void nick_handler(Message msg, Client user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map);
		
		typedef void (*PFnCommandHandler)(Message, Client, std::map<int, Client>&, std::map<std::string, Channel>&);
		std::map<std::string, PFnCommandHandler> _handler;
		
public:
		Commands();
		~Commands();
		void Commandhandler(Message msg, Client user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map);
};

#endif