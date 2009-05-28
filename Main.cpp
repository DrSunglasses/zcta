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
		std::cout << zctas.size() << " " << segments.size() << std::endl;

		AdjacencySet adj;
		HorizontalAlgorithm ha(segments, 0.001, &adj);
		t1.restart();
		ha();
		std::cout << "Computed in " << t1.elapsed() << std::endl;

//		for (AdjacencySet::iterator i = adj.begin(); i != adj.end(); ++i)
//			std::cout << i->first.id() << " " << i->second.id() << std::endl;
		std::cout << adj.size() << std::endl;
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
