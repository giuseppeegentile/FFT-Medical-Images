
#include <stdint.h>
#include <cmath>


/**
 * @brief Convert to pixel value (uint8_t) from double. FFT operation is performed on complex numbers. The real part is a double.
 *  Convert the output into a uint8_t, which is the pixel of a channel
 * 
 * @param val the double value to be converted
 * @return constexpr uint8_t 
 */
constexpr uint8_t convert_to_pixel(const double val) {
    return (uint8_t)(std::round(val) < 0 ? 0 : (std::round(val) > 255 ? 255 : std::round(val)));
}
const std::string prefix = "../medical_images/stanford/cthead-8bit0";
const std::string format = ".jpg";

/**
 * @brief Types to write on output file
 * 
 */
enum class ImageType {
    PNG, JPG, BMP, TGA
};
/**
 * @brief Channels of all images (greyscale = 2)
 * 
 */
constexpr uint8_t channel_num = 2;     

/**
 * @brief Width and heigth of dataset images
 * 
 */
constexpr int medical_img_size = 256;
/**
 * @brief How many images in input
 * 
 */
constexpr int images_num = 100;
/**
 * @brief Used to write the vtk volume
 * 
 */
constexpr int dimensions[3] = {medical_img_size, medical_img_size, images_num};

/**
 * @brief Starting index of dataset with most intense noise image
 * 
 */
constexpr int noisy_start_idx = 75;

/**
 * @brief Last index of dataset with most intense noise image
 * 
 */
constexpr int noisy_end_idx = 78;