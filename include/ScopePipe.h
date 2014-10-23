/////////////////////////////////////////////////////
//  file: ScopePipe.h
//
//  Header for ScopePipe c++ class
//
//  Revision History:
//          01/2012 original version 
//               (created 'self-standing' code, 
//                to separate from main program)
//  Author: ejo
////////////////////////////////////////////////////// 
#ifndef SCOPEPIPE_H
#define SCOPEPIPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using std::string;

class ScopePipe
{
 public:
  ScopePipe();
  ~ScopePipe();

  int init();
  int notrigger();
  int plot();
  int multiplot();
  int send_cmd(const string &plot_cmd);
  int finish();

 private:
  FILE *gp_cmd;
  string filename;
  string plot_cmd;
};

#endif
