#include "Printer.hpp"
#include <fstream>

void print(const std::string& outputFile, const boost::unordered_set<ZCTA>& zctas, const AdjacencySet& adjacencies) {
	std::ofstream out(outputFile.c_str());
	out << zctas.size() << std::endl;
	for (boost::unordered_set<ZCTA>::const_iterator i = zctas.cbegin(); i != zctas.cend(); ++i)
		out << i->id() << " " << i->interiorPoint().x().get_num() << " "
			<< i->interiorPoint().y().get_num() << std::endl;
	out << adjacencies.size() << std::endl;
	for (AdjacencySet::iterator i = adjacencies.begin(), end = adjacencies.end(); i != end; ++i)
		out << i->first.id() << " " << i->second.id() << std::endl;
}
