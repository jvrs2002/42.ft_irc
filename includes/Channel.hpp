/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:03 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/09 11:57:34 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <set>

class Client;

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
	void	ChannelMessage(std::string prefix, Client* sender, std::string command, std::string buffer);
	void	removeUser(Client *user);
	
};



#endif
