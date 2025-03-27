========================================
Configuring Trossen AI Kits with LeRobot
========================================

In order to use the Trossen AI Kits with LeRobot, you need to first configure the arms with the necessary specifications.
The specifications include the IP address, model_name, and camera serial numbers.
The following steps will guide you on how to configure the Trossen AI Kits with LeRobot.

This is an example of a configuration file for the Trossen AI Kits with LeRobot, you can find this in :guilabel:`lerobot/common/robot_devices/robots/configs.py`:

.. tabs::
    .. group-tab:: Trossen AI Stationary    

        .. code-block:: python

            @RobotConfig.register_subclass("trossen_ai_stationary")
            @dataclass
            class TrossenAIStationaryRobotConfig(ManipulatorRobotConfig):

            # /!\ FOR SAFETY, READ THIS /!\
            # `max_relative_target` limits the magnitude of the relative positional target vector for safety purposes.
            # Set this to a positive scalar to have the same value for all motors, or a list that is the same length as
            # the number of motors in your follower arms.
            # For Trossen AI Arms, for every goal position request, motor rotations are capped at 5 degrees by default.
            # When you feel more confident with teleoperation or running the policy, you can extend
            # this safety limit and even removing it by setting it to `null`.
            # Also, everything is expected to work safely out-of-the-box, but we highly advise to
            # first try to teleoperate the grippers only (by commenting out the rest of the motors in this yaml),
            # then to gradually add more motors (by uncommenting), until you can teleoperate both arms fully
            max_relative_target: int | None = 5

            leader_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "left": TrossenArmDriverConfig(
                        # wxai
                        ip="192.168.1.3",
                        model="V0_LEADER",
                    ),
                    "right": TrossenArmDriverConfig(
                        # wxai
                        ip="192.168.1.2",
                        model="V0_LEADER",
                    ),
                }
            )

            follower_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "left": TrossenArmDriverConfig(
                        ip="192.168.1.5",
                        model="V0_FOLLOWER",
                    ),
                    "right": TrossenArmDriverConfig(
                        ip="192.168.1.4",
                        model = "V0_FOLLOWER",
                    ),
                }
            )

            # Troubleshooting: If one of your IntelRealSense cameras freeze during
            # data recording due to bandwidth limit, you might need to plug the camera
            # on another USB hub or PCIe card.
            cameras: dict[str, CameraConfig] = field(
                default_factory=lambda: {
                    "cam_high": IntelRealSenseCameraConfig(
                        serial_number=130322270184,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_low": IntelRealSenseCameraConfig(
                        serial_number=218622274938,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_left_wrist": IntelRealSenseCameraConfig(
                        serial_number=218622274938,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_right_wrist": IntelRealSenseCameraConfig(
                        serial_number=128422271347,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                }
            )

            mock: bool = False
    
    .. group-tab:: Trossen AI Mobile
        
        .. code-block:: python

            @RobotConfig.register_subclass("trossen_ai_mobile")
            @dataclass
            class TrossenAIMobileRobotConfig(ManipulatorRobotConfig):

            # /!\ FOR SAFETY, READ THIS /!\
            # `max_relative_target` limits the magnitude of the relative positional target vector for safety purposes.
            # Set this to a positive scalar to have the same value for all motors, or a list that is the same length as
            # the number of motors in your follower arms.
            # For Trossen AI Arms, for every goal position request, motor rotations are capped at 5 degrees by default.
            # When you feel more confident with teleoperation or running the policy, you can extend
            # this safety limit and even removing it by setting it to `null`.
            # Also, everything is expected to work safely out-of-the-box, but we highly advise to
            # first try to teleoperate the grippers only (by commenting out the rest of the motors in this yaml),
            # then to gradually add more motors (by uncommenting), until you can teleoperate both arms fully
            max_relative_target: int | None = 5

            leader_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "left": TrossenArmDriverConfig(
                        # wxai
                        ip="192.168.1.3",
                        model="V0_LEADER",
                    ),
                    "right": TrossenArmDriverConfig(
                        # wxai
                        ip="192.168.1.2",
                        model="V0_LEADER",
                    ),
                }
            )

            follower_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "left": TrossenArmDriverConfig(
                        ip="192.168.1.5",
                        model="V0_FOLLOWER",
                    ),
                    "right": TrossenArmDriverConfig(
                        ip="192.168.1.4",
                        model = "V0_FOLLOWER",
                    ),
                }
            )

            # Troubleshooting: If one of your IntelRealSense cameras freeze during
            # data recording due to bandwidth limit, you might need to plug the camera
            # on another USB hub or PCIe card.
            cameras: dict[str, CameraConfig] = field(
                default_factory=lambda: {
                    "cam_high": IntelRealSenseCameraConfig(
                        serial_number=130322270184,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_left_wrist": IntelRealSenseCameraConfig(
                        serial_number=218622274938,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_right_wrist": IntelRealSenseCameraConfig(
                        serial_number=128422271347,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                }
            )

            mock: bool = False

    .. group-tab:: Trossen AI Solo
        
        .. code-block:: python

            @RobotConfig.register_subclass("trossen_ai_solo")
            @dataclass
            class TrossenAISoloRobotConfig(ManipulatorRobotConfig):

            # /!\ FOR SAFETY, READ THIS /!\
            # `max_relative_target` limits the magnitude of the relative positional target vector for safety purposes.
            # Set this to a positive scalar to have the same value for all motors, or a list that is the same length as
            # the number of motors in your follower arms.
            # For Trossen AI Arms, for every goal position request, motor rotations are capped at 5 degrees by default.
            # When you feel more confident with teleoperation or running the policy, you can extend
            # this safety limit and even removing it by setting it to `null`.
            # Also, everything is expected to work safely out-of-the-box, but we highly advise to
            # first try to teleoperate the grippers only (by commenting out the rest of the motors in this yaml),
            # then to gradually add more motors (by uncommenting), until you can teleoperate both arms fully
            max_relative_target: int | None = 5

            leader_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "main": TrossenArmDriverConfig(
                        # wxai
                        ip="192.168.1.3",
                        model="V0_LEADER",
                    ),
                }
            )

            follower_arms: dict[str, MotorsBusConfig] = field(
                default_factory=lambda: {
                    "main": TrossenArmDriverConfig(
                        ip="192.168.1.5",
                        model="V0_FOLLOWER",
                    ),
                }
            )

            # Troubleshooting: If one of your IntelRealSense cameras freeze during
            # data recording due to bandwidth limit, you might need to plug the camera
            # on another USB hub or PCIe card.
            cameras: dict[str, CameraConfig] = field(
                default_factory=lambda: {
                    "cam_main": IntelRealSenseCameraConfig(
                        serial_number=130322270184,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                    "cam_wrist": IntelRealSenseCameraConfig(
                        serial_number=218622274938,
                        fps=30,
                        width=640,
                        height=480,
                    ),
                }
            )

            mock: bool = False

Setup IP Address
----------------

.. note::

    By default, the IP address for a Trossen AI arm is set to ``192.168.1.2``. 
    Make sure to change the IP addresses of your Trossen AI arms to match the ones in the configuration file.
    
To set up the IP address for the Trossen AI Arms, you must first ensure that the arms are connected to the same network as your computer.
Refer to :ref:`Ethernet Setup <getting_started/software_setup:Ethernet Setup>` for correct connection instructions.
Once connected, you can find or configure the IP address using the :ref:`getting_started/demo_scripts:`configure_cleanup`_` demo or the :ref:`getting_started/demo_scripts:`set_ip_method`_` and :ref:`getting_started/demo_scripts:`set_manual_ip`_` demos.

Camera Serial Number
--------------------

#.  Open realsense-viewer

    .. code-block:: bash

        realsense-viewer

    .. note::

        If realsense-viewer is not already installed on your machine, follow `these steps on the librealsense GitHub repository <https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md>`_  to install ``librealsense2-utils``.

#.  Plug in a single camera and check the sidebar for its entry.
    If it does not show up in the side bar, click Add Source and find the Intel RealSense D405 in the drop down.

#.  Click on Info for the camera, find the Serial Number, and copy it.

    .. image:: images/rsviewer_serialno2.png
        :alt: Realsense Viewer
        :align: center

#.  Put the camera serial number in the appropriate config entry at :guilabel:`lerobot/common/robot_devices/robots/configs.py`.

#.  Repeat for the rest of the cameras.

