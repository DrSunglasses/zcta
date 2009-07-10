//Translate.cpp: process name and data files

#include "io/Parser.hpp"
#include "io/UnorderedSetStreaming.hpp"
#include "io/UnorderedMapStreaming.hpp"
#include "io/VectorStreaming.hpp"
#include "algorithm/ZCTASegment.hpp"
#include <iostream>
#include <fstream>
#include <utility>

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
	std::cout << "Parsing from name file " << argv[NAME_FILE_INDEX] << " and data file " << argv[DATA_FILE_INDEX] << "... " << std::flush;
	assembleZCTAs(argv[NAME_FILE_INDEX], argv[DATA_FILE_INDEX], &zctas, &segments);
	std::cout << "done." << std::endl;

	std::cout << "Sorting by ZCTA for serialization... " << std::flush;
	boost::unordered_map<ZCTA, std::vector<LineSegment> > blob;
	for (boost::unordered_set<ZCTA>::const_iterator i = zctas.cbegin(); i != zctas.cend(); ++i)
		blob.insert(std::make_pair(*i, std::vector<LineSegment>()));
	for (boost::unordered_set<ZCTASegment>::const_iterator i = segments.cbegin(); i != segments.cend(); ++i) {
		blob[i->zcta()].push_back(*i); //this deliberately slices the ZCTASegment into a LineSegment
	}
	std::cout << "done." << std::endl;

	std::cout << "Writing output to " << argv[OUTPUT_FILE_INDEX] << "... " << std::flush;
	std::ofstream output(argv[OUTPUT_FILE_INDEX]);
	output << blob;
	std::cout << "done." << std::endl;
}
