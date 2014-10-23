/////////////////////////////////////////////////////
//  file: PSEC4_EVAL_USB.cxx
//
//  USB interface definitions to PSEC-4 6-channel DAQ
//
//  Revision History:
//          01/2012 updated
//         
//  Author: ejo
//////////////////////////////////////////////////////
#include "stdUSB.h" 
using namespace std;

stdUSB in;

void PSEC4_EVAL::Read(bool ENABLE_TXT)
{
    int i,j;
    unsigned short buffer[BUFFERSIZE+2];
    memset(buffer,0x0,BUFFERSIZE*sizeof(unsigned short));
    int samples;
    //DWORD dw;
    GoodData = true;
    if(in.createHandles() == stdUSB::SUCCEED){
      try
    	{
	  in.readData(buffer,BUFFERSIZE, &samples);
	  if(ENABLE_TXT != false)
	    printf("Samples read %d\n",samples);                                                   
	  if(samples >= BUFFERSIZE){
	    for(i=0;i<BUFFERSIZE;i++){
	      lastGoodData[i] = buffer[i];
	    }
	  }
	  else
	    GoodData = false;
            
	  /**************************************/
	  if(ENABLE_TXT != false){
	    ofstream tempfile;	    
	    tempfile.open ("RAWREAD.txt",ios::trunc);
	    
	    for(i=2;i<BUFFERSIZE;i++){
	      tempfile << buffer[i] << " \n";
	      printf("%x, ", buffer[i]);
	    }
	    tempfile.close();
	  }
	  /*************************************/

	  for(i=0;i<CHAN;i++){   
	    for(j=0;j<256;j++){                  	
	      RAW[i][j] = buffer[2+256*i+j];
	    }
	  }  
	  
	  RO_CNT = buffer[1542];
	  PROVDD = buffer[1543];
	  VBIAS  = buffer[1544];
	  READ_MODE_FIRMWARE = buffer[1545];
	  TRIG_SETTINGS = buffer[1546];
	  TRIG_THRESHOLD = buffer[1547];
	    
	  in.freeHandles();
	}
	/*	catch (DWORD dw){
	 *		cout << "Errorcode %d\n";
	 *	}
	 */
      catch(...){
	cout << "Exception...probably a connection related issue.\n";
      }
    }
    else
      cout << "Please connect the board.\n";
}
/*
 *
 */
void PSEC4_EVAL::USB_SYNC(bool ENABLE)
{
    in.createHandles();
    if(ENABLE != false)
    {
        in.sendData((unsigned int)0x00010001);//enable USB_SYNC
    }
    else
    {
        in.sendData((unsigned int)0x00000001);//disable USB_SYNC
    }
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::SOFT_TRIG()
{
    in.createHandles();
    in.sendData((unsigned int)0x00000002);//software trigger
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::PED_SCAN(unsigned int ADC_VALUE)
{
    in.createHandles();
    const unsigned int mask = 0x00000003;
    unsigned int ManualData;
    ManualData = ADC_VALUE << 16 | mask;//DTRIG discrimator threshold
    in.sendData(ManualData);
    cout << "adjusting pedestal value.." << endl;
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::USB_DEBUG(unsigned int DEBUG)
{
    in.createHandles();
    const unsigned int mask = 0x000000FF;
    unsigned int ManualData;
    ManualData = DEBUG << 16 | mask;
    in.sendData(ManualData);
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::DLL_RESET()
{
    in.createHandles();
    in.sendData((unsigned int)0x00000004);//dll reset pulse
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::INT_TRIG_SETSIGN(bool signal_polarity)
{
    in.createHandles();
    const unsigned int mask = 0x6;
    unsigned int ManualData;
    ManualData = signal_polarity << 16 | mask;
    in.sendData(ManualData);
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::SET_SAMPLE_RATE(bool sample_bit)
{
    in.createHandles();
    const unsigned int mask = 0x7;
    unsigned int ManualData;
    ManualData = sample_bit << 16 | mask;
    in.sendData(ManualData);
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::INT_TRIG_ENABLE(bool enable_trig, unsigned int trig_mask)
{
    in.createHandles();
    const unsigned int mask = 0x8;
    unsigned int ManualData;
    ManualData = trig_mask << 17 | enable_trig << 16 | mask;
    in.sendData(ManualData);
    in.freeHandles();
}
/*
 *
 */
void PSEC4_EVAL::INT_TRIG_RESET()
{
    in.createHandles();
    in.sendData((unsigned int)0x5);
    in.freeHandles();
}
/*
 *
 */
int PSEC4_EVAL::CHECK_USB_CX()
{
    if(in.createHandles() != stdUSB::SUCCEED)
      return 1;
    else{
      in.freeHandles();
      return 0;
    }
}

int PSEC4_EVAL::CLOSE()
{
  in.freeHandle();
}
