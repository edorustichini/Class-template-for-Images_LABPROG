#include <iostream>
#include <fstream>
#include <cstdint>
#include "Image.h"
#include "PPM_IO.h"

int main() {
    try {

        Image<int, 3> img = PPM_IO::read_PPM<int, 3>("../images/skylight.ppm");

        std::cout << "L'immagine è stata letta con successo!" << std::endl;
        std::cout << "Larghezza: " << img.get_width() << " Altezza: " << img.get_height() << std::endl;


        PPM_IO::write_to_PPM("landscape.ppm", img);
        std::cout << "Immagine scritta su skylight.ppm" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Errore: " << e.what() << std::endl;
    }

    return 0;
}



