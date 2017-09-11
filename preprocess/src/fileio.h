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

int initProtectedArea(vector<Polygon> &areas, void *file = nullptr){
  char *filedic = (char *)file;
  // the file directory is to be input
  while (1/*not end of file*/) {
    //read file
  }
  return 0;
}
