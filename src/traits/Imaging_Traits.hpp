
#include <stdint.h>
#include <cmath>

constexpr uint8_t convert_to_pixel(const double val) {
    return (uint8_t)(std::round(val) < 0 ? 0 : (std::round(val) > 255 ? 255 : std::round(val)));
}
const std::string prefix = "../medical_images/stanford/cthead-8bit0";
const std::string format = ".jpg";
enum class ImageType {
    PNG, JPG, BMP, TGA
};

constexpr int noisy_start_idx = 75;
constexpr int noisy_end_idx = 78;