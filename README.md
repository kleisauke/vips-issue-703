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

# Generate an animated WebP image from the output of `./test-single`
./test-anim

# https://github.com/libvips/pyvips/issues/148
VIPS_CONCURRENCY=1 VIPS_NOVECTOR=1 python3 test-resize.py input/radial.rgb.png
```

See [output/](output) directory.

## Debugging notes
<details>
  <summary>Notes</summary>

  Only shrink an image vertically (using only `reducev`):
  ```bash
  vips reducev input/Landscape_1.jpg output-patch/lanczos3/Landscape_1.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  vips reducev input/Landscape_2.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp.v output-patch/lanczos3/Landscape_2.jpg[strip,Q=85] horizontal

  vips reducev input/Landscape_3.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips rot temp.v output-patch/lanczos3/Landscape_3.jpg[strip,Q=85] d180

  vips reducev input/Landscape_4.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips rot temp.v temp2.v d180
  vips flip temp2.v output-patch/lanczos3/Landscape_4.jpg[strip,Q=85] horizontal

  vips rot input/Landscape_5.jpg temp.v d270
  vips reducev temp.v temp2.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp2.v output-patch/lanczos3/Landscape_5.jpg[strip,Q=85] vertical

  vips rot input/Landscape_6.jpg temp.v d90
  vips reducev temp.v output-patch/lanczos3/Landscape_6.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  vips rot input/Landscape_7.jpg temp.v d90
  vips reducev temp.v temp2.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp2.v output-patch/lanczos3/Landscape_7.jpg[strip,Q=85] vertical

  vips rot input/Landscape_8.jpg temp.v d270
  vips reducev temp.v output-patch/lanczos3/Landscape_8.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  rm {temp,temp2}.v
  ```

  Only shrink an image horizontally (using only `reduceh`):
  ```bash
  vips reduceh input/Landscape_1.jpg output-patch/lanczos3/Landscape_1.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  vips reduceh input/Landscape_2.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp.v output-patch/lanczos3/Landscape_2.jpg[strip,Q=85] horizontal

  vips reduceh input/Landscape_3.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips rot temp.v output-patch/lanczos3/Landscape_3.jpg[strip,Q=85] d180

  vips reduceh input/Landscape_4.jpg temp.v 5.624992969 --kernel lanczos3 --centre
  vips rot temp.v temp2.v d180
  vips flip temp2.v output-patch/lanczos3/Landscape_4.jpg[strip,Q=85] horizontal

  vips rot input/Landscape_5.jpg temp.v d270
  vips reduceh temp.v temp2.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp2.v output-patch/lanczos3/Landscape_5.jpg[strip,Q=85] vertical

  vips rot input/Landscape_6.jpg temp.v d90
  vips reduceh temp.v output-patch/lanczos3/Landscape_6.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  vips rot input/Landscape_7.jpg temp.v d90
  vips reduceh temp.v temp2.v 5.624992969 --kernel lanczos3 --centre
  vips flip temp2.v output-patch/lanczos3/Landscape_7.jpg[strip,Q=85] vertical

  vips rot input/Landscape_8.jpg temp.v d270
  vips reduceh temp.v output-patch/lanczos3/Landscape_8.jpg[strip,Q=85] 5.624992969 --kernel lanczos3 --centre

  rm {temp,temp2}.v
  ```
</details>

## Output

### Thumbnail
<details>
  <summary>Produces pixel shifts</summary>

  ![Portrait](output/lanczos3_Portrait.webp)
  ![Landscape](output/lanczos3_Landscape.webp)
</details>

### Shrink an image vertically
<details>
  <summary>Produces pixel shifts on the vertical axis</summary>

  ![Landscape-vertical](output/Landscape-vertical.webp)
</details>

### Shrink an image horizontally
<details>
  <summary>Produces pixel shifts on the horizontal axis</summary>

  ![Landscape-horizontal](output/Landscape-horizontal.webp)
</details>

### Upscale
<details>
  <summary>Upscale does not appear to produce pixel shifts</summary>

  ![Portrait](output/Portrait-enlarge.webp)
  ![Landscape](output/Landscape-enlarge.webp)
</details>

## Output after pull request

See [libvips/libvips#1592](https://github.com/libvips/libvips/pull/1592).

### Thumbnail
<details>
  <summary>Still produces pixel shifts</summary>

  ![Portrait](output-patch/lanczos3_Portrait.webp)
  ![Landscape](output-patch/lanczos3_Landscape.webp)
</details>

### Shrink an image vertically
<details>
  <summary>Still produces pixel shifts on the vertical axis</summary>

  ![Landscape-vertical](output-patch/Landscape-vertical.webp)
</details>

### Shrink an image horizontally
<details>
  <summary>Still produces pixel shifts on the horizontal axis</summary>

  ![Landscape-horizontal](output-patch/Landscape-horizontal.webp)
</details>
