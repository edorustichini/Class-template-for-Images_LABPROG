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
    static void write_to_PPM(const std::string& filename, BaseImage& img_obj) {
        std::ofstream img_file("../images/output/"+filename, std::ios::out);
        if(!img_file.is_open()){
            throw std::runtime_error("Error opening file");
        }
        std::string magic_number = img_obj.get_magic_number();
        int max_val = 255; // FIXME: non 255, potrebbe cambiare. La documentazione di PPM dice che deve essere compreso tra 0 e 65535, dice anche che prima del 2000 il maxval era 255
        int w = img_obj.getWidth();
        int h = img_obj.getHeight();

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


    static BaseImage& read_PPM(const std::string& filename) {
        std::ifstream img_file(filename, std::ios::in);
        if (!img_file.is_open()) {
            throw std::runtime_error("Error opening file.");
        }

        std::string magic_number;
        int w, h, max_val;
        //TODO: nell'header dei file PPM possono esserci dei commenti, come gestirli?
        //read the header
        img_file >> magic_number;
        img_file >> w >> h >> max_val;

        //FIXME: mi sa il modo è creare dei factory methods
        const int ch = determine_channels(magic_number);

        //BaseImage& img_obj = create_image<int,ch>(w, h, max_val, img_file);

        //FIXME: i valori dei pixel sono sempre interi?
        /*Image<int, ch> img_obj(w, h);

        //read pixel
        int pixel_value;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Pixel<int, ch> px;
                for (int c = 0; c < ch; c++) {
                    img_file >> pixel_value;
                    px.channels[c] = static_cast<int>(pixel_value);
                }
                img_obj.set_pixel(x, y, px);
            }
        }*/

        img_file.close();
        return img_obj;
    }

    template<typename T=int, int C>
    static Image<T,C>& create_image(int w,int h,int max_val, std::ifstream& img_file){
        Image<int, C> img_obj(w, h);
        int pixel_value;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Pixel<int, C> px;
                for (int c = 0; c < C; c++) {
                    img_file >> pixel_value;
                    px.channels[c] = static_cast<int>(pixel_value);
                }
                img_obj.set_pixel(x, y, px);
            }
        }
        return img_obj;

    }


    static int determine_channels(const std::string &mn)  {
        int ch;
        if (mn == "P3") {
            ch = 3;
        } else if (mn == "P2") {
            ch = 1;
        } else {
            throw std::runtime_error("Unsupported file format: " + mn);
        }
        return ch;
    }

    static std::string get_magic_number(BaseImage& img) {
        int count =0;
        for(auto& px: img.get_pixel(0,0).channels){
            count ++;
        }

        std::string magic_number;
        if(count==1){
            magic_number = "P2";
        }
        else if(count==3){
            magic_number = "P3";
        }
        else{
            throw std::runtime_error("Formato PPM non supportato per " + std::to_string(count) + " canali");
        }
        return magic_number;
    }
};


#endif //IMAGES_TEMPLATE__LABPROGR__PPM_IO_H
