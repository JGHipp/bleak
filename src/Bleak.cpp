/*
	Written by John Gustav Hippisley
	June-July 2020
*/

#include "Engine.hpp"
#include <algorithm> 
#include <vector>

int main(int argc, char* argv[])
{
	// Parse arguments
	if((argc - 1) % 2 != 0) error("Invalid command line arguments");  
	std::vector<std::string> options;
	std::vector<std::string> data;
	for(int i = 1; i < argc - 1; i += 2)
	{
		std::string option = std::string(argv[i]);
	    std::transform(option.begin(), option.end(), option.begin(), ::toupper); 
		options.push_back(option);
		data.push_back(std::string(argv[i + 1]));
	} 
	int nOptions = (int) options.size();

	int fps = 60; // Game runs at 60 FPS by default
	for(int i = 0; i < nOptions; i++)
	{
		if(options.at(i).compare("-F") == 0)  fps = atoi(data.at(i).c_str());
	}
	Engine::run(fps);
	return 0;
}
