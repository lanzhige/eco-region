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

struct ProtectedArea{
  string name;
  vector<Polygon *> polygons;
  vector<Cell *> cells;
  BoundingBox *bbox;

  ProtectedArea(){
    name="";
    polygons.clear();
    cells.clear();
    bbox=nullptr;
  }

  ProtectedArea(const string &name){
    ProtectedArea();
    this->name = name;
    polygons.clear();
    cells.clear();
    bbox=nullptr;
  }

  int genBoundingBox(){
    if (polygons.size() == 0) return 1;
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
    bbox = new BoundingBox();
    bbox->setBoundary(origin, destination);
    return 0;
  }

  bool insidePA(Point_2d *p){
    if (bbox==nullptr) return false;
    if (bbox->origin->x<p->x&&bbox->destination->x>p->x
        &&bbox->origin->y<p->y&&bbox->destination->y>p->y) return true;
    return false;
  }

  ~ProtectedArea(){
    if (bbox!=nullptr)
      delete bbox;
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
