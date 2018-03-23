#include <iostream>
#include <vector>
#include <string>

#include "dbio.h"
#include "fileio.h"
#include "protectedarea.h"
#include "geogrid.h"

#include "geo.h"

using std::ifstream;
using std::vector;

int main() {
	mpolygon_t mpoly;
	initMultiPolygon(mpoly, "C:/Users/lzhan253/Documents/WA.json");
	initMultiPolygon(mpoly
		, "C:/Users/lzhan253/Documents/ID.json");
	initMultiPolygon(mpoly
		, "C:/Users/lzhan253/Documents/OR.json");
	std::cout << "poly size" << mpoly.size() << std::endl;
	std::cout << "start db operations" << std::endl;
	DBio db;
	db.initMongo();
	db.testIn(mpoly, "grid_Idaho");

	vector<ProtectedArea *> areas;
	vector<vector<Geogrid*>> geo_grid(0, vector<Geogrid*>(0, nullptr));
	initJsonArea(areas
		, "C:/Users/lzhan253/Documents/WA.json");
	initJsonArea(areas
		, "C:/Users/lzhan253/Documents/ID.json");
	initJsonArea(areas
		, "C:/Users/lzhan253/Documents/OR.json");
	//initJsonArea(areas, "C:/Users/lzhan253/Documents/MT_city.json");

	initJsonArea(areas, "C:/Users/lzhan253/Documents/PA_MT.json");

	initJsonArea(areas, "C:/Users/lzhan253/Documents/NV_PA.json");

	initJsonArea(areas, "C:/Users/lzhan253/Documents/UT_PA.json");

	initJsonArea(areas, "C:/Users/lzhan253/Documents/WY_PA.json");
	//initJsonRail(areas, "C:/Users/lzhan253/Documents/MT_Rail.json");

	std::cout << "json read finished!" << std::endl;
	for (int i = 0; i<areas.size(); i++)
		areas[i]->genBoundingBox();
	std::cerr << "total calculated areas: " << areas.size() << std::endl;
	std::cout.precision(17);

	std::cout << "start db operations" << std::endl;

	db.testDist(areas, geo_grid, "Idaho_PA2");

	return 0;
}