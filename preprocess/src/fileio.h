/*
shp file read and csv file read
currently synchronized io
*/
#pragma once
#include <shapelib-1.4.0\shapefil.h>
#include <fast-csv-parser\csv.h>
#include"pointandpolygon.h"

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

int initProtectedArea(vector<Polygon> &areas, char *file = nullptr){
  char *filedic = file;
  // the file directory is to be input
  SHPHandle fileHandle = SHPOpen(filedic, "rb");
  if (fileHandle == nullptr) return 1;
  int pnEntities, pnShapeType;
  double *padfMinBound, *padMaxBound;
  SHPGetInfo(fileHandle, &pnEntities, &pnShapeType, nullptr, nullptr);
  for (unsigned i = 0; i < pnEntities; i++){
    SHPObject *shpObj = SHPReadObject(fileHandle, i);
    areas.push_back(Polygon(shpObj->padfX, shpObj->padfY, shpObj->nVertices));
  }
  SHPClose(fileHandle);
  return 0;
}
