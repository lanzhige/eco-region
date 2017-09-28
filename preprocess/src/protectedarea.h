#ifndef PROTECTEDAREA_H_
#define PROTECTEDAREA_H_

#include <string>
#include <vector>

#include "geometry.h"
#include "polygon.h"
#include "definition.h"

using std::vector;

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
  ProtectedArea(){

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
    Point_2d origin(*polygons[0]->coords[0]), destination(*polygons[0]->coords[0]);
    for (unsigned i=0; i<polygons.size(); i++){
      //find the bounding box of entire protected area.
    }
   
      //calculate the bounding box of all the polygons in a protected area
  }

  int output(){
    //write the data to the database
  }
}

#endif