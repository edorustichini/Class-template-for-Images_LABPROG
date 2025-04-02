//
// Created by edo on 10/03/25.
//

#include <gtest/gtest.h>

#include "../src/Kernel.h"
#include "../src/PPM_IO.h"

// Test the Kernel class: vedendo se crea immagini sensate, del giusto formato

TEST(KernelTest, IdentityKernel) {
    Image<int, 1> img(2, 2);
    img.set_pixel(0, 0, Pixel<int, 1>{{42}});
    img.set_pixel(1, 0, Pixel<int, 1>{{50}});
    img.set_pixel(0, 1, Pixel<int, 1>{{60}});
    img.set_pixel(1, 1, Pixel<int, 1>{{70}});

    std::array<std::array<int, 3>, 3> identity = {{
                                                          {0, 0, 0},
                                                          {0, 1, 0},
                                                          {0, 0, 0}
                                                  }};
    Kernel<int, 3> kernel(identity, PaddingType::ZERO);
    Image<int, 1> result = kernel.apply_kernel(img);

    EXPECT_EQ(result.get_pixel(0, 0).channels[0], 42);
    EXPECT_EQ(result.get_pixel(1, 0).channels[0], 50);
    EXPECT_EQ(result.get_pixel(0, 1).channels[0], 60);
    EXPECT_EQ(result.get_pixel(1, 1).channels[0], 70);
}
//TODO: considerare di usare FIXTURE
TEST(KernelTest, BoxBlur3x3) {
    // Crea un'immagine 3x3 con valori noti
    Image<float, 1> img(3, 3);
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            auto val = static_cast<float>(y*3 + x + 1); // [1,9]
            img.set_pixel(x, y, &val);
        }
    }

    // Kernel box blur 3x3
    std::array<std::array<float, 3>, 3> blur_kernel = {{
                                                               {{1/9.f, 1/9.f, 1/9.f}},
                                                               {{1/9.f, 1/9.f, 1/9.f}},
                                                               {{1/9.f, 1/9.f, 1/9.f}}
                                                       }};

    Kernel<float, 3> kernel(blur_kernel); //use zero padding by default
    auto blurred = kernel.apply_kernel(img);

    // central px should be average of neighbouring pixels
    float expected_center = 5.0f;
    auto center_px = blurred.get_pixel(1, 1);
    EXPECT_FLOAT_EQ(center_px.channels[0], expected_center);

    // Corner pixel in (0,0)
    float expected_corner = 12.0f/9.0f;
    auto corner_px = blurred.get_pixel(0, 0);
    EXPECT_FLOAT_EQ(corner_px.channels[0], expected_corner);
}

TEST(KernelTest, PaddingTypes) {
    //Test if out-of-border pixels make sense

    //simple image
    Image<int, 1> img(3, 3);
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            int val = y*3 + x + 1; //[1,9]
            img.set_pixel(x, y, &val);
        }
    }
    std::array<std::array<int, 3>, 3> blur = {{
                                                                 { 1,  1,  1 },
                                                                 { 1,  1,  1 },
                                                                 { 1,  1,  1 }
                                                         }};



    // Test Zero padding
    Kernel<int, 3> zero_kernel(blur, PaddingType::ZERO);
    auto zero_result = zero_kernel.apply_kernel(img);
    EXPECT_EQ(zero_result.get_pixel(1, 0).channels[0], 21);
    EXPECT_EQ(zero_result.get_pixel(0, 1).channels[0], 27);
    EXPECT_EQ(zero_result.get_pixel(0, 0).channels[0], 12);

    // Test Extend padding
    Kernel<int, 3> extend_kernel(blur, PaddingType::EXTEND);
    auto extend_result = extend_kernel.apply_kernel(img);
    //nearest border pixels are conceptually extended with the same values
    EXPECT_EQ(extend_result.get_pixel(0, 0).channels[0], 21);
    EXPECT_EQ(extend_result.get_pixel(2, 2).channels[0], 69);

    // Test Wrap padding
    Kernel<int, 3> wrap_kernel(blur, PaddingType::WRAP);
    auto wrap_result = wrap_kernel.apply_kernel(img);
    EXPECT_EQ(wrap_result.get_pixel(0, 0).channels[0], 45);
    EXPECT_EQ(wrap_result.get_pixel(2, 2).channels[0], 45);


    // Test Mirror padding
    Kernel<int, 3> mirror_kernel(blur, PaddingType::MIRROR);
    auto mirror_result = mirror_kernel.apply_kernel(img);
    EXPECT_EQ(mirror_result.get_pixel(0, 0).channels[0], 33);
    EXPECT_EQ(mirror_result.get_pixel(2, 2).channels[0], 57);
    EXPECT_EQ(mirror_result.get_pixel(2, 0).channels[0], 39);


}