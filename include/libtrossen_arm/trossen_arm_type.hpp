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
  double effort_correction;
  /// @brief Friction transition velocity in rad/s for arm joints or m/s for the gripper joint
  /// @note It must be positive
  double friction_transition_velocity;
  /// @brief Friction constant term in Nm for arm joints or N for the gripper joint
  double friction_constant_term;
  /// @brief Friction coulomb coef in Nm/Nm for arm joints or N/N for the gripper joint
  double friction_coulomb_coef;
  /// @brief Friction viscous coef in Nm/(rad/s) for arm joints or N/(m/s) for the gripper joint
  double friction_viscous_coef;
};

/// @brief Link properties
struct Link
{
  /// @brief Mass in kg
  double mass;
  /// @brief Inertia in kg m^2
  std::array<double, 9> inertia;
  /// @brief Inertia frame translation measured in link frame in m
  std::array<double, 3> origin_xyz;
  /// @brief Inertia frame RPY angles measured in link frame in rad
  std::array<double, 3> origin_rpy;
};

/// @brief End effector properties
struct EndEffector
{
  /// @brief Properties of the palm link
  Link palm;
  /// @brief Properties of the left finger link
  Link finger_left;
  /// @brief Properties of the right finger link
  Link finger_right;
  /// @brief Offset from the palm center to the left carriage center in m in home configuration
  double offset_finger_left;
  /// @brief Offset from the palm center to the right carriage center in m in home configuration
  double offset_finger_right;
  /// @brief Pitch circle radius in m
  double pitch_circle_radius;
  /**
   * @brief The tool frame pose measured in the flange frame
   * @note The first 3 elements are the translation and the last 3 elements are the
   * angle-axis representation of the rotation
   */
  std::array<double, 6> t_flange_tool;
};

/// @brief Joint limit
struct JointLimit
{
  /** @brief Minimum position in rad for arm joints and m for gripper */
  double position_min;
  /** @brief Maximum position in rad for arm joints and m for gripper */
  double position_max;
  /** @brief Tolerance on output position in rad for arm joints and m for gripper */
  double position_tolerance;
  /** @brief Maximum velocity in rad/s for arm joints and m/s for gripper */
  double velocity_max;
  /** @brief Tolerance on output velocity in rad/s for arm joints and m/s for gripper */
  double velocity_tolerance;
  /** @brief Maximum effort in Nm for arm joints and N for gripper */
  double effort_max;
  /** @brief Tolerance on output effort in Nm for arm joints and N for gripper */
  double effort_tolerance;
};

/** @brief PID parameter structure */
struct PIDParameter
{
  /** @brief Proportional gain */
  double kp;
  /** @brief Integral gain */
  double ki;
  /** @brief Derivative gain */
  double kd;
  /** @brief Maximum integral value */
  double imax;
};

/** @brief Motor parameter */
struct MotorParameter
{
  /** @brief Position loop PID parameter */
  PIDParameter position;
  /** @brief Velocity loop PID parameter */
  PIDParameter velocity;
};

  /** @brief Parameter used for robotic algorithms */
struct AlgorithmParameter
{
  /** @brief Threshold for singularity detection */
  double singularity_threshold;
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
      std::vector<double> positions;
      /// @brief Velocities in rad/s for arm joints and m/s for the gripper joint
      std::vector<double> velocities;
      /// @brief Accelerations in rad/s^2 for arm joints and m/s^2 for the gripper joint
      std::vector<double> accelerations;
      /// @brief Efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> efforts;
      /// @brief External efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> external_efforts;
      /// @brief Compensation efforts in Nm for arm joints and N for the gripper joint
      std::vector<double> compensation_efforts;
      /// @brief Rotor temperatures in C
      std::vector<double> rotor_temperatures;
      /// @brief Driver temperatures in C
      std::vector<double> driver_temperatures;
    } all;

    /// @brief Outputs of the arm joints
    struct Arm
    {
      /// @brief Positions in rad
      std::vector<double> positions;
      /// @brief Velocities in rad/s
      std::vector<double> velocities;
      /// @brief Accelerations in rad/s^2
      std::vector<double> accelerations;
      /// @brief Efforts in Nm
      std::vector<double> efforts;
      /// @brief External efforts in Nm
      std::vector<double> external_efforts;
      /// @brief Compensation efforts in Nm
      std::vector<double> compensation_efforts;
      /// @brief Rotor temperatures in C
      std::vector<double> rotor_temperatures;
      /// @brief Driver temperatures in C
      std::vector<double> driver_temperatures;
    } arm;

    /// @brief Outputs of the gripper joint
    struct Gripper
    {
      /// @brief Position in m
      double position;
      /// @brief Velocity in m/s
      double velocity;
      /// @brief Acceleration in m/s^2
      double acceleration;
      /// @brief Effort in N
      double effort;
      /// @brief External effort in N
      double external_effort;
      /// @brief Compensation effort in N
      double compensation_effort;
      /// @brief Rotor temperature in C
      double rotor_temperature;
      /// @brief Driver temperature in C
      double driver_temperature;
    } gripper;
  } joint;

  /// @brief Outputs in Cartesian space
  struct Cartesian
  {
    // TODO(Shiming-Liang): Add helper functions to convert rotations
    /**
     * @brief Spatial position of the end effector frame measured in the base frame in m and rad
     * @note The first 3 elements are the translation and the last 3 elements are the angle-axis
     * representation of the rotation
     */
    std::array<double, 6> positions;
    /**
     * @brief Spatial velocity of the end effector frame with respect to the base frame measured
     * in the base frame in m/s and rad/s
     * @note The first 3 elements are the linear velocity and the last 3 elements are the
     * angular velocity
     */
    std::array<double, 6> velocities;
    /**
     * @brief Spatial acceleration of the end effector frame with respect to the base frame
     * measured in the base frame in m/s^2 and rad/s^2
     * @note The first 3 elements are the linear acceleration and the last 3 elements are the
     * angular acceleration
     */
    std::array<double, 6> accelerations;
    /**
     * @brief Spatial external efforts applied to the end effector frame measured in the base
     * frame in N and Nm
     * @note The first 3 elements are the force and the last 3 elements are the torque
     * @note All external efforts are assumed to be applied at the end effector frame
     */
    std::array<double, 6> external_efforts;
  } cartesian;
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_TYPE_HPP_
