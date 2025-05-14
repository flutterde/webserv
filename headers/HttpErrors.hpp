/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpErrors.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:20:55 by ochouati          #+#    #+#             */
/*   Updated: 2025/05/14 12:09:40 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Types.hpp"
#include "ClientData.hpp"
class	HttpErrors {
	public:
		static void	httpResponse400(ClientData& client);
		static void	httpResponse414(ClientData& client);
		static void	httpResponse413(ClientData& client);
};
