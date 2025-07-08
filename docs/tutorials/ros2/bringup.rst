===================
Trossen Arm Bringup
===================

The Trossen Arm Bringup package provides the necessary configuration and launch files to bring up the ros2_control hardware interface for the Trossen Arms.
This package allows you to control the arm and gripper using ROS 2.

Two different hardware interfaces are supported:

-   **Mock Hardware**: For testing and development purposes, simulating the arm's behavior.
-   **Real Hardware**: For controlling the actual Trossen Arm hardware.

These interfaces can be swapped by changing the ``ros2_control_hardware_type`` launch argument when launching the bringup package by switching between ``mock_components`` and ``real``.

Usage
=====

Launch File
-----------

We will first cover how to launch the mock Trossen Arm hardware interface with controllers for the arm and gripper.

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_bringup trossen_arm.launch.py ros2_control_hardware_type:=mock_components

Several nodes are launched and can be listed with the following command:

.. code-block:: bash

    ros2 node list

You will see output similar to the following:

.. code-block:: console

    /arm_controller
    /controller_manager
    /gripper_controller
    /joint_state_broadcaster
    /robot_state_publisher
    /rviz2
    /transform_listener_impl_6280570b14d0

.. tabs::

    .. group-tab:: Humble

        The relevant nodes are:

        -   The ``controller_manager`` node is `a ros2_control Controller Manager <https://control.ros.org/humble/doc/ros2_control/controller_manager/doc/userdoc.html>`_ responsible for managing the controllers for all ros2_control hardware interfaces.
        -   The ``arm_controller`` is a `joint_trajectory_controller/JointTrajectoryController <https://control.ros.org/humble/doc/ros2_controllers/joint_trajectory_controller/doc/userdoc.html>`_ that allows you to send joint trajectory commands to the arm.
        -   The ``gripper_controller`` is a `position_controllers/GripperActionController <https://control.ros.org/humble/doc/ros2_controllers/gripper_controllers/doc/userdoc.html>`_ that allows you to open and close the gripper.
        -   The ``joint_state_broadcaster`` is a `joint_state_broadcaster/JointStateBroadcaster <https://control.ros.org/humble/doc/ros2_controllers/joint_state_broadcaster/doc/userdoc.html>`_ node that publishes the joint states of the arm and gripper.

    .. group-tab:: Jazzy

        The relevant nodes are:

        -   The ``controller_manager`` node is `a ros2_control Controller Manager <https://control.ros.org/jazzy/doc/ros2_control/controller_manager/doc/userdoc.html>`_ responsible for managing the controllers for all ros2_control hardware interfaces.
        -   The ``arm_controller`` is a `joint_trajectory_controller/JointTrajectoryController <https://control.ros.org/jazzy/doc/ros2_controllers/joint_trajectory_controller/doc/userdoc.html>`_ that allows you to send joint trajectory commands to the arm.
        -   The ``gripper_controller`` is a `parallel_gripper_action_controller/GripperActionController <https://control.ros.org/jazzy/doc/ros2_controllers/parallel_gripper_controller/doc/userdoc.html>`_ that allows you to open and close the gripper.
        -   The ``joint_state_broadcaster`` is a `joint_state_broadcaster/JointStateBroadcaster <https://control.ros.org/jazzy/doc/ros2_controllers/joint_state_broadcaster/doc/userdoc.html>`_ node that publishes the joint states of the arm and gripper.

To further customize the trossen_arm launch file at runtime, refer to the table below, or run the command below

.. code-block:: console

    ros2 launch trossen_arm_bringup trossen_arm.launch.py --show-args

.. csv-table::
    :file: /_data/trossen_arm_bringup.csv
    :header-rows: 1
    :widths: 20, 60, 20, 20

Controller Demo Script
----------------------

A simple script is used to demonstrate how to control and monitor the ``arm_controller`` and ``gripper_controller`` using their action interfaces.
This script sends a series of commands to the arm and gripper, allowing you to test their functionality while printing off the action feedback and result.
It also demonstrates handling of goal response, feedback, and results.

To use this demo script, first make sure the arm and gripper controllers are running using this package's ``trossen_arm.launch.py`` launch file.
Then run the following commands in a new terminal:

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 run trossen_arm_bringup controllers.py

You should see the arm move upright, the gripper open, the gripper close, then the arm move back to sleep.
In the launch terminal, you will see that the arm and gripper controllers receive and respond to a series of goal requests.
In the demo terminal, you will see feedback from both action servers, and information about goal acceptance and results.

.. tabs::

    .. group-tab:: Humble

        See `the script source <https://github.com/TrossenRobotics/trossen_arm_ros/blob/humble/trossen_arm_bringup/demos/controllers.py>`_ for more details.

    .. group-tab:: Jazzy

        See `the script source <https://github.com/TrossenRobotics/trossen_arm_ros/blob/jazzy/trossen_arm_bringup/demos/controllers.py>`_ for more details.

Next Steps
==========

Now that you are familiar with the controllers required to control the Trossen Arm, you can proceed to the next tutorial to learn how to control the arm using the :doc:`Trossen Arm MoveIt configuration package<./moveit>`.
