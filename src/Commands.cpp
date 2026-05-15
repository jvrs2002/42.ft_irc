/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/15 16:55:24 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Commands.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["NICK"] = &nick_handler;
}

Commands::~Commands()
{

}

void Commands::Commandhandler(Message msg, Client user, std::map<int, Client>& client_map, std::map<std::string, Channel>& channel_map) 
{
	_handler[msg.getCommand()] (msg, user, client_map, channel_map);
}

