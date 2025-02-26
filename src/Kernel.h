//
// Created by edo on 25/02/25.
//

#ifndef IMAGES_TEMPLATE__LABPROGR__KERNEL_H
#define IMAGES_TEMPLATE__LABPROGR__KERNEL_H

#include <vector>
#include "Image.h"



template<typename T, int K_dim>
class Kernel {
private:
    std::array<std::array<T, K_dim>,K_dim> k_matrix;
public:

    explicit Kernel(const std::array<std::array<T,K_dim>,K_dim>& m){
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
                for(int kernel_y=-k_radius; kernel_y<k_radius; kernel_y++){
                    for(int kernel_x=-k_radius; kernel_x<k_radius; kernel_x++){
                        //sub-image of kernel overlappeing image
                        int sq_x = x + kernel_x;
                        int sq_y = y + kernel_y;

                        //TODO: gestire bordi controllando sq_x, sq_y
                        Pixel<T, C>& sq_pixel = old_img.get_pixel(sq_x, sq_y);
                        T k_value = k_matrix[kernel_x][kernel_y];
                        for(int i=0; i < C; i++){
                            acc[i] += k_value * sq_pixel.channels[i];
                        }
                    }

                }

                new_img.set_pixel(x,y, acc);

            }
        }

        return new_img;
    }
};


#endif //IMAGES_TEMPLATE__LABPROGR__KERNEL_H
