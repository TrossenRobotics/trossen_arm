=================
ROS 2 Controllers
=================

A few controllers specific to Trossen Arm are provided in the ``trossen_arm_controllers`` package.
These controllers provide extended functionality on top of standard ROS 2 controllers to expose features specific to Trossen Arm hardware.

GravityCompensationController
=============================

The ``GravityCompensationController`` claims the external efforts interface of the arm's joints and commands 0.0Nm to each joint to enable gravity compensation.
This allows the arm to be easily moved and held in place by an operator by hand.

This controller can be demonstrated using the ``gravity_compensation.launch.py`` launch file in the ``trossen_arm_bringup`` package.

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_bringup gravity_compensation.launch.py

Once launched, gravity compensation mode will be activated and the arm should be easily movable by hand and will hold its position when released.

CartesianPoseController
=======================

The ``CartesianPoseController`` allows commanding the end-effector of the arm to a desired pose in Cartesian space using inverse kinematics.
This controller uses the Trossen Arm driver's built-in IK solver to compute joint positions for a desired end-effector pose.

This controller can be demonstrated using the ``cartesian.launch.py`` launch file in the ``trossen_arm_bringup`` package.

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_bringup cartesian.launch.py

Once launched, the controller can be commanded by publishing a `geometry_msgs/PoseStamped <https://docs.ros2.org/latest/api/geometry_msgs/msg/PoseStamped.html>`_ message to the ``/cartesian_pose_controller/target_pose`` topic.
A demonstration script is also provided in the ``trossen_arm_bringup`` package that sends a series of target poses to the controller.
With the ``CartesianPoseController`` activated, the script can be run with the following command:

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 run trossen_arm_bringup cartesian_demo.py

The script will send a series of target poses to the controller, moving the arm's end-effector through a sequence of positions in Cartesian space.

.. Note::

    The ``CartesianPoseController`` expects commands to be streamed to the controller at a high rate, else the arm may experience jittery motion.
    For reference provided script commands poses at a rate of 200Hz.
