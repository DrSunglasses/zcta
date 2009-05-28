/* 
 * File:   Parser.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 5, 2009, 11:22 AM
 */

#ifndef _PARSER_HPP
#define	_PARSER_HPP

#include <map>
#include <string>
#include <boost/unordered_set.hpp>
#include "../algorithm/ZCTASegment.hpp"

void assembleZCTAs(const std::string& nameFile, const std::string& dataFile,
		boost::unordered_set<ZCTA>* zctas, boost::unordered_set<ZCTASegment>* segments);

#endif	/* _PARSER_HPP */

