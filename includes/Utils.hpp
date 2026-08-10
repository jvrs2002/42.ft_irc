/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manelcarvalho <manelcarvalho@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:54:26 by joao-vri          #+#    #+#             */
/*   Updated: 2026/07/30 13:03:28 by manelcarval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef utils_get_HPP
# define utils_get_HPP

# include <string>
# include <sys/socket.h>
# include <sstream>
# include <netinet/in.h>
# include <cctype>

std::string	intToString(int number);
void*		utils_get_in_addr(struct sockaddr *sa);
std::string	utils_get_port_str(struct sockaddr *sa);
void sendReply(int Clientfd, const std::string& server, const std::string& code,
const std::string& target, const std::string& params, const std::string& trailing);
bool validChannelName(const std::string& name);
std::string toLower(const std::string& str);

#endif
