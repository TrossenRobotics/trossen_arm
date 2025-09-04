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

#ifndef LIBTROSSEN_ARM__TROSSEN_ARM_TYPE_HPP_
#define LIBTROSSEN_ARM__TROSSEN_ARM_TYPE_HPP_

#include <cstdint>

#include <array>
#include <stdexcept>
#include <vector>

namespace trossen_arm
{

/// @brief Operation modes of a joint
enum class Mode : uint8_t {
  /// @brief All joints are braked
  idle,
  /// @brief Control the joint to a desired position
  position,
  /// @brief Control the joint to a desired velocity
  velocity,
  /// @brief Control the joint to a desired external effort
  external_effort,
  /// @brief Control the joint to a desired effort
  effort,
};

/// @brief IP methods
enum class IPMethod : uint8_t {
  /// @brief Use the manual IP address specified in the configuration
  manual,
  /// @brief Use DHCP to obtain the IP address, if failed, use the default IP address
  dhcp,
};

/// @brief Robot models
enum class Model : uint8_t {
  /// @brief WXAI V0
  wxai_v0,
  /// @brief VXAI V0 RIGHT
  vxai_v0_right,
  /// @brief VXAI V0 LEFT
  vxai_v0_left,
};

/// @brief Interpolation spaces
enum class InterpolationSpace : uint8_t {
  /// @brief Joint space
  joint,
  /// @brief Cartesian space
  cartesian,
};

/// @brief Joint characteristic
struct JointCharacteristic
{
  /// @brief Effort correction in motor effort unit / Nm or N
  /// @note It must be within [0.2, 5.0]
  double effort_correction{1.0};
  /// @brief Friction transition velocity in rad/s for arm joints or m/s for the gripper joint
  /// @note It must be positive
  double friction_transition_velocity{0.0};
  /// @brief Friction constant term in Nm for arm joints or N for the gripper joint
  double friction_constant_term{0.0};
  /// @brief Friction coulomb coef in Nm/Nm for arm joints or N/N for the gripper joint
  double friction_coulomb_coef{0.0};
  /// @brief Friction viscous coef in Nm/(rad/s) for arm joints or N/(m/s) for the gripper joint
  double friction_viscous_coef{0.0};
  /// @brief Position_offset in rad for arm joints or m for the gripper joint
  double position_offset{0.0};
};

/// @brief Link properties
struct Link
{
  /// @brief Mass in kg
  double mass{0.0};
  /// @brief Inertia in kg m^2
  std::array<double, 9> inertia{};
  /// @brief Inertia frame translation measured in link frame in m
  std::array<double, 3> origin_xyz{};
  /// @brief Inertia frame RPY angles measured in link frame in rad
  std::array<double, 3> origin_rpy{};
};

/// @brief End effector properties
struct EndEffector
{
  /// @brief Properties of the palm link
  Link palm{};
  /// @brief Properties of the left finger link
  Link finger_left{};
  /// @brief Properties of the right finger link
  Link finger_right{};
  /// @brief Offset from the palm center to the left carriage center in m in home configuration
  double offset_finger_left{0.0};
  /// @brief Offset from the palm center to the right carriage center in m in home configuration
  double offset_finger_right{0.0};
  /// @brief Pitch circle radius in m
  double pitch_circle_radius{0.0};
  /**
   * @brief The tool frame pose measured in the flange frame
   * @note The first 3 elements are the translation and the last 3 elements are the
   * angle-axis representation of the rotation
   */
  std::array<double, 6> t_flange_tool{};
};

/// @brief Joint limit
struct JointLimit
{
  /** @brief Minimum position in rad for arm joints and m for gripper */
  double position_min{0.0};
  /** @brief Maximum position in rad for arm joints and m for gripper */
  double position_max{0.0};
  /** @brief Tolerance on output position in rad for arm joints and m for gripper */
  double position_tolerance{0.0};
  /** @brief Maximum velocity in rad/s for arm joints and m/s for gripper */
  double velocity_max{0.0};
  /** @brief Tolerance on output velocity in rad/s for arm joints and m/s for gripper */
  double velocity_tolerance{0.0};
  /** @brief Maximum effort in Nm for arm joints and N for gripper */
  double effort_max{0.0};
  /** @brief Tolerance on output effort in Nm for arm joints and N for gripper */
  double effort_tolerance{0.0};
};

/** @brief PID parameter structure */
struct PIDParameter
{
  /** @brief Proportional gain */
  double kp{0.0};
  /** @brief Integral gain */
  double ki{0.0};
  /** @brief Derivative gain */
  double kd{0.0};
  /** @brief Maximum integral value */
  double imax{0.0};
};

/** @brief Motor parameter */
struct MotorParameter
{
  /** @brief Position loop PID parameter */
  PIDParameter position{};
  /** @brief Velocity loop PID parameter */
  PIDParameter velocity{};
};

  /** @brief Parameter used for robotic algorithms */
struct AlgorithmParameter
{
  /** @brief Threshold for singularity detection */
  double singularity_threshold{0.0};
};

/// @brief Robot output
struct RobotOutput
{
  /// @brief Outputs in joint space
  struct Joint
  {
    /// @brief Outputs of all joints
    struct All
    {
      /// @brief Positions in rad for arm joints and m for the gripper joint
      std::vector<double> positions{};
      /// @brief Velocities in rad/s for arm joints and m/s for the gripper joint
      std::vector<double> velocities{};
      /// @brief Accelerations in rad/s^2 for arm joints and m/s^2 for the gripper joint
      std::vector<double> accelerations{};
      /// @brief Efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> efforts{};
      /// @brief External efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> external_efforts{};
      /// @brief Compensation efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> compensation_efforts{};
      /// @brief Rotor temperatures in C
      std::vector<double> rotor_temperatures{};
      /// @brief Driver temperatures in C
      std::vector<double> driver_temperatures{};
    } all{};

    /// @brief Outputs of the arm joints
    struct Arm
    {
      /// @brief Positions in rad
      std::vector<double> positions{};
      /// @brief Velocities in rad/s
      std::vector<double> velocities{};
      /// @brief Accelerations in rad/s^2
      std::vector<double> accelerations{};
      /// @brief Efforts in Nm
      std::vector<double> efforts{};
      /// @brief External efforts in Nm
      std::vector<double> external_efforts{};
      /// @brief Compensation efforts in Nm
      std::vector<double> compensation_efforts{};
      /// @brief Rotor temperatures in C
      std::vector<double> rotor_temperatures{};
      /// @brief Driver temperatures in C
      std::vector<double> driver_temperatures{};
    } arm{};

    /// @brief Outputs of the gripper joint
    struct Gripper
    {
      /// @brief Position in m
      double position{0.0};
      /// @brief Velocity in m/s
      double velocity{0.0};
      /// @brief Acceleration in m/s^2
      double acceleration{0.0};
      /// @brief Effort in N
      double effort{0.0};
      /// @brief External effort in N
      double external_effort{0.0};
      /// @brief Compensation effort in N
      double compensation_effort{0.0};
      /// @brief Rotor temperature in C
      double rotor_temperature{0.0};
      /// @brief Driver temperature in C
      double driver_temperature{0.0};
    } gripper{};
  } joint{};

  /// @brief Outputs in Cartesian space
  struct Cartesian
  {
    // TODO(Shiming-Liang): Add helper functions to convert rotations
    /**
     * @brief Spatial position of the end effector frame measured in the base frame in m and rad
     * @note The first 3 elements are the translation and the last 3 elements are the angle-axis
     * representation of the rotation
     */
    std::array<double, 6> positions{};
    /**
     * @brief Spatial velocity of the end effector frame with respect to the base frame measured
     * in the base frame in m/s and rad/s
     * @note The first 3 elements are the linear velocity and the last 3 elements are the
     * angular velocity
     */
    std::array<double, 6> velocities{};
    /**
     * @brief Spatial acceleration of the end effector frame with respect to the base frame
     * measured in the base frame in m/s^2 and rad/s^2
     * @note The first 3 elements are the linear acceleration and the last 3 elements are the
     * angular acceleration
     */
    std::array<double, 6> accelerations{};
    /**
     * @brief Spatial external efforts applied to the end effector frame measured in the base
     * frame in N and Nm
     * @note The first 3 elements are the force and the last 3 elements are the torque
     * @note All external efforts are assumed to be applied at the end effector frame
     */
    std::array<double, 6> external_efforts{};
  } cartesian{};
};

/// @brief Inherited logic error
class LogicError : public std::logic_error
{
public:
  using std::logic_error::logic_error;
};

/// @brief Inherited runtime error
class RuntimeError : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};

/// @brief Forward declaration of the QuinticHermiteInterpolator class
class QuinticHermiteInterpolator;

/// @brief Forward declaration of the EthernetManager class
class EthernetManager;

/// @brief Forward declaration of the AlgorithmInterface class
class AlgorithmInterface;

/// @brief Forward declaration of the Logger class
class Logger;

/// @brief End effector properties for the standard variants
struct StandardEndEffector {
  /// @brief WXAI V0 base variant
  static constexpr EndEffector wxai_v0_base{
    .palm = {
      .mass = 0.53780000,
      .inertia = {
        0.00079919, -0.00000049, 0.00000010,
        -0.00000049, 0.00047274, 0.00000004,
        0.00000010, 0.00000004, 0.00105293
      },
      .origin_xyz = {0.04572768, -0.00000726, 0.00001402},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.05945000,
      .inertia = {
        0.00001875, 0.00000309, -0.00000149,
        0.00000309, 0.00002614, -0.00000124,
        -0.00000149, -0.00000124, 0.00002995
      },
      .origin_xyz = {0.00169016, -0.00592796, -0.00365701},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.05945000,
      .inertia = {
        0.00001930, -0.00000309, 0.00000359,
        -0.00000309, 0.00002670, -0.00000064,
        0.00000359, -0.00000064, 0.00002995
      },
      .origin_xyz = {0.00169015, 0.00592793, 0.00201818},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.0227,
    .offset_finger_right = -0.0227,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief WXAI V0 leader variant
  static constexpr EndEffector wxai_v0_leader{
    .palm = {
      .mass = 0.59570000,
      .inertia = {
        0.00117653, -0.00000040, -0.00005492,
        -0.00000040, 0.00085696, 0.00000074,
        -0.00005492, 0.00000074, 0.00107685
      },
      .origin_xyz = {0.04454388, 0.00000506, -0.00694150},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.06380000,
      .inertia = {
        0.00003556, -0.00000249, 0.00000167,
        -0.00000249, 0.00002700, 0.00000217,
        0.00000167, 0.00000217, 0.00001726
      },
      .origin_xyz = {-0.00423580, -0.00167541, -0.01050810},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.06380000,
      .inertia = {
        0.00004133, 0.00000250, 0.00000517,
        0.00000250, 0.00003277, -0.00000592,
        0.00000517, -0.00000592, 0.00001727
      },
      .origin_xyz = {-0.00423309, 0.00167373, -0.00451087},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.0179,
    .offset_finger_right = -0.0179,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief WXAI V0 follower variant
  static constexpr EndEffector wxai_v0_follower{
    .palm = {
      .mass = 0.64230000,
      .inertia = {
        0.00108484, 0.00000063, -0.00004180,
        0.00000063, 0.00075170, -0.00001558,
        -0.00004180, -0.00001558, 0.00110994
      },
      .origin_xyz = {0.04699592, 0.00045936, 0.00827772},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.05945000,
      .inertia = {
        0.00001875, 0.00000309, -0.00000149,
        0.00000309, 0.00002614, -0.00000124,
        -0.00000149, -0.00000124, 0.00002995
      },
      .origin_xyz = {0.00169016, -0.00592796, -0.00365701},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.05945000,
      .inertia = {
        0.00001930, -0.00000309, 0.00000359,
        -0.00000309, 0.00002670, -0.00000064,
        0.00000359, -0.00000064, 0.00002995
      },
      .origin_xyz = {0.00169015, 0.00592793, 0.00201818},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.0227,
    .offset_finger_right = -0.0227,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief VXAI V0 base variant
  static constexpr EndEffector vxai_v0_base{
    .palm = {
      .mass = 0.57903811,
      .inertia = {
        0.00135277, -0.00000006, -0.00009912,
        -0.00000006, 0.00077371, 0.00000049,
        -0.00009912, 0.00000049, 0.00085992
      },
      .origin_xyz = {-0.01847211, -0.00000697, -0.08476049},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.08127100,
      .inertia = {
        0.00004094, -0.00000169, 0.00000204,
        -0.00000169, 0.00003574, -0.00000422,
        0.00000204, -0.00000422, 0.00002563
      },
      .origin_xyz = {-0.00365705, -0.00592794, -0.00169016},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.08127100,
      .inertia = {
        0.00004094, -0.00000087, -0.00000491,
        -0.00000087, 0.00003650, 0.00000422,
        -0.00000491, 0.00000422, 0.00002639
      },
      .origin_xyz = {0.00201821, 0.00592796, -0.00169015},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.0227,
    .offset_finger_right = -0.0227,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {-0.021, 0, -0.200062, 0.0, 0.0, 0.0}
  };

  /// @brief no gripper variant
  static constexpr EndEffector no_gripper{
    .palm = {
      .mass = 0.0,
      .inertia = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0
      },
      .origin_xyz = {0.0, 0.0, 0.0},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.0,
      .inertia = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0
      },
      .origin_xyz = {0.0, 0.0, 0.0},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.0,
      .inertia = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0
      },
      .origin_xyz = {0.0, 0.0, 0.0},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.0179,
    .offset_finger_right = -0.0179,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
  };
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_TYPE_HPP_
