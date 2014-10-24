//EnableTrig.cpp
//
//ejo 2012

#include "PSEC4_EVAL.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int
main(int argc, char **argv){

  if(argc == 2 && std::string(argv[1]) == "-h"){
    cout <<  endl;
    cout << "EnableTrig :: is used to turn on/off self triggering option, and to specify trigger channel" << endl;
    cout << "EnableTrig :: takes 2 arguments: ENABLE [1=ON, 0=OFF]  Channel_mask [int 1-6]" << endl;
    return 1;
  }
  else if(argc != 3){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }

  else{
    PSEC4_EVAL trigger;

    bool ENABLE = atoi(argv[1]);
    unsigned int MASK = atoi(argv[2]);
    if(MASK < 0 || MASK > 6){
      cout << "pick channel 1-6 for self-trigger option" << endl;
      cout << "Board NOT updated" << endl;
      return 1;
    }
    
    if(trigger.INITIALIZE() != 0){
      return 1;
    }
    
    trigger.INT_TRIG_ENABLE(ENABLE, MASK);
    trigger.INT_TRIG_RESET();
    if(ENABLE)
      cout << "self-trigger ON, using channel " << MASK << endl;
    else
      cout << "self-trigger turned OFF " << endl;
    
    return 0;
  }
}
