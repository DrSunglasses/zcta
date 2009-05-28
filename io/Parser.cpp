
#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

void parseNameFile(const std::string& filename, std::map<int, std::string>* retval);

struct OutlineData {
	Point interiorPoint;
	std::vector<LineSegment> segments;
};

void parseOutlineFile(const std::string& filename, std::map<int, OutlineData>* retval);

inline void skipLines(std::ifstream& in, int numLines) {
	while (numLines) {
		in.ignore(1000, '\n');
		--numLines;
	}
}

void parseNameFile(const std::string& filename, std::map<int, std::string>* retval) {
	std::ifstream in(filename.c_str());
	std::string number, id;
	//deal with the dummy 0 data
	skipLines(in, 6);
	const std::string EMPTY_STRING("");
	while (getline(in, number) && number != EMPTY_STRING) {
		skipLines(in, 1);
		getline(in, id);
		skipLines(in, 3);
		boost::trim(number);
		boost::trim_if(id, boost::is_any_of(" \""));
		retval->insert(std::make_pair(boost::lexical_cast<int>(number), id));
	}
}

const boost::regex POINT_PATTERN("-?0\\.([0-9]+)E\\+0([0-9])[[:space:]]+-?0\\.([0-9]+)E\\+0([0-9])");
const int POINT_X = 1, POINT_X_EXP = 2, POINT_Y = 3, POINT_Y_EXP = 4;
const int POINT_X_SIGN = -1, POINT_Y_SIGN = 1;
const int powersOfTen[] = {1, 10, 100, 1000};
//longitude latitude
Point pointFromString(const std::string& foo) {
	boost::match_results<std::string::const_iterator> results;
	if (!regex_search(foo, results, POINT_PATTERN))
		throw std::logic_error("No matches? "+foo);
	int xExp = boost::lexical_cast<int>(results[POINT_X_EXP]);
	int yExp = boost::lexical_cast<int>(results[POINT_Y_EXP]);
	Bigint x(boost::lexical_cast<Bigint>(results[POINT_X]));
	x *= (POINT_X_SIGN * powersOfTen[xExp]);
	Bigint y(boost::lexical_cast<Bigint>(results[POINT_Y]));
	y *= (POINT_Y_SIGN * powersOfTen[yExp]);
	return Point(x, y);
}
void collectSegments(std::istream* input, std::vector<LineSegment>* retval) {
	std::string foo;
	std::vector<Point> pts;
//	while (getline(*input, foo) && !regex_search(foo, END_PATTERN))
	while (getline(*input, foo) && foo.find("END") == std::string::npos)
		pts.push_back(pointFromString(foo));
	if (pts.front() != pts.back()) {
		std::cout << "Adding terminal point" << std::endl;
		pts.push_back(pts.front());
	}
	for (int i = 0, maxIter = pts.size()-1; i < maxIter; i++) {
		if (pts[i] == pts[i+1]) {
			//TODO: check if this is accurate
//			std::cout << "Skipping zero-length segment: " << pts[i] << " " << pts[i+1] << std::endl;
			continue;
		}
		retval->push_back(LineSegment(pts[i], pts[i+1]));
	}
}

void parseOutlineFile(const std::string& filename, std::map<int, OutlineData>* retval) {
	std::ifstream in(filename.c_str());
	OutlineData data;
	std::string munch;
	int number = 1;
	bool first = true;
//	while (getline(in, munch) && !regex_search(munch, END_PATTERN)) {
	while (getline(in, munch) && munch.find("END") == std::string::npos) {
		if (munch.find("-99999") == std::string::npos) { //a new one
			if (first)
				first = false;
			else {
				retval->insert(std::make_pair(number, data));
				number++;
				data.interiorPoint = pointFromString(munch);
				data.segments.clear();
			}
		}
		collectSegments(&in, &data.segments);
	}
	retval->insert(std::make_pair(number, data));
}

void assembleZCTAs(const std::string& nameFile, const std::string& dataFile,
		boost::unordered_set<ZCTA>* zctas, boost::unordered_set<ZCTASegment>* segments) {
	std::map<int, std::string> nameMap;
	std::map<int, OutlineData> dataMap;
	parseNameFile(nameFile, &nameMap);
	parseOutlineFile(dataFile, &dataMap);

	for (int i = 1; nameMap.find(i) != nameMap.end(); i++) {
		OutlineData* data = &dataMap[i];
		ZCTA z(nameMap[i], data->interiorPoint);
		zctas->insert(z);
		for (std::vector<LineSegment>::const_iterator iter = data->segments.begin(); iter != data->segments.end(); ++iter)
			segments->insert(ZCTASegment(*iter, z));
	}
}
