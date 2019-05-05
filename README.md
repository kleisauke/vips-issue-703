# libvips issue #703

Test case for [libvips/libvips#703](https://github.com/libvips/libvips/issues/703).

The example images for the various EXIF orientation flags are taken from this repo:
https://github.com/recurser/exif-orientation-examples

## Build
```bash
git clone https://github.com/kleisauke/vips-issue-703.git
cd vips-issue-703
mkdir build && cd build
cmake .. \
  -DCMAKE_BUILD_TYPE=Release
make
```

Or if you prefer to build without CMake:
```bash
git clone https://github.com/kleisauke/vips-issue-703.git
cd vips-issue-703
g++ -g -Wall test-anim.cpp `pkg-config vips-cpp --cflags --libs` -o test-anim
g++ -g -Wall test-single.cpp `pkg-config vips-cpp --cflags --libs` -o test-single
```

## Run
```bash
G_MESSAGES_DEBUG=all ./test-single
G_MESSAGES_DEBUG=all ./test-anim
```

See [output/](output) directory.

## Output
![Landscape](output/Landscape.webp)
![Portrait](output/Portrait.webp)
