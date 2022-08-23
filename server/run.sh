[ -d "./build" ] && echo "Build dir found..." || (echo "Making build dir" && mkdir build && cd build && cmake .. && cd ..)
cd build
make
./streamerserver
cd ..
