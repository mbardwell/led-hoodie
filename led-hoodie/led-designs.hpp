/**
 * @file led-designs.h
 * @brief LED Designs
 * @details This file contains the LED designs for the LED Hoodie project.
 */

#pragma once

namespace led_designs
{

constexpr uint8_t bitmap_heart_7_x_7[7] = {
    0x36 << 1,
    0x7F << 1,
    0x7F << 1,
    0x7F << 1,
    0x3E << 1,
    0x1C << 1,
    0x08 << 1,
};

constexpr uint8_t bitmap_box_4x4[4] = {
    0xF0,  // Top line
    0x90,  // Left and right
    0x90,  // Left and right
    0xF0,  // Bottom line
};

constexpr uint8_t bitmap_lines_4x4[4] = {
    0xA0,  // Top line
    0x00,  // Left and right
    0x00,  // Left and right
    0x50,  // Bottom line
};

}
