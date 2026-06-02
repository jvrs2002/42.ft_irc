/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:03 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/01 10:49:32 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include "Client.hpp"

class Channel
{
private:
	std::string				_channel_name;
	std::set<Client*>		_users;
	std::set<Client*>		_operators;
	
	// mandatory modes
	bool _invite_active;
	std::set<std::string> _invited;
	bool _topic_active;
	std::string	_topic;
	bool _password_active;
	std::string _password;
	bool _user_limit_active;
	int _user_limit;
		
public:
	Channel();
	Channel(const std::string channel_name, Client* creator);
	Channel(const Channel& other);
	Channel& operator=(const Channel &other);
	~Channel();

	std::string getName() const;
	// bool	getInvited() const;
	bool	hasUser(Client *user) const;
	bool	emptyChannel() const;

	void	joinChannel(std::string prefix, Client* new_user, std::string password, std::string server_name);
	void	partChannel(std::string prefix, Client* user_delete, std::string reason);
	void	ChannelMessage(std::string prefix, Client* sender, std::string buffer);

	
};



#endif
