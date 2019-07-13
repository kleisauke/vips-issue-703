#!/usr/bin/env bash

if ! [[ -x "$(command -v vips)" ]]; then
  echo 'Error: vips is not installed.' >&2
  exit 125
fi

if ! [[ -x "$(command -v convert)" ]]; then
  echo 'Error: ImageMagick6 is not installed.' >&2
  exit 125
fi

# convert -define jpeg:size=1800x1200 Landscape_1.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_1.jpg
# convert -define jpeg:size=1800x1200 Landscape_2.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_2.jpg
# convert -define jpeg:size=1800x1200 Landscape_3.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_3.jpg
# convert -define jpeg:size=1800x1200 Landscape_4.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_4.jpg
# convert -define jpeg:size=1800x1200 Landscape_5.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_5.jpg
# convert -define jpeg:size=1800x1200 Landscape_6.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_6.jpg
# convert -define jpeg:size=1800x1200 Landscape_7.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_7.jpg
# convert -define jpeg:size=1800x1200 Landscape_8.jpg -auto-orient \
#         -thumbnail 320x480 -quality 85 tn_Landscape_8.jpg

vipsthumbnail Landscape_1.jpg -s 320x480 -f tn_%s.jpg &> /dev/null \
  || vipsthumbnail Landscape_1.jpg -s 320 -f tn_%s.jpg &> /dev/null \
  || vipsthumbnail Landscape_1.jpg -s 320 -o tn_%s.jpg \
  || exit 125;

vipsthumbnail Landscape_2.jpg -s 320x480 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_2.jpg -s 320 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_2.jpg -s 320 -o temp.v \
  || exit 125;
vips flip temp.v tn_Landscape_2.jpg horizontal &> /dev/null \
  || vips im_fliphor temp.v tn_Landscape_2.jpg \
  || exit 125;

vipsthumbnail Landscape_3.jpg -s 320x480 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_3.jpg -s 320 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_3.jpg -s 320 -o temp.v \
  || exit 125;
vips rot temp.v tn_Landscape_3.jpg d180 &> /dev/null \
  || vips rot temp.v tn_Landscape_3.jpg 180 &> /dev/null \
  || vips im_rot180 temp.v tn_Landscape_3.jpg \
  || exit 125;

vipsthumbnail Landscape_4.jpg -s 320x480 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_4.jpg -s 320 -f temp.v &> /dev/null \
  || vipsthumbnail Landscape_4.jpg -s 320 -o temp.v \
  || exit 125;
vips rot temp.v temp2.v d180 &> /dev/null \
  || vips rot temp.v temp2.v 180 &> /dev/null \
  || vips im_rot180 temp.v temp2.v \
  || exit 125;
vips flip temp2.v tn_Landscape_4.jpg horizontal &> /dev/null \
  || vips im_fliphor temp2.v tn_Landscape_4.jpg \
  || exit 125;

vips rot Landscape_5.jpg temp.v d270 &> /dev/null \
  || vips rot Landscape_5.jpg temp.v 270 &> /dev/null \
  || vips im_rot270 Landscape_5.jpg temp.v \
  || exit 125;
vipsthumbnail temp.v -s 320x480 -f temp2.v &> /dev/null \
  || vipsthumbnail temp.v -s 320 -f temp2.v &> /dev/null \
  || vipsthumbnail temp.v -s 320 -o temp2.v \
  || exit 125;
vips flip temp2.v tn_Landscape_5.jpg vertical &> /dev/null \
  || vips im_flipver temp2.v tn_Landscape_5.jpg \
  || exit 125;

vips rot Landscape_6.jpg temp.v d90 &> /dev/null \
  || vips rot Landscape_6.jpg temp.v 90 &> /dev/null \
  || vips im_rot90 Landscape_6.jpg temp.v \
  || exit 125;
vipsthumbnail temp.v -s 320x480 -f tn_Landscape_6.jpg &> /dev/null \
  || vipsthumbnail temp.v -s 320 -f tn_Landscape_6.jpg &> /dev/null \
  || vipsthumbnail temp.v -s 320 -o tn_Landscape_6.jpg \
  || exit 125;

vips rot Landscape_7.jpg temp.v d90 &> /dev/null \
  || vips rot Landscape_7.jpg temp.v 90 &> /dev/null \
  || vips im_rot90 Landscape_7.jpg temp.v \
  || exit 125;
vipsthumbnail temp.v -s 320x480 -f temp2.v &> /dev/null \
  || vipsthumbnail temp.v -s 320 -f temp2.v &> /dev/null \
  || vipsthumbnail temp.v -s 320 -o temp2.v \
  || exit 125;
vips flip temp2.v tn_Landscape_7.jpg vertical &> /dev/null \
  || vips im_flipver temp2.v tn_Landscape_7.jpg \
  || exit 125;

vips rot Landscape_8.jpg temp.v d270 &> /dev/null \
  || vips rot Landscape_8.jpg temp.v 270 &> /dev/null \
  || vips im_rot270 Landscape_8.jpg temp.v \
  || exit 125;
vipsthumbnail temp.v -s 320x480 -f tn_Landscape_8.jpg &> /dev/null \
  || vipsthumbnail temp.v -s 320 -f tn_Landscape_8.jpg &> /dev/null \
  || vipsthumbnail temp.v -s 320 -o tn_Landscape_8.jpg \
  || exit 125;

rm temp.v temp2.v

# `vips colourspace [IN] [OUT] b-w` isn't available in all versions.
# So, we'll use magick instead:
convert tn_Landscape_1.jpg -colorspace Gray tn_Landscape_1-bw.jpg

expected=$(vips avg "tn_Landscape_1-bw.jpg" 2> /dev/null \
             || vips im_avg "tn_Landscape_1-bw.jpg" 2> /dev/null)
expected=$(printf "%.1f" "$expected")
if [[ $? -ne 0 ]]; then exit 125; fi  # avg is broken, skip.
echo "$expected"

exit_code=0

for i in 2 3 4 5 6 7 8
do
  convert "tn_Landscape_$i.jpg" -colorspace Gray "tn_Landscape_$i-bw.jpg"

  actual=$(vips avg "tn_Landscape_$i-bw.jpg" 2> /dev/null \
             || vips im_avg "tn_Landscape_$i-bw.jpg" 2> /dev/null)
  actual=$(printf "%.1f" "$actual")
  echo "$actual"

  if (( $(echo "$actual != $expected" | bc -l) )); then
    # exit_code=1
    exit_code=125
  fi
done

rm tn_Landscape*

exit ${exit_code}
