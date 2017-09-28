#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include<iostream>
#include<vector>
//#include<unordered_map>
#include<string>
#include<math.h>

#include "definition.h"

using std::vector;

struct Point_2d{
  double x;
  double y;
  Point_2d(double x, double y) :x(x), y(y){ }
  Point_2d(const Point_2d &ref) {
    if (this == &ref) return;
    Point_2d(ref.x, ref.y);
  }

  bool operator ==( const Point_2d &point ){
    if (fabs(x - point.x) < BIAS && fabs(y - point.y) < BIAS) return true;
    else return false;
  }

  Point_2d &operator = ( const Point_2d &point ){
    if (this == &point) return;
    this->x = point.x;
    this->y = point.y;
  }

  double deg2rad(double deg)const {
    return ( deg * M_PI / 180 );
  }

  //  This function converts radians to decimal degrees
  double rad2deg(double rad) const{
    return ( rad * 180 / M_PI );
  }

  double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) const{
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin(( lat2r - lat1r ) / 2);
    v = sin(( lon2r - lon1r ) / 2);
    return 2.0 * EARTHRADIUS * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
  }

  double dotProduct(const Point_2d &p1, const Point_2d &p2) const {
    return ( p2.x - p1.x )*( x - p2.x ) + ( p2.y - p1.y )*( y - p2.y );
  }

  double crossProduct(const Point_2d &p1, const Point_2d &p2) const {
    return ( p2.x - p1.x )*( y - p1.y ) - ( p2.y - p1.y )*( x - p1.x );
  }

  double toSegDist(const Point_2d &p1, const Point_2d &p2)const{
    double dot1 = dotProduct(p1, p2);
    if (dot1 > 0) return distanceEarth(p2.y, p2.x, y, x);
    double dot2 = dotProduct(p2, p1);
    if (dot2 > 0) return distanceEarth(p1.y, p1.x, y, x);

    double k = k = ( ( x - p1.x ) * ( p2.x - p1.x ) + ( y - p1.y ) * ( p2.y - p1.y ) )
      / ( ( p2.x - p1.x ) * ( p2.x - p1.x ) + ( p2.y - p1.y ) * ( p2.y - p1.y ) );
    return distanceEarth(p1.y + k*( p2.y - p1.y ), p1.x + k*( p2.x - p1.x ), y, x);
  }

  Point_2d getDistPoint(/*double lon, double lat,*/ double distMeter, double bearing){
    double brngRad = deg2rad(bearing);
    double lonRad = deg2rad(x);
    double latRad = deg2rad(y);
    double distFrac = distMeter / EARTHRADIUS;

    double latitudeResult = asin(sin(latRad)*cos(distFrac) + cos(latRad)*sin(distFrac)*cos(brngRad));
    double a = atan2(sin(brngRad)*sin(distFrac)*cos(latRad), cos(distFrac) - sin(latRad)*sin(latitudeResult));
    double longitudeResult = lonRad + a + 3 * M_PI - ( 2 * M_PI )*floor(( lonRad + a + 3 * M_PI ) / ( 2 * M_PI )) - M_PI;
    return ( Point_2d(rad2deg(longitudeResult), rad2deg(latitudeResult)) );
  }
};

struct BoundingBox{
  Point_2d *origin, *destination;
  BoundingBox(){
    origin = new Point_2d(MIN_LONGITUDE, MIN_LATITUDE);
    destination = new Point_2d(MAX_LONGITUDE, MAX_LATITUDE);
  }
  BoundingBox(double left_lon, double bottom_lat, double right_lon, double top_lat){
    origin = new Point_2d(left_lon, bottom_lat);
    destination = new Point_2d(right_lon, top_lat);
  }

  BoundingBox(const BoundingBox &bbox){
    if (this == &bbox) return;
    *origin = *( bbox.origin );
    *destination = *( bbox.destination );
  }

  BoundingBox &operator=( const BoundingBox &bbox ){
    return BoundingBox(bbox);
  }

  int setBoundary(const Point_2d &ori, const Point_2d &des){
    *origin = ori;
    *destination = des;
    return 0;
  }

  int extend(double boundary = MAX_THRES){
    Point_2d new_origin = origin->getDistPoint(MAX_THRES, -90).getDistPoint(MAX_THRES, -180);
    Point_2d new_destination = destination->getDistPoint(MAX_THRES, 90).getDistPoint(MAX_THRES, 0);
    setBoundary(new_origin, new_destination);
    return 0;
  }

  ~BoundingBox(){
    delete origin;
    delete destination;
  }
};

#endif
