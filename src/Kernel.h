//
// Created by edo on 25/02/25.
//

#ifndef IMAGES_TEMPLATE__LABPROGR__KERNEL_H
#define IMAGES_TEMPLATE__LABPROGR__KERNEL_H

#include <vector>
#include <array>
#include "Image.h"



template<typename T, int K_dim>
class Kernel {
private:
    std::array<std::array<T, K_dim>,K_dim> k_matrix;
    int padding_choice;
public:

    explicit Kernel(const std::array<std::array<T,K_dim>,K_dim>& m, int p = 0) : padding_choice(p){
        k_matrix = m;
    }
    ~Kernel()=default;

    template<typename IMG_T, int C>
    Image<IMG_T, C> apply_kernel(const Image<IMG_T,C>& old_img) const{
        /*
        //TODO: decidere come gestire i bordi, magari funzioni che fa scegliere il modo con cui gestire
        int choice;
        choose_and_add_padding(old_img, choice);
         */

        Image<IMG_T, C> new_img(old_img.get_width(), old_img.get_height());

        //kernel radius
        int k_radius = (K_dim - 1) / 2;

        //FIXME: non può modificare l'immagine già esistente ma deve crearne un'altra
        for(int y=0; y < old_img.get_height(); y++){
            for(int x=0; x < old_img.get_width(); x++){
                //FIXME: forse da cambiare questa inizializzazione
                T acc[C] = {};
                 //convolution expression from https://en.wikipedia.org/wiki/Kernel_(image_processing)
                for(int j=-k_radius; j <= k_radius; j++){
                    for(int i=-k_radius; i < k_radius; i++){
                        //sub-image of kernel overlappeing image
                        int sq_x = x - i;
                        int sq_y = y - j;

                        int kernel_x = i + k_radius;
                        int kernel_y = j + k_radius;

                        //TODO: gestire bordi controllando sq_x, sq_y

                        Pixel<IMG_T, C> sq_pixel = get_padded_pixel<IMG_T,C>(sq_x, sq_y, old_img);
                        T k_value = k_matrix[kernel_x][kernel_y];
                        for(int k=0; k < C; k++){
                            acc[k] += k_value * sq_pixel.channels[k];
                        }
                    }

                }

                new_img.set_pixel(x,y, acc);

            }
        }

        return new_img;
    }

    template<typename IMG_T, int C>
    Pixel<IMG_T,C> get_padded_pixel(int x, int y, const Image<IMG_T, C>& img) const{
        Pixel<IMG_T,C> px;
        //controlloare se è sui bordi
        if(x>0 && x<img.get_width() && y>0 && y<img.get_height()){
            px = img.get_pixel(x,y);
        }
        else{
            if(padding_choice == 0){
                //zero padding
                for(int i=0; i<C; i++){
                    px.channels[i] = 0;
                }
            }/*
            if(padding_choice == 1){
                //extend
                //FIXME gestire angoli
                if(x==0 || y==0){ //basta che controllo questo perché che sia fuori dall'immagine è già controllato a inizio della funzione
                    //è uno degli an
                }

                //FIXME: a seconda del lato su cui si trova avrà valore diverso
                if(x<0 || x>=img.get_width()){
                    px = img.get_pixel();

                }
            };
            if(padding_choice == 2){
                //wrap
            };
            if(padding_choice == 3){
                //mirror
            };
            if(padding_choice == 4){
                //mirror
            }
            if(padding_choice == 5){
                //kernel crop
            };*/
        }

        //scegliere che tipo di padding a seconda di padding_choice (attributo di Kernel)
        return px;
    }
};


#endif //IMAGES_TEMPLATE__LABPROGR__KERNEL_H
