mkdir -p ./build
cd ./build
cmake .. -G "Visual Studio 14 Win64" -DBOOST_ROOT=c:/boost -DCMAKE_BUILD_TYPE=Release -DBSONCXX_POLY_USE_BOOST=1 -DCMAKE_INSTALL_PREFIX=C:/mongo-cxx-driver -DLIBMONGOC_DIR=C:/mongo-c-driver/ -DLIBBSON_DIR=C:/mongo-c-driver/ -DCMAKE_PREFIX_PATH=c:/mongo-c-driver
cmake --build . --config Release
cd ..
mkdir -p ./install
cp ./build/src/Release/preprocess.exe ./install
cp ./bin/* ./install