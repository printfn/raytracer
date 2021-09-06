#pragma once

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <cmath>
#include <limits>

const double epsilon = 1e-6;
const double infinity = std::numeric_limits<double>::max();

const double TAU = 6.283185307179586;
const double Deg2Rad = TAU / 360.0;
const double Rad2Deg = 360.0 / TAU;

inline int sign(double val) {
    if (std::abs(val) < epsilon) return 0;
    if (val < 0) return -1;
    return 1;
}

/// Proportionally changes the scale of x
inline double scale(double x, double min, double max, double newMin, double newMax) {
    return (x / (max - min) - min) * (newMax - newMin) + newMin;
}

#endif // UTILITY_H_INCLUDED
