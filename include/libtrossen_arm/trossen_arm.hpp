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

#include <atomic>
#include <cstdint>
#include <cstring>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "libtrossen_arm/trossen_arm_config.hpp"
#include "libtrossen_arm/trossen_arm_interpolate.hpp"
#include "libtrossen_arm/trossen_arm_logging.hpp"
#include "libtrossen_arm/trossen_arm_udp_client.hpp"

namespace trossen_arm
{

/// @brief Operation modes of a joint
enum class Mode : uint8_t {
  /// @brief Idle mode: arm joints are braked, the gripper joint closing with a safe force
  idle,
  /// @brief Position mode: control the joint to a desired position
  position,
  /// @brief Velocity mode: control the joint to a desired velocity
  velocity,
  /// @brief Effort mode: control the joint to a desired effort
  effort,
};

/// @brief IP methods
enum class IPMethod : uint8_t {
  /// @brief Manual: use the manual IP address specified in the configuration
  manual,
  /// @brief DHCP: use the DHCP to obtain the IP address, if failed, use the default IP address
  dhcp,
};

/// @brief Robot models
enum class Model : uint8_t {
  /// @brief WXAI V0
  wxai_v0,
};

/// @brief Link properties
struct LinkProperties
{
  /// @brief mass in kg
  float mass;
  /// @brief inertia in kg m^2
  std::array<float, 9> inertia;
  /// @brief inertia frame translation measured in link frame in m
  std::array<float, 3> origin_xyz;
  /// @brief inertia frame RPY angles measured in link frame in rad
  std::array<float, 3> origin_rpy;
};

/// @brief End effector properties
struct EndEffectorProperties
{
  /// @brief Properties of the palm link
  LinkProperties palm;

  /// @brief Properties of the left finger link
  LinkProperties finger_left;

  /// @brief Properties of the right finger link
  LinkProperties finger_right;

  /// @brief Offset from the palm center to the left carriage center in m in home configuration
  float offset_finger_left;

  /// @brief Offset from the palm center to the right carriage center in m in home configuration
  float offset_finger_right;
};

/// @brief End effector properties for the standard variants
struct StandardEndEffector {
  /// @brief WXAI V0 base variant
  static constexpr EndEffectorProperties wxai_v0_base{
    .palm = {
      .mass = 0.51498747f,
      .inertia = {
        0.00056037f, 5.1e-7f, -9e-8f,
        5.1e-7f, 0.00045167f, -4e-8f,
        -9e-8f, -4e-8f, 0.00081937f
      },
      .origin_xyz = {0.04399695f, -0.00000761f, 0.00001510f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_left = {
      .mass = 0.07618477f,
      .inertia = {
        0.00002759f, -0.00000495f, -0.00000052f,
        -0.00000495f, 0.00003194f, 0.00000327f,
        -0.00000052f, 0.00000327f, 0.00003791f
      },
      .origin_xyz = {0.00780825f, -0.00528697f, -0.00368479f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_right = {
      .mass = 0.07618477f,
      .inertia = {
        0.00002759f, 0.00000495f, -0.00000275f,
        0.00000495f, 0.00003194f, 0.00000203f,
        -0.00000275f, 0.00000203f, 0.00003791f
      },
      .origin_xyz = {0.00780824f, 0.00528691f, 0.00221090f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .offset_finger_left = 0.0227f,
    .offset_finger_right = -0.0227f
  };

  /// @brief WXAI V0 leader variant
  static constexpr EndEffectorProperties wxai_v0_leader{
    .palm = {
      .mass = 0.58782068f,
      .inertia = {
        0.00106679f, 4.9e-7f, 6.518e-5f,
        4.9e-7f, 0.00096664f, -1.5e-7f,
        6.518e-5f, -1.5e-7f, 0.00084059f
      },
      .origin_xyz = {0.04260435f, -0.00000564f, -0.00961530f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_left = {
      .mass = 0.07230515f,
      .inertia = {
        0.00004429f, 2.1e-6f, 8.6e-7f,
        2.1e-6f, 0.00003181f, -3.39e-6f,
        8.6e-7f, -3.39e-6f, 0.00001895f
      },
      .origin_xyz = {0.00203592f, -0.00158698f, -0.01124986f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_right = {
      .mass = 0.07230515f,
      .inertia = {
        0.00005161f, -2.1e-6f, -2.28e-6f,
        -2.1e-6f, 0.00003913f, 7.97e-6f,
        -2.28e-6f, 7.97e-6f, 0.00001895f
      },
      .origin_xyz = {0.00203592f, 0.00158689f, -0.00503780f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .offset_finger_left = 0.01485f,
    .offset_finger_right = -0.01485f
  };

  /// @brief WXAI V0 follower variant
  static constexpr EndEffectorProperties wxai_v0_follower{
    .palm = {
      .mass = 0.63463466f,
      .inertia = {
        0.00086891f, -0.00000024f, 0.00004879f,
        -0.00000024f, 0.00075399f, 0.00001658f,
        0.00004879f, 0.00001658f, 0.00088386f
      },
      .origin_xyz = {0.04550951f, 0.00050636f, 0.00947036f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_left = {
      .mass = 0.07618477f,
      .inertia = {
        0.00002759f, -0.00000495f, -0.00000052f,
        -0.00000495f, 0.00003194f, 0.00000327f,
        -0.00000052f, 0.00000327f, 0.00003791f
      },
      .origin_xyz = {0.00780825f, -0.00528697f, -0.00368479f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .finger_right = {
      .mass = 0.07618477f,
      .inertia = {
        0.00002759f, 0.00000495f, -0.00000275f,
        0.00000495f, 0.00003194f, 0.00000203f,
        -0.00000275f, 0.00000203f, 0.00003791f
      },
      .origin_xyz = {0.00780824f, 0.00528691f, 0.00221090f},
      .origin_rpy = {0.0f, 0.0f, 0.0f}
    },
    .offset_finger_left = 0.0227f,
    .offset_finger_right = -0.0227f
  };
};

/// @brief Trossen Arm Driver
class TrossenArmDriver
{
public:
  /// @brief Destroy the Trossen Arm Driver object
  ~TrossenArmDriver();

  /**
   * @brief Configure the driver
   *
   * @param model Model of the robot
   * @param end_effector End effector properties
   * @param serv_ip IP address of the robot
   * @param clear_error Whether to clear the error state of the robot
   */
  void configure(
    Model model,
    EndEffectorProperties end_effector,
    const std::string serv_ip,
    bool clear_error
  );

  /**
   * @brief Cleanup the driver
   */
  void cleanup();

  /**
   * @brief Set the positions of all joints
   *
   * @param goal_positions Positions in rad for arm joints and m for the gripper joint
   * @param goal_time Optional: goal time in s when the goal positions should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal positions are reached, default true
   * @param goal_feedforward_velocities Optional: feedforward velocities in rad/s for arm joints
   *   and m/s for the gripper joint, default zeros
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2 for arm
   *   joints and m/s^2 for the gripper joint, default zeros
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_positions(
    const std::vector<float> & goal_positions,
    float goal_time = 2.0f,
    bool blocking = true,
    const std::optional<std::vector<float>> & goal_feedforward_velocities = std::nullopt,
    const std::optional<std::vector<float>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the positions of the arm joints
   *
   * @param goal_positions Positions in rad
   * @param goal_time Optional: goal time in s when the goal positions should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal positions are reached, default true
   * @param goal_feedforward_velocities Optional: feedforward velocities in rad/s, default zeros
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2, default
   *   zeros
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_positions(
    const std::vector<float> & goal_positions,
    float goal_time = 2.0f,
    bool blocking = true,
    const std::optional<std::vector<float>> & goal_feedforward_velocities = std::nullopt,
    const std::optional<std::vector<float>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the position of the gripper
   *
   * @param goal_position Position in m
   * @param goal_time Optional: goal time in s when the goal position should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal position is reached, default true
   * @param goal_feedforward_velocity Optional: feedforward velocity in m/s, default zero
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in m/s^2, default zero
   */
  void set_gripper_position(
    float goal_position,
    float goal_time = 2.0f,
    bool blocking = true,
    float goal_feedforward_velocity = 0.0f,
    float goal_feedforward_acceleration = 0.0f);

  /**
   * @brief Set the position of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_position Position in rad for arm joints and m for the gripper joint
   * @param goal_time Optional: goal time in s when the goal position should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal position is reached, default true
   * @param goal_feedforward_velocity Optional: feedforward velocity in rad/s for arm joints and
   *   m/s for the gripper joint, default zero
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in rad/s^2 for arm
   *   joints and m/s^2 for the gripper joint, default zero
   */
  void set_joint_position(
    uint8_t joint_index,
    float goal_position,
    float goal_time = 2.0f,
    bool blocking = true,
    float goal_feedforward_velocity = 0.0f,
    float goal_feedforward_acceleration = 0.0f
  );

  /**
   * @brief Set the velocities of all joints
   *
   * @param goal_velocities Velocities in rad/s for arm joints and m/s for the gripper joint
   * @param goal_time Optional: goal time in s when the goal velocities should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal velocities are reached, default true
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2 for arm
   *   joints and m/s^2 for the gripper joint, default zeros
   *
   * @note The size of the vectors should be equal to the number of joints
   */
  void set_all_velocities(
    const std::vector<float> & goal_velocities,
    float goal_time = 2.0f,
    bool blocking = true,
    const std::optional<std::vector<float>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocities of the arm joints
   *
   * @param goal_velocities Velocities in rad
   * @param blocking Optional: whether to block until the goal velocities are reached, default true
   * @param goal_time Optional: goal time in s when the goal velocities should be reached, default
   *   2.0s
   * @param goal_feedforward_accelerations Optional: feedforward accelerations in rad/s^2, default
   *   zeros
   *
   * @note The size of the vectors should be equal to the number of arm joints
   */
  void set_arm_velocities(
    const std::vector<float> & goal_velocities,
    float goal_time = 2.0f,
    bool blocking = true,
    const std::optional<std::vector<float>> & goal_feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocity of the gripper
   *
   * @param goal_velocity Velocity in m/s
   * @param goal_time Optional: goal time in s when the goal velocity should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal velocity is reached, default true
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in m/s^2, default zero
   */
  void set_gripper_velocity(
    float goal_velocity,
    float goal_time = 2.0f,
    bool blocking = true,
    float goal_feedforward_acceleration = 0.0f
  );

  /**
   * @brief Set the velocity of a joint
   *
   * @param joint_index The index of the joint in [0, num_joints - 1]
   * @param goal_velocity Velocity in rad/s for arm joints and m/s for the gripper joint
   * @param goal_time Optional: goal time in s when the goal velocity should be reached, default
   *   2.0s
   * @param blocking Optional: whether to block until the goal velocity is reached, default true
   * @param goal_feedforward_acceleration Optional: feedforward acceleration in rad/s^2 for arm
   *   joints and m/s^2 for the gripper joint, default zero
   */
  void set_joint_velocity(
    uint8_t joint_index,
    float goal_velocity,
    float goal_time = 2.0f,
    bool blocking = true,
    float goal_feedforward_acceleration = 0.0f
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
    const std::vector<float> & goal_efforts,
    float goal_time = 2.0f,
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
    const std::vector<float> & goal_efforts,
    float goal_time = 2.0f,
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
    float goal_effort,
    float goal_time = 2.0f,
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
    float goal_effort,
    float goal_time = 2.0f,
    bool blocking = true
  );

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
   * @brief Set the effort correction
   *
   * @param effort_correction The effort correction to set
   *
   * @note This configuration is used to map the efforts in Nm or N to the motor
   *   effort unit, i.e., effort_correction = motor effort unit / Nm or N
   */
  void set_effort_correction(const std::vector<float> & effort_correction);

  /**
   * @brief Set the modes of each joint
   *
   * @param modes Desired modes for each joint, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::effort
   */
  void set_joint_modes(const std::vector<Mode> & modes);

  /**
   * @brief Set all joints to the same mode
   *
   * @param mode Desired mode for all joints, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::effort
   */
  void set_all_modes(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the arm joints
   *
   * @param mode Desired mode for the arm joints, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::effort
   *
   * @warning This method does not change the gripper joint's mode
   */
  void set_arm_modes(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the gripper joint
   *
   * @param mode Desired mode for the gripper joint, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::effort
   *
   * @warning This method does not change the arm joints' mode
   */
  void set_gripper_mode(Mode mode = Mode::idle);

  /**
   * @brief Set the end effector properties
   *
   * @param end_effector The end effector properties
   */
  void set_end_effector(const EndEffectorProperties & end_effector);

  /**
   * @brief Get the number of joints
   *
   * @return Number of joints
   */
  uint8_t get_num_joints() const;

  /**
   * @brief Get the positions
   *
   * @return Positions in rad for arm joints and m for the gripper joint
   */
  std::vector<float> get_positions();

  /**
   * @brief Get the velocities
   *
   * @return Velocities in rad/s for arm joints and m/s for the gripper joint
   */
  std::vector<float> get_velocities();

  /**
   * @brief Get the efforts
   *
   * @return Efforts in Nm for arm joints and N for the gripper joint
   */
  std::vector<float> get_efforts();

  /**
   * @brief Get the external efforts
   *
   * @return External efforts in Nm for arm joints and N for the gripper joint
   */
  std::vector<float> get_external_efforts();

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
   * @brief Get the effort correction
   *
   * @return Effort correction
   *
   * @note This configuration is used to map the efforts in Nm or N to the motor
   *  effort unit, i.e., effort_correction = motor effort unit / Nm or N
   */
  std::vector<float> get_effort_correction();

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
   * @brief Get the end effector mass properties
   *
   * @return The end effector mass property structure
   */
  EndEffectorProperties get_end_effector();

private:
  // Raw counterparts of LinkProperties and EndEffectorProperties
  struct LinkRaw
  {
    float mass;
    float inertia[9];
    float origin_xyz[3];
    float origin_rpy[3];
  };
  struct EndEffectorRaw
  {
    LinkRaw palm;
    LinkRaw finger_left;
    LinkRaw finger_right;
    float offset_finger_left;
    float offset_finger_right;
  };

  /// @brief Joint input
  /// @details The joint input is used to command a motion to a joint. Three types of motion are
  ///   supported and are corresponding to the three non-idle modes: position, velocity, and effort.
  ///   The position, velocity, and effort fields are mandatory for the respective modes. Leaving
  ///   the feedforward terms as zero is fine but filling them with the values corresponding to the
  ///   trajectory is recommended for smoother motion.
  struct JointInput
  {
    /// @brief The mode of the joint input
    /// @note If this mode is different from the configured mode, the robot will enter error state
    Mode mode{Mode::idle};
    union {
      /// @brief Joint input corresponding to the position mode
      struct {
        /// @brief Position in rad for arm joints or m for the gripper joint
        float position;
        /// @brief Feedforward velocity in rad/s for arm joints or m/s for the gripper joint
        float feedforward_velocity;
        /// @brief Feedforward acceleration in rad/s^2 for arm joints or m/s^2 for the gripper joint
        float feedforward_acceleration;
      } position{0.0f, 0.0f, 0.0f};
      /// @brief Joint input corresponding to the velocity mode
      struct {
        /// @brief Velocity in rad/s for arm joints or m/s for the gripper joint
        float velocity;
        /// @brief Feedforward acceleration in rad/s^2 for arm joints or m/s^2 for the gripper joint
        float feedforward_acceleration;
      } velocity;
      /// @brief Joint input corresponding to the effort mode
      struct {
        /// @brief Effort in Nm for arm joints or N for the gripper joint
        float effort;
      } effort;
    };
  };

  /// @brief Joint output
  struct JointOutput
  {
    /// @brief Joint position in rad for arm joints or m for the gripper joint
    float position;
    /// @brief Joint velocity in rad/s for arm joints or m/s for the gripper joint
    float velocity;
    /// @brief Joint effort in Nm for arm joints or N for the gripper joint
    float effort;
    /// @brief External effort in Nm for arm joints or N for the gripper joint
    float external_effort;
  };

  // Robot command indicators
  enum class RobotCommandIndicator : uint8_t
  {
    handshake,
    set_joint_inputs,
    get_joint_outputs,
    set_home,
    set_configuration,
    get_configuration,
    get_log,
  };

  // ErrorState
  enum class ErrorState : uint8_t {
    // No error
    none,
    // Controller's UDP interface failed to initialize
    udp_init_failed,
    // Controller's CAN interface failed to initialize
    can_init_failed,
    // Controller's CAN interface failed to send a message
    joint_command_failed,
    // Controller's CAN interface failed to receive a message
    joint_feedback_failed,
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
    // Robot command with unexpected size received
    invalid_robot_command_size,
    // Invalid configuration address
    invalid_configuration_address,
    // Invalid pending command
    invalid_pending_command,
    // Robot input with modes different than configured modes received
    robot_input_mode_mismatch,
    // Discontinuous robot input received
    robot_input_discontinous,
  };

  // Configuration addresses
  enum class ConfigurationAddress : uint8_t {
    factory_reset_flag,
    ip_method,
    manual_ip,
    dns,
    gateway,
    subnet,
    effort_correction,
    error_state,
    modes,
    end_effector
  };

  // UDP port
  static constexpr uint16_t PORT{50000};

  // Timeout in microseconds for receiving UDP packets
  static constexpr uint32_t TIMEOUT_US{1000};

  // Maximum retransmission attempts
  static constexpr uint8_t MAX_RETRANSMISSION_ATTEMPTS{100};

  // Model to number of joints mapping
  static const std::map<Model, uint8_t> MODEL_NUM_JOINTS;

  // Error information
  static const std::map<ErrorState, std::string> ERROR_INFORMATION;

  // Model name
  static const std::map<Model, std::string> MODEL_NAME;

  // Mode name
  static const std::map<Mode, std::string> MODE_NAME;

  // Interpolators for joint trajectories
  std::vector<QuinticHermiteInterpolator> trajectories_;

  // Trajectory start time
  std::vector<std::chrono::time_point<std::chrono::steady_clock>> trajectory_start_times_;

  // Robot input
  std::vector<JointInput> joint_inputs_;

  // Joint outputs
  std::vector<JointOutput> joint_outputs_;

  // Number of joints
  uint8_t num_joints_{0};

  // Whether the driver is properly configured for the robot to be controlled
  // true if configured, false if not configured
  bool configured_{false};

  // UDP client
  UDP_Client udp_client_;

  // Atomic flag for maintaining and stopping the daemon thread
  std::atomic<bool> activated_{false};

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
  std::thread daemon_thread_;

  // Mutex for data access
  std::mutex mutex_data_;

  // Mutex for preempting the next slot to run
  std::mutex mutex_preempt_;

  // Shared exception pointer
  std::exception_ptr exception_ptr_;

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
   * @brief Common steps for setting configurations
   *
   * @param configuration_address Configuration address
   */
  void get_configuration(ConfigurationAddress configuration_address);

  /**
   * @brief Check the error state
   *
   * @param clear_error Whether to clear the error state without throwing an exception
   */
  void check_error_state(bool clear_error);

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

  /**
   * @brief Function to be executed by the daemon thread
   *
   * @details The daemon thread will repeatedly do the following:
   *   1. Break if the driver is not configured
   *   2. Set the joint inputs
   *   3. Receive the joint outputs
   *   4. Block and wait for a main thread operation if there is any
   */
  void daemon();
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_HPP_
