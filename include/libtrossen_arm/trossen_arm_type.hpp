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
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace trossen_arm
{

/// @brief Log severity levels
enum class LogLevel : uint8_t {
  /// @brief Trace level (most verbose)
  trace,
  /// @brief Debug level
  debug,
  /// @brief Informational level
  info,
  /// @brief Warning level
  warn,
  /// @brief Error level
  error,
  /// @brief Critical level (least verbose)
  critical,
};

/**
 * @brief Callback type for custom logger backends
 * @param level Log severity level
 * @param name Logger name (e.g. "trossen_arm_driver" or "wxai_v0@192.168.1.2")
 * @param message Formatted log message
 */
using LogCallback =
  std::function<void(LogLevel level, const std::string & name, const std::string & message)>;

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

/// @brief Link inertial properties
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

/// @brief Joint kinematic properties
struct Joint
{
  /// @brief Unit twist of the joint expressed in the joint frame
  /// @note The first 3 elements are the angular component and the last 3 elements are the linear component
  std::array<double, 6> axis{};
  /// @brief Translation from the parent link frame to the joint frame in m
  std::array<double, 3> origin_xyz{};
  /// @brief RPY angles from the parent link frame to the joint frame in rad
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
  /// @brief Header
  struct Header
  {
    /// @brief Consecutively increasing ID since configuration
    uint32_t id{0};
    /// @brief Timestamp in microseconds since configuration
    uint64_t timestamp{0};
  } header{};

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
struct StandardEndEffector
{
  // clang-format off
  /// @brief WXAI V0 base variant first used in 2025-05-09
  static constexpr EndEffector wxai_v0_base_20250509{
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

  /// @brief WXAI V0 base variant first used in 2026-06-26
  static constexpr EndEffector wxai_v0_base_20260626{
    .palm = {
      .mass = 0.55984146,
      .inertia = {
        0.00083660, -0.00000073, 0.00000358,
        -0.00000073, 0.00038094, -0.00000017,
        0.00000358, -0.00000017, 0.00097121
      },
      .origin_xyz = {0.04530754, -0.00004231, 0.00021128},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.07283511,
      .inertia = {
        0.00002514, 0.00000487, -0.00000253,
        0.00000487, 0.00003633, -0.00000144,
        -0.00000253, -0.00000144, 0.00004148
      },
      .origin_xyz = {0.00300468, -0.00699875, -0.00383403},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.07283530,
      .inertia = {
        0.00002578, -0.00000487, 0.00000496,
        -0.00000487, 0.00003697, -0.00000055,
        0.00000496, -0.00000055, 0.00004148
      },
      .origin_xyz = {0.00300470, 0.00699864, 0.00244362},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.02449818,
    .offset_finger_right = -0.02450182,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief WXAI V0 leader variant first used in 2025-05-09
  static constexpr EndEffector wxai_v0_leader_20250509{
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

  /// @brief WXAI V0 leader variant first used in 2026-06-26
  static constexpr EndEffector wxai_v0_leader_20260626{
    .palm = {
      .mass = 0.65411940,
      .inertia = {
        0.00141948, -0.00000068, -0.00008034,
        -0.00000068, 0.00097213, 0.00000008,
        -0.00008034, 0.00000008, 0.00100814
      },
      .origin_xyz = {0.04351475, -0.00003630, -0.01014018},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.06566254,
      .inertia = {
        0.00004150, -0.00000254, 0.00000037,
        -0.00000254, 0.00003063, 0.00000328,
        0.00000037, 0.00000328, 0.00001893
      },
      .origin_xyz = {-0.00510623, -0.00176708, -0.01245444},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.06566254,
      .inertia = {
        0.00004970, 0.00000254, 0.00000415,
        0.00000254, 0.00003882, -0.00000767,
        0.00000415, -0.00000767, 0.00001893
      },
      .origin_xyz = {-0.00510588, 0.00176675, -0.00547648},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.01859818,
    .offset_finger_right = -0.01860182,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief WXAI V0 follower variant first used in 2025-05-09
  static constexpr EndEffector wxai_v0_follower_20250509{
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

  /// @brief WXAI V0 follower variant first used in 2026-06-26
  static constexpr EndEffector wxai_v0_follower_20260626{
    .palm = {
      .mass = 0.67244176,
      .inertia = {
        0.00115274, -0.00000008, -0.00001266,
        -0.00000008, 0.00067665, -0.00001573,
        -0.00001266, -0.00001573, 0.00101644
      },
      .origin_xyz = {0.04557269, 0.00040888, 0.00893526},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_left = {
      .mass = 0.07283511,
      .inertia = {
        0.00002514, 0.00000487, -0.00000253,
        0.00000487, 0.00003633, -0.00000144,
        -0.00000253, -0.00000144, 0.00004148
      },
      .origin_xyz = {0.00300468, -0.00699875, -0.00383403},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .finger_right = {
      .mass = 0.07283530,
      .inertia = {
        0.00002578, -0.00000487, 0.00000496,
        -0.00000487, 0.00003697, -0.00000055,
        0.00000496, -0.00000055, 0.00004148
      },
      .origin_xyz = {0.00300470, 0.00699864, 0.00244362},
      .origin_rpy = {0.0, 0.0, 0.0}
    },
    .offset_finger_left = 0.02449818,
    .offset_finger_right = -0.02450182,
    .pitch_circle_radius = 0.00875,
    .t_flange_tool = {0.156062, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  /// @brief VXAI V0 base variant first used in 2025-05-09
  static constexpr EndEffector vxai_v0_base_20250509{
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

  /// @brief no gripper variant first used in 2025-05-09
  static constexpr EndEffector no_gripper_20250509{
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
  // clang-format on

  /// @brief WXAI V0 base variant
  static inline const EndEffector & wxai_v0_base{wxai_v0_base_20260626};

  /// @brief WXAI V0 leader variant
  static inline const EndEffector & wxai_v0_leader{wxai_v0_leader_20260626};

  /// @brief WXAI V0 follower variant
  static inline const EndEffector & wxai_v0_follower{wxai_v0_follower_20260626};

  /// @brief VXAI V0 base variant
  static inline const EndEffector & vxai_v0_base{vxai_v0_base_20250509};

  /// @brief no gripper variant
  static inline const EndEffector & no_gripper{no_gripper_20250509};
};

/// @brief Motor parameters commonly used
struct StandardMotorParameters
{
  // clang-format off
  /**
   * @brief WXAI V0 motor parameters first used in 2025-05-09
   * @details Manually chosen parameters at the beginning of the development
   */
  static inline const std::vector<std::map<Mode, MotorParameter>> wxai_v0_20250509 {
    {  // Joint 0
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.004, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 1
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 27.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.004, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 2
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.004, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 3
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.001, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 4
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.001, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 5
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.001, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 6
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 0.05, .ki = 0.0005, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.001, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
  };

  /**
   * @brief WXAI V0 motor parameters first used in 2026-03-17
   * @details Reduced proportional gains for position loop of the first three joints
   * and removed integral gains for the velocity loop from all joints
   * compared to wxai_v0_20250509
   */
  static inline const std::vector<std::map<Mode, MotorParameter>> wxai_v0_20260317 {
    {  // Joint 0
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 16.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 1
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 27.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 16.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 2
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.04, .kd = 0.0, .imax = 9.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 16.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 120.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 8.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 3
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 80.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 4
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 5
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.01, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 40.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
    {  // Joint 6
      {
        Mode::idle,
        {
          .position = {.kp = 0.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 0.05, .ki = 0.0005, .kd = 0.0, .imax = 3.0}
        }
      },
      {
        Mode::position,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::velocity,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::external_effort,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
      {
        Mode::effort,
        {
          .position = {.kp = 20.0, .ki = 0.0, .kd = 0.0, .imax = 0.0},
          .velocity = {.kp = 1.0, .ki = 0.0, .kd = 0.0, .imax = 0.0}
        }
      },
    },
  };
  // clang-format on

  /// @brief Latest motor parameters for WXAI V0
  static inline const std::vector<std::map<Mode, MotorParameter>> & wxai_v0_latest{
    wxai_v0_20260317
  };

  /// @brief Default motor parameters for WXAI V0
  static inline const std::vector<std::map<Mode, MotorParameter>> & wxai_v0_default{
    wxai_v0_20260317
  };
};

/// @brief Link inertial properties commonly used
struct StandardLinks
{
  /**
   * @brief WXAI V0 link inertial properties first used in 2025-05-09
   * @details Order is base link, arm links 1-6, finger_left, finger_right
   */
  static inline const std::vector<Link> wxai_v0_20250509{
    {// Base link
     .mass = 0.45000000,
     .inertia =
       {0.00040781,
        0.00000011,
        -0.00000444,
        0.00000011,
        0.00043949,
        -0.00000076,
        -0.00000444,
        -0.00000076,
        0.00020951},
     .origin_xyz = {-0.00014175, -0.00005228, 0.03175177},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 1
     .mass = 0.14930000,
     .inertia =
       {0.00018096,
        -0.00000187,
        -0.00001404,
        -0.00000187,
        0.00008172,
        -0.00000127,
        -0.00001404,
        -0.00000127,
        0.00018791},
     .origin_xyz = {-0.00011075, 0.00171176, 0.02044592},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 2
     .mass = 1.18560000,
     .inertia =
       {0.00105519,
        -0.00000328,
        -0.00002754,
        -0.00000328,
        0.01784952,
        -0.00000198,
        -0.00002754,
        -0.00000198,
        0.01844965},
     .origin_xyz = {-0.13121451, -0.00292583, 0.00021345},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 3
     .mass = 0.69280000,
     .inertia =
       {0.00065456,
        0.00009210,
        -0.00052467,
        0.00009210,
        0.00552297,
        0.00001169,
        -0.00052467,
        0.00001169,
        0.00566525},
     .origin_xyz = {0.18083602, -0.00094090, 0.05554937},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 4
     .mass = 0.47700000,
     .inertia =
       {0.00056085,
        0.00000719,
        -0.00018922,
        0.00000719,
        0.00068897,
        0.00000754,
        -0.00018922,
        0.00000754,
        0.00043873},
     .origin_xyz = {0.05797842, 0.00027145, 0.05884447},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 5
     .mass = 0.36400000,
     .inertia =
       {0.00018761,
        -0.00000045,
        0.00000089,
        -0.00000045,
        0.00031805,
        0.00000003,
        0.00000089,
        0.00000003,
        0.00026536},
     .origin_xyz = {0.00412447, -0.00001138, -0.04283184},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 6
     .mass = 0.58390000,
     .inertia =
       {0.00086769,
        -0.00000054,
        0.00000011,
        -0.00000054,
        0.00051326,
        0.00000004,
        0.00000011,
        0.00000004,
        0.00114318},
     .origin_xyz = {0.04572768, -0.00000726, 0.00001402},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger left
     .mass = 0.05945000,
     .inertia =
       {0.00001875,
        0.00000309,
        -0.00000149,
        0.00000309,
        0.00002614,
        -0.00000124,
        -0.00000149,
        -0.00000124,
        0.00002995},
     .origin_xyz = {0.00169016, -0.00592796, -0.00365701},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger right
     .mass = 0.05945000,
     .inertia =
       {0.00001930,
        -0.00000309,
        0.00000359,
        -0.00000309,
        0.00002670,
        -0.00000064,
        0.00000359,
        -0.00000064,
        0.00002995},
     .origin_xyz = {0.00169015, 0.00592793, 0.00201818},
     .origin_rpy = {0.0, 0.0, 0.0}
    }
  };

  /**
   * @brief WXAI V0 link inertial properties first used in 2026-06-26
   * @details Order is base link, arm links 1-6, finger_left, finger_right
   */
  static inline const std::vector<Link> wxai_v0_20260626{
    {// Base link
     .mass = 0.46273356,
     .inertia =
       {0.00022947,
        0.00000013,
        -0.00000488,
        0.00000013,
        0.00026286,
        -0.00000028,
        -0.00000488,
        -0.00000028,
        0.00023167},
     .origin_xyz = {-0.00020063, -0.00003382, 0.03051419},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 1
     .mass = 0.15008228,
     .inertia =
       {0.00018088,
        -0.00000118,
        -0.00001396,
        -0.00000118,
        0.00008194,
        -0.00000042,
        -0.00001396,
        -0.00000042,
        0.00018797},
     .origin_xyz = {-0.00018072, 0.00138124, 0.02037991},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 2
     .mass = 1.13581111,
     .inertia =
       {0.00064547,
        -0.00000050,
        -0.00001069,
        -0.00000050,
        0.01736392,
        -0.00000063,
        -0.00001069,
        -0.00000063,
        0.01753333},
     .origin_xyz = {-0.13172378, -0.00180325, 0.00008127},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 3
     .mass = 0.67178383,
     .inertia =
       {0.00053201,
        0.00008141,
        -0.00053779,
        0.00008141,
        0.00543866,
        0.00001195,
        -0.00053779,
        0.00001195,
        0.00545384},
     .origin_xyz = {0.18204373, -0.00049130, 0.05541290},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 4
     .mass = 0.47899686,
     .inertia =
       {0.00045072,
        0.00001099,
        -0.00019411,
        0.00001099,
        0.00058606,
        0.00001185,
        -0.00019411,
        0.00001185,
        0.00045456},
     .origin_xyz = {0.05845470, 0.00047346, 0.05984125},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 5
     .mass = 0.36818897,
     .inertia =
       {0.00019188,
        -0.00000044,
        0.00000121,
        -0.00000044,
        0.00019906,
        -0.00000010,
        0.00000121,
        -0.00000010,
        0.00014673},
     .origin_xyz = {0.00357317, -0.00006705, -0.04271537},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Link 6
     .mass = 0.65411940,
     .inertia =
       {0.00141948,
        -0.00000068,
        -0.00008034,
        -0.00000068,
        0.00097213,
        0.00000008,
        -0.00008034,
        0.00000008,
        0.00100814},
     .origin_xyz = {0.04351475, -0.00003630, -0.01014018},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger left
     .mass = 0.06566254,
     .inertia =
       {0.00004150,
        -0.00000254,
        0.00000037,
        -0.00000254,
        0.00003063,
        0.00000328,
        0.00000037,
        0.00000328,
        0.00001893},
     .origin_xyz = {-0.00510623, -0.00176708, -0.01245444},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger right
     .mass = 0.06566254,
     .inertia =
       {0.00004970,
        0.00000254,
        0.00000415,
        0.00000254,
        0.00003882,
        -0.00000767,
        0.00000415,
        -0.00000767,
        0.00001893},
     .origin_xyz = {-0.00510588, 0.00176675, -0.00547648},
     .origin_rpy = {0.0, 0.0, 0.0}
    }
  };
};

/// @brief Joint kinematic properties commonly used
struct StandardJoints
{
  /**
   * @brief WXAI V0 joint kinematic properties first used in 2025-05-09
   * @details Order is arm joints 0-5, finger_left, finger_right
   */
  static inline const std::vector<Joint> wxai_v0_20250509{
    {// Joint 0
     .axis = {0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.0, 0.0, 0.05725},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 1
     .axis = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.02, 0.0, 0.04625},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 2
     .axis = {0.0, -1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {-0.264, 0.0, 0.0},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 3
     .axis = {0.0, -1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.245, 0.0, 0.06},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 4
     .axis = {0.0, 0.0, -1.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.06775, 0.0, 0.0455},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 5
     .axis = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.02895, 0.0, -0.0455},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger left
     .axis = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
     .origin_xyz = {0.078, 0.01485, 0.0},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger right
     .axis = {0.0, 0.0, 0.0, 0.0, -1.0, 0.0},
     .origin_xyz = {0.078, -0.01485, 0.0},
     .origin_rpy = {0.0, 0.0, 0.0}
    }
  };

  /**
   * @brief WXAI V0 joint kinematic properties first used in 2026-06-26
   * @details Order is arm joints 0-5, finger_left, finger_right
   */
  static inline const std::vector<Joint> wxai_v0_20260626{
    {// Joint 0
     .axis = {0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.00000000, 0.00000000, 0.05725000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 1
     .axis = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.02000000, 0.00000000, 0.04625000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 2
     .axis = {0.0, -1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {-0.26400000, 0.00000000, 0.00000000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 3
     .axis = {0.0, -1.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.24500000, 0.00000000, 0.06000000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 4
     .axis = {0.0, 0.0, -1.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.06775000, -0.00005000, 0.04550000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Joint 5
     .axis = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
     .origin_xyz = {0.02895000, 0.00000000, -0.04550000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger left
     .axis = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0},
     .origin_xyz = {0.08650000, 0.01859818, 0.00000000},
     .origin_rpy = {0.0, 0.0, 0.0}
    },
    {// Finger right
     .axis = {0.0, 0.0, 0.0, 0.0, -1.0, 0.0},
     .origin_xyz = {0.08650000, -0.01860182, 0.00000000},
     .origin_rpy = {0.0, 0.0, 0.0}
    }
  };
};

/// @brief Notices
enum class Notice : uint8_t {
  /// @brief Notice issued on 2026-03-17 about coming changes to default motor parameters
  default_motor_parameters_20260317
};

/// @brief Error states reported by the arm controller
enum class ErrorState : uint8_t {
  /// @brief No error
  none,
  /// @brief Controller's Ethernet manager failed to initialize
  ethernet_init_failed,
  /// @brief Controller's CAN interface failed to initialize
  can_init_failed,
  /// @brief Controller's CAN interface failed to send a message
  joint_command_failed,
  /// @brief Controller's CAN interface failed to receive a message
  joint_feedback_failed,
  /// @brief Joint clear error command failed
  joint_clear_error_failed,
  /// @brief Joint enable command failed
  joint_enable_failed,
  /// @brief Joint disable command failed
  joint_disable_failed,
  /// @brief Joint home calibration command failed
  joint_set_home_failed,
  /// @brief Joint disabled unexpectedly
  joint_disabled_unexpectedly,
  /// @brief Joint overheated
  joint_overheated,
  /// @brief Invalid mode command received
  invalid_mode,
  /// @brief Invalid robot command indicator received
  invalid_robot_command,
  /// @brief Invalid configuration address
  invalid_configuration_address,
  /// @brief Robot input with modes different than configured modes received
  robot_input_mode_mismatch,
  /// @brief Joint limit exceeded
  joint_limit_exceeded,
  /// @brief Robot input infinite
  robot_input_infinite
};

/// @brief Mapping from ErrorState to human-readable description
inline const std::map<ErrorState, std::string> ERROR_INFORMATION = {
  {ErrorState::none, "No error"},
  {ErrorState::ethernet_init_failed, "Controller's Ethernet manager failed to initialize"},
  {ErrorState::can_init_failed, "Controller's CAN interface failed to initialize"},
  {ErrorState::joint_command_failed, "Controller's CAN interface failed to send a message"},
  {ErrorState::joint_feedback_failed, "Controller's CAN interface failed to receive a message"},
  {ErrorState::joint_clear_error_failed, "Joint clear error command failed"},
  {ErrorState::joint_enable_failed, "Joint enable command failed"},
  {ErrorState::joint_disable_failed, "Joint disable command failed"},
  {ErrorState::joint_set_home_failed, "Joint home calibration command failed"},
  {ErrorState::joint_disabled_unexpectedly, "Joint disabled unexpectedly"},
  {ErrorState::joint_overheated, "Joint overheated"},
  {ErrorState::invalid_mode, "Invalid mode command received"},
  {ErrorState::invalid_robot_command, "Invalid robot command indicator received"},
  {ErrorState::invalid_configuration_address, "Invalid configuration address"},
  {ErrorState::robot_input_mode_mismatch,
   "Robot input with modes different than configured modes received"},
  {ErrorState::joint_limit_exceeded, "Joint limit exceeded"},
  {ErrorState::robot_input_infinite, "Robot input with infinite values received"}
};

/// @brief Mapping from Model to model name string
inline const std::map<Model, std::string> MODEL_NAME = {
  {Model::wxai_v0, "wxai_v0"},
  {Model::vxai_v0_right, "vxai_v0_right"},
  {Model::vxai_v0_left, "vxai_v0_left"},
};

/// @brief Mapping from Mode to mode name string
inline const std::map<Mode, std::string> MODE_NAME = {
  {Mode::idle, "idle"},
  {Mode::position, "position"},
  {Mode::velocity, "velocity"},
  {Mode::external_effort, "external_effort"},
  {Mode::effort, "effort"},
};

/// @brief Information about a discovered arm controller
struct DiscoverResult
{
  /// @brief IP address of the arm controller
  std::string ip{};
  /// @brief Model of the arm controller
  Model model{Model::wxai_v0};
  /// @brief Firmware version reported by the arm controller
  std::string firmware_version{};
  /// @brief Error state of the arm controller
  ErrorState error_state{ErrorState::none};
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_TYPE_HPP_
