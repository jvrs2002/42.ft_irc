/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:32:08 by joao-vri          #+#    #+#             */
/*   Updated: 2026/06/11 14:44:53 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <set>
#include <errno.h>

class Channel;

class Client
{
private:
	std::string	_username;
	std::string	_nickname;
	std::string	_realname;

	bool	_authenticated;
	bool	_registered;

	std::set<Channel*> _channels;	// stores channels pointers, maximum 10 channels per user

	std::string	_ip;
	std::string	_port;

	int	_socket_fd; // client's socket fd

	std::string	_buffer;
public:
	enum RecvReturn{
			RECV_SUCCESS,	// read success
			RECV_EOF,		// user disconnected
			RECV_ERROR		// read error
		};

	Client();
	Client(const std::string& ip, const std::string&, int fd);
	~Client();

	bool				initClient(const std::string& ip, const std::string& port, int fd);
	bool				addToChannel(Channel* channel);
	RecvReturn			receiveBuffer();
	bool				isRegistered() const;
	bool				isAuthenticated() const;
	bool				setRegistered();
	void				setAuthenticated();
	int					getClientFd() const;
	const std::string&	getClientIP() const;
	const std::string&	getUsername() const;
	const std::string&	getNickname() const;
	const std::string&	getRealName() const;
	bool				setNickname(const std::string& nickname);
	bool				setUsername(const std::string& username);
	bool				setRealname(const std::string& realname);
	bool				disconnectChannel(Channel *channel);
	std::string			handlePartialBuffer();
};

#endif
