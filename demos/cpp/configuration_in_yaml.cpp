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

// Purpose:
// This script demonstrates how to exchange persistent configurations via a YAML file.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver with the leader configuration
// 3. Stores the configuration in a YAML file
// 4. Loads the configuration from the YAML file
// 5. Power cycle to apply the new configurations

#include <iostream>
#include <string>

#include "libtrossen_arm/trossen_arm.hpp"

void print_configurations(trossen_arm::TrossenArmDriver& driver) {
  std::cout << "EEPROM factory reset flag: " << driver.get_factory_reset_flag() << std::endl;
  std::cout << "EEPROM IP method: " << static_cast<int>(driver.get_ip_method()) << std::endl;
  std::cout << "EEPROM manual IP: " << driver.get_manual_ip() << std::endl;
  std::cout << "EEPROM DNS: " << driver.get_dns() << std::endl;
  std::cout << "EEPROM gateway: " << driver.get_gateway() << std::endl;
  std::cout << "EEPROM subnet: " << driver.get_subnet() << std::endl;
  std::cout << "EEPROM effort corrections: ";
  for (float effort_correction : driver.get_effort_corrections()) {
    std::cout << effort_correction << " ";
  }
  std::cout << std::endl;
  std::cout << "EEPROM friction transition velocities: ";
  for (float friction_transition_velocity : driver.get_friction_transition_velocities()) {
    std::cout << friction_transition_velocity << " ";
  }
  std::cout << std::endl;
  std::cout << "EEPROM friction constant terms: ";
  for (float friction_constant_term : driver.get_friction_constant_terms()) {
    std::cout << friction_constant_term << " ";
  }
  std::cout << std::endl;
  std::cout << "EEPROM friction coulomb coefs: ";
  for (float friction_coulomb_coef : driver.get_friction_coulomb_coefs()) {
    std::cout << friction_coulomb_coef << " ";
  }
  std::cout << std::endl;
  std::cout << "EEPROM friction viscous coefs: ";
  for (float friction_viscous_coef : driver.get_friction_viscous_coefs()) {
    std::cout << friction_viscous_coef << " ";
  }
  std::cout << std::endl;
  std::cout << "EEPROM continuity factors: ";
  for (float continuity_factor : driver.get_continuity_factors()) {
    std::cout << continuity_factor << " ";
  }
  std::cout << std::endl;
}

int main() {
    // Initialize the driver
    trossen_arm::TrossenArmDriver driver;

    // Configure the driver
    driver.configure(
        trossen_arm::Model::wxai_v0,
        trossen_arm::StandardEndEffector::wxai_v0_leader,
        "192.168.1.2",
        false
    );

    // Print the configurations
    std::cout << "Initial configurations:" << std::endl;
    print_configurations(driver);

    // Store the configurations in a YAML file
    driver.save_configs_to_file("configurations.yaml");

    // Load the configurations from the YAML file
    driver.load_configs_from_file("configurations.yaml");

    // Print the configurations
    std::cout << "Configurations after loading from the YAML file:" << std::endl;
    print_configurations(driver);

    // Power cycle to apply the new IP method
    std::cout << "Power cycle the robot to apply the new IP method." << std::endl;

    return 0;
}
