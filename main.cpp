#include "processchecker.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main() {
	processchecker Broadcast;
	
	while(true) {
		if(Broadcast.is_running()) {
			std::cout << "FOUND:";
			for(auto &e : Broadcast.what_software())
				std::cout << " " << e;
			std::cout << '\n';
		}
		else
			std::cout << "BUBU\n";
		std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::seconds(1));
	}
}
