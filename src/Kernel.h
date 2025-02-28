//
// Created by edo on 25/02/25.
//

#ifndef IMAGES_TEMPLATE__LABPROGR__KERNEL_H
#define IMAGES_TEMPLATE__LABPROGR__KERNEL_H

#include <vector>
#include <array>
#include "Image.h"

// Enum for padding types
enum PaddingType {
    ZERO = 0,      // Fill with zeros
    EXTEND = 1,    // Extend border pixels
    WRAP = 2,      // Wrap around (periodic boundary)
    MIRROR = 3,    // Mirror reflection at the boundary
    REFLECT = 4,   // Reflection without duplicating the border pixel
    CROP = 5       // Only apply kernel to valid pixels (no padding)
};

template<typename T, int K_dim>
class Kernel {
private:
    std::array<std::array<T, K_dim>, K_dim> k_matrix;
    int padding_choice;
public:

    explicit Kernel(const std::array<std::array<T, K_dim>, K_dim>& m, int p = 0) : padding_choice(p) {
        k_matrix = m;
    }
    ~Kernel() = default;

    template<typename IMG_T, int C>
    Image<IMG_T, C> apply_kernel(const Image<IMG_T, C>& old_img) const {
        Image<IMG_T, C> new_img(old_img.get_width(), old_img.get_height());

        // Kernel radius
        int k_radius = (K_dim - 1) / 2;

        // Apply kernel to each pixel
        for(int y = 0; y < old_img.get_height(); y++) {
            for(int x = 0; x < old_img.get_width(); x++) {

                std::array<double, C> acc{};

                // Convolution expression from https://en.wikipedia.org/wiki/Kernel_(image_processing)
                for(int j = -k_radius; j <= k_radius; j++) {
                    for(int i = -k_radius; i <= k_radius; i++) {
                        // Sub-image of kernel overlapping image
                        int sub_img_x = x + i;
                        int sub_img_y = y + j;

                        int kernel_x = i + k_radius;
                        int kernel_y = j + k_radius;


                        Pixel<IMG_T, C> sq_pixel = get_padded_pixel<IMG_T, C>(sub_img_x, sub_img_y, old_img);
                        T k_value = k_matrix[kernel_y][kernel_x];


                        for(int k = 0; k < C; k++) {
                            //Used double for precision
                            acc[k] += static_cast<double>(k_value) * static_cast<double>(sq_pixel.channels[k]);
                        }
                    }
                }

                //Cast to the image_pixels's type
                IMG_T filtered_pixel[C];
                for(int k = 0; k < C; k++) {
                    filtered_pixel[k] = static_cast<IMG_T>(acc[k]);
                }

                new_img.set_pixel(x, y, filtered_pixel);
            }
        }

        return new_img;
    }

    template<typename IMG_T, int C>
    Pixel<IMG_T, C> get_padded_pixel(int x, int y, const Image<IMG_T, C>& img) const {
        Pixel<IMG_T, C> px;
        int width = img.get_width();
        int height = img.get_height();

        if(x >= 0 && x < width && y >= 0 && y < height) {
            // Pixel is inside the image
            px = img.get_pixel(x, y);
        }
        else {
            // apply padding
            switch(padding_choice) {
                case ZERO: {
                    // Zero padding
                    for(int i = 0; i < C; i++) {
                        px.channels[i] = 0;
                    }
                    break;
                }
                case EXTEND: {
                    // Extend padding
                    // Clamp coordinates to image boundaries
                    int nx = std::min(std::max(x, 0), width - 1);
                    int ny = std::min(std::max(y, 0), height - 1);
                    px = img.get_pixel(nx, ny);
                    break;
                }
                case WRAP: {
                    // Wrap padding
                    // Use modulo to wrap around
                    int nx = ((x % width) + width) % width;  // Handle negative values
                    int ny = ((y % height) + height) % height;
                    px = img.get_pixel(nx, ny);
                    break;
                }
                case MIRROR: {
                    // Mirror padding - mirror reflection at the boundary
                    // This includes duplicating the border pixel
                    int nx = x;
                    int ny = y;

                    // Handle x-coordinate
                    if(nx < 0) {
                        nx = -nx - 1;
                    } else if(nx >= width) {
                        nx = 2 * width - nx - 1;
                    }

                    // Handle y-coordinate
                    if(ny < 0) {
                        ny = -ny - 1;
                    } else if(ny >= height) {
                        ny = 2 * height - ny - 1;
                    }

                    // Ensure we're within bounds after mirroring (in case of very large values)
                    nx = std::min(std::max(nx, 0), width - 1);
                    ny = std::min(std::max(ny, 0), height - 1);

                    px = img.get_pixel(nx, ny);
                    break;
                }
                case REFLECT: {
                    // Reflect padding - reflection without duplicating the border pixel
                    int nx = x;
                    int ny = y;

                    // Handle x-coordinate
                    if(nx < 0) {
                        nx = -nx;
                    } else if(nx >= width) {
                        nx = 2 * (width - 1) - nx;
                    }

                    // Handle y-coordinate
                    if(ny < 0) {
                        ny = -ny;
                    } else if(ny >= height) {
                        ny = 2 * (height - 1) - ny;
                    }

                    // Ensure we're within bounds after reflection (in case of very large values)
                    nx = std::min(std::max(nx, 0), width - 1);
                    ny = std::min(std::max(ny, 0), height - 1);

                    px = img.get_pixel(nx, ny);
                    break;
                }
                case CROP: {
                    // Crop padding - only consider valid pixels
                    // Fill with zeros for pixels outside the image (effectively ignoring them)
                    for(int i = 0; i < C; i++) {
                        px.channels[i] = 0;
                    }
                    break;
                }
                default: {
                    // Default to zero padding if padding_choice is not recognized
                    for(int i = 0; i < C; i++) {
                        px.channels[i] = 0;
                    }
                    break;
                }
            }
        }

        return px;
    }
};

#endif //IMAGES_TEMPLATE__LABPROGR__KERNEL_H