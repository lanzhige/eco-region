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

    //calculate cells and output

  }

  int output(){
    //write the data to the database
  }
}

#endif
