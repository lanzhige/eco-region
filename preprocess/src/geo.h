#ifndef GEO_H_
#define GEO_H_

#include<iostream>
#include<boost/geometry.hpp>
#include<boost/geometry/geometries/box.hpp>
#include<boost/geometry/geometries/point_xy.hpp>
#include<boost/geometry/geometries/polygon.hpp>

using namespace boost::geometry;

typedef boost::geometry::model::point
<
	double, 2, 
	boost::geometry::cs::spherical_equatorial<boost::geometry::degree>
> spherical_point;

typedef model::polygon<spherical_point> polygon_t;
typedef model::multi_polygon<polygon_t> mpolygon_t;



#endif // GEO_H_