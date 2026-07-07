#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include <fstream>
#include <iostream>

class ConfigReader
{
public:
   const std::string leerConfig(const std::string& configIni); 
};

#endif