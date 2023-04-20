#pragma once

#include <cmath>
#include <numbers>

#ifndef M_PI
inline constexpr auto M_PI = std::numbers::pi_v<float>;
#endif
