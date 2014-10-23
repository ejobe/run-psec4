/////////////////////////////////////////////////////
//  file: PSEC4_EVAL_VCALS.cxx
//
//  PSEC-4 calibration definitions 
//  pedestal & linearity -- perform & load voltage cals
//
//  Revision History:
//          01/2012 original verison
//          04/2012 changed to median pedestal subtraction
//         
//  Author: ejo
//////////////////////////////////////////////////////
#include <stdio.h>

using namespace std;
double PSEC4_EVAL::GENERATE_PED(bool ENABLE_FILESAVE)
{
    unsigned int i,j,k;
    const unsigned int num_reads = 120;
    //unsigned int raw_array[CHAN][256];   //for mean subtraction
    unsigned int raw_array[CHAN][256][num_reads];  //for median subtraction
    double average_RO_cnt = 0;
    /*
    for(i=0;i<CHAN; i++){
        for(j=0;j<256; j++){
            raw_array[i][j]=0;            
        }
    } 
    */
    cout << "Start pedestal readout......\n";
    
    for(k=0;k<num_reads; k++){
      
      //printf("PED_READOUT: %d of %d\n",k+1,num_reads);
      SOFT_TRIG(); 
               
      Read(false);       
      for(i=0;i<6; i++){
	for(j=0;j<256; j++){
	  raw_array[i][j][k] = RAW[i][j];
	}
      }    
    
      usleep(5000);
    }
    /* mean subtraction*/
    /*
    for(i=0;i<CHAN; i++){
      for(j=0;j<256; j++){                              
	PED_DATA[i][j] = raw_array[i][j]/num_reads;
      }
    }
    */

    /* median subtraction */
    
    int w = int(num_reads/2);
    //float raw_temp[256][num_reads];
    for (i=0;i<CHAN; i++){
      for(j=0; j<256; j++){
	sort(raw_array[i][j], raw_array[i][j]+num_reads);
	PED_DATA[i][j] = (int) raw_array[i][j][w];
      }
    }
    
    average_RO_cnt += RO_CNT;
    cout << "......Finished pedestal readout\n";
    
    if(ENABLE_FILESAVE == true){
      FILE* fped = fopen("cal_data/PED_DATA.txt", "w");

      for(int i=0; i < 256; i++){
	//printf("%d:%d, ", i, PED_DATA[0][i]);
	fprintf(fped, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t\n", 
		i,PED_DATA[0][i],PED_DATA[1][i],PED_DATA[2][i],
		PED_DATA[3][i],PED_DATA[4][i],PED_DATA[5][i]);
      }
      fclose(fped);
      cout << "Pedestal values saved to file: PED_DATA.txt\n\n";
    }
    
    average_RO_cnt /= num_reads;
    return average_RO_cnt;
}

void PSEC4_EVAL::LOAD_PED()
{
    FILE* fped_in = fopen("cal_data/PED_DATA.txt", "r");
    if (fped_in == NULL)
        return;
    
    int rows=0;
    
    while (!feof(fped_in)) {
        int i;
        char buf[1000];
        
        fgets(buf, 1000, fped_in);
        
        if (feof(fped_in))
            continue;
            
        if (count(buf, buf + strlen(buf), '\t') < CHAN)
            continue;  // not 5-column or more
        
        sscanf(buf, "%d\t", &i);
        
        int tmp;
        sscanf(buf, "%d\t%i\t%i\t%i\t%i\t%i\t%i\t\n", 
	       &tmp, &PED_DATA[0][i], &PED_DATA[1][i], 
	       &PED_DATA[2][i], &PED_DATA[3][i], 
	       &PED_DATA[4][i], &PED_DATA[5][i]);
	
	//cout << PED_DATA[0][i] << endl;
        
        rows++;
    }
    fclose(fped_in);
    cout << "Pedestal loaded from file...\n";
}
/*
 *
 */
void PSEC4_EVAL::LOAD_DNL_LUT() 
{
    FILE* fin = fopen("cal_data/LUT.txt", "r");
    if (fin == NULL)
        return;
    
    int rows=0;
    
    while (!feof(fin)) {
        int i;
        char buf[1000];
        
        fgets(buf, 1000, fin);
        
        if (feof(fin))
            continue;
            
        if (count(buf, buf + strlen(buf), '\t') < CHAN)
            continue;  // not 5-column or more
        
        sscanf(buf, "%d\t", &i);
        
        int tmp;
        sscanf(buf, "%i\t%f\t%f\t%f\t%f\t%f\t%f\t\n", 
	       &tmp, &DNL_LUT[i][0], &DNL_LUT[i][1], 
	       &DNL_LUT[i][2], &DNL_LUT[i][3], &DNL_LUT[i][4], 
	       &DNL_LUT[i][5]);
        
        rows++;
    }
    fclose(fin);
    cout << "Linearity LUT loaded from file...\n";
}   
/*
 *
 */
void PSEC4_EVAL::DAC_SCAN()
{
  bool SAVE_ALL = true;
    FILE* fout = fopen("cal_data/LUT.txt", "w");
    ofstream fout2;
    fout2.open("cal_data/DAC_SCAN.txt", ios::trunc);
    //if(SAVE_ALL){
    ofstream fout3, fout4, fout5, fout6, fout7, fout8;
    fout3.open("cal_data/cell_scan_1.txt", ios::trunc);
    fout4.open("cal_data/cell_scan_2.txt", ios::trunc);
    fout5.open("cal_data/cell_scan_3.txt", ios::trunc);
    fout6.open("cal_data/cell_scan_4.txt", ios::trunc);
    fout7.open("cal_data/cell_scan_5.txt", ios::trunc);
    fout8.open("cal_data/cell_scan_6.txt", ios::trunc);
    //}     
    //float oldLUT[4096][CHAN];
    //int oldPed[CHAN*256];
    //memcpy(oldLUT, LUT, sizeof(LUT) );
    //memcpy(oldPed, PED_DATA, sizeof(PED_DATA) );
    double temp[CHAN];
    int temp_cell[256];

    for(int i=0; i < 4096; i++)
        LUT[i][0] = LUT[i][1] = LUT[i][2] = 
	  LUT[i][3] = LUT[i][4] = LUT[i][5] = -1;
    
    //dac control
    for(int i=10; i<4002; i+=20){       
        for(int k=0;k<CHAN;k++){
            temp[k] = 0.0;
        }
	for(int k=0; k<256; k++){
	  temp_cell[k] = 0;
	}

        fout2 << i <<   "\t";
	if(SAVE_ALL){
	  fout3 << i <<   "\t";
	  fout4 << i <<   "\t";
	  fout5 << i <<   "\t";
	  fout6 << i <<   "\t";
	  fout7 << i <<   "\t";
	  fout8 << i <<   "\t";
	}
        dac_level = i;
        float voltage = i * 1.200/4096.0;
        
	PED_SCAN(i);
        printf("waiting .....setting pedestal = %d \n", i);
        usleep(1800000); //allow DAC voltage to settle
        
	double avg_cnt = GENERATE_PED(false);
        
        for(int m=0;m<CHAN; m++){
            for(int j=0;j<256; j++){
                temp[m] += PED_DATA[m][j];
		if(SAVE_ALL){
		  if(m == 0) fout3 << PED_DATA[m][j] << "\t"; 
		  if(m == 1) fout4 << PED_DATA[m][j] << "\t"; 
		  if(m == 2) fout5 << PED_DATA[m][j] << "\t"; 
		  if(m == 3) fout6 << PED_DATA[m][j] << "\t"; 
		  if(m == 4) fout7 << PED_DATA[m][j] << "\t"; 
		  if(m == 5) fout8 << PED_DATA[m][j] << "\t"; 
		}
            }
            temp[m] /= 256;
            fout2<< temp[m] <<  "\t";

            if (temp[m] > 0 && temp[m] < 4096)
	      LUT[(int)temp[m]][m] = voltage;  
                     
        }
	if(SAVE_ALL){
	  fout3 << "\n";
	  fout4 << "\n";
	  fout5 << "\n";
	  fout6 << "\n";
	  fout7 << "\n";
	  fout8 << "\n";
	}
        //fout3 << avg_cnt << "\n";
	fout2 << avg_cnt <<"\n";
	
    }
    fout2.close();
    if(SAVE_ALL){
      fout3.close();
      fout4.close();
      fout5.close();
      fout6.close();
      fout7.close();
      fout8.close();
    }
    //////////////////////////
    /***/
    // do linear interpolations
    int first, second, j;
    for(int i=0; i < 6; i++) {  // for each channel
        // find first value
        for(j=0; LUT[j][i] < 0; j++);  
	// don't test equality with floating point #s
        
        first = j;  // first is now the first LUT value
        
        while (j < 4096) {
            for(j++; LUT[j][i] < 0 && j < 4096; j++);
            
            if (j >= 4096) continue;
            
            second = j;
            
            for(int k=first+1; k < second; k++) {
                LUT[k][i] = LUT[first][i] + 
		  (float)(k-first)/(second-first) * 
		  (LUT[second][i] - LUT[first][i] );
            }

            first = second;
        }
    }

    for(int i=0; i < 4096; i++)
        fprintf(fout, "%d\t%f\t%f\t%f\t%f\t%f\t%f\t\n", 
		i,LUT[i][0],LUT[i][1],LUT[i][2],
		LUT[i][3],LUT[i][4],LUT[i][5]);
    
    fclose(fout);       
}         
