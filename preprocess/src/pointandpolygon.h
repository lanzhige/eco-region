#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<math.h>

using std::vector;
using std::unordered_map;

#define BIAS 0.0000001
#define MAX_DIST (double)INT_MAX

struct Point_2d{
  double x;
  double y;
  Point_2d(double x, double y) :x(x), y(y){ }
  Point_2d(const Point_2d &ref) {
    Point_2d(ref.x, ref.y);
  }

  bool operator ==( const Point_2d &point ){
    if (fabs(x - point.x) < BIAS && fabs(y - point.y) < BIAS) return true;
    else return false;
  }

  double toSegDist(const Point_2d &point1, const Point_2d &point2)const{
    double cross = ( point2.x - point1.x )*( x - point1.x )
      + ( point2.y - point1.y )*( y - point1.y );
    std::cout << "cross: " << cross << std::endl;
    if (cross <= 0)
      return sqrt(( x - point1.x )*( x - point1.x ) + ( y - point1.y )*( y - point1.y ));
    double d2 = ( point2.x - point1.x )*( point2.x - point1.x )
      + ( point2.y - point1.y )*( point2.y - point1.y );
    std::cout << "d2: " << d2 << std::endl;
    if (cross >= d2)
      return sqrt(( x - point2.x )*( x - point2.x ) + ( y - point2.y )*( y - point2.y ));
    double r = cross / d2;
    double px = point1.x + ( point2.x - point1.x )*r;
    double py = point1.y + ( point2.y - point1.y )*r;
    return sqrt(( x - px )*( x - px ) + ( py - y )*( py - y ));
  }
};

struct Polygon{
  vector<Point_2d> coords;
  Polygon(const vector<Point_2d> &ref) {
    coords = ref;
  }

  Polygon(double *data, unsigned int size) {
    //read data as a list of  x0, y0, x1, y1, etc
    for (unsigned int i = 1; i < size; i = i + 2) {
      coords.push_back(Point_2d(data[i - 1], data[i]));
    }
    if (!( coords[coords.size() - 1] == coords[0] )) coords.push_back(coords[0]);
    //link the last node to the first node
  }

  bool contain(const Point_2d &point) const {
    unsigned crossings = 0;
    for (unsigned int i = 1; i < coords.size(); i++){
      if (fabs(coords[i].x - coords[i - 1].x)<BIAS) continue;
      double slope = ( coords[i].y - coords[i - 1].y ) / ( coords[i].x - coords[i - 1].x );
      bool cond1 = ( coords[i - 1].x <= point.x ) && ( point.x < coords[i].x );
      bool cond2 = ( coords[i].x <= point.x ) && ( point.x < coords[i - 1].x );
      bool above = ( point.y <( slope*( point.x - coords[i - 1].x ) + coords[i - 1].y ) );
      if (( cond1 || cond2 ) && above) crossings++;
    }
    return crossings & 1;
    //can only judge if a point is inside a simple polygon
  }

  double shortestDistance(const Point_2d &point) const {
    double min_dist = MAX_DIST, dist;
    for (unsigned i = 1; i < coords.size(); i++) {
      dist = point.toSegDist(coords[i - 1], coords[i]);
      if (dist < min_dist) min_dist = dist;
    }
    return min_dist;
  }
};

struct Grid{
  unsigned int row, column;
  vector<Point_2d> coords;
  //double length, width; 
  // if the cells of grid can't be replaced by a point or the length and width of each cell should be recorded
  Grid(){ }

  Grid(unsigned int row, unsigned int column) :row(row), column(column) { }

  Grid(unsigned int row, unsigned int column, const Point_2d *points){
    this->row = row;
    this->column = column;
    for (unsigned int i = 0; i < row*column; i++){
      coords.push_back(points[i]);
    }
  }
};

struct ValueGrid{
  vector<double> dist;
  const Grid *grid;

  ValueGrid(const Grid * grid) {
    this->grid = grid;
    dist.clear();
    dist.resize(grid->row*grid->column, MAX_DIST);
  }

  void getValue(const Polygon& polygon){
    for (unsigned i = 0; i < dist.size(); i++) {
      if (polygon.contain(grid->coords[i])) dist[i] = -1;
      else dist[i] = polygon.shortestDistance(grid->coords[i]);
    }
  }
};