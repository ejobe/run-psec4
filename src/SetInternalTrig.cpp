//SetInternalTrig.cpp
//
//ejo 2012

#include "PSEC4_EVAL.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

int
main(int argc, char **argv){
  
  if(argc == 2 && std::string(argv[1]) == "-h"){
    cout << endl;
    cout << "SetInternalTrig :: is used to set the self trigger sign and voltage threshold level" << endl;
    cout << "SetInternalTrig :: takes 2 arguments: TRIG_SIGN [1=(-pulse), 0=(+pulse)]  TRIG_TRESH [unsigned int]" << endl;
    return 1;
  }
  else if(argc != 3){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }
  
  else{  
    PSEC4_EVAL internal_trigger;
  
    int trig_sign = atoi(argv[1]);
    int trig_threshold = atoi(argv[2]);
    if(trig_threshold < 0 || trig_threshold > 4095 || trig_sign < 0 || trig_sign > 1){
      cout << "error: threshold value must be 12 bit unsigned, sign must be 0 or 1" << endl;
      return -2;
    }
    
    if(internal_trigger.INITIALIZE() != 0){
      return 1;
    }

    internal_trigger.INT_TRIG_RESET();

    internal_trigger.USB_DEBUG((unsigned int)trig_threshold);
    internal_trigger.INT_TRIG_SETSIGN((bool)trig_sign);
    cout << "threshold level set to " << (unsigned int)trig_threshold*1200/4096 << " mV !!\n";

    //internal_trigger.INT_TRIG_RESET();
    //
    return 0;
  }
}

  
  
