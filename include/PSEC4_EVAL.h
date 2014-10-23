/////////////////////////////////////////////////////
//  file: PSEC4_EVAL.h
//
//  Header for USB PSEC-4 6-channel DAQ
//
//  Revision History:
//          01/2012 modified from existing psec4 eval 
//                  code for initial linux DAQ use at 
//                  APS laser stand
//         
//  Author: ejo
//////////////////////////////////////////////////////
#ifndef PSEC4_EVAL_H
#define PSEC4_EVAL_H

#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>

//Some Definitions:
/*****************/
//do NOT change:
#define BUFFERSIZE 1550
#define CHAN 6
/*****************/

class PSEC4_EVAL 
{
public:
  PSEC4_EVAL();
  ~PSEC4_EVAL()
    {
      delete[] lastGoodData;
    }

  void LOG_DATA(const char* filename, unsigned int NUM_READS, 
		int trig, int acq_rate);
  int  LOG_DATA_SCOPE(bool k, unsigned int num_frames, int trig, int scope);
  double GENERATE_PED(bool ENABLE_FILESAVE);
  void LOAD_PED();
  void LOAD_DNL_LUT();
  void Read(bool ENABLE_TXT);
  void USB_SYNC(bool ENABLE);
  void SOFT_TRIG();
  void DLL_RESET();
  void INT_TRIG_RESET();
  void INT_TRIG_SETSIGN(bool signal_polarity);
  void SET_SAMPLE_RATE(bool sample_bit);
  void INT_TRIG_ENABLE(bool enable_trig, unsigned int trig_mask);
  void PED_SCAN(unsigned int ADC_VALUE);
  void USB_DEBUG(unsigned int DEBUG);
  void DAC_SCAN();
  int  CHECK_USB_CX();
  int  CLOSE();
  int  INITIALIZE();
  int  UNWRAP();
  int* UNWRAP_BASELINE(bool ENABLE_SHIFT);
  
  int dac_level;
  //some board config values
  int VBIAS;
  int RO_CNT;
  int PROVDD;
  int READ_MODE_FIRMWARE;
  int TRIG_SETTINGS;
  int TRIG_THRESHOLD;
  
  
  //declare object of stdUSB class 
  //stdUSB in;

  ////////////////////

private:
  unsigned short *lastGoodData;
  bool GoodData;
  unsigned short buffer[BUFFERSIZE]; //readout buffers
  int RAW[CHAN][256];

  float DNL_LUT[4096][CHAN];
  float LUT[4096][CHAN];
  int PED_DATA[CHAN][256];
};

#endif
