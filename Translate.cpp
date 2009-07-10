//Translate.cpp: process name and data files

#include "io/Parser.hpp"
#include "io/UnorderedStreaming.hpp"
#include "algorithm/ZCTASegment.hpp"
#include <boost/unordered_set.hpp>
#include <iostream>
#include <fstream>

#define EXPECTED_ARGC 4
#define NAME_FILE_INDEX 1
#define DATA_FILE_INDEX 2
#define OUTPUT_FILE_INDEX 3

int main(int argc, char* argv[]) {
	if (argc != EXPECTED_ARGC) {
		std::cout << "Expected " << EXPECTED_ARGC-1 << " arguments, but got " << argc-1 << std::endl;
		std::cout << "Usage: translate.exe [name file] [data file] [output file]" << std::endl;
		return 1;
	}

	boost::unordered_set<ZCTA> zctas;
	boost::unordered_set<ZCTASegment> segments;
	std::cout << "Parsing from name file " << argv[NAME_FILE_INDEX] << " and data file " << argv[DATA_FILE_INDEX] << "... ";
	assembleZCTAs(argv[NAME_FILE_INDEX], argv[DATA_FILE_INDEX], &zctas, &segments);
	std::cout << "done." << std::endl;

	std::cout << "Writing output to " << argv[OUTPUT_FILE_INDEX] << "... ";
	std::ofstream output(argv[OUTPUT_FILE_INDEX]);
	output << zctas << " " << segments;
	std::cout << "done." << std::endl;
}
