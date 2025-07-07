===================
Trossen Arm Bringup
===================

The Trossen Arm Bringup package provides the necessary configuration and launch files to bring up the ros2_control hardware interface for the Trossen Arms.
This package allows you to control the arm and gripper using ROS 2.

Two different hardware interfaces are supported:

-   **Mock Hardware**: For testing and development purposes, simulating the arm's behavior.
-   **Real Hardware**: For controlling the actual Trossen Arm hardware.

These interfaces can be swapped by changing the ``ros2_control_hardware_type`` launch argument when launching the bringup package.

Usage
=====

.. tabs::

    .. group-tab:: Humble

        We will first cover how to launch the mock Trossen Arm hardware interface with controllers for the arm and gripper.

        .. code-block:: bash

            source ~/ros2_ws/install/setup.bash
            ros2 launch trossen_arm_bringup trossen_arm.launch.py ros2_control_hardware_type:=mock_components

        Several nodes are launched and can be listed with the following command:

        .. code-block:: bash

            ros2 node list
            /arm_controller
            /controller_manager
            /gripper_controller
            /joint_state_broadcaster
            /robot_state_publisher
            /rviz2
            /transform_listener_impl_6280570b14d0

        The relevant nodes are:

        -   The ``controller_manager`` node is `a ros2_control Controller Manager <https://control.ros.org/humble/doc/ros2_control/controller_manager/doc/userdoc.html>`_ responsible for managing the controllers for all ros2_control hardware interfaces.
        -   The ``arm_controller`` is a `joint_trajectory_controller/JointTrajectoryController <https://control.ros.org/humble/doc/ros2_controllers/joint_trajectory_controller/doc/userdoc.html>`_ that allows you to send joint trajectory commands to the arm.
        -   The ``gripper_controller`` is a `position_controllers/GripperActionController <https://control.ros.org/humble/doc/ros2_controllers/gripper_controllers/doc/userdoc.html>`_ that allows you to open and close the gripper.
        -   The ``joint_state_broadcaster`` is a `joint_state_broadcaster/JointStateBroadcaster <https://control.ros.org/humble/doc/ros2_controllers/joint_state_broadcaster/doc/userdoc.html>`_ node that publishes the joint states of the arm and gripper.

    .. group-tab:: Jazzy

        We will first cover how to launch the mock Trossen Arm hardware interface with controllers for the arm and gripper.

        .. code-block:: bash

            source ~/ros2_ws/install/setup.bash
            ros2 launch trossen_arm_bringup trossen_arm.launch.py ros2_control_hardware_type:=mock_components

        Several nodes are launched and can be listed with the following command:

        .. code-block:: bash

            $ ros2 node list
            /arm_controller
            /controller_manager
            /gripper_controller
            /joint_state_broadcaster
            /robot_state_publisher
            /rviz2
            /transform_listener_impl_6280570b14d0

        The relevant nodes are:

        -   The ``controller_manager`` node is `a ros2_control Controller Manager <https://control.ros.org/jazzy/doc/ros2_control/controller_manager/doc/userdoc.html>`_ responsible for managing the controllers for all ros2_control hardware interfaces.
        -   The ``arm_controller`` is a `joint_trajectory_controller/JointTrajectoryController <https://control.ros.org/jazzy/doc/ros2_controllers/joint_trajectory_controller/doc/userdoc.html>`_ that allows you to send joint trajectory commands to the arm.
        -   The ``gripper_controller`` is a `parallel_gripper_action_controller/GripperActionController <https://control.ros.org/jazzy/doc/ros2_controllers/parallel_gripper_controller/doc/userdoc.html>`_ that allows you to open and close the gripper.
        -   The ``joint_state_broadcaster`` is a `joint_state_broadcaster/JointStateBroadcaster <https://control.ros.org/jazzy/doc/ros2_controllers/joint_state_broadcaster/doc/userdoc.html>`_ node that publishes the joint states of the arm and gripper.

Next Steps
==========

Now that you are familiar with the controllers required to control the Trossen Arm, you can proceed to the next tutorial to learn how to control the arm using the :doc:`Trossen Arm MoveIt configuration package<./moveit>`.
