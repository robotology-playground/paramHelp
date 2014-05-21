/*
 * Copyright (C) 2013  CoDyCo Consortium
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 *
 * Authors: Andrea Del Prete, Alessio Rocchi
 * email: andrea.delprete@iit.it, alessio.rocchi@iit.it
 */

#include <paramHelp/paramProxyBasic.h>

using namespace paramHelp;

template <>
std::string ParamProxyBasic<double>::getAsString() const
{
    if(size==0)
        return std::string("");
    std::stringstream ss;
    ss << std::showpoint << value[0];
    for(int i=1; i<size; i++)
        ss << " " << std::showpoint << value[i];
    return ss.str();
}

template <>
std::string ParamProxyBasic<bool>::getAsString() const
{
    if(size==0)
        return std::string("");
    std::stringstream ss;
    ss << std::boolalpha << value[0];
    for(int i=1; i<size; i++)
        ss << " " << std::boolalpha << value[i];
    return ss.str();
}
