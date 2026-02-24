build:
  cmake --build build

run: build
  ./build/VI-RT && open image.ppm
