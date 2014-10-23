//SetSampleRate.cpp
//adjust psec-4 sampling rate (slower = larger window)
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

  PSEC4_EVAL command;
  
  int sampling_mode_select = atoi(argv[1]);

  if(command.INITIALIZE() != 0){
    return 1;
  }

  if(sampling_mode_select == 0)
    command.SET_SAMPLE_RATE(false);

  else if(sampling_mode_select == 1)
    command.SET_SAMPLE_RATE(true);

  else
    command.SET_SAMPLE_RATE(false);
  
  usleep(1000);
  command.DLL_RESET();
  
  cout << "resetting DLL..." << endl;  
  usleep(3000000)  ;
  
  cout <<"sampling rate adjusted...check LED indicator" << endl << endl;

  return 0;


}
