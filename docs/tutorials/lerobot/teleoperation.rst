=============
Teleoperation
=============

By running the following code, you can start your first **SAFE** teleoperation:

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --robot.max_relative_target=5 \
     --control.type=teleoperate

By adding ``--robot.max_relative_target=5``, we override the default value for ``max_relative_target`` defined in
`TrossenAIBimanualRobot <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/robot_devices/robots/configs.py>`_.

It is expected to be ``5`` to limit the magnitude of the movement for **safety**, but the teleoperation **won't be smooth**.
When you feel confident, you can disable this limit by adding ``--robot.max_relative_target=null`` to the command line:

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --robot.max_relative_target=null \
     --control.type=teleoperate


Teleoperation Configuration
===========================

When using the robot in teleoperation mode you can specify command line arguments to customize the behavior:

- ``--control.fps``: The number of frames per second to send to the robot.
- ``--control.teleop_time_s``: The duration of the teleoperation in seconds.
- ``--control.display_cameras``: Flag to display the camera feeds.