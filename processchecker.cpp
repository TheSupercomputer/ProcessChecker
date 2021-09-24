#include "processchecker.hpp"

processchecker::processchecker() {
#if __linux__
	SoftwaresToCheck = {"/usr/bin/obs",
						"/usr/bin/vokoscreenNG",
						"/usr/bin/local/ffscreencast"
	};
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	SoftwaresToCheck = {"obs64.exe",
						"obs32.exe",
						"XSplit.Core.exe", //Might lead to false positives, needs testing.
						"XSplit.xbcbp.exe",
						"vokoscreenNG.exe"
	};

#endif
}

processchecker::processchecker(std::vector<std::string> identifiers) {
	SoftwaresToCheck = identifiers;
}

bool processchecker::is_running() {
	bool found = false;
	RunningSoftwares.clear();
#if __linux__
	auto user_id = getuid();
	std::ifstream cmdgetter;
	std::filesystem::path processes("/proc");
	std::string command;

	for (auto const& dir_entry : std::filesystem::directory_iterator(processes)) {
		if (dir_entry.is_directory()) {
			struct stat info;
			stat(dir_entry.path().c_str(), &info);
			if (info.st_uid == user_id) {
				cmdgetter.open(dir_entry.path().string() + "/cmdline", std::ios::binary);
				cmdgetter >> command;
				cmdgetter.close();
				if (auto pos = command.find('\0'); pos != std::string::npos)
					command.resize(pos);

				for (auto& entry : SoftwaresToCheck) {
					if (command == entry) {
						found = true;
						RunningSoftwares.push_back(entry);
					}
				}
				command.clear();
			}
		}
	}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), & cbNeeded))
		throw std::runtime_error("Can't enumerate processes");

	cProcesses = cbNeeded / sizeof(DWORD);

	for (auto& process : aProcesses) {
		if (process) {
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process);
			if (hProcess) {
				HMODULE hMod;

#if _WIN64
				if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
#else
				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
#endif
				for (auto& entry : SoftwaresToCheck) {
					std::wostringstream wstm;
					const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
					for (auto character : entry)
						wstm << ctfacet.widen(character);

					if (!_tcscmp(wstm.str().c_str(), szProcessName)) {
						found = true;
						RunningSoftwares.push_back(entry);
					}
				}
			}
		}
	}
#endif
	return found;
}

bool processchecker::is_running(std::string identifier) {
	std::vector<std::string> buffer = SoftwaresToCheck;
	SoftwaresToCheck.clear();
	SoftwaresToCheck.push_back(identifier);
	bool found = is_running();
	SoftwaresToCheck = buffer;
	return found;
}

bool processchecker::is_running(std::vector<std::string> identifiers) {
	std::vector<std::string> buffer = SoftwaresToCheck;
	SoftwaresToCheck = identifiers;
	bool found = is_running();
	SoftwaresToCheck = buffer;
	return found;
}

std::vector<std::string> processchecker::what_software() {
	return RunningSoftwares;
}

void processchecker::add_software(std::string identifier) {
	SoftwaresToCheck.push_back(identifier);
}