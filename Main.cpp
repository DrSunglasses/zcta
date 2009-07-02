/* 
 * File:   Main.cpp
 * Author: Jeffrey Bosboom
 *
 * Created on April 21, 2009, 10:02 PM
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/unordered_set.hpp>
#include "memory/GMPMemory.hpp"
#include "algorithm/HorizontalComparators.hpp"
#include "algorithm/VerticalComparators.hpp"
#include "math/Rational.hpp"
#include "io/Parser.hpp"
#include "io/Printer.hpp"

#define ADJACENCY_TOLERANCE 0.0001
#define REMOVE_HH_ZCTAS 1

int main(int argc, char** argv) {
	std::cout << "Setting up custom memory allocation... " << std::endl;
	initGMPMemoryFunctions();
	std::cout << "Testing the math (sanity check on GMP)... " <<std::endl;
	test_atan2();
	testHorizontalPointComparator();
	testHorizontalLineComparator();
	testVerticalPointComparator();
	testVerticalLineComparator();
	try {
		boost::unordered_set<ZCTA> zctas;
		boost::unordered_set<ZCTASegment> segments;
		boost::timer t1;
		assembleZCTAs("data/zt44_d00a.dat", "data/zt44_d00.dat", &zctas, &segments);
		std::cout << "Parsed in " << t1.elapsed() << std::endl;
		std::cout << zctas.size() << " ZCTAs with " << segments.size() << " segments" << std::endl;

#ifdef REMOVE_HH_ZCTAS
		t1.restart();
		{
			std::vector<boost::unordered_set<ZCTA>::iterator> iters;
			for (boost::unordered_set<ZCTA>::iterator i = zctas.begin(); i != zctas.end(); i++)
				if (i->id().find("HH") != std::string::npos)
					iters.push_back(i);
			for (int i = 0; i < iters.size(); i++)
				zctas.erase(iters[i]);
		}
		{
			std::vector<boost::unordered_set<ZCTASegment>::iterator> iters;
			for (boost::unordered_set<ZCTASegment>::iterator i = segments.begin(); i != segments.end(); i++)
				if (i->zcta().id().find("HH") != std::string::npos)
					iters.push_back(i);
			for (int i = 0; i < iters.size(); i++)
				segments.erase(iters[i]);
		}
		std::cout << "Removed HH ZCTAs in " << t1.elapsed() << std::endl;
		std::cout << zctas.size() << " ZCTAs with " << segments.size() << " segments" << std::endl;
#endif

		AdjacencySet hadj;
		HorizontalAlgorithm ha(segments, ADJACENCY_TOLERANCE, &hadj);
		t1.restart();
		ha();
		std::cout << "Computed with horizontal sweep line in " << t1.elapsed() << ": " << hadj.size() << " adjacencies" << std::endl;
		
		AdjacencySet vadj;
		VerticalAlgorithm va(segments, ADJACENCY_TOLERANCE, &vadj);
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
