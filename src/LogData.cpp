//LogData.cpp
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
  
  char log_data_filename[200];

  if(argc == 2 && std::string(argv[1]) == "-h"){
    cout << endl;
    cout << "LogData :: is used to save PSEC4 data to a specified .txt file" << endl;
    cout << "LogData :: takes 3 arguments: filename [string]  number_readouts [unsigned int]  trig_mode [0=software, 1=external source]" << endl;
    return 1;
  }
  else if(argc != 4){
    cout << "error: wrong number of arguments" << endl;
    return -1;
  }

  else{
    PSEC4_EVAL command;

    sprintf(log_data_filename, "%s.txt", argv[1]);
    int num_readouts = 100;
    num_readouts = atoi(argv[2]);
    int trig_mode = atoi(argv[3]);
    int acq_rate_hertz = 1000; //atoi(argv[4]);
    int acq_rate_usecs = (int) (1000000 / acq_rate_hertz); 
    
    if(command.INITIALIZE() != 0){
      return 1;
    }

    //if(trig_mode != 0)
    //  command.USB_SYNC(true);
	  
    command.LOG_DATA(log_data_filename, num_readouts, trig_mode, acq_rate_usecs);

    return 0;
  }
}
