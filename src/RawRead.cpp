//RawRead.cpp
//ejo 2012

#include "PSEC4_EVAL.h"
//#include "PSEC4_EVAL_USB.cxx"
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv){
  if(argc == 2 && std::string(argv[1]) == "-h" ){
    cout << endl;
    cout << "RawRead :: dumps raw event to screen and RAWREAD.txt" << endl;
    cout << "RawRead :: takes 0 arguments" << endl;
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
    command.Read(true);
    cout << "\n sent to file: RAWREAD.txt \n\n";
    
    command.CLOSE();
    return 0;
  }
}
