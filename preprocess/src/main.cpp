#include <iostream>
#include <vector>
#include <string>

#include "dbio.h"
#include "fileio.h"
#include "protectedarea.h"
#include "geogrid.h"

using std::ifstream;
using std::vector;

int main(){
  vector<ProtectedArea *> areas;
  vector<vector<Geogrid*>> geo_grid(0,vector<Geogrid*>(0,nullptr));
  /*initJsonArea(areas
      , "C:/Users/lzhan253/Documents/WA.json");
  initJsonArea(areas
      , "C:/Users/lzhan253/Documents/ID.json");
  initJsonArea(areas
      , "C:/Users/lzhan253/Documents/OR.json");*/
  initJsonArea(areas, "C:/Users/lzhan253/Documents/MT_city.json");

  std::cout<<"json read finished!"<<std::endl;
  for (int i=0;i<areas.size();i++)
    areas[i]->genBoundingBox();
  std::cerr<<"total calculated areas: "<<areas.size()<<std::endl;
  std::cout.precision(17);
  
  std::cout << "start db operations" << std::endl;

  DBio db;
  db.initMongo();
  db.dbfind(areas, geo_grid);

  return 0;
}
/*for (int i = 0; i < (GRID_LARGEST_LAT - GRID_ORIGION_LAT) / TEN_KM_LAT + 1;
	i++) {
	vector<Geogrid*> temp(0, nullptr);
	for (int j = 0;
		j < (GRID_LARGEST_LNG - GRID_ORIGION_LNG) / TEN_KM_LNG + 1; j++) {
		Geogrid* grid = new Geogrid();
		temp.push_back(grid);
	}
	geo_grid.push_back(temp);
}

for (int i = 0; i < areas.size(); i++) {
	for (int j = 0; j < geo_grid.size(); j++) {
		for (int k = 0; k < geo_grid[j].size(); k++) {
			Point_2d p(GRID_ORIGION_LNG + TEN_KM_LNG*k + TEN_KM_LNG / 2
				, GRID_ORIGION_LAT + TEN_KM_LAT*j + TEN_KM_LAT / 2);
			if (areas[i]->insidePA(&p)) {
				double dist;
				for (int l = 0; l < areas[i]->polygons.size(); l++) {
					for (int m = 1; m < areas[i]->polygons[l]->coords.size(); m++) {
						dist = p.toSegDist(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]);
						if (dist <= INNER_RADIUS) geo_grid[j][k]->inner.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
						else if (dist <= MIDDLE_RADIUS) geo_grid[j][k]->middle.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
						else if (dist <= OUTTER_RADIUS) geo_grid[j][k]->outter.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
					}
				}
			}
		}
	}
}*/