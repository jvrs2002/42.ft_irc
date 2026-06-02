/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:15 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/01 12:25:57 by manelcarval      ###   ########.fr       */
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

#define CHANNEL '#'

class Commands
{
private:
		static void join_handler(Message msg, Client* user, Server* server);		
		static void part_handler(Message msg, Client* user, Server* server);
		static void privmsg_handler(Message msg, Client* user, Server* server);
		static void notice_handler(Message msg, Client* user, Server* server);	

		
		typedef void (*PFnCommandHandler)(Message, Client*, Server*);
		std::map<std::string, PFnCommandHandler> _handler;
		
public:
		Commands();
		~Commands();
		void Commandhandler(Message msg, Client* user, Server* server);
};

#endif