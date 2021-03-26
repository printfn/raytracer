#include "Colour.h"
#include <cmath>
#include <iostream>

Colour::Colour(double r, double g, double b) :
        red(r), green(g), blue(b) {
}

bool Colour::is_zero() const {
    return this->red == 0 && this->green == 0 && this->blue == 0;
}

static double apply_gamma_correction(double linearValue) {
    if (linearValue < 0.0031308) {
        return 12.92 * linearValue;
    } else {
        return 1.055 * pow(linearValue, 1.0 / 2.4) - 0.055;
    }
}

static uint8_t safe_double_to_u8(double d) {
    if (std::isnan(d)) {
        std::cerr << "Warning: NaN detected in colour value" << std::endl;
        return 0;
    }
    if (d >= 255) {
        return 255;
    }
    if (d <= 0) {
        return 0;
    }
    return (uint8_t)d;
}

std::array<uint8_t, 3> Colour::srgb_gamma_correction() const {
    auto rl = 3.24096994 * this->red - 1.53738318 * this->green - 0.49861076 * this->blue;
    auto gl = -0.96924364 * this->red + 1.8759675 * this->green + 0.04155506 * this->blue;
    auto bl = 0.05563008 * this->red - 0.20397696 * this->green + 1.05697151 * this->blue;
    return std::array<uint8_t, 3>{
            safe_double_to_u8(apply_gamma_correction(rl) * 255),
            safe_double_to_u8(apply_gamma_correction(gl) * 255),
            safe_double_to_u8(apply_gamma_correction(bl) * 255)
    };
}

static double apply_reverse_gamma_correction(double u) {
    if (u < 0.04045) {
        return u / 12.92;
    } else {
        return pow((u + 0.055) / 1.055, 2.4);
    }
}

Colour Colour::with_reverse_gamma_correction(double r, double g, double b) {
    // linear rgb values
    double l[3] = {
            apply_reverse_gamma_correction(r),
            apply_reverse_gamma_correction(g),
            apply_reverse_gamma_correction(b),
    };
    return {
        0.41239080 * l[0] + 0.35758434 * l[1] + 0.18048079 * l[2],
        0.21263901 * l[0] + 0.71516868 * l[1] + 0.07219232 * l[2],
        0.01933082 * l[0] + 0.11919478 * l[1] + 0.95053215 * l[2],
    };
}

Colour Colour::operator-() const {
    return { -red, -green, -blue };
}

Colour operator+(const Colour &lhs, const Colour &rhs) {
    return Colour{lhs} += rhs;
}

Colour &Colour::operator+=(const Colour &colour) {
    red += colour.red;
    green += colour.green;
    blue += colour.blue;
    return *this;
}

Colour operator-(const Colour &lhs, const Colour &rhs) {
    return Colour{lhs} -= rhs;
}

Colour &Colour::operator-=(const Colour &colour) {
    red -= colour.red;
    green -= colour.green;
    blue -= colour.blue;
    return *this;
}


Colour operator*(const Colour &lhs, const Colour &rhs) {
    return Colour{lhs} *= rhs;
}

Colour &Colour::operator*=(const Colour &colour) {
    red *= colour.red;
    green *= colour.green;
    blue *= colour.blue;
    return *this;
}

Colour operator*(double s, const Colour &colour) {
    return Colour{colour} *= s;
}

Colour operator*(const Colour &colour, double s) {
    return Colour{colour} *= s;
}

Colour &Colour::operator*=(double s) {
    red *= s;
    green *= s;
    blue *= s;
    return *this;
}

Colour operator/(const Colour &colour, double s) {
    return Colour{colour} /= s;
}

Colour &Colour::operator/=(double s) {
    red /= s;
    green /= s;
    blue /= s;
    return *this;
}
