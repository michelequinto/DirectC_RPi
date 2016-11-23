//
// Filename            : DirectC_RPi
// Description         : Porting of Microsemi DirectC code to RaspberryPi 3
//                       works on any other ARM based platform running Linux
//                       JTAG lines are driven using GPIOs
//  
// Author              : Michele Quinto
// Created On          : Nov 23 15:40:00 2016

//----------------------------------------------------------------
// Copyright 2016 Michele Quinto

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//----------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <map>
#include <iomanip>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "dpuser.hh"

void print_menu();

int main(int ac, char** av) {

  std::string fname, action;
  float jtag_freq;
  
  //Declare the supported options.
  po::options_description opts("DirectC_RPi options");
  opts.add_options()
    ("help", "Produce this help message")
    ("ifile", po::value<std::string>(&fname)->default_value(""), "The input programming file name in .dat format")
    ("freq", po::value<float>(&jtag_freq)->default_value(1.), "The JTAG TCK frequency in the range [0.1 to 50] kHz")
    ;
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, opts), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    cout << opts << "\n";
    exit(0);
  }
  // END cmdLine parser

 

  std::ifstream is (fname.c_str(), std::ifstream::binary);
  if (!is) {
    cout << "[ERROR] Invalid file exiting..." << endl;
    return -1;
  }
  is.seekg (0, is.end);
  int length = is.tellg();
  is.seekg (0, is.beg);
  
  char * buffer = new char [length];
  std::cout << "[INFO] Reading " << std::dec << length << " bytes. " << std::endl;
  is.read (buffer,length);
  
  if (is)
    std::cout << "[INFO] Read successfully in buffer." << std::endl;
  else
    std::cout << "[ERROR] Only " << is.gcount() << "bytes could be read." << std::endl;
    
  //-------------------                                                                                
  image_buffer = (DPUCHAR*)buffer; //init direct C buffer
  //-------------------       
  
  print_menu();

  while( cin >> Action_code ){
    dp_top();
    std::cout << "\n[INFO] Exit code: " << static_cast<int>(error_code) << std::endl;
    print_menu();
  }

  // close and cleanup
  delete[] buffer;
  is.close();

  exit(0);
} 

void print_menu(){
  typedef std::map<uint, std::string> action_mapT;
  static action_mapT action_map;
  action_map[DP_DEVICE_INFO_ACTION_CODE] = "device_info";
  action_map[DP_READ_IDCODE_ACTION_CODE] = "read_id";
  action_map[DP_ERASE_ACTION_CODE] = "erase";
  action_map[DP_PROGRAM_ACTION_CODE] = "program";
  action_map[DP_VERIFY_ACTION_CODE] = "verify";
  /* Array only actions */
  action_map[DP_ENC_DATA_AUTHENTICATION_ACTION_CODE] = "enc_data_auth";
  action_map[DP_ERASE_ARRAY_ACTION_CODE] = "erase_array";
  action_map[DP_PROGRAM_ARRAY_ACTION_CODE] = "progam_array";
  action_map[DP_VERIFY_ARRAY_ACTION_CODE] = "verify_array";
  /* FROM only actions */
  action_map[DP_ERASE_FROM_ACTION_CODE] = "erase_from_action";
  action_map[DP_PROGRAM_FROM_ACTION_CODE] = "program_from_action";
  action_map[DP_VERIFY_FROM_ACTION_CODE] = "verify_from_action";
  /* Security only actions */
  action_map[DP_ERASE_SECURITY_ACTION_CODE] = "erase_security";
  action_map[DP_PROGRAM_SECURITY_ACTION_CODE] = "program_security";
  /* NVM only actions */
  action_map[DP_PROGRAM_NVM_ACTION_CODE] = "program_nvm";
  action_map[DP_VERIFY_NVM_ACTION_CODE] = "verify_nvm";
  action_map[DP_VERIFY_DEVICE_INFO_ACTION_CODE] = "verify_device_info";
  action_map[DP_READ_USERCODE_ACTION_CODE] = "read_usercode";

  for( action_mapT::const_iterator it = action_map.begin();
       it != action_map.end(); ++it )
    cout << std::setw(2) << std::left << it->first << std::right << " - " << it->second << endl;
  cout << "** Please select an action code by entering the corresponding number **" << endl;
}
