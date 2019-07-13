#!/usr/bin/env bash

# Example:
# git reset --hard 044f8b74f6103fd36864e54af18fafd1323dffbb
# git clean -dfx
# git bisect start
# git bisect bad
# git bisect good fa7d5939f5e8462ca045f42854b7706645e18802
# git bisect run /home/kleisauke/vips-issue-703/input/unlabelled/bisect-test.sh > /home/kleisauke/bisect-output2.txt

vips_dir=$(pwd)

export CFLAGS="-O3"
export CXXFLAGS="-O3"
export VIPS_WARNING=1

# Workaround for: `automake: error: cannot open < doc/gtk-doc.make`
cat <<EOF >>gtk-doc.make
# We need dummy variables so we don't fail if someone appends to them
# with +=
GPATH=
TARGET_DIR=
SETUP_FILES=
EXTRA_DIST=
DOC_STAMPS=
SCANOBJ_FILES=
REPORT_FILES=
CLEANFILES=
EOF

# Workaround for: `cp: cannot create regular file './m4/': Not a directory`
mkdir -p m4

# Only enable lcms2, libjpeg(-turbo), libexif and liborc
OPTIONS=( "prefix=/usr" 
          "enable-debug=no"
          "disable-debug"
          "disable-gtk-doc"
          "disable-introspection"
          "without-analyze"
          "without-cfitsio"
          "without-cimg"
          "without-fftw"
          "without-giflib"
          "without-gsf"
          "without-heif"
          "without-imagequant"
          "without-liboil"
          "without-libwebp"
          "without-magick"
          "without-matio"
          "without-nifti"
          "without-OpenEXR"
          "without-openslide"
          "without-pangoft2"
          "without-pdfium"
          "without-png"
          "without-poppler"
          "without-ppm"
          "without-python"
          "without-radiance"
          "without-rsvg"
          "without-tiff"
          "without-v4l"
          "without-zip"
          "without-zlib"
          )

./autogen.sh "${OPTIONS[@]/#/--}" &> /dev/null \
  || ./bootstrap.sh &> /dev/null \
  && ./configure "${OPTIONS[@]/#/--}" &> /dev/null

make -j$(nproc) &> /dev/null
make install > /dev/null

# Run the actual test
# If it exits with code 0, it means that it was successful
# If it exits with code 125, it means that it should be skipped
# If it exits with any other code, it means that it failed

# TODO: Arghh, hard-coded paths
cd /home/kleisauke/vips-issue-703/input/unlabelled/
./test.sh

exit_code=$?
echo "Exit code: $exit_code"

# Cleanup
cd "$vips_dir"
make uninstall &> /dev/null
make clean &> /dev/null

git reset --quiet --hard HEAD
git clean --quiet -dfx

exit ${exit_code}
