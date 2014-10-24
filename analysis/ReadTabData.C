//This code reads PSEC4 data from an ASCII file (specified by the filename
//string), into a root tree (specified by the file_out string).
//Modified format - tab separated file
//Line 1 - Event 0 CH1 cell 0 voltage /t CH2 cell 0 voltage /t ..
//Line 2 - Event 0 CH1 cell 1 voltage /t CH2 cell 1 voltage /t ..
//...
//Line 256 - Event 0 CH1 cell 255 voltage /t CH2 cell 255 voltage /t ..
//Line 257 - Event 1 CH1 cell 0 voltage   /t CH2 cell 0 voltage /t ..
//...
//etc.
//
//also, skips data header info..to be extracted in a future version
//
//The output ROOT file contains a tree called "T" with branches for the 
//voltage and sample number.
//
//A typical use is as follows:
//root
//.L ReadTabData.C
//ReadTabData("input_file.txt","output_file.root")
//.q
//
//Then to plot, for example, the waveform for event 0
//root
//TFile *f1 = new TFile("output_file.root")
//T->Draw("Vx:sample","event == 0")

void ReadTabData(char *filename, char *file_out) {

	TFile *f = new TFile(file_out,"RECREATE");
	
	double V1[256];
	double V2[256];
	double V3[256];
	double V4[256];
	double V5[256];
	double V6[256];

	int sample[256], event = 0, wraparound = 0, ro_cnt = 0;

	TTree *T = new TTree("T","T");

	T->Branch("V1",V1,"V1[256]/D");
	T->Branch("V2",V2,"V2[256]/D");
	T->Branch("V3",V3,"V3[256]/D");
	T->Branch("V4",V4,"V4[256]/D");
	T->Branch("V5",V5,"V5[256]/D");
	T->Branch("V6",V6,"V6[256]/D");

	T->Branch("sample",sample,"sample[256]/I");
	T->Branch("wraparound", &wraparound, "wraparound/I");
	T->Branch("ro_cnt", &ro_cnt, "ro_cnt/I");
	T->Branch("event",&event,"event/I");
	
	//ifstream fin(filename);
	FILE* fin = fopen(filename, "r");
	if(fin == NULL){
	  cout << "could not open file" << endl;
	  return;
	}

	int N = 0;
        float this_V[6];
	
	cout << "printing read file header: " << endl;

	while (!feof(fin)) {
	  char buf[1000];

	  fgets(buf, 1000, fin);
	  
	  if(buf[0] == '#'){
	    cout << buf;
	    continue;
	  }

	  if(feof(fin))
	    continue;

	  int tmp;
	  sscanf(buf, "%f\t%f\t%f\t%f\t%f\t%f\t%d\n", 
		  &this_V[0], &this_V[1], &this_V[2], &this_V[3], &this_V[4], &this_V[5], &tmp);
       	  
	  V1[N] = this_V[0];
	  V2[N] = this_V[1];
	  V3[N] = this_V[2];
	  V4[N] = this_V[3];
	  V5[N] = this_V[4];
	  V6[N] = this_V[5];
	  if(N == 1) wraparound = tmp;
	  if(N == 2) ro_cnt = tmp;

	  sample[N] = N;
	  N = (N + 1);

	  if (N == 256) {
          //  cout << event << "  " << wraparound << endl;
	    T->Fill();
	    event++;
	    N %= 256;
	  }	
	}
	fclose(fin);

	f->Write();
}
