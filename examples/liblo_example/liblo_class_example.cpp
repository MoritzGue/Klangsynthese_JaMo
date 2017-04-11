/**
 * \file
 *
 * \brief This is an example of using the 'OscMan' class.
 *
 *
 *
 * \author Henrik von Coler
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2016/08/18$
 *
 * Contact: von_coler@tu-berlin.de
 *
 */

#include "../../src/oscman.h"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstdlib>

OscMan *oscMan;

std::vector<int> *m;




int main(int argc, char* argv[])
{

    if(argc<=1)
    {
        std::cout << "Need port number!" << std::endl;
        return 0;
    }

    int port  = atoi(argv[1]);

    /// create new OscMan Object
    oscMan = new OscMan(port);


    while(1)
    {

        /// sleep for a period and gather messagess
        usleep(1000*1000);

        /// print all messages using OscMan::printAllMessages
          oscMan->printAllMessages();

	//std::cout << oscMan->getLastType() << std::endl;

    }

    return 0;

}

