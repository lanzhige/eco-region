#ifndef GEOGRID_H_
#define GEOGRID_H_
#include "definition.h"
#include "geometry.h"

struct Geogrid {
	vector<std::pair<Point_2d*, Point_2d*>> inner;
	vector<std::pair<Point_2d*, Point_2d*>> middle;
	vector<std::pair<Point_2d*, Point_2d*>> outter;
	double shortestDistance(Point_2d *p) {
		double dist = MAX_DIST;
		for (int i = 0; i < inner.size(); i++) {
			dist = min(dist, p->toSegDist(inner[i].first, inner[i].second));
		}
		if (dist<INNER_THRES) return dist;
		for (int i = 0; i < middle.size(); i++) {
			dist = min(dist, p->toSegDist(middle[i].first, middle[i].second));
		}
		if (dist < MIDDLE_THRES) return dist;
		for (int i = 0; i < outter.size(); i++) {
			dist = min(dist, p->toSegDist(outter[i].first, outter[i].second));
		}
		return dist;
	}
	Geogrid(){
		inner.clear();
		middle.clear();
		outter.clear();
	}
};

#endif // !GEOGRID_H_
