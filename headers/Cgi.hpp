/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:52:57 by mboujama          #+#    #+#             */
/*   Updated: 2025/05/21 08:03:48 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Types.hpp"

class Cgi {
    private:
        char                        **createEnvironmentVariables(Request &request, char **systemEnv);
        std::vector<std::string>    extractBinaryPaths(char **environmentVariables);
        std::string                 locateExecutable(const std::vector<std::string> &searchPaths,
													const std::string &executableName);

    public:
        Cgi();
        ~Cgi();
        std::string	executeCgiScript(Request &request, char **systemEnv);
};
