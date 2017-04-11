/**
 * \file
 *
 * \brief This is an example of using the 'MidiMan' class.
 *
 *
 *
 * \author Henrik von Coler
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2016/11/02$
 *
 * Contact: von_coler@tu-berlin.de
 *
 */


#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstdlib>


#include "../../src/midiman.h"


MidiMan         *midiMan;


int main(int argc, char* argv[])
{

    /// allocate a new midi manager
    midiMan = new MidiMan();

    /// tell midi manager to output everything
    // midiMan->setVerbose();

    while(1)
    {


        /// sleep for a period and gather messagess
        usleep(10*1000);

        /// get messages (and print, since verbose)
        MidiMan::midiMessage     mm =  midiMan->get_rtmidi();

        /// flush messages
        midiMan->flushProcessedMessages();


        if(mm.byte1!=-1)
        {
        std::cout << mm.byte1 << std::endl ;
        std::cout << mm.byte2 << std::endl ;
        std::cout << mm.byte3 << std::endl ;
        }

    }

    return 0;

}

