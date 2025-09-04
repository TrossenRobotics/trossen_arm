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

#ifndef LIBTROSSEN_ARM__TROSSEN_ARM_HPP_
#define LIBTROSSEN_ARM__TROSSEN_ARM_HPP_

#include <cstdint>

#include <array>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#include "libtrossen_arm/trossen_arm_type.hpp"

namespace trossen_arm
{

/// @brief Trossen Arm Driver
class TrossenArmDriver
{
public:
  /// @brief Construct the Trossen Arm Driver object
  TrossenArmDriver();

  /// @brief Destroy the Trossen Arm Driver object
  ~TrossenArmDriver();

  /**
   * @brief Configure the driver
   *
   * @param model Model of the robot
   * @param end_effector End effector properties
   * @param serv_ip IP address of the robot
   * @param clear_error Whether to clear the error state of the robot
   * @param timeout Timeout for connection to the arm controller's TCP server in seconds, default is
   * 20.0s
   */
  void configure(
    Model model,
    EndEffector end_effector,
    const std::string serv_ip,
    bool clear_error,
    double timeout = 20.0
  );

  /**
   * @brief Cleanup the driver
   *
   * @param reboot_controller Whether to reboot the controller
   */
  void cleanup(bool reboot_controller = false);

  /**
   * @brief Reboot the controller and cleanup the driver
   *
   * @note This function is a wrapper for cleanup(true)
   */
  inline void reboot_controller()
  {
    cleanup(true);
  }

  /**
   * @brief Set the positions of all joints
   *
   * @param goal_positions Positions in rad for arm joints and m for the gripper joint
   * @param goal_time Optional: goal time in s when the goal positions should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal positions are reached, default true
   * @param goal_feedforward_velocities Optional: feedforward velocities in rad/s for arm joints
   * and m/s for the gripper joint, default zeros
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2 for arm
   * joints and m/s^2 for the gripper joint, default zeros
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_positions(
    const std::vector<double> & goal_positions,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::vector<double>> & goal_feedforward_velocities = std::nullopt,
    const std::optional<std::vector<double>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the positions of the arm joints
   *
   * @param goal_positions Positions in rad
   * @param goal_time Optional: goal time in s when the goal positions should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal positions are reached, default true
   * @param goal_feedforward_velocities Optional: feedforward velocities in rad/s, default zeros
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2, default
   * zeros
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_positions(
    const std::vector<double> & goal_positions,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::vector<double>> & goal_feedforward_velocities = std::nullopt,
    const std::optional<std::vector<double>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the position of the gripper
   *
   * @param goal_position Position in m
   * @param goal_time Optional: goal time in s when the goal position should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal position is reached, default true
   * @param goal_feedforward_velocity Optional: feedforward velocity in m/s, default zero
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in m/s^2, default zero
   */
  void set_gripper_position(
    double goal_position,
    double goal_time = 2.0,
    bool blocking = true,
    double goal_feedforward_velocity = 0.0,
    double goal_feedforward_acceleration = 0.0);

  /**
   * @brief Set the position of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_position Position in rad for arm joints and m for the gripper joint
   * @param goal_time Optional: goal time in s when the goal position should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal position is reached, default true
   * @param goal_feedforward_velocity Optional: feedforward velocity in rad/s for arm joints and
   * m/s for the gripper joint, default zero
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in rad/s^2 for arm
   * joints and m/s^2 for the gripper joint, default zero
   */
  void set_joint_position(
    uint8_t joint_index,
    double goal_position,
    double goal_time = 2.0,
    bool blocking = true,
    double goal_feedforward_velocity = 0.0,
    double goal_feedforward_acceleration = 0.0
  );

  /**
   * @brief Set the position of the end effector in Cartesian space
   *
   * @param goal_positions Spatial position of the end effector frame measured in the base frame
   * in m and rad
   * @param interpolation_space Interpolation space, one of InterpolationSpace::joint or
   * InterpolationSpace::cartesian
   * @param goal_time Optional: goal time in s when the goal positions should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal positions are reached, default true
   * @param goal_feedforward_velocities Optional: spatial velocity of the end effector frame with
   * respect to the base frame measured in the base frame in m/s and rad/s, default zeros
   * @param goal_feedforward_accelerations Optional: spatial acceleration of the end effector frame
   * with respect to the base frame measured in the base frame in m/s^2 and rad/s^2, default
   * zeros
   * @param num_trajectory_check_samples Optional: number of evenly spaced sampled time steps to
   * check trajectory feasibility, default 1000
   *
   * @note The first 3 elements of goal_positions are the translation and the last 3 elements are
   * the angle-axis representation of the rotation
   * @note The first 3 elements of goal_feedforward_velocities are the linear velocity and the last
   * 3 elements are the angular velocity
   * @note The first 3 elements of goal_feedforward_accelerations are the linear acceleration and
   * the last 3 elements are the angular acceleration
   */
  void set_cartesian_positions(
    const std::array<double, 6> & goal_positions,
    InterpolationSpace interpolation_space,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::array<double, 6>> & goal_feedforward_velocities = std::nullopt,
    const std::optional<std::array<double, 6>> & goal_feedforward_accelerations = std::nullopt,
    int num_trajectory_check_samples = 1000
  );

  /**
   * @brief Set the velocities of all joints
   *
   * @param goal_velocities Velocities in rad/s for arm joints and m/s for the gripper joint
   * @param goal_time Optional: goal time in s when the goal velocities should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal velocities are reached, default true
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2 for arm
   * joints and m/s^2 for the gripper joint, default zeros
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_velocities(
    const std::vector<double> & goal_velocities,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::vector<double>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocities of the arm joints
   *
   * @param goal_velocities Velocities in rad
   * @param blocking Optional: whether to block until the goal velocities are reached, default true
   * @param goal_time Optional: goal time in s when the goal velocities should be reached, default
   * 2.0s
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2, default
   * zeros
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_velocities(
    const std::vector<double> & goal_velocities,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::vector<double>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocity of the gripper
   *
   * @param goal_velocity Velocity in m/s
   * @param goal_time Optional: goal time in s when the goal velocity should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal velocity is reached, default true
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in m/s^2, default zero
   */
  void set_gripper_velocity(
    double goal_velocity,
    double goal_time = 2.0,
    bool blocking = true,
    double goal_feedforward_acceleration = 0.0
  );

  /**
   * @brief Set the velocity of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_velocity Velocity in rad/s for arm joints and m/s for the gripper joint
   * @param goal_time Optional: goal time in s when the goal velocity should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal velocity is reached, default true
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in rad/s^2 for arm
   * joints and m/s^2 for the gripper joint, default zero
   */
  void set_joint_velocity(
    uint8_t joint_index,
    double goal_velocity,
    double goal_time = 2.0,
    bool blocking = true,
    double goal_feedforward_acceleration = 0.0
  );

  /**
   * @brief Set the velocity of the end effector in Cartesian space
   *
   * @param goal_velocities Spatial velocity of the end effector frame with respect to the base
   * frame measured in the base frame in m/s and rad/s
   * @param interpolation_space Interpolation space, one of InterpolationSpace::joint or
   * InterpolationSpace::cartesian
   * @param goal_time Optional: goal time in s when the goal velocities should be reached, default
   * 2.0s
   * @param blocking Optional: whether to block until the goal velocities are reached, default true
   * @param goal_feedforward_accelerations Optional: spatial acceleration of the end effector frame
   * with respect to the base frame measured in the base frame in m/s^2 and rad/s^2, default
   * zeros
   *
   * @note The first 3 elements of goal_velocities are the linear velocity and the last 3 elements
   * are the angular velocity
   * @note The first 3 elements of goal_feedforward_accelerations are the linear acceleration and
   * the last 3 elements are the angular acceleration
   */
  void set_cartesian_velocities(
    const std::array<double, 6> & goal_velocities,
    InterpolationSpace interpolation_space,
    double goal_time = 2.0,
    bool blocking = true,
    const std::optional<std::array<double, 6>> & goal_feedforward_accelerations = std::nullopt
  );

  /**
   * @brief Set the external efforts of all joints
   *
   * @param goal_external_efforts External efforts in Nm for arm joints and N for the gripper joint
   * @param goal_time Optional: goal time in s when the goal external efforts should be
   * reached, default 2.0s
   * @param blocking Optional: whether to block until the goal external efforts are reached, default
   * true
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_external_efforts(
    const std::vector<double> & goal_external_efforts,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the external efforts of the arm joints
   *
   * @param goal_external_efforts External efforts in Nm
   * @param goal_time Optional: goal time in s when the goal external efforts should be
   * reached, default 2.0s
   * @param blocking Optional: whether to block until the goal external efforts are reached, default
   * true
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_external_efforts(
    const std::vector<double> & goal_external_efforts,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the external effort of the gripper
   *
   * @param goal_external_effort External effort in N
   * @param goal_time Optional: goal time in s when the goal external effort should be
   * reached, default 2.0s
   * @param blocking Optional: whether to block until the goal external effort is reached, default
   * true
   */
  void set_gripper_external_effort(
    double goal_external_effort,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the external effort of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_external_effort External effort in Nm for arm joints and N for the gripper joint
   * @param goal_time Optional: goal time in s when the goal external effort should be
   * reached, default 2.0s
   * @param blocking Optional: whether to block until the goal external effort is reached, default
   * true
   */
  void set_joint_external_effort(
    uint8_t joint_index,
    double goal_external_effort,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the external efforts of the end effector in Cartesian space
   *
   * @param goal_external_efforts Spatial external efforts applied to the end effector frame
   * measured in the base frame in N and Nm
   * @param interpolation_space Interpolation space, one of InterpolationSpace::joint or
   * InterpolationSpace::cartesian
   * @param goal_time Optional: goal time in s when the goal external efforts should be
   * reached, default 2.0s
   * @param blocking Optional: whether to block until the goal external efforts are reached, default
   * true
   *
   * @note The first 3 elements of goal_external_efforts are the force and the last 3 elements
   * are the torque
   */
  void set_cartesian_external_efforts(
    const std::array<double, 6> & goal_external_efforts,
    InterpolationSpace interpolation_space,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the efforts of all joints
   *
   * @param goal_efforts Efforts in Nm for arm joints and N for the gripper joint
   * @param goal_time Optional: goal time in s when the goal efforts should be reached, default 2.0s
   * @param blocking Optional: whether to block until the goal efforts are reached, default true
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_efforts(
    const std::vector<double> & goal_efforts,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the efforts of the arm joints
   *
   * @param goal_efforts Efforts in Nm
   * @param goal_time Optional: goal time in s when the goal efforts should be reached, default 2.0s
   * @param blocking Optional: whether to block until the goal efforts are reached, default true
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_efforts(
    const std::vector<double> & goal_efforts,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the effort of the gripper
   *
   * @param goal_effort Effort in N
   * @param goal_time Optional: goal time in s when the goal effort should be reached, default 2.0s
   * @param blocking Optional: whether to block until the goal effort is reached, default true
   */
  void set_gripper_effort(
    double goal_effort,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Set the effort of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_effort Effort in Nm for arm joints and N for the gripper joint
   * @param goal_time Optional: goal time in s when the goal effort should be reached, default 2.0s
   * @param blocking Optional: whether to block until the goal effort is reached, default true
   */
  void set_joint_effort(
    uint8_t joint_index,
    double goal_effort,
    double goal_time = 2.0,
    bool blocking = true
  );

  /**
   * @brief Load configurations from a YAML file and set them
   * @param file_path The file path to load the configurations
   */
  void load_configs_from_file(const std::string & file_path);

  /**
   * @brief Set the factory reset flag
   *
   * @param flag Whether to reset the configurations to factory defaults at the next startup
   */
  void set_factory_reset_flag(bool flag = true);

  /**
   * @brief Set the IP method
   *
   * @param method The IP method to set, one of IPMethod::manual or IPMethod::dhcp
   */
  void set_ip_method(IPMethod method = IPMethod::manual);

  /**
   * @brief Set the manual IP
   *
   * @param manual_ip The manual IP address to set
   */
  void set_manual_ip(const std::string manual_ip = "192.168.1.2");

  /**
   * @brief Set the DNS
   *
   * @param dns The DNS to set
   */
  void set_dns(const std::string dns = "8.8.8.8");

  /**
   * @brief Set the gateway
   *
   * @param gateway The gateway to set
   */
  void set_gateway(const std::string gateway = "192.168.1.1");

  /**
   * @brief Set the subnet
   *
   * @param subnet The subnet to set
   */
  void set_subnet(const std::string subnet = "255.255.255.0");

  /**
   * @brief Set the joint characteristics
   *
   * @param joint_characteristics Joint characteristics
   *
   * @note The size of the vector should be equal to the number of joints
   *
   * @note Some joint characteristics are required to be within the following ranges
   *
   * - effort_correction: [0.2, 5.0]
   *
   * - friction_transition_velocity: positive
   */
  void set_joint_characteristics(const std::vector<JointCharacteristic> & joint_characteristics);

  /**
   * @brief Set the effort corrections
   *
   * @param effort_corrections Effort corrections in motor effort unit / Nm or N
   *
   * @note This configuration is used to map the efforts in Nm or N to the motor
   * effort unit, i.e., effort_correction = motor effort unit / Nm or N
   *
   * @note The size of the vector should be equal to the number of joints
   *
   * @note Each element in the vector should be within the range [0.2, 5.0]
   */
  void set_effort_corrections(const std::vector<double> & effort_corrections);

  /**
   * @brief Set the friction transition velocities
   *
   * @param friction_transition_velocities Friction transition velocities in rad/s for arm joints
   * and m/s for the gripper joint
   *
   * @note The size of the vector should be equal to the number of joints
   *
   * @note Each element in the vector should be positive
   */
  void set_friction_transition_velocities(
    const std::vector<double> & friction_transition_velocities
  );

  /**
   * @brief Set the friction constant terms
   *
   * @param friction_constant_terms Friction constant terms in Nm for arm joints and N for the
   * gripper joint
   *
   * @note The size of the vector should be equal to the number of joints
   */
  void set_friction_constant_terms(const std::vector<double> & friction_constant_terms);

  /**
   * @brief Set the friction coulomb coefs
   *
   * @param friction_coulomb_coefs Friction coulomb coefs in Nm/Nm for arm joints and N/N for the
   * gripper joint
   *
   * @note The size of the vector should be equal to the number of joints
   */
  void set_friction_coulomb_coefs(const std::vector<double> & friction_coulomb_coefs);

  /**
   * @brief Set the friction viscous coefs
   *
   * @param friction_viscous_coefs Friction viscous coefs in Nm/(rad/s) for arm joints and N/(m/s)
   * for the gripper joint
   *
   * @note The size of the vector should be equal to the number of joints
   */
  void set_friction_viscous_coefs(const std::vector<double> & friction_viscous_coefs);

  /**
   * @brief Set the position offsets
   *
   * @param position_offsets Position offsets in rad for arm joints and m for the gripper joint
   *
   * @note The size of the vector should be equal to the number of joints
   */
  void set_position_offsets(const std::vector<double> & position_offsets);

  /**
   * @brief Set the modes of each joint
   *
   * @param modes Desired modes for each joint, one of
   *
   * - Mode::idle
   *
   * - Mode::position
   *
   * - Mode::velocity
   *
   * - Mode::external_effort
   *
   * - Mode::effort
   *
   * @note The size of the vector should be equal to the number of joints
   */
  void set_joint_modes(const std::vector<Mode> & modes);

  /**
   * @brief Set all joints to the same mode
   *
   * @param mode Desired mode for all joints, one of
   *
   * - Mode::idle
   *
   * - Mode::position
   *
   * - Mode::velocity
   *
   * - Mode::external_effort
   *
   * - Mode::effort
   */
  void set_all_modes(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the arm joints
   *
   * @param mode Desired mode for the arm joints, one of
   *
   * - Mode::idle
   *
   * - Mode::position
   *
   * - Mode::velocity
   *
   * - Mode::external_effort
   *
   * - Mode::effort
   *
   * @warning This method does not change the gripper joint's mode
   */
  void set_arm_modes(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the gripper joint
   *
   * @param mode Desired mode for the gripper joint, one of
   *
   * - Mode::idle
   *
   * - Mode::position
   *
   * - Mode::velocity
   *
   * - Mode::external_effort
   *
   * - Mode::effort
   *
   * @warning This method does not change the arm joints' mode
   */
  void set_gripper_mode(Mode mode = Mode::idle);

  /**
   * @brief Set the end effector properties
   *
   * @param end_effector The end effector properties
   */
  void set_end_effector(const EndEffector & end_effector);

  /**
   * @brief Set the joint limits
   *
   * @param joint_limits Joint limits of all joints
   */
  void set_joint_limits(const std::vector<JointLimit> & joint_limits);

  /**
   * @brief Set the motor parameters
   *
   * @param motor_parameters Motor parameters of all modes of all joints
   */
  void set_motor_parameters(const std::vector<std::map<Mode, MotorParameter>> & motor_parameters);

  /**
   * @brief Set the algorithm parameter
   *
   * @param algorithm_parameter Parameter used for robotic algorithms
   */
  void set_algorithm_parameter(const AlgorithmParameter & algorithm_parameter);

  /**
   * @brief Get the number of joints
   *
   * @return Number of joints
   */
  uint8_t get_num_joints() const;

  /**
   * @brief Get driver version
   *
   * @return Driver version
   */
  const std::string & get_driver_version() const;

  /**
   * @brief Get controller firmware version
   *
   * @return Controller firmware version
   */
  const std::string & get_controller_version() const;

  /**
   * @brief Get the robot output
   *
   * @return Robot output
   */
  const RobotOutput & get_robot_output();

  /**
   * @brief Get the positions
   *
   * @return Positions in rad for arm joints and m for the gripper joint
   */
  [[deprecated(
    "get_positions will be deprecated in the next version, "
    "please use get_all_positions instead"
  )]]
  const std::vector<double> & get_positions();

  /**
   * @brief Get the positions of all joints
   *
   * @return Positions in rad for arm joints and m for the gripper joint
   */
  const std::vector<double> & get_all_positions();

  /**
   * @brief Get the positions of the arm joints
   *
   * @return Positions in rad
   */
  const std::vector<double> & get_arm_positions();

  /**
   * @brief Get the position of the gripper
   *
   * @return Position in m
   */
  double get_gripper_position();

  /**
   * @brief Get the position of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Position in rad for arm joints and m for the gripper joint
   */
  double get_joint_position(uint8_t joint_index);

  /**
   * @brief Get the Cartesian positions
   *
   * @return Spatial position of the end effector frame measured in the base frame in m and rad
   *
   * @note The first 3 elements are the translation and the last 3 elements are the angle-axis
   * representation of the rotation
   */
  const std::array<double, 6> & get_cartesian_positions();

  /**
   * @brief Get the velocities
   *
   * @return Velocities in rad/s for arm joints and m/s for the gripper joint
   */
  [[deprecated(
    "get_velocities will be deprecated in the next version, "
    "please use get_all_velocities instead"
  )]]
  const std::vector<double> & get_velocities();

  /**
   * @brief Get the velocities of all joints
   *
   * @return Velocities in rad/s for arm joints and m/s for the gripper joint
   */
  const std::vector<double> & get_all_velocities();

  /**
   * @brief Get the velocities of the arm joints
   *
   * @return Velocities in rad/s
   */
  const std::vector<double> & get_arm_velocities();

  /**
   * @brief Get the velocity of the gripper
   *
   * @return Velocity in m/s
   */
  double get_gripper_velocity();

  /**
   * @brief Get the velocity of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Velocity in rad/s for arm joints and m/s for the gripper joint
   */
  double get_joint_velocity(uint8_t joint_index);

  /**
   * @brief Get the Cartesian velocities
   *
   * @return Spatial velocity of the end effector frame with respect to the base frame measured
   * in the base frame in m/s and rad/s
   *
   * @note The first 3 elements are the linear velocity and the last 3 elements are the angular
   * velocity
   */
  const std::array<double, 6> & get_cartesian_velocities();

  /**
   * @brief Get the accelerations
   *
   * @return Accelerations in rad/s^2 for arm joints and m/s^2 for the gripper joint
   */
  const std::vector<double> & get_all_accelerations();

  /**
   * @brief Get the accelerations of all joints
   *
   * @return Accelerations in rad/s^2 for arm joints and m/s^2 for the gripper joint
   */
  const std::vector<double> & get_arm_accelerations();

  /**
   * @brief Get the acceleration of the gripper
   *
   * @return Acceleration in m/s^2
   */
  double get_gripper_acceleration();

  /**
   * @brief Get the acceleration of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Acceleration in rad/s^2 for arm joints and m/s^2 for the gripper joint
   */
  double get_joint_acceleration(uint8_t joint_index);

  /**
   * @brief Get the Cartesian accelerations
   *
   * @return Spatial acceleration of the end effector frame with respect to the base frame
   * measured in the base frame in m/s^2 and rad/s^2
   *
   * @note The first 3 elements are the linear acceleration and the last 3 elements are the
   * angular acceleration
   */
  const std::array<double, 6> & get_cartesian_accelerations();

  /**
   * @brief Get the efforts
   *
   * @return Efforts in Nm for arm joints and N for the gripper joint
   */
  [[deprecated(
    "get_efforts will be deprecated in the next version, "
    "please use get_all_efforts instead"
  )]]
  const std::vector<double> & get_efforts();

  /**
   * @brief Get the efforts of all joints
   *
   * @return Efforts in Nm for arm joints and N for the gripper joint
   */
  const std::vector<double> & get_all_efforts();

  /**
   * @brief Get the efforts of the arm joints
   *
   * @return Efforts in Nm
   */
  const std::vector<double> & get_arm_efforts();

  /**
   * @brief Get the effort of the gripper
   *
   * @return Effort in N
   */
  double get_gripper_effort();

  /**
   * @brief Get the effort of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Effort in Nm for arm joints and N for the gripper joint
   */
  double get_joint_effort(uint8_t joint_index);

  /**
   * @brief Get the external efforts
   *
   * @return External efforts in Nm for arm joints and N for the gripper joint
   */
  [[deprecated(
    "get_external_efforts will be deprecated in the next version, "
    "please use get_all_external_efforts instead"
  )]]
  const std::vector<double> & get_external_efforts();

  /**
   * @brief Get the external efforts of all joints
   *
   * @return External efforts in Nm for arm joints and N for the gripper joint
   */
  const std::vector<double> & get_all_external_efforts();

  /**
   * @brief Get the external efforts of the arm joints
   *
   * @return External efforts in Nm
   */
  const std::vector<double> & get_arm_external_efforts();

  /**
   * @brief Get the external effort of the gripper
   *
   * @return External effort in N
   */
  double get_gripper_external_effort();

  /**
   * @brief Get the external effort of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return External effort in Nm for arm joints and N for the gripper joint
   */
  double get_joint_external_effort(uint8_t joint_index);

  /**
   * @brief Get the compensation efforts
   *
   * @return Spatial external efforts applied to the end effector frame measured in the base
   * frame in N and Nm
   *
   * @note The first 3 elements are the force and the last 3 elements are the torque
   */
  const std::array<double, 6> & get_cartesian_external_efforts();

  /**
   * @brief Get the compensation efforts
   *
   * @return Compensation efforts in Nm for arm joints and N for the gripper joint
   */
  [[deprecated(
    "get_compensation_efforts will be deprecated in the next version, "
    "please use get_all_compensation_efforts instead"
  )]]
  const std::vector<double> & get_compensation_efforts();

  /**
   * @brief Get the compensation efforts of all joints
   *
   * @return Compensation efforts in Nm for arm joints and N for the gripper joint
   */
  const std::vector<double> & get_all_compensation_efforts();

  /**
   * @brief Get the compensation efforts of the arm joints
   *
   * @return Compensation efforts in Nm
   */
  const std::vector<double> & get_arm_compensation_efforts();

  /**
   * @brief Get the compensation effort of the gripper
   *
   * @return Compensation effort in N
   */
  double get_gripper_compensation_effort();

  /**
   * @brief Get the compensation effort of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Compensation effort in Nm for arm joints and N for the gripper joint
   */
  double get_joint_compensation_effort(uint8_t joint_index);

  /**
   * @brief Get the rotor temperatures of all joints
   *
   * @return Rotor temperatures in C
   */
  const std::vector<double> & get_all_rotor_temperatures();

  /**
   * @brief Get the rotor temperatures of the arm joints
   *
   * @return Rotor temperatures in C
   */
  const std::vector<double> & get_arm_rotor_temperatures();

  /**
   * @brief Get the rotor temperature of the gripper
   *
   * @return Rotor temperature in C
   */
  double get_gripper_rotor_temperature();

  /**
   * @brief Get the rotor temperature of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Rotor temperature in C
   */
  double get_joint_rotor_temperature(uint8_t joint_index);

  /**
   * @brief Get the driver temperatures of all joints
   *
   * @return Driver temperatures in C
   */
  const std::vector<double> & get_all_driver_temperatures();

  /**
   * @brief Get the driver temperatures of the arm joints
   *
   * @return Driver temperatures in C
   */
  const std::vector<double> & get_arm_driver_temperatures();

  /**
   * @brief Get the driver temperature of the gripper
   *
   * @return Driver temperature in C
   */
  double get_gripper_driver_temperature();

  /**
   * @brief Get the driver temperature of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @return Driver temperature in C
   */
  double get_joint_driver_temperature(uint8_t joint_index);

  /**
   * @brief Save configurations to a YAML file
   * @param file_path The file path to store the configurations
   */
  void save_configs_to_file(const std::string & file_path);

  /**
   * @brief Get the factory reset flag
   *
   * @return true The configurations will be reset to factory defaults at the next startup
   * @return false The configurations will not be reset to factory defaults at the next startup
   */
  bool get_factory_reset_flag();

  /**
   * @brief Get the IP method
   *
   * @return The IP method of the robot
   */
  IPMethod get_ip_method();

  /**
   * @brief Get the manual IP
   *
   * @return Manual IP address
   */
  std::string get_manual_ip();

  /**
   * @brief Get the DNS
   *
   * @return DNS address
   */
  std::string get_dns();

  /**
   * @brief Get the gateway
   *
   * @return Gateway address
   */
  std::string get_gateway();

  /**
   * @brief Get the subnet
   *
   * @return Subnet address
   */
  std::string get_subnet();

  /**
   * @brief Get the joint characteristics
   *
   * @return Joint characteristics
   */
  std::vector<JointCharacteristic> get_joint_characteristics();

  /**
   * @brief Get the effort corrections
   *
   * @return Effort corrections in motor effort unit / Nm or N
   */
  std::vector<double> get_effort_corrections();

  /**
   * @brief Get the friction transition velocities
   *
   * @return Friction transition velocities in rad/s for arm joints and m/s for the gripper joint
   */
  std::vector<double> get_friction_transition_velocities();

  /**
   * @brief Get the friction constant terms
   *
   * @return Friction constant terms in Nm for arm joints and N for the gripper joint
   */
  std::vector<double> get_friction_constant_terms();

  /**
   * @brief Get the friction coulomb coefs
   *
   * @return Friction coulomb coefs in Nm/Nm for arm joints and N/N for the gripper joint
   */
  std::vector<double> get_friction_coulomb_coefs();

  /**
   * @brief Get the friction viscous coefs
   *
   * @return Friction viscous coefs in Nm/(rad/s) for arm joints and N/(m/s) for the gripper joint
   */
  std::vector<double> get_friction_viscous_coefs();

  /**
   * @brief Get the position offsets
   *
   * @return Position offsets in rad for arm joints and m for the gripper joint
   */
  std::vector<double> get_position_offsets();

  /**
   * @brief Get the error information of the robot
   *
   * @return Error information
   */
  std::string get_error_information();

  /**
   * @brief Get the modes
   *
   * @return Modes of all joints, a vector of Modes
   */
  std::vector<Mode> get_modes();

  /**
   * @brief Get the end effector properties
   *
   * @return The end effector properties
   */
  EndEffector get_end_effector();

  /**
   * @brief Get the joint limits
   *
   * @return Joint limits of all joints
   */
  std::vector<JointLimit> get_joint_limits();

  /**
   * @brief Get the motor parameters
   *
   * @return Motor parameters of all modes of all joints
   */
  std::vector<std::map<Mode, MotorParameter>> get_motor_parameters();

  /**
   * @brief Get the algorithm parameter
   *
   * @return Parameter used for robotic algorithms
   */
  AlgorithmParameter get_algorithm_parameter();

  /**
   * @brief Get the configured status of the robot
   *
   * @return true The robot is configured
   * @return false The robot is not configured
   */
  bool get_is_configured();

  /**
   * @brief Get the logger name
   * @param model Model of the robot
   * @param serv_ip IP address of the robot
   * @return Logger name
   */
  static std::string get_logger_name(Model model, const std::string & serv_ip);

  /**
   * @brief Get the default logger name
   *
   * @return Default logger name
   */
  static std::string get_default_logger_name();

private:
  // Raw counterpart of JointCharacteristic
  struct JointCharacteristicRaw
  {
    float effort_correction{0.0f};
    float friction_transition_velocity{0.0f};
    float friction_constant_term{0.0f};
    float friction_coulomb_coef{0.0f};
    float friction_viscous_coef{0.0f};

    /**
     * @brief Convert JointCharacteristic to JointCharacteristicRaw
     *
     * @param joint_characteristic The JointCharacteristic to convert
     */
    void to_raw(const JointCharacteristic & joint_characteristic);

    /**
     * @brief Convert JointCharacteristicRaw to JointCharacteristic
     *
     * @return The converted JointCharacteristic
     */
    JointCharacteristic from_raw() const;
  };

  // Raw counterpart of Link
  struct LinkRaw
  {
    float mass{0.0f};
    float inertia[9];
    float origin_xyz[3];
    float origin_rpy[3];

    /**
     * @brief Default constructor
     */
    LinkRaw() : inertia{}, origin_xyz{}, origin_rpy{} {}

    /**
     * @brief Convert Link to LinkRaw
     *
     * @param link The Link to convert
     */
    void to_raw(const Link & link);

    /**
     * @brief Convert LinkRaw to Link
     *
     * @return The converted Link
     */
    Link from_raw() const;
  };

  // Raw counterpart of EndEffector
  struct EndEffectorRaw
  {
    LinkRaw palm{};
    LinkRaw finger_left{};
    LinkRaw finger_right{};
    float offset_finger_left{0.0f};
    float offset_finger_right{0.0f};
    float pitch_circle_radius{0.0f};

    /**
     * @brief Convert EndEffector to EndEffectorRaw
     *
     * @param end_effector The EndEffector to convert
     */
    void to_raw(const EndEffector & end_effector);

    /**
     * @brief Convert EndEffectorRaw to EndEffector
     *
     * @return The converted EndEffector
     */
    EndEffector from_raw() const;
  };

  // Raw counterpart of JointLimit
  struct JointLimitRaw
  {
    /** @brief Minimum position in rad for arm joints and m for gripper */
    float position_min{0.0f};
    /** @brief Maximum position in rad for arm joints and m for gripper */
    float position_max{0.0f};
    /** @brief Tolerance on output position in rad for arm joints and m for gripper */
    float position_tolerance{0.0f};
    /** @brief Maximum velocity in rad/s for arm joints and m/s for gripper */
    float velocity_max{0.0f};
    /** @brief Tolerance on output velocity in rad/s for arm joints and m/s for gripper */
    float velocity_tolerance{0.0f};
    /** @brief Maximum effort in Nm for arm joints and N for gripper */
    float effort_max{0.0f};
    /** @brief Tolerance on output effort in Nm for arm joints and N for gripper */
    float effort_tolerance{0.0f};

    /**
     * @brief Convert JointLimit to JointLimitRaw
     *
     * @param joint_limit The JointLimit to convert
     */
    void to_raw(const JointLimit & joint_limit);

    /**
     * @brief Convert JointLimitRaw to JointLimit
     *
     * @return The converted JointLimit
     */
    JointLimit from_raw() const;
  };

  // Raw counterpart of PIDParameter
  struct PIDParameterRaw
  {
    /** @brief Proportional gain */
    float kp{0.0f};
    /** @brief Integral gain */
    float ki{0.0f};
    /** @brief Derivative gain */
    float kd{0.0f};
    /** @brief Maximum integral value */
    float imax{0.0f};

    /**
     * @brief Convert PIDParameter to PIDParameterRaw
     *
     * @param pid_parameter The PIDParameter to convert
     */
    void to_raw(const PIDParameter & pid_parameter);

    /**
     * @brief Convert PIDParameterRaw to PIDParameter
     *
     * @return The converted PIDParameter
     */
    PIDParameter from_raw() const;
  };

  // Raw counterpart of MotorParameter
  struct MotorParameterRaw
  {
    /** @brief Position loop PID parameter */
    PIDParameterRaw position;
    /** @brief Velocity loop PID parameter */
    PIDParameterRaw velocity;

    /**
     * @brief Convert MotorParameter to MotorParameterRaw
     *
     * @param motor_parameter The MotorParameter to convert
     */
    void to_raw(const MotorParameter & motor_parameter);

    /**
     * @brief Convert MotorParameterRaw to MotorParameter
     *
     * @return The converted MotorParameter
     */
    MotorParameter from_raw() const;
  };

  /**
   * @brief Joint input
   * @details The joint input is used to command a motion to a joint. Three types of motion are
   * supported and are corresponding to the three non-idle modes: position, velocity, and
   * external_effort. The position, velocity, and external_effort fields are mandatory for the
   * respective modes. Leaving the feedforward terms as zero is fine but filling them with the
   * values corresponding to the trajectory is recommended for smoother motion.
   */
  struct JointInputRaw
  {
    /// @brief The mode of the joint input
    /// @note If this mode is different from the configured mode, the robot will enter error state
    Mode mode{Mode::idle};
    union Command {
      /// @brief Joint input corresponding to the position mode
      struct Position {
        /// @brief Position in rad for arm joints or m for the gripper joint
        float position{0.0f};
        /// @brief Feedforward velocity in rad/s for arm joints or m/s for the gripper joint
        float feedforward_velocity{0.0f};
        /// @brief Feedforward acceleration in rad/s^2 for arm joints or m/s^2 for the gripper joint
        float feedforward_acceleration{0.0f};
      } position{};
      /// @brief Joint input corresponding to the velocity mode
      struct Velocity {
        /// @brief Velocity in rad/s for arm joints or m/s for the gripper joint
        float velocity{0.0f};
        /// @brief Feedforward acceleration in rad/s^2 for arm joints or m/s^2 for the gripper joint
        float feedforward_acceleration{0.0f};
      } velocity;
      /// @brief Joint input corresponding to the external_effort mode
      struct ExternalEffort {
        /// @brief external effort in Nm for arm joints or N for the gripper joint
        float external_effort{0.0f};
      } external_effort;
      /// @brief Joint input corresponding to the effort mode
      struct Effort {
        /// @brief Effort in Nm for arm joints or N for the gripper joint
        float effort{0.0f};
      } effort;
    } command{};
  };

  /// @brief Joint output
  struct JointOutputRaw
  {
    /// @brief Joint position in rad for arm joints or m for the gripper joint
    float position{0.0f};
    /// @brief Joint velocity in rad/s for arm joints or m/s for the gripper joint
    float velocity{0.0f};
    /// @brief Joint effort in Nm for arm joints or N for the gripper joint
    float effort{0.0f};
    /// @brief External effort in Nm for arm joints or N for the gripper joint
    float external_effort{0.0f};
    /// @brief Motor/rotor temperature in °C
    float rotor_temperature{0.0f};
    /// @brief Driver/MOSFET temperature in °C
    float driver_temperature{0.0f};
  };

  /// @brief Robot input
  struct RobotInput
  {
    /// @brief Inputs in joint space
    struct Joint
    {
      /// @brief Inputs of all joints
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
      } all{};

      /// @brief Inputs of the arm joints
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
      } arm{};

      /// @brief Inputs of the gripper joint
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
      } gripper{};
    } joint{};

    /// @brief Inputs in Cartesian space
    struct Cartesian
    {
      /// @brief Position twist in axis coordinates in m and rad
      std::array<double, 6> positions{};
      /// @brief Velocity twist in axis coordinates in m/s and rad/s
      std::array<double, 6> velocities{};
      /// @brief Acceleration twist in axis coordinates in m/s^2 and rad/s^2
      std::array<double, 6> accelerations{};
      /// @brief Effort wrench in ray coordinates in Nm and N
      std::array<double, 6> external_efforts{};
    } cartesian{};
  };

  /** @brief Robot command indicators */
  struct RobotCommandIndicator
  {
    /** @brief Commands transmitted over UDP */
    enum class UDP : uint8_t
    {
      /** @brief Set robot input command */
      set_robot_input,
      /** @brief Get robot output command */
      get_robot_output
    };

    /** @brief Commands transmitted over TCP */
    enum class TCP : uint8_t
    {
      /** @brief Handshake command */
      handshake,
      /** @brief Set home command */
      set_home,
      /** @brief Set configuration command */
      set_configuration,
      /** @brief Get configuration command */
      get_configuration,
      /** @brief Get log command */
      get_log,
      /** @brief Update default EEPROM command */
      update_default_eeprom,
      /** @brief Reboot command */
      reboot
    };
  };

  // ErrorState
  enum class ErrorState : uint8_t {
    // No error
    none,
    // Controller's Ethernet manager failed to initialize
    ethernet_init_failed,
    // Controller's CAN interface failed to initialize
    can_init_failed,
    // Controller's CAN interface failed to send a message
    joint_command_failed,
    // Controller's CAN interface failed to receive a message
    joint_feedback_failed,
    // Joint clear error command failed
    joint_clear_error_failed,
    // Joint enable command failed
    joint_enable_failed,
    // Joint disable command failed
    joint_disable_failed,
    // Joint home calibration command failed
    joint_set_home_failed,
    // Joint disabled unexpectedly
    joint_disabled_unexpectedly,
    // Joint overheated
    joint_overheated,
    // Invalid mode command received
    invalid_mode,
    // Invalid robot command indicator received
    invalid_robot_command,
    // Invalid configuration address
    invalid_configuration_address,
    // Robot input with modes different than configured modes received
    robot_input_mode_mismatch,
    // Joint limit exceeded
    joint_limit_exceeded,
    // Robot input infinite
    robot_input_infinite
  };

  // Configuration addresses
  enum class ConfigurationAddress : uint8_t {
    // Controller configurations
    factory_reset_flag,
    ip_method,
    manual_ip,
    dns,
    gateway,
    subnet,
    joint_characteristics,
    error_state,
    modes,
    end_effector,
    joint_limits,
    motor_parameters,
    // Local configurations
    algorithm_parameter,
  };

  // Maximum retransmission attempts
  static constexpr uint8_t MAX_RETRANSMISSION_ATTEMPTS{100};

  // Number of modes
  static constexpr uint8_t NUM_MODES{5};

  // Model to number of joints mapping
  static const std::map<Model, uint8_t> MODEL_NUM_JOINTS;

  // Error information
  static const std::map<ErrorState, std::string> ERROR_INFORMATION;

  // Model name
  static const std::map<Model, std::string> MODEL_NAME;

  // Mode name
  static const std::map<Mode, std::string> MODE_NAME;

  // Configuration name
  static const std::map<ConfigurationAddress, std::string> CONFIGURATION_NAME;

  // Interpolators for joint trajectories
  std::vector<std::unique_ptr<QuinticHermiteInterpolator>> trajectory_ptrs_{};

  // Trajectory start time
  std::vector<std::chrono::time_point<std::chrono::steady_clock>> trajectory_start_times_{};

  // Interpolation space
  InterpolationSpace interpolation_space_{InterpolationSpace::joint};

  // Joint inputs
  std::vector<JointInputRaw> joint_input_raws_{};

  // Joint outputs
  std::vector<JointOutputRaw> joint_output_raws_{};

  // Number of joints
  uint8_t num_joints_{0};

  // Robot model
  Model model_{Model::wxai_v0};

  // Driver version
  std::string driver_version_{};

  // Controller firmware version
  std::string controller_version_{};

  // Whether the driver is properly configured for the robot to be controlled
  // true if configured, false if not configured
  bool configured_{false};

  // Ethernet manager
  std::unique_ptr<EthernetManager> ethernet_manager_ptr_{nullptr};

  // Atomic flag for maintaining and stopping the daemon thread
  std::atomic<bool> activated_{false};

  // Joint characteristic position offsets
  // TODO: remove at next minor
  std::vector<double> joint_characteristic_position_offsets_{};

  // Multithreading design
  //
  // Goal
  //
  // - only one thread can run at a time
  // - another thread cannot cut in until the full communication cycle is completed
  //   for example, set_joint_inputs --nothing-in-between--> receive_joint_outputs
  // - the other thread has priority to run after the current thread finishes
  //
  // Mutex ownership
  //
  // call mutex_preempt_ 1 and mutex_data_ 2 for simplicity
  // daemon: |-|-1-|-12-|-2-|--------|-1-|-12-|-2-|-|
  // main:   |------------|-1-|-12-|-2-|------------|
  //
  // Exception handling
  //
  // - if an exception is thrown in the main thread
  //   - the daemon thread gets std::terminate
  //   - the main thread unwind the stack: ~TrossenArmDriver() -> cleanup()
  // - if an exception is thrown in the daemon thread
  //   - the exception is stored in exception_ptr_
  //   - the daemon thread returns
  //   - the main thread gets the exception and rethrows it at the next operation
  //   - the main thread unwind the stack: ~TrossenArmDriver() -> cleanup()
  //
  // Notes
  //
  // - the mutex claiming cannot be nested or there will be deadlocks, i.e., |-1-|-12-|-2-|-12-|-2-|
  //   is not allowed
  // - when an exception is thrown by the main thread, the program is expected to terminate either
  //   immediately or right after cleaning up the resources not related to the driver

  // Daemon thread
  std::thread daemon_thread_{};

  // Mutex for data access
  std::mutex mutex_data_{};

  // Mutex for preempting the next slot to run
  std::mutex mutex_preempt_{};

  // Shared exception pointer
  std::exception_ptr exception_ptr_{nullptr};

  // Algorithm interface
  std::unique_ptr<AlgorithmInterface> algorithm_interface_ptr_{nullptr};

  // Robot input
  RobotInput robot_input_{};

  // Robot output
  RobotOutput robot_output_{};

  // Arm mode
  Mode arm_mode_{Mode::idle};

  // Logger
  std::shared_ptr<Logger> logger_ptr_{nullptr};

  /**
   * @brief Update the robot output
   *
   * @details This function does the following:
   *
   * 1. Extract the joint outputs from the joint outputs raw data
   * 2. Do forward kinematics
   * 3. Update the cartesian outputs
   */
  void update_robot_output();

  /**
   * @brief Update the robot input
   *
   * @details This function does the following:
   *
   * 1. If the interpolation space is joint
   *   1. Evaluate the joint inputs
   *   2. Do forward kinematics if all arm joints have the same mode
   *   3. Update the cartesian inputs
   * 2. If the interpolation space is cartesian
   *   1. Evaluate the cartesian inputs
   *   2. Do inverse kinematics
   *   3. Update the arm joint inputs
   *   4. Evaluate the gripper joint inputs
   * 3. Update the raw joint inputs
   */
  void update_robot_input();

  /**
   * @brief Set the joint inputs
   *
   * @note The joint inputs' modes should be consistent with the configured modes
   */
  void set_joint_inputs();

  /**
   * @brief Receive the joint outputs
   *
   * @return true Successfully received the joint outputs
   * @return false Failed to receive the joint outputs within the timeout
   */
  bool receive_joint_outputs();

  /**
   * @brief Check the error state
   *
   * @param buffer The buffer containing the error state
   * @param clear_error Whether to clear the error state without throwing an exception
   */
  void check_error_state(
    const std::vector<uint8_t> & buffer,
    bool clear_error
  );

  /**
   * @brief Reset the error state of the robot
   */
  void reset_error_state();

  /**
   * @brief Get the more detailed log message from the arm controller
   *
   * @return The last log message
   */
  std::string get_detailed_log();

  /// @brief Configuration variant
  using ConfigurationVariant = std::variant<
    std::monostate,
    bool,
    IPMethod,
    std::string,
    std::vector<JointCharacteristic>,
    std::vector<Mode>,
    EndEffector,
    std::vector<JointLimit>,
    std::vector<std::map<Mode, MotorParameter>>,
    AlgorithmParameter
  >;

  /**
   * @brief Set a configuration
   *
   * @param configuration_address The address of the configuration to set
   * @param configuration_variant The value of the configuration to set
   */
  void set_configuration(
    ConfigurationAddress configuration_address,
    const ConfigurationVariant & configuration_variant
  );

  /**
   * @brief Get a configuration
   *
   * @param configuration_address The address of the configuration to get
   * @return The value of the configuration
   */
  ConfigurationVariant get_configuration(ConfigurationAddress configuration_address);

  /**
   * @brief Function to be executed by the daemon thread
   *
   * @details The daemon thread will repeatedly do the following:
   *
   * 1. Break if the driver is not configured
   *
   * 2. Set the joint inputs
   *
   * 3. Update the robot output
   *
   * 3. Update the robot input
   *
   * 4. Receive the joint outputs
   *
   * 5. Block and wait for a main thread operation if there is any
   */
  void daemon();
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_HPP_
