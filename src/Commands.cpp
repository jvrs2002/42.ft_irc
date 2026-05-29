/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/28 11:41:15 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Utils.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["PART"] = &part_handler;
	_handler["PRIVMSG"] = &privmsg_handler;
}

Commands::~Commands()
{

}

void Commands::Commandhandler(Message msg, Client* user, Server* server) 
{
	_handler[msg.getCommand()] (msg, user, server);
}

// static void sendReply(int Clientfd, const std::string& server, const std::string& code, 
// 	const std::string& target, const std::string& params, const std::string& trailing) 
// {
// 	std::string msg = ":" + server + " " + code + " " + target + " " + params + " :" + trailing + "\r\n";
// 	send(Clientfd, msg.c_str(), msg.size(), 0);
// }

void Commands::privmsg_handler(Message msg, Client* user, Server* server) 
{
	std::vector<std::string> params = msg.getParams();

	if (params[0].empty())
	{
		sendReply(user->getClientFd(), server->NAME, "411", user->getNickname(), "PRIVMSG", "No recipient given");
		return ;
	}
	if (params[1].empty())
	{
		sendReply(user->getClientFd(), server->NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (target[0] == CHANNEL)
	{
		Channel channel = server->getChannel(target);
		if (!channel)
		{
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), target, "No such channel");
			return ;
		}
		if (!channel.hasUser(user))
		{	
			sendReply(user->getClientFd(), server->NAME, "404", user->getNickname(), target, "Cannot send to channel");
			return ;
		}
		channel.ChannelMessage(msg.getPrefix(), user, message);
	}
	else
	{

		int target_fd = server->getClientFd(target);
		if (!target_fd)
		{
			sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target, "No such nick");
			return ;
		}
		std::string priv_msg = msg.getPrefix() + " PRIVMSG " + target + " :" + message;
		send(target_fd, priv_msg.c_str(), priv_msg.size(), 0);
	}

}

void Commands::join_handler(Message msg, Client* user, Server* server)
{
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "JOIN", "Not enough parameters");
		return ;
	}

	std::string channel_name = params[0];
	Channel	channel = server->getChannel(channel_name); // error code?
	std::string password = (params.size() > 1) ? params[1] : "";

	if (channel_name.size() <= 1 || channel_name[0] != '#')
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (channel)
		channel.joinChannel(msg.getPrefix(),user, password);
	else {
		channel = Channel(channel_name, user);
		channel.joinChannel(msg.getPrefix(), user, password);
	}
}

void Commands::part_handler(Message msg, Client* user, Server* server) 
{
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PART", "Not enough parameters");
		return ;
	}
	std::string channel_name = params[0];
	Channel	channel = server->getChannel(channel_name);
	std::string reason = (params.size() > 1) ? params [1] : "Leaving";
	//return error of channel?
	if (!channel)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (!channel.hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return ;
	}

	channel.partChannel(msg.getPrefix(), user, reason);
	if (channel.emptyChannel())
		server->deleteChannel(channel_name);
}