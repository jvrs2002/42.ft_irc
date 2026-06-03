/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:11 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/02 19:03:49 by ppassos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Utils.hpp"

Commands::Commands()
{
	_handler["JOIN"] = &join_handler;
	_handler["PART"] = &part_handler;
	_handler["PRIVMSG"] = &privmsg_handler;
	_handler["NOTICE"] = &notice_handler;
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

void Commands::join_handler(Message msg, Client* user, Server* server)
{
	std::vector<std::string> params = msg.getParams();

	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "JOIN", "Not enough parameters");
		return ;
	}

	std::string channel_name = params[0];
	Channel*	channel = server->getChannel(channel_name);
	std::string password = (params.size() > 1) ? params[1] : "";

	if (channel_name.size() <= 1 || channel_name[0] != '#')
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (channel != NULL) 
	{
		if (!user->addToChannel(channel))
		{
			sendReply(user->getClientFd(), server->NAME, "443", user->getNickname(), channel_name, "User is already in channel");
			return ;
		}
		channel->joinChannel(msg.getPrefix(), user, password, server->NAME);
	}
	else {
		if (!server->createChannel(channel_name, user))
		{ 	//Correct error?
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
			return ;
		}
		channel = server->getChannel(channel_name);
		if (!user->addToChannel(channel))
		{	//Correct error?
			sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), channel_name, "Error joining channel");
			return ;
		}
		channel->joinChannel(msg.getPrefix(), user, password, server->NAME);
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
	Channel* channel = server->getChannel(channel_name);
	std::string reason = (params.size() > 1) ? params [1] : "Leaving";
	
	if (channel == NULL)
	{
		sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), channel_name, "No such channel");
		return ;
	}
	if (!channel->hasUser(user))
	{
		sendReply(user->getClientFd(), server->NAME, "442", user->getNickname(), channel_name, "You're not on that channel");
		return ;
	}

	channel->partChannel(msg.getPrefix(), user, reason);
	if (channel->emptyChannel())
		server->deleteChannel(channel_name);
}

void Commands::privmsg_handler(Message msg, Client* user, Server* server) 
{
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 1)
	{
		sendReply(user->getClientFd(), server->NAME, "411", user->getNickname(), "PRIVMSG", "No recipient given");
		return ;
	}
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (target[0] == CHANNEL)
	{
		Channel* channel = server->getChannel(target);
		if (channel == NULL)
		{
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), target, "No such channel");
			return ;
		}
		if (!channel->hasUser(user))
		{	
			sendReply(user->getClientFd(), server->NAME, "404", user->getNickname(), target, "Cannot send to channel");
			return ;
		}
		channel->ChannelMessage(msg.getPrefix(), user, message);
	}
	else
	{
		int target_fd = server->getClientFd(target);
		if (target_fd == -1)
		{
			sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target, "No such nick");
			return ;
		}
		std::string priv_msg = msg.getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		send(target_fd, priv_msg.c_str(), priv_msg.size(), 0);
	}

}

void Commands::notice_handler(Message msg, Client* user, Server* server) 
{
	std::vector<std::string> params = msg.getParams();

	if (params.size() < 1)
	{
		sendReply(user->getClientFd(), server->NAME, "411", user->getNickname(), "NOTICE", "No recipient given");
		return ;
	}
	if (params.size() < 2)
	{
		sendReply(user->getClientFd(), server->NAME, "412", user->getNickname(), "", "No text to send");
		return ;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (target[0] == CHANNEL)
	{
		Channel* channel = server->getChannel(target);
		if (channel == NULL)
		{
			sendReply(user->getClientFd(), server->NAME, "403", user->getNickname(), target, "No such channel");
			return ;
		}
		if (!channel->hasUser(user))
		{	
			sendReply(user->getClientFd(), server->NAME, "404", user->getNickname(), target, "Cannot send to channel");
			return ;
		}
		channel->ChannelMessage(msg.getPrefix(), user, message);
	}
	else
	{

		int target_fd = server->getClientFd(target);
		if (target_fd == -1)
		{
			sendReply(user->getClientFd(), server->NAME, "401", user->getNickname(), target, "No such nick");
			return ;
		}
		std::string notice_msg = msg.getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
		send(target_fd, notice_msg.c_str(), notice_msg.size(), 0);
	}

}


//protocolist work(on going)

void Commands::nick_handler(Message msg, Client* user, Server* server) 
{
	if (!user.isAuthenticated()) //fazer funcao
		return ;
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PART", "Not enough parameters");
		return ;
	}
	if (!user.isRegisted()) //fazer funcao
	{
		//fazer funcao que ve se o novo nick existe com todos os clientes AUTHENTICADOS
		return ;
	}
	//avisar todos se for mudado o nick em que o cliente esta ligado pelos canais 
	//fazer funcao que ve
	
}

void Commands::pass_handler(Message msg, Client* user, Server* server) 
{
	///o get nickname ou user tem de vericar se exite ou nao porque caso nao exista tenho de mandar unknow ou '*' como o nick ou user (if(empty) = '*' || = unknow)
	std::string password = server.getpassowrd();//fazer funcao 
	if (user.isAuthenticated()) //fazer funcao
		return ;
	if (params.empty())
	{
		sendReply(user->getClientFd(), server->NAME, "461", user->getNickname(), "PASS", "Not enough parameters");
		return ;
	}
	std::string password = params[0];
	if (params[0] !=  password)
	{
		sendReply(user->getClientFd(), server->NAME, "464", user->getNickname(), "", "Password incorrect");
		return ;
	}
	user.authenticationSuccess();	//fazer funcao
}

//authenticado = password aceite que o user deu ou o sv nao tem pass
//registrado = com um user valido e um nick porem diferente de todos os presentes