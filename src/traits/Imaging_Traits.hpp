
#include <stdint.h>
#include <cmath>
constexpr uint8_t convert_to_pixel(const double val) {
    return (uint8_t)(std::round(val) < 0 ? 0 : (std::round(val) > 255 ? 255 : std::round(val)));
}