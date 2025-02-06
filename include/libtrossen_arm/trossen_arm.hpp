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
#include <cstring>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "libtrossen_arm/trossen_arm_config.hpp"
#include "libtrossen_arm/trossen_arm_interpolate.hpp"
#include "libtrossen_arm/trossen_arm_logging.hpp"
#include "libtrossen_arm/trossen_arm_udp_client.hpp"

namespace trossen_arm
{

// Modes
enum class Mode : uint8_t {
  idle,
  position,
  velocity,
  torque,
};

// IP methods
enum class IPMethod : uint8_t {
  manual,
  dhcp,
};

// Models
enum class Model : uint8_t {
  wxai_v0,
};

// Joint input
struct JointInput
{
  // Command of what mode does the union represent
  // One of Mode::idle, Mode::position, Mode::velocity, Mode::torque
  Mode mode;
  union {
    // Command for position mode
    struct
    {
      // Desired position in radians
      float desired_position;
      // Feedforward velocity in radians per second
      float feedforward_velocity;
      // Feedforward acceleration in radians per second squared
      float feedforward_acceleration;
    } position;
    // Command for velocity mode
    struct
    {
      // Desired velocity in radians per second
      float desired_velocity;
      // Feedforward acceleration in radians per second squared
      float feedforward_acceleration;
    } velocity;
    // Command for torque mode
    struct
    {
      // Desired torque in Newton meters
      float desired_torque;
    } torque;
  };
};

// Joint output
struct JointOutput
{
  // joint position in radians
  float position;
  // joint velocity in radians per second
  float velocity;
  // joint torque in Newton meters
  float torque;
  // external torque in Newton meters
  float external_torque;
};

// End effector properties
struct LinkProperties
{
  // mass in kg
  float mass;
  // inertia in kg m^2
  std::array<float, 9> inertia;
  // inertia frame translation measured in link frame in meters
  std::array<float, 3> origin_xyz;
  // inertia frame RPY angles measured in link frame in radians
  std::array<float, 3> origin_rpy;
};
struct EndEffectorProperties
{
  LinkProperties palm;
  LinkProperties finger_left;
  LinkProperties finger_right;
  float offset_finger_left;
  float offset_finger_right;
};

// Standard end effector variants
struct StandardEndEffector {
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
    .offset_finger_left = 0.02165f,
    .offset_finger_right = -0.02165f
  };
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
    .offset_finger_left = 0.02165f,
    .offset_finger_right = -0.02165f
  };
};

class TrossenArmDriver
{
public:
  /**
   * @brief Destroy the Trossen Arm Driver object
   */
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
   * @brief Move the arm joints to the desired positions
   *
   * @param goal_time Time to reach the goal positions in seconds
   * @param goal_positions Desired positions in radians
   * @param goal_velocities Optional: desired velocities in radians per second
   * @param goal_accelerations Optional: desired accelerations in radians per second squared
   *
   * @details It does the following:
   *   1. Check the size of the vectors
   *   2. Check the goal time
   *   3. Construct a vector of quintic Hermite interpolators
   *   4. Construct vectors for the robot input
   *   5. Get the robot output
   *   6. Compute the coefficients for the interpolators
   *   7. Set arm mode to position
   *   8. Move the arm joints along the trajectory
   *
   * @note The gripper joint will use the previous joint input
   */
  void move_arm_to(
    float goal_time,
    const std::vector<float> & goal_positions,
    const std::optional<std::vector<float>> & goal_velocities = std::nullopt,
    const std::optional<std::vector<float>> & goal_accelerations = std::nullopt);

  /**
   * @brief Set the positions of the robot
   *
   * @param positions Desired positions in radians
   * @param feedforward_velocities Optional: feedforward velocities in radians per second
   * @param feedforward_accelerations Optional: feedforward accelerations in radians per second
   *   squared
   */
  void set_positions(
    const std::vector<float> & positions,
    const std::optional<std::vector<float>> & feedforward_velocities = std::nullopt,
    const std::optional<std::vector<float>> & feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the positions of the arm joints
   *
   * @param positions Desired positions in radians
   * @param feedforward_velocities Optional: feedforward velocities in radians per second
   * @param feedforward_accelerations Optional: feedforward accelerations in radians per second
   *   squared
   */
  void set_arm_positions(
    const std::vector<float> & positions,
    const std::optional<std::vector<float>> & feedforward_velocities = std::nullopt,
    const std::optional<std::vector<float>> & feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the position of the gripper
   *
   * @param position Desired position in meters
   * @param feedforward_velocity Optional: feedforward velocity in meters per second
   * @param feedforward_acceleration
   *   Optional: feedforward acceleration in meters per second squared
   */
  void set_gripper_position(
    float position,
    const std::optional<float> & feedforward_velocity = std::nullopt,
    const std::optional<float> & feedforward_acceleration = std::nullopt);

  /**
   * @brief Set the velocities of the robot
   *
   * @param velocities Desired velocities in radians per second
   * @param feedforward_accelerations
   *   Optional: feedforward accelerations in radians per second squared
   */
  void set_velocities(
    const std::vector<float> & velocities,
    const std::optional<std::vector<float>> & feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocities of the arm joints
   *
   * @param velocities Desired velocities in radians per second
   * @param feedforward_accelerations
   *   Optional: feedforward accelerations in radians per second squared
   */
  void set_arm_velocities(
    const std::vector<float> & velocities,
    const std::optional<std::vector<float>> & feedforward_accelerations = std::nullopt);

  /**
   * @brief Set the velocity of the gripper
   *
   * @param velocity Desired velocity in meters per second
   * @param feedforward_acceleration
   *   Optional: feedforward acceleration in meters per second squared
   */
  void set_gripper_velocity(
    float velocity,
    const std::optional<float> & feedforward_acceleration = std::nullopt);

  /**
   * @brief Set the torques of the robot
   *
   * @param torques Desired torques in Newton meters
   */
  void set_torques(const std::vector<float> & torques);

  /**
   * @brief Set the torques of the arm joints
   *
   * @param torques Desired torques in Newton meters
   */
  void set_arm_torques(const std::vector<float> & torques);

  /**
   * @brief Set the torque of the gripper
   *
   * @param torque Desired torque in Newton meters
   */
  void set_gripper_torque(float torque);

  /**
   * @brief Set the robot input
   *
   * @param robot_input Desired robot input
   */
  void set_robot_input(const std::vector<JointInput> & robot_input);

  /**
   * @brief Request the robot output
   */
  void request_robot_output();

  /**
   * @brief Receive the robot output
   *
   * @return true Successfully received the robot output
   * @return false Failed to receive the robot output
   */
  bool receive_robot_output();

  /**
   * @brief Set the factory reset flag
   *
   * @param flag Reset the configurations to factory defaults at the next startup
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
   * @brief Set the torque correction
   *
   * @param torque_correction The torque correction to set
   *
   * @note This configuration is used to map the torques in Nm to the motor
   *   torque unit, i.e., torque_correction = (motor torque unit) / (Nm)
   */
  void set_torque_correction(const std::vector<float> & torque_correction);

  /**
   * @brief Reset the error state of the robot
   */
  void reset_error_state();

  /**
   * @brief Set the modes of the robot
   *
   * @param modes Desired modes for each joint, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::torque
   */
  void set_modes(const std::vector<Mode> & modes);

  /**
   * @brief Set all joints to the same mode
   *
   * @param mode Desired mode for all joints, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::torque
   */
  void set_mode(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the arm joints
   *
   * @param mode Desired mode for the arm joints, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::torque
   */
  void set_arm_mode(Mode mode = Mode::idle);

  /**
   * @brief Set the mode of the gripper joints
   *
   * @param mode Desired mode for the gripper joints, one of
   *   Mode::idle
   *   Mode::position
   *   Mode::velocity
   *   Mode::torque
   */
  void set_gripper_mode(Mode mode = Mode::idle);

  /**
   * @brief Set the end effector mass properties
   *
   * @param end_effector The end effector mass property structure
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
   * @return Positions in radians
   */
  std::vector<float> get_positions() const;

  /**
   * @brief Get the velocities
   *
   * @return Velocities in radians per second
   */
  std::vector<float> get_velocities() const;

  /**
   * @brief Get the torques
   *
   * @return Torques in Newton meters
   */
  std::vector<float> get_torques() const;

  /**
   * @brief Get the external torques
   *
   * @return External torques in Newton meters
   */
  std::vector<float> get_external_torques() const;

  /**
   * @brief Get the robot output
   *
   * @return Robot output, a vector of JointOutput
   */
  std::vector<JointOutput> get_robot_output() const;

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
   * @brief Get the torque correction
   *
   * @return Torque correction
   *
   * @note This configuration is used to map the torques in Nm to the motor
   *   torque unit, i.e., torque_correction = (motor torque unit) / (Nm)
   */
  std::vector<float> get_torque_correction();

  /**
   * @brief Get the error information of the robot
   *
   * @return Error information
   */
  std::string get_error_information();

  /**
   * @brief Get the modes of the robot
   *
   * @return Modes of the robot, a vector of Mode
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


  // Robot command indicators
  enum class RobotCommandIndicator : uint8_t
  {
    handshake,
    set_robot_input,
    get_robot_output,
    set_home,
    set_configuration,
    get_configuration
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
    torque_correction,
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

  // UDP client
  UDP_Client udp_client_;

  // Number of joints
  uint8_t num_joints_{0};

  // Whether the driver is properly configured for the robot to be controlled
  // true if configured, false if not configured
  bool configured_{false};

  // Robot input
  std::vector<JointInput> robot_input_;

  // Robot output
  std::vector<JointOutput> robot_output_;

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
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_HPP_
