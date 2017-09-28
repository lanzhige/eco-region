#ifndef DBIO_H_
#define DBIO_H_
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

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

#endif