/*
shp file read and csv file read
currently synchronized io
*/
#ifndef FILEIO_H_
#define FILEIO_H_
#include <iostream>
#include <fstream>
#include <json/json.h>

#include "geometry.h"
#include "polygon.h"
#include "protectedarea.h"
#include "geo.h"

int initJsonArea(vector<ProtectedArea *> &areas,const char *file = nullptr) {
  Json::Reader reader;
  Json::Value root;

  std::ifstream is;
  is.open(file, std::ios::binary);
  if (reader.parse(is, root)) {
    int file_size;
    if (!root["features"].isNull()) file_size = root["features"].size();
    for (int i = 0; i < file_size; i++){
      Json::Value coords = root["features"][i]["geometry"]["rings"];
      int coords_size = coords.size();
      ProtectedArea *area = new ProtectedArea("WA"+std::to_string(i));
      for (int k = 0; k < coords_size; k++){
        Polygon *polygon = new Polygon();
        Json::Value polys = coords[k];
        int poly_size = polys.size();
        //for (int p = 0; p < poly_size; p++){
        //  Json::Value pairs = polys[p];
        //  int pair_size = pairs.size();
        for (int q = 0; q < poly_size; q++){
          Point_2d *point = 
              new Point_2d(polys[q][0].asDouble(), polys[q][1].asDouble());
          polygon->addPoint(point);
        }    
        area->polygons.push_back(polygon);
      }
      areas.push_back(area);
    }
  }
  is.close();
  return 0;
}

int initJsonRail(vector<ProtectedArea *> &areas, const char *file = nullptr) {
	Json::Reader reader;
	Json::Value root;

	std::ifstream is;
	is.open(file, std::ios::binary);
	if (reader.parse(is, root)) {
		int file_size;
		if (!root["features"].isNull()) file_size = root["features"].size();
		for (int i = 0; i < file_size; i++) {
			Json::Value coords = root["features"][i]["geometry"]["path"];
			int coords_size = coords.size();
			ProtectedArea *area = new ProtectedArea("MT" + std::to_string(i));
			for (int k = 0; k < coords_size; k++) {
				Polygon *polygon = new Polygon();
				//temporarily use polygon as path
				Json::Value polys = coords[k];
				int poly_size = polys.size();
				//for (int p = 0; p < poly_size; p++){
				//  Json::Value pairs = polys[p];
				//  int pair_size = pairs.size();
				for (int q = 0; q < poly_size; q++) {
					Point_2d *point =
						new Point_2d(polys[q][0].asDouble(), polys[q][1].asDouble());
					polygon->addPoint(point);
				}
				area->polygons.push_back(polygon);
			}
			areas.push_back(area);
		}
	}
	is.close();
	return 0;
}

int initMultiPolygon(mpolygon_t &mpoly, const char *file = nullptr) {
	Json::Reader reader;
	Json::Value root;

	std::ifstream is;
	is.open(file, std::ios::binary);
	if (reader.parse(is, root)) {
		int file_size;
		if (!root["features"].isNull()) file_size = root["features"].size();
		int base_size = mpoly.size();
		mpoly.resize(base_size+file_size);
		for (int i = 0; i < file_size; i++) {
			Json::Value coords = root["features"][i]["geometry"]["rings"];
			int coords_size = coords.size();
			if (coords_size > 1) {
				mpoly[base_size + i].inners().resize(coords_size-1);
			}
			for (int k = 0; k < coords_size; k++) {
				Json::Value polys = coords[k];
				int poly_size = polys.size();
				//for (int p = 0; p < poly_size; p++){
				//  Json::Value pairs = polys[p];
				//  int pair_size = pairs.size();
				for (int q = 0; q < poly_size; q++) {
					if (k > 0) {
						append(mpoly[base_size + i].inners()[k-1], spherical_point(polys[q][0].asDouble(), polys[q][1].asDouble()));
					}
					else {
						append(mpoly[base_size + i].outer(), spherical_point(polys[q][0].asDouble(), polys[q][1].asDouble()));
					}
				}
			}
		}
	}
	is.close();
	return 0;
}

#endif
