#include <vips/vips8>

using vips::VImage;

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    std::vector<VImage> landscape_images{};

    // Landscape
    for (int i = 1; i <= 8; ++i) {
        std::string input = "output/Landscape_" + std::to_string(i) + ".jpg";

        landscape_images.emplace_back(VImage::new_from_file(
            input.c_str(),
            VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL)));
    }

    VImage landscape =
        VImage::arrayjoin(landscape_images, VImage::option()->set("across", 1));

    landscape.set(VIPS_META_PAGE_HEIGHT, 213);
    landscape.set(VIPS_META_N_PAGES, 8);
    landscape.set("gif-delay", 20);
    landscape.write_to_file("output/Landscape.webp",
                            VImage::option()->set("strip", true)->set("Q", 85));

    std::vector<VImage> portrait_images{};

    // Portrait
    for (int i = 1; i <= 8; ++i) {
        std::string input = "output/Portrait_" + std::to_string(i) + ".jpg";

        portrait_images.emplace_back(VImage::new_from_file(
            input.c_str(),
            VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL)));
    }

    VImage portrait =
        VImage::arrayjoin(portrait_images, VImage::option()->set("across", 1));

    portrait.set(VIPS_META_PAGE_HEIGHT, 480);
    portrait.set(VIPS_META_N_PAGES, 8);
    portrait.set("gif-delay", 20);
    portrait.write_to_file("output/Portrait.webp",
                           VImage::option()->set("strip", true)->set("Q", 85));

    vips_shutdown();

    return 0;
}
