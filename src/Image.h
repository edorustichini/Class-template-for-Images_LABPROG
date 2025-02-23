//
// Created by edo on 23/02/25.
//


#ifndef IMAGES_TEMPLATE__LABPROGR__IMAGE_H
#define IMAGES_TEMPLATE__LABPROGR__IMAGE_H

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

    int get_width(){
        return width;
    }
    int get_height(){
        return height;
    }

    void isValidIndex(int x, int y){
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("Pixel index out of bounds");
        }
    }

    Pixel& getPixel(int x, int y) {
        isValidIndex(x, y);
        int index = y * width + x;
        return pixels[index];
    }

    void setPixel(int x, int y, const Pixel px) {
        isValidIndex(x, y);
        int index = y * width + x;
        for (int i = 0; i < C; i++) {
            pixels[index].channels[i] = px.channels[i];
        }
    }



};

#endif //IMAGES_TEMPLATE__LABPROGR__IMAGE_H
