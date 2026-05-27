/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:54:26 by joao-vri          #+#    #+#             */
/*   Updated: 2026/05/27 18:57:17 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.hpp"
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>

void*		utils_in_addr(struct sockaddr *sa);
std::string	utils_port_str(struct sockaddr *sa);

#endif
