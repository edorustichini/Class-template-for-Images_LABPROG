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

//product interface (factory pattern)
class BaseImage {
private:
    int width, height;
public:
    explicit BaseImage(int w=0, int h=0) : width(w), height(h) {}
    virtual ~BaseImage() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }

    void is_valid_index(int x, int y) const {
        if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) {
            throw std::out_of_range("Index out of bounds");
        }
    }

    virtual const Pixel<int , 3> get_pixel(int x, int y) const = 0;
};

//concrete product (factory pattern)
template <typename T, int C>
class Image : public BaseImage{
private:
    std::vector<Pixel<T,C>> pixels; //FIXME: forse fa mettere privato, ma nel caso cambiare funzione write/read(??)
public:
    explicit Image(int w=0, int h=0) : BaseImage(w, h) {
        pixels.resize(w*h);
    }

    Image(const Image& other): BaseImage(other.getWidth(), other.getHeight()), pixels(other.pixels) {}

    Image& operator=(const Image& img){
        if (this != &img) {
            setWidth(img.getWidth());
            setHeight(img.getHeight());
            pixels = img.pixels;
        }
        return *this;
    }

    ~Image()= default;

    const Pixel<T,C>& get_pixel(int x, int y) const {
        is_valid_index(x, y);
        return pixels[y * getWidth() + x];
    }

    void set_pixel(int x, int y, const Pixel<T,C>& px) {
        is_valid_index(x, y);
        int index = y * getWidth() + x;
        for (int i = 0; i < C; i++) {
            pixels[index].channels[i] = px.channels[i];
        }
    }

    void set_pixel(int x, int y, const T values[C]){
        is_valid_index(x, y);
        int index = y * getWidth() + x;
        for(int i=0; i < C; i++){
            pixels[index].channels[i] = values[i];
        }
    }
};
#endif //IMAGES_TEMPLATE__LABPROGR__IMAGE_H