#include <iostream>
#include <vips/vips8>
#include <vips/vector.h>

using vips::VImage;
using vips::VError;

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

/**
 * Process the image.
 * @param input_file Input file.
 * @param output_file Output file.
 * @param resize_kernel Resize kernel.
 */
void process(const std::string &input_file, const std::string &output_file,
             VipsKernel resize_kernel) {
    const int target_width = 450;

    VImage image = VImage::new_from_file(
        input_file.c_str(),
        VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

    int exif_tag = image.get_int(VIPS_META_ORIENTATION);

    // Convert to the processing space
    image = image.colourspace(VIPS_INTERPRETATION_sRGB);

    int width = image.width();
    int height = image.height();

    if (exif_tag >= 5) {
        // Swap input width and height when rotating by 90 or 270 degrees.
        std::swap(width, height);
    }

    // Fixed width
    double hshrink =
        static_cast<double>(width) / static_cast<double>(target_width);

    // Auto height
    double vshrink = hshrink;

    try {
        image = image.resize(1.0 / hshrink, VImage::option()
                                                ->set("vscale", 1.0 / vshrink)
                                                ->set("kernel", resize_kernel));
    } catch (VError &error) {
        std::cerr << "Resizing `" << input_file << "` to a width of `"
                  << target_width << "` with kernel `"
                  << vips_enum_string(VIPS_TYPE_KERNEL, resize_kernel)
                  << "` will produce pixels shifts." << std::endl;
    }

    if (image.get_typeof(VIPS_META_ICC_NAME) != 0) {
        // We're in device space and we need a combined import/export to
        // transform to the target space
        image = image.icc_transform(
            "srgb", VImage::option()
                        // Fallback to srgb
                        ->set("input_profile", "srgb")
                        // Use "perceptual" intent to better match imagemagick
                        ->set("intent", VIPS_INTENT_PERCEPTUAL)
                        ->set("embedded", true));
    }

    // Orient image
    image = autorot_enhanced(image, exif_tag);

    image.write_to_file(output_file.c_str(),
                        VImage::option()->set("strip", true)->set("Q", 85));
}

int main(int argc, const char *argv[]) {
    if (vips_init(argv[0]) != 0) {
        vips_error_exit(nullptr);
    }

    const std::string input_dir = "input";
    const std::string output_dir = "output-patch";
    /*const std::string output_dir = "output";*/

    vips_vector_set_enabled(0);

    std::vector<std::string> orientations = {"Landscape", "Portrait"};

    std::vector<VipsKernel> kernels = {
        VIPS_KERNEL_NEAREST,  VIPS_KERNEL_LINEAR,   VIPS_KERNEL_CUBIC,
        VIPS_KERNEL_MITCHELL, VIPS_KERNEL_LANCZOS2, VIPS_KERNEL_LANCZOS3};

    for (VipsKernel kernel : kernels) {
        std::string kernel_str = vips_enum_nick(VIPS_TYPE_KERNEL, kernel);

        for (const std::string &orientation : orientations) {
            for (int i = 1; i <= 8; ++i) {
                std::string exif_tag = std::to_string(i);
                std::string input =
                    input_dir + "/" + orientation + "_" + exif_tag + ".jpg";
                std::string output = output_dir + "/" + kernel_str + "/" +
                                     orientation + "_" + exif_tag + ".jpg";

                process(input, output, kernel);
            }
        }
    }

    vips_shutdown();

    return 0;
}
