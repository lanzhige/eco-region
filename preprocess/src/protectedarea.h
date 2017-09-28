#ifndef PROTECTEDAREA_H_
#define PROTECTEDAREA_H_

#include <string>
#include <vector>

#include "geometry.h"
#include "polygon.h"
#include "definition.h"

using std::vector;
using std::string;

struct Cell{
  int row_num, col_num;
  double dist;
  Cell() :row_num(-1), col_num(-1){}
  Cell(int row, int col, double distance = MAX_DIST) :row_num(row), col_num(col), dist(distance){}
};

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
    
    Point_2d origin(*polygons[0]->coords[0]);
    Point_2d destination(*polygons[0]->coords[0]);

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

    //find the row num and col num for each cell
    double row_start, row_end, col_start, col_end;
    row_start = distanceEarth(origin.y, origin.x, MIN_LATITUDE, origin.x) / 30;
    row_end = distanceEarth(destination.y, destination.x, MIN_LATITUDE, destination.x) / 30 + 1;
    col_start = distanceEarth(origin.y, origin.x, origin.y, MIN_LONGITUDE) / 30;
    col_end = distanceEarth(destination.y, destination.x, destination.y, MIN_LONGITUDE) / 30 + 1;

    //calculate cells and output
    for (int j = static_cast<int>row_start; j <= row_end; j++){
      for (int i = static_cast<int>col_start; i <= col_end; i++){
        Point_2d center(MIN_LONGITUDE, MIN_LATITUDE);
        center.move(i * 30 + 15, 0);
        center.move(j * 30 + 15, 90);
        double distance = MAX_DIST;
        for (int k = 0; k < polygons.size(); k++){
          if (polygons[i]->contain(center)) distance = -1.0;
          else distance = min(distance, polygons[i]->shortestDistance(center));
        }
        if (double <= MAX_THRES) {
          Cell *cell = new Cell(j, i, distance);
          cells.push_back(cell);
        }
      }
    }    
  }

  int output(){
    //write the data to the database
  }
}

#endif
