# eco-region

Preprocessing program for a geographic system in Vader lab ASU.

Read shp files and load data from Mongo and store shortest distance 
from the grids provided in data to the shapes.

To compile:

cmake .. -G "Visual Studio 14 Win64" -DBOOST_ROOT=c:\boost -DCMAKE_BUILD_TYPE=Release -DBSONCXX_POLY_USE_BOOST=1 -DCMAKE_INSTALL_PREFIX=C:\mongo-cxx-driver -DLIBMONGOC_DIR=C:\mongo-c-driver\ -DLIBBSON_DIR=C:\mongo-c-driver\ -DCMAKE_PREFIX_PATH=c:\mongo-c-driver 