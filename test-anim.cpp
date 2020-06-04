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

    const std::string output_dir = "output-patch";
    /*const std::string output_dir = "output";*/

    const int delayMs = 200;

#if VIPS_VERSION_AT_LEAST(8, 9, 0)
    std::vector<int> delays(8, delayMs);
#endif

    std::vector<std::string> orientations = {"Landscape", "Portrait"};

    std::vector<std::string> kernels = {"nearest",  "linear",   "cubic",
                                        "mitchell", "lanczos2", "lanczos3",
                                        "thumbnail-lanczos3"};

    for (const std::string &kernel : kernels) {
        for (const std::string &orientation : orientations) {
            std::string output =
                output_dir + "/" + kernel + "_" + orientation + ".webp";

            std::vector<VImage> images;
            images.reserve(8);

            for (int i = 1; i <= 8; ++i) {
                std::string exif_tag = std::to_string(i);
                std::string input = output_dir + "/" + kernel + "/" +
                                    orientation + "_" + exif_tag + ".jpg";

                images.emplace_back(VImage::new_from_file(
                    input.c_str(),
                    VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL)));
            }

            VImage image =
                VImage::arrayjoin(images, VImage::option()->set("across", 1))
                    .copy();

            image.set(VIPS_META_PAGE_HEIGHT, VIPS_RINT(image.height() / 8.0));
            image.set(VIPS_META_N_PAGES, 8);
#if VIPS_VERSION_AT_LEAST(8, 9, 0)
            // Multiple delay values are supported, set an array of ints instead
            image.set("delay", delays);
#else
            // Multiple delay values are not supported, set the gif-delay field.
            // Note: this is centiseconds (the GIF standard).
            portrait.set("gif-delay", std::rint(delayMs / 10.0));
#endif
            image.write_to_file(
                output.c_str(),
                VImage::option()->set("strip", true)->set("Q", 85));
        }
    }

    vips_shutdown();

    return 0;
}
