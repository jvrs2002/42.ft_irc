/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:03 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/15 17:24:42 by manelcarval      ###   ########.fr       */
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

class Channel
{
private:
		std::string					_channel_name;
		std::vector<std::string>	_users;
		std::vector<std::string>	_operators;
		
		// mandatory modes
		bool _invite;
		bool _topic_ops;
		bool _password_active;
		std::string _password;
		bool _user_limit_active;
		bool _user_limit;
		
				
		
public:
	Channel(/* args */);
	~Channel();
};

Channel::Channel()
{
}

Channel::~Channel()
{
}

#endif
