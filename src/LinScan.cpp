//LinScan.cpp
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
    cout << "LinScan :: generates linearity correction and count-to-voltage conversion LUT for PSEC4 ADC" << endl;
    cout << "LinScan :: takes 0 arguments" << endl;
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
    
    command.DAC_SCAN();
    //level = command.dac_level;

    return 0;
  }
}
