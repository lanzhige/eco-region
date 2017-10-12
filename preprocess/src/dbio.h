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
#include "protectedarea.h"
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

  int initMongo(){
    instance = Instance();
    uri = new mongocxx::uri("mongodb://localhost:27017");
    client = new mongocxx::client(*uri);
    db = (*client)["eco_region"];
  }

  int insertDB(const ProtectedArea *pa, string collection_name = "test") {
    mongocxx::collection coll = db[collection_name];
    for (int sp=0; sp<pa->cells.size();sp+=100000){
    auto builder = bsoncxx::builder::stream::document();
    //builder
    //  <<"name"<<pa->name;
    //  <<bsoncxx::builder::stream::finalize;
    auto in_array = bsoncxx::builder::stream::array();
    for (int i=0;i<pa->polygons.size();i++){
      in_array<<bsoncxx::builder::stream::open_array;
      for (int j=0;j<pa->polygons[i]->coords.size();j++){
        in_array<<bsoncxx::builder::stream::open_array
          <<pa->polygons[i]->coords[j]->x
          <<pa->polygons[i]->coords[j]->y
          <<bsoncxx::builder::stream::close_array;
      }
      in_array<<close_array;
    }
    //builder<<"polygons"<<in_array;
    // <<"cells"<<bsoncxx::builder::stream::open_array;
    auto after_array = array();
    for (int i=sp;i<pa->cells.size()&&i<sp+100000;i++){
      after_array<<bsoncxx::builder::stream::open_document
        <<"row"<<pa->cells[i]->row_num
        <<"col"<<pa->cells[i]->col_num
        <<"dist"<<pa->cells[i]->dist
        <<bsoncxx::builder::stream::close_document;
    }
    //builder<<"cells"<<after_array
    //    <<bsoncxx::builder::stream::finalize;    
    bsoncxx::document::value doc_value = builder
        <<"name"<<pa->name
        <<"polygons"<<in_array
        <<"cells"<<after_array
        <<bsoncxx::builder::stream::finalize;
    //    in_array<<bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
      coll.insert_one(doc_value.view());
    }
  }

/*  int insertDB(const ProtectedArea *pa, string collection_name = "test") {
    mongocxx::collection coll = db[collection_name];
    auto builder = bsoncxx::builder::stream::document();
    //builder
    //  <<"name"<<pa->name;
    //  <<bsoncxx::builder::stream::finalize;
    auto in_array = bsoncxx::builder::stream::array();
    for (int i=0;i<pa->polygons.size();i++){
      in_array<<bsoncxx::builder::stream::open_array;
      for (int j=0;j<pa->polygons[i]->coords.size();j++){
        in_array<<bsoncxx::builder::stream::open_array
          <<pa->polygons[i]->coords[j]->x
          <<pa->polygons[i]->coords[j]->y
          <<bsoncxx::builder::stream::close_array;
      }
      in_array<<close_array;
    }
    //builder<<"polygons"<<in_array;
    // <<"cells"<<bsoncxx::builder::stream::open_array;
    auto after_array = array();
    for (int i=0;i<pa->cells.size();i++){
      after_array<<bsoncxx::builder::stream::open_document
        <<"row"<<pa->cells[i]->row_num
        <<"col"<<pa->cells[i]->col_num
        <<"dist"<<pa->cells[i]->dist
        <<bsoncxx::builder::stream::close_document;
    }
    //builder<<"cells"<<after_array
    //    <<bsoncxx::builder::stream::finalize;    
    bsoncxx::document::value doc_value = builder
        <<"name"<<pa->name
        <<"polygons"<<in_array
        <<"cells"<<after_array
        <<bsoncxx::builder::stream::finalize;
    //    in_array<<bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
      coll.insert_one(doc_value.view());
  }
*/
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
