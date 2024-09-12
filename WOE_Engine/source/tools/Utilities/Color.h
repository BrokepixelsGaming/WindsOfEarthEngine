#pragma once
#include <cstdint>
struct Color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;

    // Overload the < operator for Color
    bool operator<(const Color& other) const 
    {
        // Compare RGBA components lexicographically
        if (red != other.red) {
            return red < other.red;
        }
        if (green != other.green) {
            return green < other.green;
        }
        if (blue != other.blue) {
            return blue < other.blue;
        }
        // Compare alpha only if necessary
        return alpha < other.alpha;
    }
    bool operator==(const Color& other) const 
    {
        // Compare all RGBA components for equality
        return (red == other.red) && (green == other.green) && (blue == other.blue) && (alpha == other.alpha);
    }
};
///////////////////////////////////////////////////////////////
//   STATIC COLORS FOR USE
///////////////////////////////////////////////////////////////
static constexpr Color kColorGreen = { 0,255,0,255 };
static constexpr Color kColorDarkGreen = { 24,44,37,255 };
static constexpr Color kColorLightGreen = { 31,198,0,255 };
static constexpr Color kColorRed = { 255,0,0,255 };
static constexpr Color kColorWhite = { 255,255,255,255 };
static constexpr Color kColorBlack = { 0,0,0,255 };
static constexpr Color kColorBlue = { 0,0,255,255 };
static constexpr Color kColorIceBlue = { 162,210,223,255 };
static constexpr Color kColorDarkBlue = { 0,26,51,255 };
static constexpr Color kColorLightBlue = { 0,102,204,255 };
static constexpr Color kColorBrown = { 86,43,0,255 };
static constexpr Color kColorLightBrown = { 124,63,0,255 };
static constexpr Color kColorDarkBrown = { 62,28,0,255 };
static constexpr Color kColorLightBeige = { 231,196,150,255 };
static constexpr Color kColorSapphire = { 15,82,186,255 };
static constexpr Color kColorGrey = { 128,128,128,255 };
static constexpr Color kColorIceGrey = { 134,214,216,255 };
