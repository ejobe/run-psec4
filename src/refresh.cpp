//refresh.cpp
//read firmware settings to software
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
    cout << "refresh :: prints some PSEC4 diagnostic info to screen" << endl;
    cout << "refresh :: takes 0 arguments" << endl;
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
      
    usleep(10000);
    
    //update config settings and display
    cout << " <:---------------------------:>\n";
    
    command.SOFT_TRIG();
    command.Read(false);   
    
    char timestring[100];
    
    time_t now;  
    
    time(&now);
    strftime(timestring, 80, " Local Time: %H:%M on %d-%m-%Y ", 
	     localtime(&now));
    
    float ro_rate_check = (float)command.RO_CNT * 10 * pow(2,11);
    ro_rate_check /= pow(10,6);
    
    float ro_cntrl_voltage = (float)command.PROVDD * 1200/4096;
    float ped_voltage = (float)command.VBIAS * 1200/4096;
    float trig_thresh = (float)command.TRIG_THRESHOLD * 1200/4096;
    bool sync_mode = command.READ_MODE_FIRMWARE & 0x1;
    bool sampling_speed = command.READ_MODE_FIRMWARE & 0x2;
    char last_sampling_bin = command.READ_MODE_FIRMWARE >> 2 ;
    bool trig_enable = command.TRIG_SETTINGS & 0x40;
    bool trig_sign   = command.TRIG_SETTINGS & 0x80;
    int trig_channel = command.TRIG_SETTINGS >> 8;

    cout << timestring << endl;
    cout << "...  Ring Oscillator Frequency: " << ro_rate_check << " MHz" << endl;
    cout << "...  Ring Osc. Control Voltage: " << ro_cntrl_voltage << " mV\n";
    cout << "...  Pedestal Voltage: " << ped_voltage << " mV" << endl;
    cout << "...  USB Sync Mode: " << sync_mode << endl;
    
    if(sampling_speed != 0)
      cout << "...  Sampling Speed Set: 5.12 GSPS" << endl;
    else
      cout << "...  Sampling Speed Set: 10.24 GSPS" << endl;
        
    if(trig_enable != 0)
      cout << "...  Internal Trigger Enabled?  YES, " << endl;
    else
      cout << "...  Internal Trigger Enabled?  NO!" << endl;
 
    if(trig_sign == true)
	cout << "...  :: pulse (-) polarity" << endl;
    else
	cout << "...  :: pulse (+) polarity" << endl;

    cout << "...  :: trigger on channel " << trig_channel << endl;
    cout << "...  :: internal trigger threshold: " << trig_thresh << " mV\n";

    cout << "...  Last Trigger Sample Bin: " << (int)last_sampling_bin << endl;
    cout << endl << "     (you've been refreshed)" << endl;

    return 0;
  }
}
