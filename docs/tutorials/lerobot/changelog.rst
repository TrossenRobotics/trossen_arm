===================
Updates & Changelog
===================

This page documents major updates to the project, such as changes to dataset formats, codebase improvements, new features, and compatibility notes.

Trossen v1.0 Dataset Format
===========================

:date: 05-01-2025
:commit: `Interbotix/lerobot@8f72211 <https://github.com/Interbotix/lerobot/commit/8f7221114505e770f1f987b6cd909e0f4a323993>`_


We have introduced a new dataset format for the Trossen AI arms, which is now available in the :guilabel:`Interbotix/lerobot` repository.
This new datasets format allows for storing joints angles in ``radians``, which is the default for the Trossen AI arms.
We also have removed the scaling factor from the gripper joint angles, as it is no longer needed.
The  new dataset format allows us to have smaller and normalized joint angles, which is more efficient for training and inference.

We add :guilabel:`trossen_subversion` to the dataset metadata to indicate that this dataset was created using ``Interbotix/lerobot`` 
This also allows us to run ``backward compatibility`` checks in the future.
The ``backward compatibility`` checks are run when you load a dataset using the ``Dataset`` class.
The checks will verify that the dataset is compatible with the current version of the code.

The new dataset format is not compatible with the previous one, so you will need to convert your old datasets to the new format :guilabel:`convert_dataset_v21_to_v21_t10.py` script is provided.
To convert your old datasets to the new format, you can use the following command:

.. code-block:: bash

    python lerobot/scripts/convert_dataset_v21_to_v21_t10.py \
      --repo_id=my_dataset \
      --push_to_hub \
      --private \
      --tags "lerobot" "tutorial"

This script converts :guilabel:`LeRobot v2.1` datasets to the :guilabel:`LeRobot v2.1 Trossen v1.0` subversion by:

* Converting joint angles to radians.
* Converting gripper positions to millimeters.
* Updating dataset actions, states, and statistics.

Arguments:

* ``--repo_id``: Repository ID of the dataset to modify.
* ``--push_to_hub``: Push the modified dataset to the Hugging Face Hub.
* ``--private``: Upload the dataset to a private repository.
* ``--tags``: Optional tags for the dataset on the Hugging Face Hub.

OpenCV Camera Interface
=======================

:date: 04-30-2025
:commit: `Interbotix/lerobot@ccbbbc1 <https://github.com/Interbotix/lerobot/commit/ccbbbc15914a11fb4d4e7368e402baab244862c6>`_

We have introduced easy to use support for the OpenCV camera interface.
You can setup multiple camera interfaces in the configuration file and select the one you want to use in the command line.
The OpenCV camera interface is a simple and easy to use camera interface that allows you to use any camera that is supported by OpenCV.

A single robot configuration file can now support multiple camera interfaces as shown in the example below.

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

For more information on how to configure the robot, please refer to the :ref:`tutorials/lerobot/configuration:Camera Serial Number` OpenCV page.