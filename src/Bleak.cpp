/*
	Written by John Gustav Hippisley
	June-July 2020
*/

#include "Engine.hpp"

int main(int argc, char* argv[])
{
	Engine engine(60); // Game runs at 60 FPS
	engine.run();
	return 0;
}
