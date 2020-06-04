#include <vips/vips8>

using vips::VImage;

/**
 * `vips_autorot` + the various mirror modes, see:
 * https://github.com/libvips/libvips/issues/1461
 * @param image Source image.
 * @return An image which is rotated/mirrored correctly.
 */
VImage autorot_enhanced(VImage &image, int exif_tag) {

    VipsAngle rotate = VIPS_ANGLE_D0;
    bool flip = false;
    bool flop = false;

    switch (exif_tag) {
    case 6:
        rotate = VIPS_ANGLE_D90;
        break;
    case 3:
        rotate = VIPS_ANGLE_D180;
        break;
    case 8:
        rotate = VIPS_ANGLE_D270;
        break;
    case 2: // flop 1
        flop = true;
        break;
    case 7: // flip 6
        flip = true;
        rotate = VIPS_ANGLE_D90;
        break;
    case 4: //  flop 3
        flop = true;
        rotate = VIPS_ANGLE_D180;
        break;
    case 5: // flip 8
        flip = true;
        rotate = VIPS_ANGLE_D270;
        break;
    default:
        break;
    }

    // Rotate if required.
    if (rotate != VIPS_ANGLE_D0) {
        // Need to copy to memory, we have to stay seq.
        image = image.copy_memory().rot(rotate);
    }

    // Flip (mirror about Y axis) if required.
    if (flip) {
        image = image.flipver();
    }

    // Flop (mirror about X axis) if required.
    if (flop) {
        image = image.fliphor();
    }

    // Removing metadata, need to copy the image
    auto copy = image.copy();

    copy.remove(VIPS_META_ORIENTATION);

    return copy;
}

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    const std::string input_dir = "input";
    const std::string output_dir = "output-patch/thumbnail-lanczos3";
    //const std::string output_dir = "output/thumbnail-lanczos3";
    const int target_width = 320;

    // Landscape
    for (int i = 1; i <= 8; ++i) {
        int width = target_width;
        int height = VIPS_MAX_COORD;

        std::string input =
            input_dir + "/Landscape_" + std::to_string(i) + ".jpg";
        std::string output =
            output_dir + "/Landscape_" + std::to_string(i) + ".jpg";

        if (i >= 5) {
            // Swap input width and height when rotating by 90 or 270 degrees.
            std::swap(width, height);
        }

        VImage thumb = VImage::thumbnail(input.c_str(), width,
                                         VImage::option()
                                             ->set("no_rotate", true)
                                             ->set("height", height)
                                             ->set("size", VIPS_SIZE_DOWN));

        thumb = autorot_enhanced(thumb, i);

        thumb.write_to_file(output.c_str(),
                            VImage::option()->set("strip", true)->set("Q", 85));
    }

    // Portrait
    for (int i = 1; i <= 8; ++i) {
        int width = target_width;
        int height = VIPS_MAX_COORD;

        std::string input =
            input_dir + "/Portrait_" + std::to_string(i) + ".jpg";
        std::string output =
            output_dir + "/Portrait_" + std::to_string(i) + ".jpg";

        if (i >= 5) {
            // Swap input width and height when rotating by 90 or 270 degrees.
            std::swap(width, height);
        }

        VImage thumb = VImage::thumbnail(input.c_str(), width,
                                         VImage::option()
                                             ->set("no_rotate", true)
                                             ->set("height", height)
                                             ->set("size", VIPS_SIZE_DOWN));

        thumb = autorot_enhanced(thumb, i);

        thumb.write_to_file(output.c_str(),
                            VImage::option()->set("strip", true)->set("Q", 85));
    }

    vips_shutdown();

    return 0;
}
