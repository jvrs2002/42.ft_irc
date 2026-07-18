/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppassos <ppassos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:09:58 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/18 16:00:13 by ppassos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string intToString(int number)
{
	std::stringstream	ss;
	ss << number;
	return ss.str();
}

// Helper function to get the socket address (IPv4 or IPv6)
void *utils_get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Helper function to get the socket port
std::string utils_get_port_str(struct sockaddr *sa)
{
	int port = 0;

	if (sa->sa_family == AF_INET) {
		port = ntohs(((struct sockaddr_in*)sa)->sin_port);
	}
	else {
		port = ntohs(((struct sockaddr_in6*)sa)->sin6_port);
	}

	return intToString(port);
}

void sendReply(int Clientfd, const std::string& server, const std::string& code, 
	const std::string& target, const std::string& params, const std::string& trailing) //tinha de por msg_nosignal
{
	std::string msg = ":" + server + " " + code + " " + target + (params.empty() ? "" : " " + params) + " :" + trailing + "\r\n";
	send(Clientfd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

/*void sendsReply(int Clientfd, const std::string& msg) // nova por aplicar no codigo
{
	send(Clientfd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}*/
