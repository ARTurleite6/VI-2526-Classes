alias b := build
alias r := run

build:
  cmake --build build -j16

run: build
  ./build/VI-RT && open image.ppm

clean:
  cmake --build build --target clean
