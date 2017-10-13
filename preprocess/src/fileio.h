/*
shp file read and csv file read
currently synchronized io
*/
#ifndef FILEIO_H_
#define FILEIO_H_
#include <iostream>
#include <fstream>
#include <shapelib-1.4.0/shapefil.h>
#include <fast-csv-parser/csv.h>
#include <jsoncpp-master/include/json/json.h>
//#include <bsoncxx/json.hpp>
//#include <mongocxx/client.hpp>
//#include <mongocxx/stdx.hpp>
//#include <mongocxx/uri.hpp>
//#include <mongocxx/instance.hpp>

#include "geometry.h"
#include "polygon.h"
#include "protectedarea.h"
/*
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

int initMongo(){
  mongocxx::instance instance{};
  mongocxx::uri uri("mongodb://localhost:27017");
  mongocxx::client client(uri);
  mongocxx::database db = client["eco_region"];
  mongocxx::collection coll = db["test"];
  auto builder = bsoncxx::builder::stream::document{};
  bsoncxx::document::value doc_value = builder
  << "name" << "MongoDB"
  << "type" << "database"
  << "count" << 1
  << "versions" << bsoncxx::builder::stream::open_array
    << "v3.2" << "v3.0" << "v2.6"
  << close_array
  << "info" << bsoncxx::builder::stream::open_document
    << "x" << 203
    << "y" << 102
  << bsoncxx::builder::stream::close_document
  << bsoncxx::builder::stream::finalize;
  bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
  coll.insert_one(doc_value.view());
  return 0;
}

int initCsv(void *file = nullptr) {
  //example:
  io::CSVReader<3> in("ram.csv");
  in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
  std::string vendor; int size; double speed;
  while (in.read_row(vendor, size, speed)){
    // do stuff with the data
  }

  return 0;
}

int initProtectedArea(vector<Polygon *> &areas, char *file = nullptr){
  char *filedic = file;
  // the file directory is to be input
  SHPHandle fileHandle = SHPOpen(filedic, "rb");
  if (fileHandle == nullptr) return 1;
  int pnEntities, pnShapeType;
  double *padfMinBound, *padMaxBound;
  SHPGetInfo(fileHandle, &pnEntities, &pnShapeType, nullptr, nullptr);
  for (unsigned i = 0; i < pnEntities; i++){
    SHPObject *shpObj = SHPReadObject(fileHandle, i);
    std::cout<< *(shpObj->padfX)<<" "<<*(shpObj->padfY)<<std::endl;
    areas.push_back(
        new Polygon(shpObj->padfX, shpObj->padfY, shpObj->nVertices)
        );
  }
  SHPClose(fileHandle);
  return 0;
}
*/

int initProtectedArea(vector<ProtectedArea *> &areas, char *file = nullptr){
  char *filedic = file;
  // the file directory is to be input
  SHPHandle fileHandle = SHPOpen(filedic, "rb");
  if (fileHandle == nullptr) return 1;
  int pnEntities, pnShapeType;
  double *padfMinBound, *padMaxBound;
  SHPGetInfo(fileHandle, &pnEntities, &pnShapeType, nullptr, nullptr);
  for (unsigned i = 0; i < pnEntities; i++){
    SHPObject *shpObj = SHPReadObject(fileHandle, i);
    /*if (i==0) {
      std::cout<<"parts: "<<shpObj->nParts<<std::endl;
      for (int j=0;j<shpObj->nParts;j++){
        std::cout<<"part: "<<j<<" start index: "<<shpObj->panPartStart[j]<<" part type: "<<shpObj->panPartType[j]<<std::endl;
      }
    }*/
    ProtectedArea *area = new ProtectedArea("MO"+std::to_string(i));
    for (unsigned j=1;j<shpObj->nParts;j++){
      area->polygons.push_back(
          new Polygon(shpObj->padfX+shpObj->panPartStart[j-1]
              , shpObj->padfY+shpObj->panPartStart[j-1]
              , shpObj->panPartStart[j]-shpObj->panPartStart[j-1])
      );
    }
    area->polygons.push_back(
        new Polygon(shpObj->padfX+shpObj->panPartStart[shpObj->nParts-1]
            , shpObj->padfY+shpObj->panPartStart[shpObj->nParts-1]
            , shpObj->nVertices-shpObj->panPartStart[shpObj->nParts-1])
    );
    areas.push_back(area);
  }
  SHPClose(fileHandle);
  return 0;
}


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
/*
int outJson(vector<Polygon *> &areas, Grid &grid){
  std::ofstream os("/home/lzhan253/project/eco-region/preprocess/data/out.json", std::ofstream::out);
  os.precision(17);
  os << "[\n";
  for (unsigned i = 0; i < areas.size(); i++){
    ValueGrid value_grid(&grid);
    value_grid.getValue(*( areas[i] ));
std::cerr<<"add PA: "<<i<<std::endl;
    if (i > 0) os << ",\n";
    os << "{\"PA_id\": "<<i<<","<<"\"dist_set\": [";
    for (unsigned j = 0; j < grid.coords.size(); j++){
      if (j > 0) os << ",";
      os << "{" << "\"grid_id\":" << j << ",\"dist\":" << value_grid.dist[j] << "}";
        //<< grid.coords[j]->x << "," << grid.coords[j]->y << "," << value_grid.dist[j] << "]";
    }
    os << "]}";
   
    //value_grids.emplace_back(value_grid);
    //break;
    //add value_grid dist to mongodb
  }
  os << "]";
  os.close();
  return 0;
}

int txtArrayToJson(const char* file) {
  std::ofstream os("/home/lzhan253/project/eco-region/preprocess/data/array.json", std::ofstream::out);
  os.precision(17);
  os << "[\n";
  int total = 0;
  std::ifstream fin;
  fin.open(file);
  if (!fin.good()) return 1;
  double p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
 // char c;
  while (!fin.eof()) {
   // fin >> c;
    fin >> p1x >> p1y >> p2x >> p2y >> p3x >> p3y >> p4x >> p4y;
    if (total > 0) os << ",\n";
    os << "{" << "\"id\":" << total<<",";
    os << "\"coordinates\":[[" << p1y << "," << p1x << "],[" << p2y << "," << p2x << "],[" << p3y << "," << p3x << "],[" << p4y << "," << p4x << "]]}";
    total++;
  //  fin >> c;
    if (total > 159999) break;
  }
  os << "]";
  return 0;
}*/

#endif
