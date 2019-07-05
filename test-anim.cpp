#include <vips/vips8>

using vips::VImage;

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    std::vector<VImage> landscape_images{};

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
        VImage::arrayjoin(landscape_images, VImage::option()->set("across", 1));

    landscape.set(VIPS_META_PAGE_HEIGHT, VIPS_RINT(landscape.height() / 8.0));
    landscape.set(VIPS_META_N_PAGES, 8);
    landscape.set("gif-delay", 20);
    landscape.write_to_file(output_landscape.c_str(),
                            VImage::option()->set("strip", true)->set("Q", 85));

    std::vector<VImage> portrait_images{};

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
        VImage::arrayjoin(portrait_images, VImage::option()->set("across", 1));

    portrait.set(VIPS_META_PAGE_HEIGHT, VIPS_RINT(portrait.height() / 8.0));
    portrait.set(VIPS_META_N_PAGES, 8);
    portrait.set("gif-delay", 20);
    portrait.write_to_file(output_portrait.c_str(),
                           VImage::option()->set("strip", true)->set("Q", 85));

    vips_shutdown();

    return 0;
}
