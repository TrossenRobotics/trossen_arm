=============
Teleoperation
=============

By running the following code, you can start your first **SAFE** teleoperation:

.. note::

  Make sure to replace the IP addresses and camera serial numbers/index with your own.

.. tabs:: 
  
  .. group-tab:: Trossen AI Solo

      .. code-block:: bash

        uv run lerobot-teleoperate \
          --robot.type=widowxai_follower_robot \
          --robot.ip_address=192.168.1.4 \
          --robot.id=follower \
          --robot.cameras='{
            cam_low: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
            cam_high: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
          }' \
          --teleop.type=widowxai_leader_teleop \
          --teleop.ip_address=192.168.1.2 \
          --teleop.id=leader \
          --display_data=true

  .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

        uv run lerobot-teleoperate \
          --robot.type=bi_widowxai_follower_robot \
          --robot.left_arm_ip_address=192.168.1.5 \
          --robot.right_arm_ip_address=192.168.1.4 \
          --robot.id=bimanual_follower \
          --robot.cameras= '{
              cam_low: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
              cam_high: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
              cam_left_wrist: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
              cam_right_wrist: {"type": "opencv", "index_or_path": "0", "width": 640, "height": 480, "fps": 30},
            }' \
          --teleop.type=bi_widowxai_leader_teleop \
          --teleop.left_arm_ip_address=192.168.1.3 \
          --teleop.right_arm_ip_address=192.168.1.2 \
          --teleop.id=bimanual_leader \
          --display_data=false
        

Teleoperation Configuration
===========================

.. note::
  for additional configuration options, use the command below to see all available commad line arguments:

  .. code-block:: bash

      uv run lerobot-teleoperate --help

When using the robot in teleoperation mode you can specify command line arguments to customize the behavior:

- ``--fps``: The number of frames per second to send to the robot.
- ``--teleop_time_s``: The duration of the teleoperation in seconds.
- ``--robot.max_relative_target [float]``: limits the magnitude of the relative positional target vector for safety purposes (default: 5.0). Once you’re confident in controlling the robot safely, you can remove this restriction by setting it to null. 
- ``--robot.loop_rate``: Control loop rate in Hz (default: 30)