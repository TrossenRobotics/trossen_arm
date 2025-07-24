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
//    * Neither the name of the copyright holder nor the names of its
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

// Purpose:
// This script demonstrates how to recover from an error in the driver.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Configures the logging
// 2. Initializes the driver
// 3. Configures the driver
// 4. Sets the arm to position mode
// 5. Moves the arm to the home position
// 6. Triggers an error by setting a joint position to an invalid value
// 7. Recovers from the error
// 8. Moves the arm to the sleep position

#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "libtrossen_arm/trossen_arm.hpp"

int main() {
  const trossen_arm::Model MODEL = trossen_arm::Model::wxai_v0;
  const std::string SERV_IP = "192.168.1.2";

  std::cout << "Configuring logging..." << std::endl;
  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
    "error_recovery_and_logging.log",
    true
  );
  std::vector<spdlog::sink_ptr> sinks {stdout_sink, file_sink};
  auto default_logger = std::make_shared<spdlog::logger>(
    trossen_arm::TrossenArmDriver::get_default_logger_name(), sinks.begin(), sinks.end()
  );
  spdlog::register_logger(default_logger);
  auto logger = std::make_shared<spdlog::logger>(
    trossen_arm::TrossenArmDriver::get_logger_name(
      MODEL,
      SERV_IP
    ), sinks.begin(), sinks.end()
  );
  spdlog::register_logger(logger);

  std::cout << "Initializing the driver..." << std::endl;
  trossen_arm::TrossenArmDriver driver;

  std::cout << "Configuring the driver..." << std::endl;
  driver.configure(
    MODEL,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    SERV_IP,
    false
  );

  driver.set_all_modes(trossen_arm::Mode::position);

  std::vector<double> sleep_positions = driver.get_all_positions();
  std::vector<double> home_positions(driver.get_num_joints(), 0.0f);
  home_positions[1] = M_PI_2;
  home_positions[2] = M_PI_2;

  try {
    std::cout << "Moving the arm to the home position..." << std::endl;
    driver.set_all_positions(home_positions);

    std::cout << "Triggering a discontinuity error..." << std::endl;
    // Command a huge step change that the arm cannot physically follow
    // which triggers an error for safety reasons
    home_positions[5] += M_PI;
    driver.set_all_positions(home_positions, 0.0f);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Moving the arm to the sleep position..." << std::endl;
    driver.set_all_positions(sleep_positions);
  } catch (const std::exception &e) {
    std::cout << "An error occurred: " << e.what() << std::endl;
    std::cout << "Recovering from the error..." << std::endl;
    driver.cleanup();
    driver.configure(
      trossen_arm::Model::wxai_v0,
      trossen_arm::StandardEndEffector::wxai_v0_leader,
      "192.168.1.2",
      true
    );
    driver.set_all_modes(trossen_arm::Mode::position);
    driver.set_all_positions(sleep_positions);
  }

  return 0;
}
