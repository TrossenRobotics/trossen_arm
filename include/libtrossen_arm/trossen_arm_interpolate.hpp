// Copyright 2025 Trossen Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef LIBTROSSEN_ARM__TROSSEN_ARM_INTERPOLATE_HPP_
#define LIBTROSSEN_ARM__TROSSEN_ARM_INTERPOLATE_HPP_

#include <array>
#include <optional>

namespace trossen_arm
{

/// @brief Quintic Hermite Interpolator
class QuinticHermiteInterpolator
{
public:
  /**
   * @brief Compute the coefficients for the quintic hermite interpolation y = f(x).
   *   If no optional arguments are specified, it serves as a linear interpolator.
   *   If dy0 and dy1 are specified but not ddy0 or ddy1, it serves as a cubic hermite interpolator.
   *   If all optional arguments are specified, it serves as a quintic hermite interpolator.
   *
   * @param x0 Initial x value
   * @param x1 Final x value
   * @param y0 Initial y value, f(x0)
   * @param y1 Final y value, f(x1)
   * @param dy0 Optional: Initial first order derivative, f'(x0)
   * @param dy1 Optional: Final first order derivative, f'(x1)
   * @param ddy0 Optional: Initial second order derivative, f''(x0)
   * @param ddy1 Optional: Final second order derivative, f''(x1)
   *
   * @details A quintic hermite interpolator is a function of the form
   *
   *   y = f(x) = a0 * x^5 + a1 * x^4 + a2 * x^3 + a3 * x^2 + a4 * x + a5
   *   such that f(x0) = y0
   *             f(x1) = y1
   *             f'(x0) = dy0
   *             f'(x1) = dy1
   *             f''(x0) = ddy0
   *             f''(x1) = ddy1
   *
   *   Since the number of constraints is equal to that of unknowns and the constraints are
   *   linearly independent, the solution is unique.
   *   The coefficients are derived in ../docs/interpolation/quintic_hermite_interpolation.py.
   *
   *   The cubic hermite interpolator and linear interpolator are given by
   *
   *   y = f(x) = a0 * x^3 + a1 * x^2 + a2 * x + a3
   *   such that f(x0) = y0
   *             f(x1) = y1
   *             f'(x0) = dy0
   *             f'(x1) = dy1
   *
   *   and
   *
   *   y = f(x) = a0 * x + a1
   *   such that f(x0) = y0
   *             f(x1) = y1
   *
   *   respectively.
   *
   *   By specifying the corresponding ddy0 and ddy1 of the cubic hermite interpolator,
   *   the resulting quintic hermite interpolator must exactly match the cubic hermite interpolator
   *   due to the uniqueness of the solution.
   *   ddy0 and ddy1 are derived in ../docs/interpolation/cubic_hermite_interpolation.py.
   *
   *   Similarly, the quintic hermite interpolator must exactly match the linear interpolator
   *   when the corresponding dy0, dy1, ddy0, and ddy1 are specified.
   */
  void compute_coefficients(
    float x0,
    float x1,
    float y0,
    float y1,
    std::optional<float> dy0 = std::nullopt,
    std::optional<float> dy1 = std::nullopt,
    std::optional<float> ddy0 = std::nullopt,
    std::optional<float> ddy1 = std::nullopt);

  /// @brief Evaluate f(x)
  float y(float x);

  /// @brief Evaluate f'(x)
  float dy(float x);

  /// @brief Evaluate f''(x)
  float ddy(float x);

private:
  // Coefficients
  std::array<double, 6> a_;

  // Bounds
  double x0_{0.0f};
  double x1_{0.0f};
  double y0_{0.0f};
  double y1_{0.0f};
  double dy0_{0.0f};
  double dy1_{0.0f};
  double ddy0_{0.0f};
  double ddy1_{0.0f};
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_INTERPOLATE_HPP_
