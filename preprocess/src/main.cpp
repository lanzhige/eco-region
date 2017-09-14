#include <iostream>
#include <fstream>
#include <string>
#include"fileio.h"
#include"pointandpolygon.h"

using std::ifstream;

int main() {
  vector<Polygon> areas;
  vector<ValueGrid> value_grids;
  if (initProtectedArea(areas,"D:\\zl\\Ross Lab\\sept8-sept15\\point and polygon\\preprocessing\\data\\test.shp")) return 1;
  std::cout << "start read txt file" << std::endl;
  ifstream fin;
  fin.open("D:\\zl\\Ross Lab\\sept8-sept15\\point and polygon\\preprocessing\\data\\test1.txt");
  if (!fin.good()) return 1;
  Grid grid; 
  unsigned total = 0;
  double p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
  while (!fin.eof()){
    fin >> p1x >> p1y >> p2x >> p2y >> p3x >> p3y >> p4x >> p4y;
    //scanf
    grid.addPoint(( p1x + p2x + p3x + p4x ) / 4, ( p1y + p2y + p3y + p4y ) / 4);
    total++;
    //std::cout.precision(17);
    //std::cout << p1x << p1y << p2x << p2y << p3x << p3y << p4x << p4y << std::endl;
    //if (total > 5) break;
    //test codes
  }
  for (unsigned i = 0; i < areas.size(); i++){
    ValueGrid value_grid(&grid);
    value_grid.getValue(areas[i]);
    //add value_grid dist to mongodb
  }
  std::cout << total << std::endl;
  
  int test;
  std::cin >> test;
  return 0;
}