/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:27:49 by mboujama          #+#    #+#             */
/*   Updated: 2025/04/15 13:03:28 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sys/stat.h>
# include <iostream>
# include <sstream>
# include <time.h>
# include <map>

class ResponseUtils {
    public:
        static std::string getDateTime();
        static std::string allowHeaderValue(std::map<std::string, bool> allowedMethods);
        static bool pathExists(const std::string& path);
};