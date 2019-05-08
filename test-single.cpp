#include <vips/vips8>

using vips::VImage;

/**
 * Take care of the various mirror modes that `vips_autorot` doesn't support.
 * @param image Source image.
 * @return An image which is rotated/mirrored correctly.
 */
VImage autorot_enhanced(VImage &image, int exif_tag) {

    VipsAngle rotate = VIPS_ANGLE_D0;
    bool flip = false;
    bool flop = false;

    switch (exif_tag) {
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

    // Remove EXIF Orientation from image, if mirroring is required.
    if (flip || flop) {
        image.remove(VIPS_META_ORIENTATION);
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

    return image;
}

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    const std::string input_dir = "input";
    const int target_width = 320;
    
    // Landscape
    for (int i = 1; i <= 8; ++i) {
        int width = target_width;
        int height = VIPS_MAX_COORD;

        std::string input = input_dir + "/Landscape_" + std::to_string(i) + ".jpg";
        std::string output = "output/Landscape_" + std::to_string(i) + ".jpg";

        if (i == 5 || i == 7) {
            // Swap input width and height when rotating by 90 or 270 degrees.
            std::swap(width, height);
        }

        VImage thumb = VImage::thumbnail(input.c_str(), width,
                                         VImage::option()
                                             ->set("no_rotate", false)
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

        std::string input = input_dir + "/Portrait_" + std::to_string(i) + ".jpg";
        std::string output = "output/Portrait_" + std::to_string(i) + ".jpg";

        if (i == 5 || i == 7) {
            // Swap input width and height when rotating by 90 or 270 degrees.
            std::swap(width, height);
        }

        VImage thumb = VImage::thumbnail(input.c_str(), width,
                                         VImage::option()
                                             ->set("no_rotate", false)
                                             ->set("height", height)
                                             ->set("size", VIPS_SIZE_DOWN));

        thumb = autorot_enhanced(thumb, i);

        thumb.write_to_file(output.c_str(),
                            VImage::option()->set("strip", true)->set("Q", 85));
    }

    vips_shutdown();

    return 0;
}
