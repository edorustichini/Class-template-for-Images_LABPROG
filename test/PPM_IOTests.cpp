//
// Created by edo on 10/03/25.
//
#include "gtest/gtest.h"
#include "../src/PPM_IO.h"
#include "../src/Kernel.h"

//TODO: considerare di usare fixture per test di read e write

TEST(PPM_IOTest, write){
    Image<int, 3> img(2, 2);
    Pixel<int, 3> px[] = {{255, 255, 255}, {255, 0, 0}, {0, 0, 255},{0, 128, 0}};
    int  i = 0;
    //two ways to set a pixel
    for(int y = 0; y < img.get_height(); y++) {
        for(int x = 0; x < img.get_width(); x++) {
            img.set_pixel(x, y, px[i]);//with a Pixel object
            i++;
        }
    }
    ASSERT_NO_THROW(PPM_IO::write_to_PPM("../../images/output/tests/simmple-rgb.ppm", img));
}

TEST(PPM_IOTest, read){
    //TODO
}


TEST(PPM_IOTest, readwrite) {
    std::vector<std::string> list = {"apollo.ppm", "landscape.ppm", "bird2.ppm", "monument.ppm"};

    for(int i=0;i < list.size(); i++){
        Image<int, 3> img = PPM_IO::read_PPM<int, 3>("../../images/"+list[i]);
        std::string output_path = "../../images/output/tests/"+list[i]+"_Test.ppm";
        PPM_IO::write_to_PPM(output_path, img);
        Image<int, 3> img2 = PPM_IO::read_PPM<int, 3>(output_path);

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                EXPECT_EQ(img.get_pixel(x, y).channels[0], img2.get_pixel(x, y).channels[0]);
                EXPECT_EQ(img.get_pixel(x, y).channels[1], img2.get_pixel(x, y).channels[1]);
                EXPECT_EQ(img.get_pixel(x, y).channels[2], img2.get_pixel(x, y).channels[2]);
            }
        }
    }
}