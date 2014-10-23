//ScopeData.cpp
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

int main(int argc, char **argv){
  
  if(argc == 2 && std::string(argv[1]) == "-h"){
    cout << endl;
    cout << "ScopeData :: is used look at waveforms via gnuplot" << endl;
    cout << "ScopeData :: takes 2 arguments: number_waveforms [unsigned int]  mode [1=ext-trig 0=soft-trig]" << endl;
    return 1;
  }
  else if(argc != 3){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }

  else{
    PSEC4_EVAL command;

    unsigned int num_frames = atoi(argv[1]);
    int trig_mode  = atoi(argv[2]);
    //int scope_mode  = atoi(argv[3]);
    
    if(command.INITIALIZE() != 0){
      return 1;
    }
  
    //if(trig_mode != 0)
    //  command.USB_SYNC(true);

    //command.LOG_DATA_SCOPE(1, num_frames, trig_mode, scope_mode);
    command.LOG_DATA_SCOPE(1, num_frames, trig_mode, 0);

    return 0;
  }

}
