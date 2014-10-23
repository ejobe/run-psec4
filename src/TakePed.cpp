//TakePed.cpp
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
    cout << "TakePed :: runs pedestal calibration and saves constants to file: cal_data/PED_DATA.txt" << endl;
    cout << "TakePed :: takes 0 arguments" << endl;
    return 1;
  }

  else if(argc != 1){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }

  else{
    PSEC4_EVAL command;

    if(command.INITIALIZE() != 0){
      return 1;
    }
    
    command.SOFT_TRIG();
    command.Read(false);
    float ped_voltage = command.VBIAS * 1200/4096;
    //cout << ped_voltage << endl; //

    usleep(10000);

    command.GENERATE_PED(true);
    cout << "PSEC-4 ped data taken @ " << ped_voltage  << " mV\n";

    return 0;
  }
}
