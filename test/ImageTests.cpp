//
// Created by edo on 10/03/25.
//
#include <gtest/gtest.h>
#include "../src/Image.h"


//forse fare test separati per get e set

TEST(ImageTest, PixelSetAndGet) {
    Image<int, 3> img(2, 2);
    Pixel<int, 3> px = {{255, 128, 64}};

    //two ways to set a pixel
    img.set_pixel(0, 0, px);//with a Pixel object
    auto px_read = img.get_pixel(0, 0);
    EXPECT_EQ(px_read.channels[0], 255);
    EXPECT_EQ(px_read.channels[1], 128);
    EXPECT_EQ(px_read.channels[2], 64);

    img.set_pixel(1,1, {35, 46, 77});//with a list of values
    px_read = img.get_pixel(1, 1);
    EXPECT_EQ(px_read.channels[0], 35);
    EXPECT_EQ(px_read.channels[1], 46);
    EXPECT_EQ(px_read.channels[2], 77);
}

TEST(ImageTest, ValidIndexTest){
    Image<int, 3> img(2, 2);
    EXPECT_NO_THROW(img.get_pixel(0, 0));
    EXPECT_NO_THROW(img.get_pixel(1, 1));
    EXPECT_THROW(img.get_pixel(0, 2), std::out_of_range);
    EXPECT_THROW(img.get_pixel(2, 2), std::out_of_range);
}