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
    std::vector<Pixel<T,C>> pixels;//FIXME: forse da mettere pubblico, però se fatto, da cambiare anche altri metodi (readPPM)

public:

    explicit Image(int w=0, int h=0) : width(w), height(h) {
        pixels.resize(w*h);
    }

    Image(const Image& other): width(other.width), height(other.height), pixels(other.pixels) {}
    //TODO:operatore di asssegnazione
    Image& operator=(const Image& img){

    }
    ~Image()= default;

    int get_width(){ return width; }
    int get_height(){ return height; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }

    void is_valid_index(int x, int y) const{
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("Pixel index out of bounds");
        }
    }


    //FIXME: probabilmente da levare perché si possono accedere tramire vector
    Pixel<T,C>& get_pixel(int x, int y) {
        is_valid_index(x, y);
        return pixels[y * width + x];
    }

    //FIXME: ha senso, forse sì per copy constructor o operatore di assegnazione
    void set_pixel(int x, int y, const Pixel<T,C> px) {
        is_valid_index(x, y);
        int index = y * width + x;
        for (int i = 0; i < C; i++) {
            pixels[index].channels[i] = px.channels[i];
        }
    }
    //avendo i valori per ogni canale
    void set_pixel(int x, int y, T values[C]){
        is_valid_index(x, y);
        int index = y * width + x;
        for(int i=0; i < C; i++){
            pixels[index].channels[i] = values[i];
        }
    }

    std::string get_magic_number(){
        std::string magic_number;
        // TODO:  gestire anche 2, 4 CANALI
        //TODO: per ora gestita solo forma testuale, gestire anche forma binaria?
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
    void write_to_PPM(const std::string& filename) {
        std::ofstream img_file(filename, std::ios::app);
        if(!img_file.is_open()){
            throw std::runtime_error("Error");
        }
        std::string magic_number = get_magic_number();
        int max_val = 255; //FIXME: da definire per ogni canale?

        //ppm header
        img_file<< magic_number << "\n" << width << " " << height << "\n"<< max_val<< "\n";

        //pixels values
        for (const Pixel px : pixels) {
            for (int i = 0; i < C; i++) {
                img_file << static_cast<int>(px.channels[i]) << " ";  //Guarantee PPM correct format
            }
            img_file << "\n";//FIXME?: forse da cambiare per risparmiare spazio?
        }
        img_file.close();
    }

    /*TODO: un possibile test è: leggere un file già in PPM e creare l'oggetto Image equivalente, poi trasformalo con il metodo creato
     * in un PPM e vedere se sono uguali*/
};

#endif //IMAGES_TEMPLATE__LABPROGR__IMAGE_H
