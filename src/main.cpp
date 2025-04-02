#include <iostream>
#include "Image.h"
#include "Kernel.h"
#include "PPM_IO.h"

// Definiamo alcuni kernel predefiniti
const std::array<std::array<int, 3>, 3> SHARPEN = {{
                                                           {  0, -1,  0 },
                                                           { -1,  5, -1 },
                                                           {  0, -1,  0 }
                                                   }};

const std::array<std::array<int, 3>, 3> BLUR = {{
                                                        { 1,  2,  1 },
                                                        { 2,  4,  2 },
                                                        { 1,  2,  1 }
                                                }};

const std::array<std::array<int, 3>, 3> EDGE_DETECTION = {{
                                                                  { -1, -1, -1 },
                                                                  { -1,  8, -1 },
                                                                  { -1, -1, -1 }
                                                          }};


int main() {
    std::string image_name;

    std::cout << "Inserisci il nome del file immagine (PPM): ";
    std::cin >> image_name;
    std::string r = "../images/"+ image_name + ".ppm";
    Image<int, 3> img = PPM_IO::read_PPM<int, 3>(r);

    int filter_choice;
    std::cout << "Scegli un filtro:\n";
    std::cout << "1 - Sharpen\n";
    std::cout << "2 - Blur\n";
    std::cout << "3 - Edge Detection\n";
    std::cout << "Scelta: ";
    std::cin >> filter_choice;


    int padding_choice;
    std::cout << "Scegli un tipo di padding:\n";
    std::cout << "0 - Zero\n";
    std::cout << "1 - Extend\n";
    std::cout << "2 - Wrap\n";
    std::cout << "3 - Mirror\n";
    std::cout << "Scelta: ";
    std::cin >> padding_choice;


    Kernel<int, 3> kernel(
            (filter_choice == 1) ? SHARPEN :
            (filter_choice == 2) ? BLUR :
            EDGE_DETECTION,
            static_cast<PaddingType>(padding_choice)
    );
    Image<int, 3> filtered_img = kernel.apply_kernel(img);

    std::string output_name = "../images/output/image_name" +image_name+ "_filtered.ppm";
    PPM_IO::write_to_PPM(output_name, filtered_img);

    std::cout << "Immagine filtrata salvata come: " << output_name << std::endl;

    return 0;

}
