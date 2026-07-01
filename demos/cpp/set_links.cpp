// Copyright 2026 Trossen Robotics
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
// This script demonstrates how to get and set the link inertial properties of the arm.
// The links vector contains, in order, the base link, every arm link, the palm,
// and both finger links (size = number of joints + 2).

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Gets the current links and prints them
// 4. Slightly perturbs the base link mass and sets the new links
// 5. Prints the updated links

#include <iostream>
#include <vector>

#include "libtrossen_arm/trossen_arm.hpp"

void print_links(const std::vector<trossen_arm::Link> & links)
{
  std::cout << "Link properties (" << links.size() << " links):" << std::endl;
  for (size_t i = 0; i < links.size(); ++i) {
    const trossen_arm::Link & link = links.at(i);
    std::cout << "  Link " << i << ":" << std::endl;
    std::cout << "    mass: " << link.mass << " kg" << std::endl;
    std::cout << "    inertia:";
    for (double v : link.inertia) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
    std::cout << "    origin_xyz:";
    for (double v : link.origin_xyz) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
    std::cout << "    origin_rpy:";
    for (double v : link.origin_rpy) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
  }
}

int main()
{
  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Get and print the current links
  std::vector<trossen_arm::Link> links = driver.get_links();
  print_links(links);

  // Set new links

  // Note: changing link inertial properties affects gravity compensation and dynamics.
  // Inaccurate values may degrade tracking performance or cause instability.
  // The standard properties below are nominal CAD values for the WXAI V0 and only
  // approximate any given physical arm. Calibrate per-arm for best performance.

  // Option 1: use the dated standard links
  driver.set_links(trossen_arm::StandardLinks::wxai_v0_20260626);

  // // Option 2: use custom links
  // links.front().mass += 0.01;
  // driver.set_links(links);

  // Get and print the new links
  print_links(driver.get_links());

  return 0;
}
