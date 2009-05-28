/* 
 * File:   Printer.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 9, 2009, 4:11 PM
 */

#ifndef _PRINTER_HPP
#define	_PRINTER_HPP

#include <string>
#include <boost/unordered_set.hpp>
#include "../algorithm/AdjacencySet.hpp"

/*
 * Number of ZCTAs
 * Each ZCTA is on its own line
 * (ZCTA ID) (latitude) (longitude)
 * No ordering is guaranteed
 * Number of edges
 * (ZCTA ID) (ZCTA ID)
 * The first ID is lexicographically less than the second, but no interedge
 * ordering is guaraenteed.
 */

void print(const std::string& outputFile, const boost::unordered_set<ZCTA>& zctas, const AdjacencySet& adjacencies);

#endif	/* _PRINTER_HPP */

