/* 
 * File:   Main.cpp
 * Author: Jeffrey Bosboom
 *
 * Created on April 21, 2009, 10:02 PM
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <set>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_set.hpp>
#include "memory/GMPMemory.hpp"
#include "algorithm/HorizontalComparators.hpp"
#include "algorithm/VerticalComparators.hpp"
#include "io/Parser.hpp"
#include "io/Printer.hpp"

int main(int argc, char** argv) {
	initGMPMemoryFunctions();
	try {
		boost::unordered_set<ZCTA> zctas;
		boost::unordered_set<ZCTASegment> segments;
		boost::timer t1;
		assembleZCTAs("data/zt44_d00a.dat", "data/zt44_d00.dat", &zctas, &segments);
		std::cout << "Parsed in " << t1.elapsed() << std::endl;
		std::cout << zctas.size() << " ZCTAs with " << segments.size() << " segments" << std::endl;

		AdjacencySet hadj;
		HorizontalAlgorithm ha(segments, 0.001, &hadj);
		t1.restart();
		ha();
		std::cout << "Computed with horizontal sweep line in " << t1.elapsed() << ": " << hadj.size() << " adjacencies" << std::endl;
		
		AdjacencySet vadj;
		VerticalAlgorithm va(segments, 0.001, &vadj);
		t1.restart();
		va();
		std::cout << "Computed with vertical sweep line in " << t1.elapsed() << ": " << vadj.size() << " adjacencies" << std::endl;
		
		t1.restart();
		vadj.merge(hadj);
		std::cout << "Merged adjacency sets in " << t1.elapsed() << std::endl;

		std::cout << "Total adjacencies: " << vadj.size() << std::endl;
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
