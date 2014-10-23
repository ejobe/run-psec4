/////////////////////////////////////////////////////
//  file: PSEC4_EVAL.cxx
//
//  DAQ Definitions for USB PSEC-4 6-channel board
//
//  Revision History:
//          01/2012 modified from existing psec4 eval 
//                  code for initial linux DAQ use at 
//                  APS laser stand
//         
//  Author: ejo
//////////////////////////////////////////////////////
/****************************************************/
//////////////////////////////////////////////////////
//			 USB tester
//			 ¯¯¯¯¯¯¯¯¯¯
//TESTING ONE, TWO, THREE.
//////////////////////////////////////////////////////

#include <fstream>
#include <string.h>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "PSEC4_EVAL.h"
#include "PSEC4_EVAL_USB.cxx"
#include "PSEC4_EVAL_VCALS.cxx"
#include "ScopePipe.h"

using namespace std;

PSEC4_EVAL::PSEC4_EVAL()
{
    GoodData = false;
    lastGoodData = new unsigned short[BUFFERSIZE];
}

void PSEC4_EVAL::LOG_DATA(const char* log_filename, unsigned int NUM_READS, 
			  int trig_mode, int acq_rate)
{
  LOAD_DNL_LUT(); //load linearity correction
  LOAD_PED();     //load pedestal

  unsigned int i,j,k;
  float sample;
  float rundat[CHAN][256];
  int EVENT_META_DATA[4];
  int check_event;
  float DNL_temp;
  
  FILE* fdatout = fopen(log_filename, "w");
    
  char timestring[80];
  time_t now;
  time(&now);
  strftime(timestring, 80, "%Y-%m-%d %H:%M:%S", localtime(&now));

  /*****************************/
  //add some data header info: RUN_META_DATA
  fprintf(fdatout, "######################################\n");
  fprintf(fdatout, "#  %s\n", log_filename);
  fprintf(fdatout, "#  %s\n", timestring);  
  fprintf(fdatout, "#  %i\n", NUM_READS); 
  /*****************************/
  
  int* baseline = new int[256];
  
  for(k=0;k<NUM_READS; k++){
    check_event = 0;
        
    if(trig_mode == 0) 
      SOFT_TRIG();//for debugging
    if(trig_mode == 1){
      INT_TRIG_RESET();
    }      
    
    usleep(acq_rate);
    Read(false);
              
    for(i=0;i<CHAN; i++){
      for(j=0;j<256; j++){
	
	if (i==0 && j < 10) 
	  check_event += (int)RAW[i][j];
	
	//sample = (float)RAW[i][j]; //uncommment to check stream	
	sample =  DNL_LUT[(int)RAW[i][j]][i];
	sample -= DNL_LUT[(int)PED_DATA[i][j]][i];                        
	rundat[i][j] = sample;      

      }
    }

    if(check_event == 0){
      k--;
      continue;
    }
    
    EVENT_META_DATA[0] = k+1;
    EVENT_META_DATA[1] = UNWRAP();	
    EVENT_META_DATA[2] = RO_CNT;
    EVENT_META_DATA[3] = PROVDD;	
    //baseline = UNWRAP_BASELINE(true);  //unwrap baseline

    for(int log=0; log<256; log++){
      if(log < 4){
      	fprintf(fdatout, "%f\t%f\t%f\t%f\t%f\t%f\t%d\n",
	      rundat[0][log], rundat[1][log],
	      rundat[2][log], rundat[3][log],
	      rundat[4][log], rundat[5][log],
	      EVENT_META_DATA[log]);     
      }
      else{
      	fprintf(fdatout, "%f\t%f\t%f\t%f\t%f\t%f\n",
	      rundat[0][log], rundat[1][log],
	      rundat[2][log], rundat[3][log],
	      rundat[4][log], rundat[5][log]); 
      }     
    }
  
  
    if((k+1) % 2 == 0){
      cout << "Readout:  " << k+1 << " of " << NUM_READS << "      \r";
      cout.flush();
    }

  }
  cout << "Readout:  " << NUM_READS << " of " << NUM_READS << "  ........Finished Data Run...      \r";
  cout << endl;
  fclose(fdatout);
  cout << "Data saved in file: "<< log_filename <<"\n\n";
}
/*
 *
 */
int PSEC4_EVAL::LOG_DATA_SCOPE(bool k, unsigned int num_frames, int trig_mode, int scope_mode)
{
    LOAD_DNL_LUT();
    LOAD_PED();        
    unsigned int i,j;
    unsigned int count = 0; 
    float sample;
    int check_event;
    float pipedat[CHAN][256];
    float DNL_temp;
    
    ScopePipe mypipe;
    mypipe.init();
    usleep(1000000);
 
    int* baseline = new int[256];

    while(k != 0){

        count++;
         
        if(trig_mode == 0){
           SOFT_TRIG(); //for debugging
	    //cout << "software trigger sent to board" << endl;
	}
	if(trig_mode == 1){
	  INT_TRIG_RESET();
	}
	  
	//usleep(2000);
        Read(false);
        check_event = 0;
              
        for(i=0;i<CHAN; i++){
	  //cout << i << endl;
	  for(j=0;j<256; j++){
	    if(CHAN == 0) baseline[j] = j;
	    //check for null readout
	    if (i==0 && j < 10) 
	      check_event += (int)RAW[i][j];
	    /********************************/
                    
	    sample =  DNL_LUT[(int)RAW[i][j]][i]*1000; 
	    sample -= DNL_LUT[(int)PED_DATA[i][j]][i]*1000; 
	    //sample = RAW[i][j] - PED_DATA[i][j]; //bypass LUT
	    pipedat[i][j] = sample;
	    //pipedat[i][j] = RAW[i][j];  //for debugging
            }
        }
	 
	if(check_event == 0){
          mypipe.notrigger();
	  usleep(250000);
	  cout << "no event: " << count << endl;
	  
	  if(count == num_frames){
	    cout << "exiting scope" << endl;
	    break;
	      }
	  else{
	    continue;
	  }
	}

	baseline = UNWRAP_BASELINE(true);
	//cout << baseline[0] << endl;
       
        //send scope data to file to be read by gnuplot 
        FILE* foutdat = fopen("scope.dat", "w");       
	for(int g=0;g<256;g++){        
            fprintf(foutdat, "%d\t%f\t%f\t%f\t%f\t%f\t%f\t\n",g,
		    pipedat[0][baseline[g]],pipedat[1][baseline[g]],
		    pipedat[2][baseline[g]],pipedat[3][baseline[g]],
		    pipedat[4][baseline[g]],pipedat[5][baseline[g]]);
        }
        //fflush(foutdat);
	fclose(foutdat);  
	usleep(10000);
	
	if(scope_mode == 1)
	   mypipe.multiplot();
	else
	   mypipe.plot();
	
	//(sort-of) arbitrary scope refresh rate..presumably cpu limited
	//hard coded to 7.5 Hz -- for most cpus, but: 
	//actually slowed wayyy down for APS netbook t=270 ms
	usleep(200000);   

	//take several frames,then ask to continue? 
	//?need better way to quit 'scope'
        if(count == num_frames){
	  // cout << " to continue scoping enter: 1\n to quit enter: 0\n>>";
	  //  cin >> k;
	  //  if(k <= 0 || k > 1)
	  
	  //hold last scope shot for a few seconds before closing:
	  usleep(7000000);
	  break;
        }            
    }
    mypipe.finish();
    return 0;
}
/*
 *
 */
int PSEC4_EVAL::UNWRAP()
{
  bool sampling_speed = READ_MODE_FIRMWARE & 0x2;
  unsigned int last_sampling_bin = READ_MODE_FIRMWARE >> 2;
  unsigned int BIN_20[8];
  unsigned int BIN_40[4];
  
  //cout << "LSB " << last_sampling_bin << endl;

  for(int k=0; k<8; k++){
    BIN_20[k] = (int)256/8*k;
    if(k < 4)
    BIN_40[k] = (int)256/4*k;
  }

  int wraparound_loc = 0;
  
  if(sampling_speed){
//     if(last_sampling_bin == 0)
//       wraparound_loc = BIN_20[15];
//     else
      wraparound_loc = BIN_20[last_sampling_bin];
  }
  else if(!sampling_speed){
//     if(last_sampling_bin ==  0)
//       wraparound_loc = BIN_40[6];
//     else if(last_sampling_bin ==  1)
//       wraparound_loc = BIN_40[7];
//     else
      wraparound_loc = BIN_40[last_sampling_bin];
  }
  else{
    wraparound_loc = 0;
  }

  //cout << wraparound_loc << endl;
  return wraparound_loc;
}
/*
 *
 */
int* PSEC4_EVAL::UNWRAP_BASELINE(bool enable)
{
  int* shifted_baseline = new int[256];
  for(int i = 0; i<256; i++){
    shifted_baseline[i] = i;
  }

  int wraparound = UNWRAP();
  if(enable == true)
    rotate(shifted_baseline, shifted_baseline+wraparound, &shifted_baseline[256]);
 
  //  for(int i = 0; i<256; i++)
  //  cout << shifted_baseline[i] << endl;
  
  return shifted_baseline;
}
/*
 *
 */
int PSEC4_EVAL::INITIALIZE()
{
  if(CHECK_USB_CX() != 0){
    cout << "\n---> PSEC-4 evaluation board not "; 
    cout << "connected...\n\n";
    usleep(1000);
    return 1;
  }

  bool check_usb_working = false;
  int ro_rate = 0;
  int num_checks = 0;
  bool SYNC_MODE_FIRMWARE = 1;
  USB_SYNC(false);
  usleep(1000);
  USB_SYNC(false);

  while(!check_usb_working){
    num_checks++;
    SOFT_TRIG();
    Read(false);
    ro_rate = RO_CNT;
    SYNC_MODE_FIRMWARE = READ_MODE_FIRMWARE & 0x1;
    cout << "checking connection...." << endl;
    if(num_checks > 10){
      cout << "something's wrong with the USB connection. Please check" << endl;
      return 1;
    }
    if(ro_rate > 0 && SYNC_MODE_FIRMWARE == 0)
      check_usb_working = true;
      
    
  }
  return 0;
}
