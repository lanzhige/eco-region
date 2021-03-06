#ifndef DBIO_H_
#define DBIO_H_
#include <vector>
#include <string>

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/pool.hpp>
#include <json/json.h>
#include <cstdio>
#include <ctime>

#include "definition.h"
#include "geometry.h"
#include "protectedarea.h"
#include "geogrid.h"
#include "fileio.h"

#include "geo.h"

using std::vector;
using std::string;

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::array;

class DBio{
  //should be vector pointers to use multi-thread 
  mongocxx::uri *uri;
  mongocxx::client *client;
  mongocxx::database db;

public:  
  mongocxx::instance *instance; 
  mongocxx::instance *Instance(){
    if (instance == nullptr){
      instance = new mongocxx::instance();
    }
    return instance;
  }

  int initMongo() {
	  instance = Instance();
	  uri = new mongocxx::uri("mongodb://vaderserver0.cidse.dhcp.asu.edu:27017/");
	  client = new mongocxx::client(*uri);
	  db = (*client)["eco_region"];
	  std::cout << "db connected" << std::endl;
	  return 0;
  }

  int testIn(mpolygon_t &mpoly, string collection_name) {
	  std::cout << "gen bbox of polygons" << std::endl;
	  vector<model::box<spherical_point>> boxes;
	  boxes.resize(mpoly.size());
	  for (int i = 0; i < mpoly.size(); i++) {
		  envelope(mpoly[i], boxes[i]);
	  }
	  mongocxx::collection coll = db[collection_name];
	  mongocxx::collection coll_out = db["test"];
	  std::cout << "fuck the mongo lib" << std::endl;

	  std::cout << "setting cursor configuration" << std::endl;
	  mongocxx::options::find opts;
	  opts = opts.no_cursor_timeout(true);
	  opts = opts.allow_partial_results(true);
	  opts = opts.limit(700000000);
	  mongocxx::cursor cursor = coll.find(document{} << finalize, opts);
	  vector<bsoncxx::document::value> documents;
	  int count_document = 1;
	  int count = 0, total = 0;
	  double dist;
	  Json::Reader reader;
	  Json::Value root;
	  std::cout << "cursor read finished!" << std::endl;
	  string s, out_str;
	  std::clock_t start;
	  double duration;
	  Point_2d *p = new Point_2d(0, 0);
	  start = std::clock();
	  for (auto doc : cursor) {
		  s = bsoncxx::to_json(doc);
		  out_str = "";
		  total++;
		  if (reader.parse(s, root)) {
			  spherical_point p(root["properties"]["center"][0].asDouble()
			      , root["properties"]["center"][1].asDouble());
			  bool flag = true;
			  for (int i = 0; i < mpoly.size(); i++) {
				  if (within(p, boxes[i]) && within(p, mpoly[i])) {
					  flag = false;
					  break;
				  }
			  }

			  if (flag) {
				  Json::StyledWriter writer;
				  out_str = writer.write(root);
				  documents.push_back(
					  bsoncxx::document::value(bsoncxx::from_json(out_str))
				  );
				  count_document++;
				  if (count_document>10000) {
					  std::cout << "out put documents No." << count << " totally: " << total << std::endl;
					  count++;
					  count_document -= 10000;
					  coll_out.insert_many(documents);
					  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
					  std::cout << "time used: " << duration << std::endl;
					  documents.clear();
				  }
			  }
		  }
		  else {
			  std::cout << s << std::endl;
			  std::cout << "Failed to parse" << reader.getFormatedErrorMessages() << std::endl;
		  }
	  }
	  delete p;
	  if (documents.size() > 0) {
		  coll_out.insert_many(documents);
	  }
	  return 0;
  }

  int testDist(vector<ProtectedArea *> &areas,
	  vector<vector<Geogrid*>> &geo_grid, string collection_name) {
	  mongocxx::collection coll = db["test"];
	  mongocxx::collection coll_out = db[collection_name];
	  std::cout << "fuck the mongo lib" << std::endl;
	  for (int i = 0; i < (GRID_LARGEST_LAT - GRID_ORIGION_LAT) / TEN_KM_LAT + 1;
		  i++) {
		  vector<Geogrid*> temp(0, nullptr);
		  for (int j = 0;
			  j < (GRID_LARGEST_LNG - GRID_ORIGION_LNG) / TEN_KM_LNG + 1; j++) {
			  Geogrid* grid = new Geogrid();
			  temp.push_back(grid);
		  }
		  geo_grid.push_back(temp);
	  }

	  for (int i = 0; i < areas.size(); i++) {
		  for (int j = 0; j < geo_grid.size(); j++) {
			  for (int k = 0; k < geo_grid[j].size(); k++) {
				  Point_2d p(GRID_ORIGION_LNG + TEN_KM_LNG*k + TEN_KM_LNG / 2
					  , GRID_ORIGION_LAT + TEN_KM_LAT*j + TEN_KM_LAT / 2);
				  if (areas[i]->insidePA(&p)) {
					  double dist;
					  for (int l = 0; l < areas[i]->polygons.size(); l++) {
						  for (int m = 1; m < areas[i]->polygons[l]->coords.size(); m++) {
							  dist = p.toSegDist(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]);
							  if (dist <= INNER_RADIUS) geo_grid[j][k]->inner.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
							  else if (dist <= MIDDLE_RADIUS) geo_grid[j][k]->middle.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
							  else if (dist <= OUTTER_RADIUS) geo_grid[j][k]->outter.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
						  }
					  }
				  }
			  }
		  }
	  }

	  std::cout << "setting cursor configuration" << std::endl;
	  mongocxx::options::find opts;
	  opts = opts.no_cursor_timeout(true);
	  opts = opts.allow_partial_results(true);
	  opts = opts.limit(700000000);
	  mongocxx::cursor cursor = coll.find(document{} << finalize, opts);
	  vector<bsoncxx::document::value> documents;
	  int count_document = 1;
	  int count = 0, total = 0;
	  double dist;
	  Json::Reader reader;
	  Json::Value root;
	  std::cout << "cursor read finished!" << std::endl;
	  string s, out_str;
	  std::clock_t start;
	  double duration;
	  double x, y;
	  Point_2d *p = new Point_2d(0, 0);
	  start = std::clock();
	  for (auto doc : cursor) {
		  s = bsoncxx::to_json(doc);
		  out_str = "";
		  total++;
		  if (reader.parse(s, root)) {
			  root.removeMember("_id");
			  p->x = root["properties"]["center"][0].asDouble();
			  p->y = root["properties"]["center"][1].asDouble();
			  dist = MAX_DIST;

			  int row = static_cast<int>(std::floor((p->y - GRID_ORIGION_LAT) / TEN_KM_LAT));
			  int col = static_cast<int>(std::floor((p->x - GRID_ORIGION_LNG) / TEN_KM_LNG));
			  dist = min(dist, geo_grid[row][col]->shortestDistance(p));

			  if (dist <= MAX_THRES&&dist >= 0) {
				  Json::StyledWriter writer;
				  root["properties"]["dist"] = dist;
				  out_str = writer.write(root);
				  documents.push_back(
					  bsoncxx::document::value(bsoncxx::from_json(out_str))
				  );
				  count_document++;
				  if (count_document>10000) {
					  std::cout << "out put documents No." << count << " totally: " << total << std::endl;
					  count++;
					  count_document -= 10000;
					  coll_out.insert_many(documents);
					  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
					  std::cout << "time used: " << duration << std::endl;
					  documents.clear();
				  }
			  }
		  }
		  else {
			  std::cout << s << std::endl;
			  std::cout << "Failed to parse" << reader.getFormatedErrorMessages() << std::endl;
		  }
	  }
	  delete p;
	  if (documents.size() > 0) {
		  coll_out.insert_many(documents);
	  }
	  return 0;
  }

  int dbfind(vector<ProtectedArea *> &areas,
	  vector<vector<Geogrid*>> &geo_grid,
      string syntax = "{}", string collection_name = "grid"){
    mongocxx::collection coll = db["grid_Washington"];
	mongocxx::collection coll_out = db["test"];
	std::cout << "fuck the mongo lib" << std::endl;
	for (int i = 0; i < (GRID_LARGEST_LAT - GRID_ORIGION_LAT) / TEN_KM_LAT + 1;
		i++) {
		vector<Geogrid*> temp(0, nullptr);
		for (int j = 0;
			j < (GRID_LARGEST_LNG - GRID_ORIGION_LNG) / TEN_KM_LNG + 1; j++) {
			Geogrid* grid = new Geogrid();
			temp.push_back(grid);
		}
		geo_grid.push_back(temp);
	}

	for (int i = 0; i < areas.size(); i++) {
		for (int j = 0; j < geo_grid.size(); j++) {
			for (int k = 0; k < geo_grid[j].size(); k++) {
				Point_2d p(GRID_ORIGION_LNG + TEN_KM_LNG*k + TEN_KM_LNG / 2
					, GRID_ORIGION_LAT + TEN_KM_LAT*j + TEN_KM_LAT / 2);
				if (areas[i]->insidePA(&p)) {
					double dist;
					for (int l = 0; l < areas[i]->polygons.size(); l++) {
						for (int m = 1; m < areas[i]->polygons[l]->coords.size(); m++) {
							dist = p.toSegDist(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]);
							if (dist <= INNER_RADIUS) geo_grid[j][k]->inner.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
							else if (dist <= MIDDLE_RADIUS) geo_grid[j][k]->middle.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
							else if (dist <= OUTTER_RADIUS) geo_grid[j][k]->outter.push_back(std::make_pair(areas[i]->polygons[l]->coords[m - 1], areas[i]->polygons[l]->coords[m]));
						}
					}
				}
			}
		}
	}

	std::cout << "setting cursor configuration" << std::endl;
	mongocxx::options::find opts;
	opts=opts.no_cursor_timeout(true);
	opts=opts.allow_partial_results(true);
	opts = opts.limit(700000000);
	mongocxx::cursor cursor = coll.find(document{}<<finalize,opts);
    vector<bsoncxx::document::value> documents;
    int count_document=1;
    int count=0, total=0;
	double dist;
	Json::Reader reader;
	Json::Value root;
	std::cout << "cursor read finished!" << std::endl;
	string s, out_str;
	std::clock_t start;
	double duration;
	double x, y;
	Point_2d *p=new Point_2d(0, 0);
	start = std::clock();
    for (auto doc : cursor) {
	  //if (count > 10) break;
	  s=bsoncxx::to_json(doc);
      out_str="";
	  total++;
      if (reader.parse(s,root)) {
		root.removeMember("_id");
        p->x=root["properties"]["center"][0].asDouble();
        p->y=root["properties"]["center"][1].asDouble();
        dist = MAX_DIST;

		int test_flag = 0;

        for (int i=0; i<areas.size(); i++){
          if (!areas[i]->insidePA(p)) continue;
		  bool flag = false;
          for (int j=0; j<areas[i]->polygons.size(); j++){
            if (areas[i]->polygons[j]->contain(*p)) {
				flag = !flag;
				test_flag++;
            }
          }
		  if (flag) {
			  dist = -1.0;
			  break;
		  }
        }
		if (dist > 0&&test_flag>0) {
			int row = static_cast<int>(std::floor((p->y - GRID_ORIGION_LAT) / TEN_KM_LAT));
			int col = static_cast<int>(std::floor((p->x - GRID_ORIGION_LNG) / TEN_KM_LNG));
			dist = min(dist, geo_grid[row][col]->shortestDistance(p));
		}
		else continue;

        if (dist<=MAX_THRES&&dist>=0) {
          Json::StyledWriter writer;
          root["properties"]["dist"]=dist;
          out_str = writer.write(root);
          documents.push_back(
              bsoncxx::document::value(bsoncxx::from_json(out_str))
              );
          count_document++;
          if (count_document>10000){
            std::cout<<"out put documents No."<<count<<" totally: "<<total<<std::endl;
            count++;
            count_document-=10000;
            coll_out.insert_many(documents);
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			std::cout << "time used: " << duration << std::endl;
			documents.clear();
          }
        }
	  }
	  else {
		  std::cout << s << std::endl;
		  std::cout << "Failed to parse" << reader.getFormatedErrorMessages()<< std::endl;
	  }
    }
	delete p;
	if (documents.size() > 0) {
		coll_out.insert_many(documents);
	}
    return 0;
  }

  int dbload(string collection_name = "test"){
  //mongocxx::instance instance{};
    mongocxx::collection coll = db[collection_name];
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
};

#endif
