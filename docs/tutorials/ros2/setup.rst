===========
ROS 2 Setup
===========

ROS 2 Installation
==================

See the `ROS 2 installation guide <https://docs.ros.org/en/humble/Installation.html>`_ for instructions on how to install ROS 2 Humble on your system.
We recommend using the deb package installation method for Ubuntu 22.04.

Trossen Arm Workspace Setup
===========================

#.  Create a ROS 2 workspace and clone the trossen_arm_ros repository into it:

  .. code-block:: bash

      mkdir -p ~/ros2_ws/src
      cd ~/ros2_ws/src
      git clone https://github.com/TrossenRobotics/trossen_arm_ros.git

#.  Install the dependencies using `rosdep` and `vcs`:

    .. code-block:: bash

        cd ~/ros2_ws
        vcs import src < src/trossen_arm_ros/dependencies.repos
        rosdep install --from-paths src --ignore-src -r -y

#.  Build the workspace:

    .. code-block:: bash

        cd ~/ros2_ws
        colcon build --symlink-install

Usage
=====

Launching the Trossen Arm Hardware Interface
--------------------------------------------

To launch the Trossen Arm hardware interface with controllers for the arm and gripper, use the following command:

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_bringup trossen_arm.launch.py

Launching MoveIt with Mock Hardware
-----------------------------------

To launch simulated hardware with MoveIt, use the following command:

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_moveit moveit.launch.py ros2_control_hardware_type:=mock_components

Launching MoveIt with Real Hardware
-----------------------------------

To launch real hardware with MoveIt, use the following command:

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_moveit moveit.launch.py ros2_control_hardware_type:=real
