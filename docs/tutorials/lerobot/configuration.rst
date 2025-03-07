========================================
Configuring Trossen AI Arms with LeRobot
========================================

Inorder to use the Trossen AI Arms with LeRobot, you need to configure the armsby providing the necessary specifications.
The specifications include the ip address,model_name, and camera serial number.
The following steps will guide you on how to configure the Trossen AI Arms with LeRobot.


This is an example of a configuration file for the Trossen AI Arms with LeRobot, you can find this in :guilabel:`lerobot/common/robot_devices/robots/configs.py`:

.. code-block:: python

    @RobotConfig.register_subclass("trossen_ai_bimanual")
    @dataclass
    class TrossenAIBimanualRobotConfig(ManipulatorRobotConfig):

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


Setup IP Address
----------------

To setup the IP address for the Trossen AI Arms, you need to connect the arms to the same network as your computer.
You can then find the IP address/ Set them using the following documentation.


Camera Serial Number
--------------------

To find the camera serial number use realsense-viewer, you can find the serial number in the device info tab.