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

        // kernel radius
        int k_radius = (K_dim - 1) / 2;

        // apply kernel to each pixel
        for(int y = 0; y < old_img.get_height(); y++) {
            for(int x = 0; x < old_img.get_width(); x++) {

                std::array<double, C> acc{};

                // Convolution expression from https://en.wikipedia.org/wiki/Kernel_(image_processing)
                for(int j = -k_radius; j <= k_radius; j++) {
                    for(int i = -k_radius; i <= k_radius; i++) {
                        // coordinates of pixels to be considered
                        int sub_img_x = x + i;
                        int sub_img_y = y + j;

                        int kernel_x = i + k_radius;
                        int kernel_y = j + k_radius;


                        Pixel<IMG_T, C> sq_pixel = get_padded_pixel<IMG_T, C>(sub_img_x, sub_img_y, old_img);
                        T k_value = k_matrix[kernel_y][kernel_x];


                        for(int k = 0; k < C; k++) {
                            acc[k] += static_cast<double>(k_value) * static_cast<double>(sq_pixel.channels[k]);
                        }
                    }
                }

                //Cast to image_pixels's type
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

        if(x >= 0 && x < width && y >= 0 && y < height) { // Pixel is inside the image
            px = img.get_pixel(x, y);
        }

        else { //(x,y) is outside the image
            int extended_x = x, extended_y = y;

            // Edge handling technique described in: https://en.wikipedia.org/wiki/Kernel_(image_processing)
            switch(padding_choice) {
                case ZERO: {
                    // Zero padding
                    for(int i = 0; i < C; i++) {
                        px.channels[i] = 0;
                    }
                    break;
                }
                case EXTEND: {
                    if(x < 0){
                        extended_x = 0;
                    } else if (x >= width){
                        extended_x = width - 1;
                    }
                    if(y < 0){
                        extended_y = 0;
                    } else if(y >= height){
                        extended_y = height - 1;
                    }
                    px = img.get_pixel(extended_x, extended_y);
                    break;
                }
                case WRAP: {
                    extended_x = ((x % width) + width) % width; //first modulo to handle negative numbers
                    extended_y = ((y % height) + height) % height;
                    px = img.get_pixel(extended_x, extended_y);
                    break;
                }
                case MIRROR: {
                    if(extended_x < 0) {
                        extended_x = -extended_x - 1;
                    } else if(extended_x >= width) {
                        extended_x = 2 * width - extended_x - 1;
                    }
                    if(extended_y < 0) {
                        extended_y = -extended_y - 1;
                    } else if(extended_y >= height) {
                        extended_y = 2 * height - extended_y - 1;
                    }
                    //FIXME: gestire caso in cui è così tanto fuori dallìimmagine, da uscire dall'altro bordo

                    px = img.get_pixel(extended_x, extended_y);
                    break;
                }

                default: {
                    //CONSTANT
                    for(int i=0; i<C; i++){
                        px.channels[i] = 0;
                    }
                }
            }
        }

        return px;
    }
};

#endif //IMAGES_TEMPLATE__LABPROGR__KERNEL_H