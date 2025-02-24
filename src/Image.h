//
// Created by edo on 23/02/25.
//


#ifndef IMAGES_TEMPLATE__LABPROGR__IMAGE_H
#define IMAGES_TEMPLATE__LABPROGR__IMAGE_H
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T, size_t C>
class Image {
public:
    struct Pixel{
        T channels[C];
    };

private:
    std::vector<Pixel> pixels;
    int width, height;

public:
    Image(int w, int h) : width(w), height(h) {
        pixels.resize(w * h);
    }

    //TODO: copy constructor e operatore di asssegnazione
    Image(const Image& img){
    }
    Image& operator=(const Image& img){
    }

    int get_width(){ return width; }
    int get_height(){ return height; }

    void is_valid_index(int x, int y) const{
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("Pixel index out of bounds");
        }
    }

    Pixel& get_pixel(int x, int y) {
        is_valid_index(x, y);
        int index = y * width + x;
        return pixels[index];
    }

    void set_pixel(int x, int y, const Pixel px) {
        is_valid_index(x, y);
        int index = y * width + x;
        for (int i = 0; i < C; i++) {
            pixels[index].channels[i] = px.channels[i];
        }
    }

    std::string get_magic_number(){
        std::string magic_number;
        // TODO: per ora gestito solo formato testuale, gestire anche binario?
        if(C==1){
            magic_number = "P2";
        }
        if(C==3){
            magic_number = "P3";
        }
        else{
            throw std::runtime_error("Formato PPM non supportato per C=" + std::to_string(C));
        }
        return magic_number;
    }
    void write_to_PPM(const std::string& filename) const{
        std::ofstream img_file(filename, std::ios::out);
        if(!img_file.is_open()){
            throw std::runtime_error("bbbb");
        }
        std::string magic_number = get_magic_number();
        img_file<< magic_number << "\n" << width << " " << height << 255<< "\n"; // FIXME: CREDO CHE  255 non abbia senso
        //TODO: scrivere i valori dei pixel
        for(Pixel px:pixels){
            for(T val:px.channels){
                img_file << val << " ";
            }
            img_file << "\n";
        }
        img_file.close();
    }

    /*TODO: un possibile test è: leggere un file già in PPM e creare l'oggetto Image equivalente, poi trasformalo con il metodo creato
     * in un PPM e vedere se sono uguali*/







};

#endif //IMAGES_TEMPLATE__LABPROGR__IMAGE_H
