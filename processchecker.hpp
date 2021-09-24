#ifndef PROCESSCHECKER_HPP
#define PROCESSCHECKER_HPP
#include <vector>
#include <string>

#if __linux__
#include <filesystem>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <cstring>
#include <sstream>

// Might be needed on some Versions, with a Visual Studio 2019 installation it wasn't
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
#else
#error "Unsupported platform / compiler"
#endif

class processchecker {
public:
	processchecker();	// Default identifiers are common broadcaster softwares
	processchecker(std::vector<std::string> identifiers);

	bool						is_running();
	bool						is_running(std::string identifier);
	bool 						is_running(std::vector<std::string> identifiers);
	std::vector<std::string>	what_software();

	void 						add_software(std::string identifier);
private:
	std::vector<std::string>	SoftwaresToCheck;
	std::vector<std::string>	RunningSoftwares;
};

#endif