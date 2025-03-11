//
// Created by edo on 25/02/25.
//

#include "PPM_IO.h"
//TODO: capire dove è usata
/*
int PPM_IO::determine_channels(const std::string &filename) {
    std::ifstream img_file(filename, std::ios::in);
    if (!img_file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file.");
    }

    std::string magic_number;
    img_file >> magic_number;
    img_file.close();

    int nch = 0;
    if (magic_number == "P3") {
        nch = 3;
    } else if (magic_number == "P2") {
        nch = 1;
    } else {
        throw std::runtime_error("Formato file non supportato: " + magic_number);
    }
    return nch;

}*/