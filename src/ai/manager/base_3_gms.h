/*
    This file is part of SSL.

    Copyright 2018 Name Surname (mail)

    SSL is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SSL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MANAGER__BASE_3_GMS__H__
#define __MANAGER__BASE_3_GMS__H__

#include "Manager.h"
#include <referee/Referee.h>

namespace RhobanSSL {
namespace Manager {

class Base_3_gms : public Manager {
    private:
    const Referee & referee;

    unsigned int last_referee_changement;
            
    std::list<std::string> future_strats;
    
    public:

    Base_3_gms(
        Ai::AiData & ai_data,
        const Referee & referee
    );

    void update(double time);
    void choose_a_strategy(double time);

    virtual ~Base_3_gms();

};

};
};

#endif
