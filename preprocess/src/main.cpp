#include <iostream>
#include <vector>
#include <string>

#include "dbio.h"
#include "fileio.h"
#include "protectedarea.h"
#include "dbio.h"
using std::ifstream;
using std::vector;

/*int main() {
  vector<Polygon *> areas;
  vector<ValueGrid> value_grids;
  txtArrayToJson("/home/lzhan253/project/eco-region/preprocess/data/test1.txt");
  //return 0;
  if (initJsonArea(areas,"/home/lzhan253/project/eco-region/preprocess/data/test.json")) return 1;
  std::cout << "start read txt file" << std::endl;
  ifstream fin;
  fin.open("/home/lzhan253/project/eco-region/preprocess/data/test1.txt");
  if (!fin.good()) return 1;
  Grid grid; 
  unsigned total = 0;
  double p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
  std::cerr << "start read to var" << std::endl;
  std::cout.precision(17);
  std::cerr.precision(17);
//  char c;
  while (!fin.eof()){
//    fin >> c;
    fin >> p1x >> p1y >> p2x >> p2y >> p3x >> p3y >> p4x >> p4y;
    //scanf
    grid.addPoint(( p1y + p2y + p3y + p4y ) / 4.0, ( p1x + p2x + p3x + p4x ) / 4.0);
    //std::cerr<<"grid 0: "<<grid.coords[0]->x<<std::endl;
    total++;
//    fin >> c;
    if (total > 159999) break;
    
    //if (total<4) std::cout << p1x << p1y << p2x << p2y << p3x << p3y << p4x << p4y << std::endl;
    //if (total > 5) break;
    //test codes
  }
  grid.setRowCol(400, 400);
  //std::cerr << grid.coords[0]->x << "  " << grid.coords[0]->y << std::endl;
  //std::cerr << grid.coords[total-1]->x << " " << grid.coords[total-1]->y << std::endl;
  outJson(areas, grid);
  std::cerr << "finished!" << std::endl;
  
 // int test;
 // std::cin >> test;
  return 0;
}*/
int main(){
  vector<Polygon *> areas;
  //initJsonArea(areas
  //    , "/home/lzhan253/project/eco-region/preprocess/data/test.json");
  initProtectedArea(area
      , "/home/lzhan253/project/eco-region/preprocess/data/MO.shp");
  std::cout<<"protected area json input finished!"<<std::endl;
  std::cout<<"total protected areas: "<<area.size()<<std::endl;
  for (int i=0;i<areas.size();i++) areas[i]->resizeBoundingBox();
  

  //Point_2d p(-93.1076, 37.8306);
  //p.move(300000.0, -90.0);
  //std::cout<<"p x: "<<p.x<<" py: "<<p.y<<std::endl;
  //std::cout<<"distance: "<<distanceEarth(p.y,p.x,37.8306, -93.1076);
  
  /*for (int i=0;i<2;i++){
    areas[i]->genBoundingBox();
    std::cout<<areas[i]->polygons.size()<<std::endl;
  }
  std::cout<<"protected area calculation finished"<<std::endl;
  DBio mongodb;
  //mongodb.instance = mongodb.Instance();
  mongodb.initMongo();
  mongodb.insertDB(areas[0]);
  mongodb.insertDB(areas[1]);
  std::cout<<"data insert finished"<<std::endl;
  */
  return 0;
}
