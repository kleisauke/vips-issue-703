#include <vips/vips8>

/**
 * Performs a compile-time version check for libvips.
 */
#define VIPS_VERSION_AT_LEAST(major, minor, patch)                             \
    (((major) < VIPS_MAJOR_VERSION) ||                                         \
     ((major) == VIPS_MAJOR_VERSION && (minor) < VIPS_MINOR_VERSION) ||        \
     ((major) == VIPS_MAJOR_VERSION && (minor) == VIPS_MINOR_VERSION &&        \
      (patch) <= VIPS_MICRO_VERSION))

using vips::VImage;

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    const int delayMs = 200;

#if VIPS_VERSION_AT_LEAST(8, 9, 0)
    std::vector<int> delays(8, delayMs);
#endif

    std::vector<VImage> landscape_images;
    landscape_images.reserve(8);

    const std::string output_dir = "output-patch";
    /*const std::string output_dir = "output";*/

    // Landscape
    for (int i = 1; i <= 8; ++i) {
        std::string input =
            output_dir + "/Landscape_" + std::to_string(i) + ".jpg";

        landscape_images.emplace_back(VImage::new_from_file(
            input.c_str(),
            VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL)));
    }

    const std::string output_landscape = output_dir + "/Landscape.webp";

    VImage landscape =
        VImage::arrayjoin(landscape_images, VImage::option()->set("across", 1))
            .copy();

    landscape.set(VIPS_META_PAGE_HEIGHT, VIPS_RINT(landscape.height() / 8.0));
    landscape.set(VIPS_META_N_PAGES, 8);
#if VIPS_VERSION_AT_LEAST(8, 9, 0)
    // Multiple delay values are supported, set an array of ints instead
    landscape.set("delay", delays);
#else
    // Multiple delay values are not supported, set the gif-delay field.
    // Note: this is centiseconds (the GIF standard).
    landscape.set("gif-delay", std::rint(delayMs / 10.0));
#endif
    landscape.write_to_file(output_landscape.c_str(),
                            VImage::option()->set("strip", true)->set("Q", 85));

    std::vector<VImage> portrait_images;
    portrait_images.reserve(8);

    // Portrait
    for (int i = 1; i <= 8; ++i) {
        std::string input =
            output_dir + "/Portrait_" + std::to_string(i) + ".jpg";

        portrait_images.emplace_back(VImage::new_from_file(
            input.c_str(),
            VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL)));
    }

    const std::string output_portrait = output_dir + "/Portrait.webp";

    VImage portrait =
        VImage::arrayjoin(portrait_images, VImage::option()->set("across", 1))
            .copy();

    portrait.set(VIPS_META_PAGE_HEIGHT, VIPS_RINT(portrait.height() / 8.0));
    portrait.set(VIPS_META_N_PAGES, 8);
#if VIPS_VERSION_AT_LEAST(8, 9, 0)
    // Multiple delay values are supported, set an array of ints instead
    portrait.set("delay", delays);
#else
    // Multiple delay values are not supported, set the gif-delay field.
    // Note: this is centiseconds (the GIF standard).
    portrait.set("gif-delay", std::rint(delayMs / 10.0));
#endif
    portrait.write_to_file(output_portrait.c_str(),
                           VImage::option()->set("strip", true)->set("Q", 85));

    vips_shutdown();

    return 0;
}
