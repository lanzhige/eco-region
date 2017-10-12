#ifndef PROTECTEDAREA_H_
#define PROTECTEDAREA_H_

#include <string>
#include <vector>

#include "geometry.h"
#include "polygon.h"
#include "definition.h"

using std::vector;
using std::string;
using std::min;
using std::max;

struct Cell{
  int row_num, col_num;
  double dist;
  Cell() :row_num(-1), col_num(-1){}
  Cell(int row, int col, double distance = MAX_DIST) :row_num(row), col_num(col), dist(distance){}
};
/*
struct ProtectedArea{
  string name;
  vector<Polygon *> polygons;
  vector<Cell *> cells;

  ProtectedArea() { clear(); }
  ProtectedArea(string name) {
    this->name = name;
  }

  int clear(){
    name.clear();
    polygons.clear();
    cells.clear();
    return 0;
  }

  int genBoundingBox(){
    if (polygons.size() == 0) return 0;
    for (unsigned i = 0; i < polygons.size(); i++){
      polygons[i]->resizeBoundingBox();
    }
    
    Point_2d origin(*polygons[0]->bbox.origin);
    Point_2d destination(*polygons[0]->bbox.destination);

    for (unsigned i = 0; i < polygons.size(); i++){
     //calculate the bounding box of all the polygons in a protected area
      if (polygons[i]->bbox.origin->x<origin.x) 
        origin.x = polygons[i]->bbox.origin->x;
      if (polygons[i]->bbox.origin->y<origin.y)
        origin.y = polygons[i]->bbox.origin->y;
      if (polygons[i]->bbox.destination->x>destination.x)
        destination.x = polygons[i]->bbox.destination->x;
      if (polygons[i]->bbox.destination->y>destination.y)
        destination.y = polygons[i]->bbox.destination->y;
     // std::cout<<polygons[i]->bbox.origin->x<<" "<<polygons[i]->bbox.origin->y<<std::endl;
    }
    for (int i=0;i<polygons.size();i++){
      for (int j=0;j<polygons[i]->coords.size();j++){
       // std::cout<<" "<<polygons[i]->coords[j]->x<<std::endl;
      }
    }   

    //std::cout<<"origin x: "<<origin.x<<"origin y: "<<origin.y<<std::endl;
    //std::cout<<"des x: "<<destination.x<<"des y: " <<destination.y<<std::endl;
    //find the row num and col num for each cell
    double row_start, row_end, col_start, col_end;
    /row_start = distanceEarth(origin.y, origin.x, MIN_LATITUDE, origin.x) / 30;
    row_end = distanceEarth(destination.y, destination.x
        , MIN_LATITUDE, destination.x) / 30 + 1;
    col_start = distanceEarth(origin.y, origin.x, origin.y, MIN_LONGITUDE) / 30;
    col_end = distanceEarth(destination.y, destination.x
        , destination.y, MIN_LONGITUDE) / 30 + 1;
    std::cout<<"origin.y"<<origin.y<<" origin.x:"<<origin.x<<std::endl;
    std::cout<<MIN_LATITUDE<<std::endl;/
    row_start = distanceEarth(origin.y, MIN_LONGITUDE
        , MIN_LATITUDE, MIN_LONGITUDE) / 30;
    row_end = distanceEarth(destination.y, MIN_LONGITUDE
        , MIN_LATITUDE, MIN_LONGITUDE) / 30 + 1;
    col_start = distanceEarth(origin.y, origin.x
        , origin.y, MIN_LONGITUDE) / 30;
    col_end = distanceEarth(destination.y, destination.x
        , destination.y, MIN_LONGITUDE) / 30 + 1;

    std::cout<<"min: "<<MIN_LONGITUDE<<" "<<MIN_LATITUDE<<std::endl;
    std::cout<<"origin: "<<origin.x<<" "<<origin.y<<std::endl;

    Point_2d center(MIN_LONGITUDE, MIN_LATITUDE);
    //center.move(row_end*30, 0);
    std::cout<<"col end: "<<col_end<<std::endl;
    center.move(row_end*30, 0);
    std::cout<<center.x<<" wtf "<<center.y<<std::endl;
    center.move(col_end*30, 90.0);
    std::cout<<destination.x<<std::endl;
    std::cout<<center.x<<" fuck "<<center.y<<std::endl;
    
    //std::cout<<"row start: "<<row_start<<"row end: "<<row_end<<std::endl;
    //std::cout<<"col start: "<<col_start<<"col end: "<<col_end<<std::endl;
    //calculate cells and output
    Point_2d o(MIN_LONGITUDE, MIN_LATITUDE);
    std::cout<<row_start<<std::endl;
    for (int j = static_cast<int>(row_start); j <= row_end; j++){
      for (int i = static_cast<int>(col_start); i <= col_end; i++){
        double x = o.getDistPoint(j*30+15, 90.0).x;
        double y = o.getDistPoint(i*30+15, 0.0).y;
        Point_2d center(x, y);
        std::cout<<center.x<< " "<<center.y<<std::endl;
        double distance = MAX_DIST;
        for (int k = 0; k < polygons.size(); k++){
          if (polygons[i]->contain(center)) distance = -1.0;
          else distance = min(distance, polygons[i]->shortestDistance(center));
        }
        if (distance <= MAX_THRES) {
          Cell *cell = new Cell(j, i, distance);
          cells.push_back(cell);
          std::cout<<"f"<<std::endl;
        }
      }
    }    
  }

  int output(){
    //write the data to the database
    return 0;
  }
};
*/
struct ProtectedArea{
  string name;
  vector<Polygon *> polygons;
  vector<Cell *> cells;

  ProtectedArea(){
    name="";
    polygons.clear();
    cells.clear();
  }

  ProtectedArea(const string &name){
    ProtectedArea();
    this->name = name;
  }

  int genBoundingBox(){
    if (polygons.size() == 0) return 0;
    for (unsigned i = 0; i < polygons.size(); i++){
      polygons[i]->resizeBoundingBox();
    }
    
    Point_2d origin(*polygons[0]->bbox.origin);
    Point_2d destination(*polygons[0]->bbox.destination);
    
    for (unsigned i = 0; i < polygons.size(); i++){
     //calculate the bounding box of all the polygons in a protected area
      if (polygons[i]->bbox.origin->x<origin.x) 
        origin.x = polygons[i]->bbox.origin->x;
      if (polygons[i]->bbox.origin->y<origin.y)
        origin.y = polygons[i]->bbox.origin->y;
      if (polygons[i]->bbox.destination->x>destination.x)
        destination.x = polygons[i]->bbox.destination->x;
      if (polygons[i]->bbox.destination->y>destination.y)
        destination.y = polygons[i]->bbox.destination->y;
    }
    std::cout<<origin.x<<" "<<origin.y<<std::endl;
    std::cout<<destination.x<<" "<<destination.y<<std::endl;

    int lower_x = static_cast<int>(origin.x/30);
    int upper_x = static_cast<int>(destination.x/30)+2;
    int lower_y = static_cast<int>(origin.y/30);
    int upper_y = static_cast<int>(destination.y/30)+2;
    int size = (upper_x-lower_x)*(upper_y-lower_y);
    cells.resize(size);
    int count = 0;
    for (int i=lower_x;i<upper_x;i++){
      for (int j=lower_y;j<upper_y;j++){
        Point_2d p(i*30+15, j*30+15);
        double dist=MAX_DIST;
        for (int k=0;k<polygons.size();k++){
          if (polygons[k]->contain(p)){
            dist = -1.0;
            break;
          } else{
            double distant = polygons[k]->shortestDistance(p);
            if (distant<dist) dist = distant;
          }
        }
        if (dist<MAX_THRES) {
          cells[count] = new Cell(i, j, dist);
          count++;
        }
      }
    }
    cells.resize(count);
    return 0;
  }

  ~ProtectedArea(){
    if (cells.size()>0){
      for (int i=0;i<cells.size();i++)
        delete cells[i];
    }
    cells.clear();
    if (polygons.size()>0){
      for (int i=0;i<cells.size();i++)
        delete polygons[i];
    }
    polygons.clear();
  }
};
#endif
