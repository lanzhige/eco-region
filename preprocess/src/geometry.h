#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include<iostream>
#include<vector>
#include<string>
#include<math.h>

#include "definition.h"

using std::vector;

double deg2rad(double deg) {
  return ( deg * M_PI / 180 );
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return ( rad * 180 / M_PI );
}

double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin(( lat2r - lat1r ) / 2.0);
  v = sin(( lon2r - lon1r ) / 2.0);
  return 2.0 * EARTHRADIUS*asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

/*double distanceEarth(double y1, double x1, double y2, double x2){
  return sqrt((y1-y2)*(y1-y2)+(x1-x2)*(x1-x2));
}*/

struct Point_2d{
  double x;
  double y;
  Point_2d(double x, double y) :x(x), y(y){ }
  Point_2d(const Point_2d &ref) {
    if (this != &ref) {
      this->x = ref.x;
      this->y = ref.y;
    }
  }

  bool operator ==( const Point_2d &point ){
    if (fabs(x - point.x) < BIAS && fabs(y - point.y) < BIAS) return true;
    else return false;
  }

  Point_2d &operator = ( const Point_2d &point ){
    if (this != &point) {
      this->x = point.x;
      this->y = point.y;
    }
  }

  /*double dotProduct(const Point_2d &p1, const Point_2d &p2) const {
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

    double k = k = ( ( x - p1.x ) * ( p2.x - p1.x ) 
        + ( y - p1.y ) * ( p2.y - p1.y ) )
        / ((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return distanceEarth(p1.y + k*( p2.y - p1.y ), p1.x + k*( p2.x - p1.x )
        , y, x);
  }*/

  double dotProduct(const Point_2d *p1, const Point_2d *p2) const {
	  return (p2->x - p1->x)*(x - p2->x) + (p2->y - p1->y)*(y - p2->y);
  }

  double crossProduct(const Point_2d *p1, const Point_2d *p2) const {
	  return (p2->x - p1->x)*(y - p1->y) - (p2->y - p1->y)*(x - p1->x);
  }

  double toSegDist(const Point_2d *p1, const Point_2d *p2)const {
	  // maybe I can record the result of p2->x - p1->x 
	  // and p2->y - p1->y to reduce a constant calculation
	  double dot1 = dotProduct(p1, p2);
	  if (dot1 > 0) return distanceEarth(p2->y, p2->x, y, x);
	  double dot2 = dotProduct(p2, p1);
	  if (dot2 > 0) return distanceEarth(p1->y, p1->x, y, x);

	  /*double k1 = ((p2->x - p1->x) * (p2->x - p1->x)
		+ (p2->y - p1->y) * (p2->y - p1->y));
	  if (k1 < BIAS) {
		  std::cout << "smaller than 1e-17: " << k1 << std::endl;
		  return distanceEarth(p1->y, p1->x, y, x);
	  }*/

	  double k = k = ((x - p1->x) * (p2->x - p1->x)
		  + (y - p1->y) * (p2->y - p1->y))
		  / (
				(p2->x - p1->x) * (p2->x - p1->x) 
			  + (p2->y - p1->y) * (p2->y - p1->y)
			  );
	  return distanceEarth(p1->y + k*(p2->y - p1->y)
		  , p1->x + k*(p2->x - p1->x), y, x);
  }

  int move(double distMeter, double bearing){
    double brngRad = deg2rad(bearing);
    double lonRad = deg2rad(x);
    double latRad = deg2rad(y);
    double distFrac = distMeter / EARTHRADIUS;

    double latitudeResult = asin(sin(latRad)*cos(distFrac) 
        + cos(latRad)*sin(distFrac)*cos(brngRad));
    double a = atan2(sin(brngRad)*sin(distFrac)*cos(latRad)
        , cos(distFrac) - sin(latRad)*sin(latitudeResult));
    double longitudeResult = lonRad + a + 3 * M_PI 
        - ( 2 * M_PI )*floor(( lonRad + a + 3 * M_PI ) / ( 2 * M_PI )) - M_PI;
    x = rad2deg(longitudeResult);
    y = rad2deg(latitudeResult);
    //std::cout<<"x: "<<x<<" y: "<<std::endl;
    return 0;
  }

  Point_2d getDistPoint(double distMeter, double bearing){
    double brngRad = deg2rad(bearing);
    double lonRad = deg2rad(x);
    double latRad = deg2rad(y);
    double distFrac = distMeter / EARTHRADIUS;

    double latitudeResult = asin(sin(latRad)*cos(distFrac) 
        + cos(latRad)*sin(distFrac)*cos(brngRad));
    double a = atan2(sin(brngRad)*sin(distFrac)*cos(latRad)
        , cos(distFrac) - sin(latRad)*sin(latitudeResult));
    double longitudeResult = lonRad + a + 3 * M_PI 
      - ( 2 * M_PI )*floor(( lonRad + a + 3 * M_PI ) / ( 2 * M_PI )) - M_PI;
    return ( Point_2d(rad2deg(longitudeResult), rad2deg(latitudeResult)) );
  }
};

struct BoundingBox{
  Point_2d *origin, *destination;
  BoundingBox(){
    origin = new Point_2d(MIN_LONGITUDE, MIN_LATITUDE);
    destination = new Point_2d(MAX_LONGITUDE, MAX_LATITUDE);
  }
  BoundingBox(double left_lon, double bottom_lat
      , double right_lon, double top_lat){
    origin = new Point_2d(left_lon, bottom_lat);
    destination = new Point_2d(right_lon, top_lat);
  }

  BoundingBox(const BoundingBox &bbox){
    if (this != &bbox) {
      origin->x = bbox.origin->x;
      origin->y = bbox.origin->y;
      destination->x = bbox.destination->x;
      destination->y = bbox.destination->y;
    }
  }

  BoundingBox &operator=( const BoundingBox &bbox ){
    if (this != &bbox){
      origin->x = bbox.origin->x;
      origin->y = bbox.origin->y;
      destination->x = bbox.destination->x;
      destination->y = bbox.destination->y;
    }
    return *this;
  }

  int setBoundary(const Point_2d &ori, const Point_2d &des){
    origin->x = ori.x;
    origin->y = ori.y;
    destination->x = des.x;
    destination->y = des.y;
    return 0;
  }

  /*int extend(double boundary = MAX_THRES){
    Point_2d new_origin = 
        origin->getDistPoint(MAX_THRES, -180.0).getDistPoint(MAX_THRES, -90.0);
    Point_2d new_destination 
        = destination->getDistPoint(MAX_THRES, 0.0
            ).getDistPoint(MAX_THRES, 90.0);
    setBoundary(new_origin, new_destination);
    return 0;
  }*/

  int extend(){
    this->origin->x -= 0.4;
    this->origin->y -= 0.25;
    this->destination->x += 0.4;
    this->destination->y += 0.25;
	return 0;
  }

  ~BoundingBox(){
	  std::cout << "destruct bbox" << std::endl;
    delete origin;
    delete destination;
  }
};

#endif
