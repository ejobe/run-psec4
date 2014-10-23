//////////////////////////////////////////////////////
//  file: ScopePipe.cpp
//
//  Definitions for ScopePipe c++ class
//
//  Revision History:
//          01/2012 original version 
//               (created 'self-standing' code, 
//                to separate from main program)
//  Author: ejo
////////////////////////////////////////////////////// 
#include <cmath>
#include <sstream>
using std::ostringstream;

#include "ScopePipe.h" 

ScopePipe::ScopePipe()
{
  //assign default values..
  gp_cmd = 0;
  filename = "scope.dat";

}

ScopePipe::~ScopePipe()
{
  // need proper destructor at some point..
}

int ScopePipe::init()
{  
  
  gp_cmd = popen("/usr/bin/gnuplot -background black " 
  		 "-xrm \'gnuplot*borderColor:white\' "
  		 , "w"); // open gnuplot
  //gp_cmd = popen("/usr/bin/gnuplot","w");
  if(!gp_cmd){
    std::cout << "scope failed to initialize!" << std::endl;
    return (1);
  }
  else{
    std::cout << "opening gnuplot..." << std::endl;
  }
  //time to let gnuplot window open
  usleep(10000);

  //set up the  graph
  //send_cmd("set terminal x11");
  send_cmd("set timestamp");
  send_cmd("set object 1 rect from screen 0, screen 0 to screen 1, screen 1 behind");
  send_cmd("set object 1 rect fc rgb \'black\'");
  send_cmd("set pointsize .2");

  send_cmd("set autoscale y");
  send_cmd("set xrange [0:300]");

  send_cmd("set style line 1 lt 0 lw 0.5 pt 1 linecolor rgb \'white\' ");
  send_cmd("set style line 10 lt 1 lw 1.5 pt 1 linecolor rgb \'white\' ");
  send_cmd("set style line 2 lt 1 lw 1 pt 1 linecolor rgb \'red\' ");
  send_cmd("set style line 3 lt 1 lw 1 pt 1 linecolor rgb \'yellow\' ");
  send_cmd("set style line 4 lt 1 lw 1 pt 1 linecolor rgb \'cyan\' ");
  send_cmd("set style line 5 lt 1 lw 1 pt 1 linecolor rgb \'magenta\' ");
  send_cmd("set style line 6 lt 1 lw 1 pt 2 linecolor rgb \'green\' ");
  send_cmd("set style line 7 lt 1 lw 1 pt 1 linecolor rgb \'coral\' ");
 
  send_cmd("set grid ls 1");
  send_cmd("set border ls 10");
  send_cmd("plot sin(x)");
 
  send_cmd("set key tc rgb \'white\'");
  //send_cmd("set xlabel \'Sample Number\' ");
  //send_cmd("set ylabel \'voltage [mV]\' ");


  return (0);
}

int ScopePipe::notrigger()
{
  send_cmd("set term x11 size 1000,480 position 0,0");
  send_cmd("plot 0 title \'NO TRIGGER' \n"); 
  return (0);	
}

int ScopePipe::plot()
{
  //check that plot file was successfully written:
  FILE* fcheck = fopen(filename.c_str(), "r");
  if (fcheck == NULL){
    std::cout << "no data to for scope!" << std::endl;
    return (1);
  }
        
  //set up plotting
  ostringstream cmd_stream;
  send_cmd("set term x11 size 800,380 position 0,0");
  cmd_stream.str("");
  cmd_stream << "plot \"" << filename
	     << "\" using ($1):2 smooth csplines ls 2 title \'CHAN 1\',\""
	     << filename
	     << "\" using ($1):3 smooth csplines ls 3 title \'CHAN 2\',\""
	     << filename
	     << "\" using ($1):4 smooth csplines ls 4 title \'CHAN 3\',\""
	     << filename
	     << "\" using ($1):5 smooth csplines ls 5 title \'CHAN 4\',\""
	     << filename
	     << "\" using ($1):6 smooth csplines ls 6 title \'CHAN 5\',\""
	     << filename
	     << "\" using ($1):7 smooth csplines ls 7 title \'CHAN 6\'"; 
  plot_cmd = cmd_stream.str();
  send_cmd(plot_cmd);
  
  //std::cout << "data plotted"<< std::endl;  
  
  //fflush(gp_cmd);
  
  return (0);
}

int ScopePipe::multiplot()
{
  //check that plot file was successfully written:
  FILE* fcheck = fopen(filename.c_str(), "r");
  if (fcheck == NULL){
    std::cout << "no data to for scope!" << std::endl;
    return (1);
  }

  ostringstream cmd_stream_1, cmd_stream_2;
  
  cmd_stream_1.str("");
  cmd_stream_1 << "plot \"" << filename
	     << "\" using ($1):2 smooth csplines ls 2 title \'Trigger\'";
  cmd_stream_2.str("");
  cmd_stream_2 << "plot \"" << filename
	     << "\" using ($1):3 smooth csplines ls 3 title \'CHAN 2\',\""
	     << filename
	     << "\" using ($1):4 smooth csplines ls 4 title \'CHAN 3\',\""
	     << filename
	     << "\" using ($1):5 smooth csplines ls 5 title \'CHAN 4\',\""
	     << filename
	     << "\" using ($1):6 smooth csplines ls 6 title \'CHAN 5\',\""
	     << filename
	     << "\" using ($1):7 smooth csplines ls 7 title \'CHAN 6\'"; 
 
 send_cmd("set term x11 1 size 800,200 position 0,0" );
 plot_cmd = cmd_stream_1.str();
 send_cmd(plot_cmd);
 
 send_cmd("set term x11 2 size 800,380 position 0,250"); 
 plot_cmd = cmd_stream_2.str();
 send_cmd(plot_cmd);

  //replosend_cmd("unset multiplot");
  
  //std::cout << "data plotted"<< std::endl;  
  
  //fflush(gp_cmd);
  
  return (0);
}

int ScopePipe::send_cmd(const string &plot_cmd)
{
  ostringstream cmd_stream;
  cmd_stream << plot_cmd << std::endl;
  //std::cout << cmd_stream.str() << std::endl;

  fprintf(gp_cmd, "%s", cmd_stream.str().c_str());   
  fflush(gp_cmd);
  
  return (0);
}

int ScopePipe::finish()
{
  send_cmd("quit");
  pclose(gp_cmd);

  return (0);
}
  
  
