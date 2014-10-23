//SetPed.cpp
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

  if(argc == 2 && std::string(argv[1]) == "-h" ){
    cout << endl;
    cout << "SetPed :: is used to set the offset voltage at PSEC4 input" << endl;
    cout << "SetPed :: takes 1 argument: PED_VALUE [unsigned int]" << endl;
    return 1;
  }
  else if(argc != 2){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }

  else{
    PSEC4_EVAL command;

    int level_temp = atoi(argv[1]);
    
    if(command.INITIALIZE() != 0){
      return 1;
    }
    //int level_temp = 4000;
    //cout << level_temp << endl;
    if(level_temp > 4095 || level_temp < 0){
      cout << "invalid entry!\n\n";
      return 1;
    }
    else{
      command.INT_TRIG_ENABLE(false,0);
      int level = level_temp;
      command.PED_SCAN(level);
      command.PED_SCAN(level); //twice for good measure
      cout << "setting level...\n";
      cout << "level set to " << level*1200/4096 << " mV !!\n\n";
      //printf("pedestal voltage set to %f mV\n\n", level);
      return 0;
    }
  }
}
