#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>

#include "definition.h"
#include "geometry.h"

using std::vector;

struct Polygon{
  vector<Point_2d *> coords;
  BoundingBox bbox;
  Polygon(){ coords.clear(); }
  Polygon(const vector<Point_2d *> &ref) {
    coords.resize(ref.size());
    for (unsigned i = 0; i < coords.size(); i++){
      coords[i]->x = ref[i]->x;
      coords[i]->y = ref[i]->y;
    }
  }

  Polygon(double *data, unsigned int size) {
    //construction: read data as a list of  x0, y0, x1, y1, etc
    for (unsigned int i = 1; i < size; i = i + 2) {

      coords.push_back(new Point_2d(data[i - 1], data[i]));
    }
    if (!( coords[coords.size() - 1] == coords[0] ))
        coords.emplace_back(coords[0]);
    //link the last node to the first node
  }

  Polygon(double *data_x, double *data_y, unsigned int size) {
    //construct from a list of x and y
    for (unsigned i = 0; i < size; i++) {
      coords.push_back(new Point_2d(data_x[i], data_y[i]));
    }
    if (!( coords[coords.size() - 1] == coords[0] )) 
        coords.emplace_back(coords[0]);
    //link the last node to the first node
  }

  void addPoint(Point_2d *p){
    coords.push_back(p);
  }

  bool contain(const Point_2d &point) const {
    unsigned crossings = 0;
    for (unsigned int i = 1; i < coords.size(); i++){
      bool cond1 = 
          ( coords[i - 1]->x <= point.x ) && ( point.x < coords[i]->x );
      bool cond2 = 
          ( coords[i]->x <= point.x ) && ( point.x < coords[i - 1]->x );
      bool above = 
        ( point.y <= ( ( point.x - coords[i - 1]->x )
            *( coords[i]->y - coords[i - 1]->y ) 
        / ( coords[i]->x - coords[i - 1]->x ) + coords[i - 1]->y ) );
      // if the program crashes, probable the divider is zero
      if (( cond1 || cond2 ) && above) crossings++;
    }
    if (crossings % 2 == 0) return false; else return true;
  }

  double shortestDistance(const Point_2d &point) const {
    double min_dist = MAX_DIST, dist;
    for (unsigned i = 1; i < coords.size(); i++) {
      dist = point.toSegDist(*coords[i - 1], *coords[i]);
      if (dist < min_dist) min_dist = dist;
    }
    return min_dist;
  }

  int resizeBoundingBox(){
    Point_2d origin(*coords[0]), destination(*coords[0]);
    for (unsigned i = 0; i < coords.size(); i++){
      if (coords[i]->x < origin.x) origin.x = coords[i]->x;
      if (coords[i]->x > destination.x) destination.x = coords[i]->x;
      if (coords[i]->y < origin.y) origin.y = coords[i]->y;
      if (coords[i]->y > destination.y) destination.y = coords[i]->y;
    }
    bbox.setBoundary(origin, destination);
    bbox.extend();
    return 0;
  }

  ~Polygon(){
    if (coords.size()>0)
      for (int i=0;i<coords.size();i++){
        delete coords[i];
      }
    coords.clear();
  }
};
#endif
