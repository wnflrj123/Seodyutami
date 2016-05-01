#ifndef TIMEPRINTER_H
#define TIMEPRINTER_H

#include <iostream>
#include <string>
#include <ctime>

class TimePrinter {
public:
	TimePrinter() : startTime(clock()), endTime(0) {}
	void start() {
		startTime = clock();
	}
	void end() {
		endTime = clock();
		print();
	}
	void print() {
		std::cout << (double)(endTime - startTime) / 1000 << " sec" << std::endl;
	}
private:
	clock_t startTime;
	clock_t endTime;

};

#endif