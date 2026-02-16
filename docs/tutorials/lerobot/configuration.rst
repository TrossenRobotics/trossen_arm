========================
Trossen AI Configuration
========================

.. note::

    Ongoing support for the LeRobot fork is deprecated.
    We recommend using the :doc:`plugin-based LeRobot integration <../lerobot_plugin>` instead.

    However, this integration is necessary for users who wish to use the :doc:`openpi integration <../openpi>`.

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

                # Gain applied to external efforts sensed on the follower arm and transmitted to the leader arm.
                # This enables the user to feel external forces (e.g., contact with objects) through force feedback.
                # A value of 0.0 disables force feedback. A good starting value for a responsive experience is 0.1.
                force_feedback_gain: float = 0.0

                # Set this according to the camera interface you want to use.
                # "intel_realsense" is the default and recommended option.
                # "opencv" is a fallback option that uses OpenCV to access the cameras.
                camera_interface: Literal["intel_realsense", "opencv"] = "intel_realsense"

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
                            model="V0_FOLLOWER",
                        ),
                    }
                )

                if camera_interface == "opencv":
                    print("Using OpenCV camera interface")
                    cameras: dict[str, CameraConfig] = field(
                        default_factory=lambda: {
                            "cam_high": OpenCVCameraConfig(
                                camera_index=26,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_low": OpenCVCameraConfig(
                                camera_index=14,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_left_wrist": OpenCVCameraConfig(
                                camera_index=8,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_right_wrist": OpenCVCameraConfig(
                                camera_index=20,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                        }
                    )
                elif camera_interface == "intel_realsense":
                    # Troubleshooting: If one of your IntelRealSense cameras freeze during
                    # data recording due to bandwidth limit, you might need to plug the camera
                    # on another USB hub or PCIe card.
                    cameras: dict[str, CameraConfig] = field(
                        default_factory=lambda: {
                            "cam_high": IntelRealSenseCameraConfig(
                                serial_number=218622270304,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_low": IntelRealSenseCameraConfig(
                                serial_number=130322272628,
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
                else:
                    raise ValueError(
                        f"Unknown camera interface: {camera_interface}. Supported values are 'opencv' and 'intel_realsense'."
                    )

                mock: bool = False

    .. group-tab:: Trossen AI Mobile

        .. code-block:: python

            @RobotConfig.register_subclass("trossen_ai_mobile")
            @dataclass
            class TrossenAIMobileRobotConfig(RobotConfig):
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

                # Gain applied to external efforts sensed on the follower arm and transmitted to the leader arm.
                # This enables the user to feel external forces (e.g., contact with objects) through force feedback.
                # A value of 0.0 disables force feedback. A good starting value for a responsive experience is 0.1.
                force_feedback_gain: float = 0.0

                # Set this according to the camera interface you want to use.
                # "intel_realsense" is the default and recommended option.
                # "opencv" is a fallback option that uses OpenCV to access the cameras.
                camera_interface: Literal["intel_realsense", "opencv"] = "intel_realsense"

                enable_motor_torque: bool = False

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
                            model="V0_FOLLOWER",
                        ),
                    }
                )

                if camera_interface == "opencv":
                    cameras: dict[str, CameraConfig] = field(
                        default_factory=lambda: {
                            "cam_high": OpenCVCameraConfig(
                                camera_index=26,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_left_wrist": OpenCVCameraConfig(
                                camera_index=8,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_right_wrist": OpenCVCameraConfig(
                                camera_index=20,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                        }
                    )
                elif camera_interface == "intel_realsense":
                    # Troubleshooting: If one of your IntelRealSense cameras freeze during
                    # data recording due to bandwidth limit, you might need to plug the camera
                    # on another USB hub or PCIe card.

                    cameras: dict[str, CameraConfig] = field(
                        default_factory=lambda: {
                            "cam_high": IntelRealSenseCameraConfig(
                                serial_number=130322274102,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_left_wrist": IntelRealSenseCameraConfig(
                                serial_number=130322271087,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_right_wrist": IntelRealSenseCameraConfig(
                                serial_number=130322270184,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                        }
                    )
                else:
                    raise ValueError(
                        f"Unknown camera interface: {camera_interface}. Supported values are 'opencv' and 'intel_realsense'."
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

                # Gain applied to external efforts sensed on the follower arm and transmitted to the leader arm.
                # This enables the user to feel external forces (e.g., contact with objects) through force feedback.
                # A value of 0.0 disables force feedback. A good starting value for a responsive experience is 0.1.
                force_feedback_gain: float = 0.0

                # Set this according to the camera interface you want to use.
                # "intel_realsense" is the default and recommended option.
                # "opencv" is a fallback option that uses OpenCV to access the cameras.
                camera_interface: Literal["intel_realsense", "opencv"] = "intel_realsense"

                leader_arms: dict[str, MotorsBusConfig] = field(
                    default_factory=lambda: {
                        "main": TrossenArmDriverConfig(
                            # wxai
                            ip="192.168.1.2",
                            model="V0_LEADER",
                        ),
                    }
                )

                follower_arms: dict[str, MotorsBusConfig] = field(
                    default_factory=lambda: {
                        "main": TrossenArmDriverConfig(
                            ip="192.168.1.3",
                            model="V0_FOLLOWER",
                        ),
                    }
                )

                if camera_interface == "opencv":
                    cameras: dict[str, CameraConfig] = field(
                        default_factory=lambda: {
                            "cam_main": OpenCVCameraConfig(
                                camera_index=26,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                            "cam_wrist": OpenCVCameraConfig(
                                camera_index=8,
                                fps=30,
                                width=640,
                                height=480,
                            ),
                        }
                    )
                elif camera_interface == "intel_realsense":
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
                else:
                    raise ValueError(
                        f"Unknown camera interface: {camera_interface}. Supported values are 'opencv' and 'intel_realsense'."
                    )

                mock: bool = False

Setup IP Address
----------------

.. note::

    By default, the IP address for a Trossen AI arm is set to ``192.168.1.2``.
    Make sure to change the IP addresses of your Trossen AI arms to match the ones in the configuration file.

To set up the IP address for the Trossen AI Arms, you must first ensure that the arms are connected to the same network as your computer.
Refer to the :ref:`PC Network Setup guide <getting_started/software_setup:PC Network Setup>` for correct connection instructions.
Once connected, you can find or configure the IP address using the :ref:`getting_started/demo_scripts:`configure_cleanup`_` demo or the :ref:`getting_started/demo_scripts:`set_ip_method`_` and :ref:`getting_started/demo_scripts:`set_manual_ip`_` demos.

Camera Serial Number
--------------------

There are two ways to set up the camera serial numbers for the Trossen AI Kits with LeRobot: using the Intel RealSense interface or the OpenCV interface.
Based on the camera interface you choose, follow the appropriate steps below to set up the camera serial numbers.
You can setup both interfaces at the same time, and use the ``--robot.camera_interface`` argument to switch between them.
By default, the camera interface is set to ``intel_realsense``.
We will look at this in more detail in the next sections.

.. tabs::
    .. group-tab:: Intel RealSense Interface

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

    .. group-tab:: OpenCV Interface

        The `OpenCVCamera <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/robot_devices/cameras/opencv.py>`_ class allows you to efficiently record frames from most cameras using the `opencv2 <https://docs.opencv.org>`_ library.
        For more details on compatibility, see `Video I/O with OpenCV Overview <https://docs.opencv.org/4.x/d0/da7/videoio_overview.html>`_.

        To instantiate an `OpenCVCamera <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/robot_devices/cameras/opencv.py>`_, you need a camera index (e.g. :guilabel:`OpenCVCamera(camera_index=0)`).
        When you only have one camera like a webcam of a laptop, the camera index is usually ``0`` but it might differ, and the camera index might change if you reboot your computer or re-plug your camera.
        This behavior depends on your operating system.


        #.  To find the camera indices, run the following utility script, which will save a few frames from each detected camera:

            .. code-block:: bash

                python lerobot/common/robot_devices/cameras/opencv.py \
                --images-dir outputs/images_from_opencv_cameras

            The output will look something like this if you have two cameras connected:

            .. code-block:: bash

                Mac or Windows detected. Finding available camera indices through scanning all indices from 0 to 60
                [...]
                Camera found at index 0
                Camera found at index 1
                [...]
                Connecting cameras
                OpenCVCamera(0, fps=30.0, width=1920.0, height=1080.0, color_mode=rgb)
                OpenCVCamera(1, fps=24.0, width=1920.0, height=1080.0, color_mode=rgb)
                Saving images to outputs/images_from_opencv_cameras
                Frame: 0000	Latency (ms): 39.52
                [...]
                Frame: 0046	Latency (ms): 40.07
                Images have been saved to outputs/images_from_opencv_cameras

        #. Check the saved images in :guilabel:`outputs/images_from_opencv_cameras` to identify which camera index corresponds to which physical camera (e.g. ``0`` for ``camera_00`` or ``1`` for ``camera_01``):

            .. code-block:: bash

                camera_00_frame_000000.png
                [...]
                camera_00_frame_000047.png
                camera_01_frame_000000.png
                [...]
                camera_01_frame_000047.png


            .. note::

                Some cameras may take a few seconds to warm up, and the first frame might be black or green.

            .. note::

                RealSense cameras expose multiple video streams when accessed via OpenCV. One stream is optimized for RGB color output with full image processing, 
                while others are optimized for depth calculation and may appear darker or less processed.
                Review the captured images and select the video index that provides properly processed RGB color output.

        #. Put the camera index in the appropriate config entry at :guilabel:`lerobot/common/robot_devices/robots/configs.py`.


            .. code-block:: python

                cameras: dict[str, CameraConfig] = field(
                    default_factory=lambda: {
                        "cam_xxxx": OpenCVCameraConfig(
                            camera_index=0,
                            fps=30,
                            width=640,
                            height=480,
                        ),
                        "cam_xxxx": OpenCVCameraConfig(
                            camera_index=1,
                            fps=30,
                            width=640,
                            height=480,
                        ),
                    }
                )
