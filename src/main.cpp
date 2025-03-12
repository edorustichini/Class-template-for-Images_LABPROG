#include <iostream>
#include <filesystem>  // Per gestire i percorsi delle immagini
#include "Image.h"
#include "Kernel.h"
#include "PPM_IO.h"


int main() {
    try {
        std::string input_path = "../images/apollo.ppm";
        std::string output_path = "output.ppm";
        std::string filtered_path = "prova filtr.ppm";

        Image<int, 3> img = PPM_IO::read_PPM<int, 3>(input_path);
        std::cout << "✅ Immagine letta con successo da: " << input_path << std::endl;



        PPM_IO::write_to_PPM(output_path, img);
        std::cout << "✅ Immagine copiata in: " << output_path << std::endl;

        std::array<std::array<int, 3>, 3> sobel_x = {{
                                                             { 0,  1, 0 },
                                                             { 1,  -4, 1 },
                                                             { 0,  1, 0 }
                                                     }};

        Kernel<int, 3> kernel(sobel_x, 0); // Zero-padding

        Image<int, 3> filtered_img = kernel.apply_kernel(img);
        std::cout << "✅ Filtro applicato correttamente." << std::endl;

        //Salviamo l'immagine filtrata
        PPM_IO::write_to_PPM(filtered_path, filtered_img);
        std::cout << "✅ Immagine filtrata salvata in: " << filtered_path << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Errore: " << e.what() << std::endl;
    }


    return 0;
}