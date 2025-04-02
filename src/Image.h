//
// Created by edo on 23/02/25.
//

#ifndef IMAGES_TEMPLATE__LABPROGR__IMAGE_H
#define IMAGES_TEMPLATE__LABPROGR__IMAGE_H
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T, int C>
struct Pixel{
    T channels[C];
};

template <typename T, int C>
class Image {
private:
    int width, height;
    std::vector<Pixel<T,C>> pixels;

    void is_valid_index(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("Index out of bounds");
        }
    }

public:

    explicit Image(int w = 1, int h = 1) : width(w), height(h) {
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Width and height must be positive");
        }
        pixels.resize(w*h);
    }
    ~Image()= default;
    Image(const Image& other): width(other.width), height(other.height), pixels(other.pixels) {}

    Image& operator=(const Image& img){
        if (this != &img) {
            width = img.width;
            height = img.height;
            pixels = img.pixels;
        }
        return *this;
    }


    int get_width() const { return width; }
    int get_height() const { return height; }
    void set_width(int w) { width = w; }
    void set_height(int h) { height = h; }

    const Pixel<T,C>& get_pixel(int x, int y) const {
        is_valid_index(x, y);
        return pixels[y * width + x];
    }

    void set_pixel(int x, int y, const Pixel<T,C>& px) {
        is_valid_index(x, y);
        int index = y * width + x;
        for (int i = 0; i < C; i++) {
            pixels[index].channels[i] = px.channels[i];
        }
    }

    void set_pixel(int x, int y, const T values[C]){
        is_valid_index(x, y);
        int index = y * width + x;
        for(int i=0; i < C; i++){
            pixels[index].channels[i] = values[i];
        }
    }

    std::string get_magic_number() const {
        std::string magic_number;
        if(C==1){
            magic_number = "P2";
        }
        else if(C==3){
            magic_number = "P3";
        }
        else{
            throw std::runtime_error("Formato PPM non supportato per " + std::to_string(C) + " canali");
        }
        return magic_number;
    }
};
#endif //IMAGES_TEMPLATE__LABPROGR__IMAGE_H