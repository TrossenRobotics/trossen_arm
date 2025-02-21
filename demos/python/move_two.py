# Copyright 2025 Trossen Robotics
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#    * Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

'''
Purpose:
This script demonstrates how to move two robots to different positions using interpolation.

Hardware setup:
1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
2. A WXAI V0 arm with follower end effector and ip at 192.168.1.3

The script does the following:
1. Initializes the drivers
2. Configures the drivers with the leader and follower configurations
3. Sets the robots to position mode
4. Records the sleep positions
5. Generates trajectories: sleep -> sleep -> home -> home -> sleep -> sleep
6. Moves the robots along the trajectories
7. Sets the robots to the idle mode
8. The driver automatically sets the mode to idle at the destructor
'''

import time

import numpy as np
from scipy.interpolate import PchipInterpolator
import trossen_arm

if __name__=='__main__':
    leader_server_ip = '192.168.1.2'
    follower_server_ip = '192.168.1.3'

    print("Initializing the drivers...")
    leader_driver = trossen_arm.TrossenArmDriver()
    follower_driver = trossen_arm.TrossenArmDriver()

    print("Configuring the drivers...")
    leader_driver.configure(
        trossen_arm.Model.wxai_v0,
        trossen_arm.StandardEndEffector.wxai_v0_leader,
        leader_server_ip,
        False
    )
    follower_driver.configure(
        trossen_arm.Model.wxai_v0,
        trossen_arm.StandardEndEffector.wxai_v0_follower,
        follower_server_ip,
        False
    )

    print("Moving to home positions...")
    leader_driver.set_all_modes(trossen_arm.Mode.position)
    follower_driver.set_all_modes(trossen_arm.Mode.position)

    leader_sleep_positions = np.array(leader_driver.get_positions())
    follower_sleep_positions = np.array(follower_driver.get_positions())
    home_positions = np.zeros(leader_driver.get_num_joints())
    home_positions[1] = np.pi/2
    home_positions[2] = np.pi/2

    leader_waypoints = np.array(
        [
            leader_sleep_positions,
            leader_sleep_positions,
            home_positions,
            home_positions,
            leader_sleep_positions,
            leader_sleep_positions,
        ]
    )
    follower_waypoints = np.array(
        [
            follower_sleep_positions,
            follower_sleep_positions,
            home_positions,
            home_positions,
            follower_sleep_positions,
            follower_sleep_positions,
        ]
    )
    timepoints = np.array([0, 1, 3, 4, 6, 7])

    pchip_positions_leader = PchipInterpolator(timepoints, leader_waypoints, axis=0)
    pchip_feedforward_velocities_leader = pchip_positions_leader.derivative()
    pchip_feedforward_accelerations_leader = pchip_feedforward_velocities_leader.derivative()
    pchip_positions_follower = PchipInterpolator(timepoints, follower_waypoints, axis=0)
    pchip_feedforward_velocities_follower = pchip_positions_follower.derivative()
    pchip_feedforward_accelerations_follower = pchip_feedforward_velocities_follower.derivative()

    start_time = time.time()
    end_time = start_time + timepoints[-1]

    while time.time() < end_time:
        loop_start_time = time.time()
        current_time = loop_start_time - start_time

        positions_leader = pchip_positions_leader(current_time)
        feedforward_velocities_leader = pchip_feedforward_velocities_leader(current_time)
        feedforward_accelerations_leader = pchip_feedforward_accelerations_leader(current_time)
        positions_follower = pchip_positions_follower(current_time)
        feedforward_velocities_follower = pchip_feedforward_velocities_follower(current_time)
        feedforward_accelerations_follower = pchip_feedforward_accelerations_follower(current_time)

        leader_driver.set_all_positions(
            positions_leader,
            0.0,
            False,
            feedforward_velocities_leader,
            feedforward_accelerations_leader
        )
        follower_driver.set_all_positions(
            positions_follower,
            0.0,
            False,
            feedforward_velocities_follower,
            feedforward_accelerations_follower
        )
