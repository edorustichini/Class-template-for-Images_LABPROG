//
// Created by edo on 10/03/25.
//
#include "gtest/gtest.h"
#include "../src/PPM_IO.h"
#include "../src/Kernel.h"

/*TODO:
 * - test che legge un'immagine, la scrive in un oggetto e crea una nuova immagine.
 *   poi si guarda se quello che è scritto è uguale, questo è un test di integrazione(?)
 *   */

TEST(PPM_IO, read_write) {
    try {
        // 1️⃣ Percorso dell'immagine da leggere
        std::string input_path = "../../images/bird2.ppm";
        std::string output_path = "output.ppm";
        std::string filtered_path = "prova filtr.ppm";

        // 2️⃣ Leggiamo l'immagine PPM
        Image<int, 3> img = PPM_IO::read_PPM<int, 3>(input_path);
        std::cout << "✅ Immagine letta con successo da: " << input_path << std::endl;



        // Salviamo una copia dell'immagine originale
        PPM_IO::write_to_PPM(output_path, img);
        std::cout << "✅ Immagine copiata in: " << output_path << std::endl;

        // Definiamo un kernel 3x3 di edge detection (Sobel)
        std::array<std::array<int, 3>, 3> sobel_x = {{
                                                             { 0,  1, 0 },
                                                             { 1,  -4, 1 },
                                                             { 0,  1, 0 }
                                                     }};

        Kernel<int, 3> kernel(sobel_x, 0); // Zero-padding

        //Applichiamo il filtro all'immagine
        Image<int, 3> filtered_img = kernel.apply_kernel(img);
        std::cout << "✅ Filtro applicato correttamente." << std::endl;

        //Salviamo l'immagine filtrata
        PPM_IO::write_to_PPM(filtered_path, filtered_img);
        std::cout << "✅ Immagine filtrata salvata in: " << filtered_path << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Errore: " << e.what() << std::endl;
    }
}