//
// Created by edo on 25/02/25.
//

#ifndef IMAGES_TEMPLATE__LABPROGR__PPM_IO_H
#define IMAGES_TEMPLATE__LABPROGR__PPM_IO_H

#include "Image.h"
#include <algorithm>

class PPM_IO {
public:

    template<typename T, int C>
    static void write_to_PPM(const std::string& filename, Image<T,C>& img_obj) {
        std::ofstream img_file("../images/output/"+filename, std::ios::out);
        if(!img_file.is_open()){
            throw std::runtime_error("Error opening file");
        }
        std::string magic_number = img_obj.get_magic_number();
        int max_val = 255; // PPM format
        int w = img_obj.get_width();
        int h = img_obj.get_height();

        //write header
        img_file << magic_number << "\n" << w << " " << h << "\n" << max_val << "\n";

        //write pixels
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                const auto& px = img_obj.get_pixel(x, y);
                for (int i = 0; i < C; i++) {

                    int clamped_value = std::clamp(px.channels[i], 0, 255); //Clamp to 0-255 for correct PPM format
                    img_file << clamped_value << " ";
                }
                //img_file << "\n";
            }
        }
        img_file.close();
    }

    template<typename T, int C>
    static Image<T,C> read_PPM(const std::string& filename) {
        std::ifstream img_file(filename, std::ios::in);
        if (!img_file.is_open()) {
            throw std::runtime_error("Error opening file.");
        }

        std::string magic_number;
        int w, h, max_val;

        img_file >> magic_number;

        if ((C == 3 && magic_number != "P3") || (C == 1 && magic_number != "P2")) {
            throw std::runtime_error("Incorrect format");
        }

        img_file >> w >> h >> max_val;

        Image<T, C> img_obj(w, h);

        //read pixel
        int pixel_value;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Pixel<T, C> px;
                for (int c = 0; c < C; c++) {
                    img_file >> pixel_value;
                    px.channels[c] = static_cast<T>(pixel_value);
                }
                img_obj.set_pixel(x, y, px);
            }
        }

        img_file.close();
        return img_obj;
    }

    //TODO: nei file PPM possono esserci dei commenti, come gestirli?
};


#endif //IMAGES_TEMPLATE__LABPROGR__PPM_IO_H
