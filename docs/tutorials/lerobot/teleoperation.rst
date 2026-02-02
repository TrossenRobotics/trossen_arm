=============
Teleoperation
=============

.. note::

    Ongoing support for the LeRobot fork is deprecated.
    We recommend using the :doc:`plugin-based LeRobot integration <../lerobot_plugin>` instead.

    However, this integration is necessary for users who wish to use the :doc:`openpi integration <../openpi>`.

By running the following code, you can start your first **SAFE** teleoperation:

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_stationary \
                --robot.max_relative_target=5 \
                --control.type=teleoperate

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_mobile \
                --robot.max_relative_target=5 \
                --control.type=teleoperate

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_solo \
                --robot.max_relative_target=5 \
                --control.type=teleoperate

By adding ``--robot.max_relative_target=5``, we override the default value for **max_relative_target** defined in
`Trossen AI Robot Configurations <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/robot_devices/robots/configs.py>`_.

This limit is in place to prioritize **safety** by restricting the magnitude of movement.
However, it may result in **less smooth** teleoperation.
Once you're confident in controlling the robot safely, you can remove this restriction by setting ``--robot.max_relative_target=null`` in the command line.

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash
            :emphasize-lines: 3

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_stationary \
                --robot.max_relative_target=null \
                --control.type=teleoperate

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash
            :emphasize-lines: 3

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_mobile \
                --robot.max_relative_target=null \
                --control.type=teleoperate

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash
            :emphasize-lines: 3

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_solo \
                --robot.max_relative_target=null \
                --control.type=teleoperate

If you have configured multiple camera interfaces as exaplained in :ref:`tutorials/lerobot/configuration:Camera Serial Number`.
You can specify the camera interface to use for teleoperation by adding the following command line argument:

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash
            :emphasize-lines: 5

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_stationary \
                --robot.max_relative_target=null \
                --control.type=teleoperate \
                --robot.camera_interface='opencv'

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash
            :emphasize-lines: 5

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_mobile \
                --robot.max_relative_target=null \
                --control.type=teleoperate \
                --robot.camera_interface='opencv'

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash
            :emphasize-lines: 5

            python lerobot/scripts/control_robot.py \
                --robot.type=trossen_ai_solo \
                --robot.max_relative_target=null \
                --control.type=teleoperate \
                --robot.camera_interface='opencv'

Teleoperation Configuration
===========================

When using the robot in teleoperation mode you can specify command line arguments to customize the behavior:

- ``--control.fps``: The number of frames per second to send to the robot.
- ``--control.teleop_time_s``: The duration of the teleoperation in seconds.
- ``--control.display_cameras``: Flag to display the camera feeds.
