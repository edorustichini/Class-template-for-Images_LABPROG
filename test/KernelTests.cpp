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
    Kernel<int, 3> kernel(identity, ZERO);
    Image<int, 1> result = kernel.apply_kernel(img);

    EXPECT_EQ(result.get_pixel(0, 0).channels[0], 42);
    EXPECT_EQ(result.get_pixel(1, 0).channels[0], 50);
    EXPECT_EQ(result.get_pixel(0, 1).channels[0], 60);
    EXPECT_EQ(result.get_pixel(1, 1).channels[0], 70);
}