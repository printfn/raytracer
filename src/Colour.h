#pragma once

#ifndef COLOUR_H_INCLUDED
#define COLOUR_H_INCLUDED

#include <cstdint>
#include <array>

/// Class to store colour information.
///
/// Internally all colours are stored in the linear CIE 1931 colour space.
class Colour {

public:

    /// Creates a new colour with all values set to 0 (black).
    Colour() = default;

    /// Creates a new colour with the given linear red, green, and blue values.
    Colour(double r, double g, double b);

    bool is_zero() const;

    /// Convert this linear colour value to gamma-corrected sRGB values
    std::array<uint8_t, 3> srgb_gamma_correction() const;

    /// Create a linear colour value from the given sRGB values (in range 0-1)
    static Colour with_reverse_gamma_correction(double r, double g, double b);

    Colour operator-() const;
    Colour &operator+=(const Colour &colour);
    Colour &operator-=(const Colour &colour);
    Colour &operator*=(const Colour &colour);
    Colour &operator*=(double s);
    Colour &operator/=(double s);

    double red = 0;
    double green = 0;
    double blue = 0;

};

Colour operator+(const Colour &lhs, const Colour &rhs);
Colour operator-(const Colour &lhs, const Colour &rhs);
Colour operator*(const Colour &lhs, const Colour &rhs);
Colour operator*(double s, const Colour &colour);
Colour operator*(const Colour &colour, double s);
Colour operator/(const Colour &colour, double s);


#endif
