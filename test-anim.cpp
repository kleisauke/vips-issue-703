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

    VImage landscape = VImage::black(320, 213 * 8);

    // Landscape
    for (int i = 1; i <= 8; ++i) {
        int width = 320;
        int height = 213;

        std::string input = "input/Landscape_" + std::to_string(i) + ".jpg";

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

        landscape = landscape.insert(thumb, 0, 213 * (i - 1));
    }

    landscape.set(VIPS_META_PAGE_HEIGHT, 213);
    landscape.set(VIPS_META_N_PAGES, 8);
    landscape.set("gif-delay", 20);
    landscape.write_to_file("output/Landscape.webp",
                            VImage::option()->set("strip", true)->set("Q", 85));

    VImage portrait = VImage::black(320, 480 * 8);

    // Portrait
    for (int i = 1; i <= 8; ++i) {
        int width = 320;
        int height = 480;

        std::string input = "input/Portrait_" + std::to_string(i) + ".jpg";

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

        portrait = portrait.insert(thumb, 0, 480 * (i - 1));
    }

    portrait.set(VIPS_META_PAGE_HEIGHT, 480);
    portrait.set(VIPS_META_N_PAGES, 8);
    portrait.set("gif-delay", 20);
    portrait.write_to_file("output/Portrait.webp",
                           VImage::option()->set("strip", true)->set("Q", 85));

    vips_shutdown();

    return 0;
}
