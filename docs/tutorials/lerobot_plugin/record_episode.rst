===============
Record Episodes
===============

Once you're familiar with teleoperation, you can record your first dataset.

Logging into Hugging Face
=========================

If you want to use Hugging Face Hub features for uploading your dataset and you haven't done so before, make sure you've logged in using a write-access token which can be generated from the `Hugging Face settings <https://huggingface.co/settings/tokens>`_.

.. code-block:: bash

    huggingface-cli login --token ${HUGGINGFACE_TOKEN} --add-to-git-credential

Store your Hugging Face repository name in a variable:

.. code-block:: bash

    HF_USER=$(huggingface-cli whoami | head -n 1)
    echo $HF_USER

Recording and Uploading a Dataset
=================================

Record two episodes and upload your dataset to the Hugging Face Hub:

.. tabs::

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash
            :emphasize-lines: 16,17

            uv run lerobot-record \
                --robot.type=widowxai_follower_robot \
                --robot.ip_address=192.168.1.4 \
                --robot.id=follower \
                --robot.cameras="{
                    cam_main: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
                }" \
                --teleop.type=widowxai_leader_teleop \
                --teleop.ip_address=192.168.1.2 \
                --teleop.id=leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/widowxai-cube-pickup \
                --dataset.episode_time_s=45 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Grab the cube"

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash
            :emphasize-lines: 20,21

            uv run lerobot-record \
                --robot.type=bi_widowxai_follower_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=bimanual_follower \
                --robot.cameras='{
                cam_high: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_low: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_left_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_right_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                }' \
                --teleop.type=bi_widowxai_leader_teleop \
                --teleop.left_arm_ip_address=192.168.1.3 \
                --teleop.right_arm_ip_address=192.168.1.2 \
                --teleop.id=bimanual_leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/bimanual-widowxai-handover-cube \
                --dataset.episode_time_s=60 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Grab and handover the red cube to the other arm"

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash
            :emphasize-lines: 19,20

            uv run lerobot-record \
                --robot.type=mobileai_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=mobile_follower \
                --robot.cameras='{
                cam_high: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_left_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_right_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                }' \
                --teleop.type=mobileai_leader_teleop \
                --teleop.left_arm_ip_address=192.168.1.3 \
                --teleop.right_arm_ip_address=192.168.1.2 \
                --teleop.id=mobile_leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/mobileai-pick-and-place \
                --dataset.episode_time_s=60 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Pick and place the object"

.. note::

    The units for each joint are as follows:

    - **Joints 0-5**: Radians
    - **Joint 6 (Gripper)**: Meters

Handling Camera FPS Issues
==========================

.. note::

    If the camera FPS is unstable, consider increasing the number of image writers per thread or disable camera display.

.. tabs::

   .. group-tab:: Trossen AI Solo

        .. code-block:: bash
            :emphasize-lines: 18,19

            uv run lerobot-record \
                --robot.type=widowxai_follower_robot \
                --robot.ip_address=192.168.1.4 \
                --robot.id=follower \
                --robot.cameras='{
                cam_main: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30}}' \
                --teleop.type=widowxai_leader_teleop \
                --teleop.ip_address=192.168.1.2 \
                --teleop.id=leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/widowxai-cube-pickup \
                --dataset.episode_time_s=45 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Grab the cube" \
                --dataset.num_image_writer_threads_per_camera=8 \
                --display_data=false

   .. group-tab:: Trossen AI Stationary

        .. code-block:: bash
            :emphasize-lines: 23,24

            uv run lerobot-record \
                --robot.type=bi_widowxai_follower_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=bimanual_follower \
                --robot.cameras='{
                cam_high: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_low: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_left_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_right_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                }' \
                --teleop.type=bi_widowxai_leader_teleop \
                --teleop.left_arm_ip_address=192.168.1.3 \
                --teleop.right_arm_ip_address=192.168.1.2 \
                --teleop.id=bimanual_leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/bimanual-widowxai-handover-cube \
                --dataset.episode_time_s=60 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Grab and handover the red cube to the other arm" \
                --dataset.num_image_writer_threads_per_camera=8 \
                --display_data=false

   .. group-tab:: Trossen AI Mobile

        .. code-block:: bash
            :emphasize-lines: 22,23

            uv run lerobot-record \
                --robot.type=mobileai_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=mobile_follower \
                --robot.cameras='{
                cam_high: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_left_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                cam_right_wrist: {"type": "intelrealsense", "serial_number_or_name": "0123456789", "width": 640, "height": 480, "fps": 30},
                }' \
                --teleop.type=mobileai_leader_teleop \
                --teleop.left_arm_ip_address=192.168.1.3 \
                --teleop.right_arm_ip_address=192.168.1.2 \
                --teleop.id=mobile_leader \
                --display_data=true \
                --dataset.repo_id=${HF_USER}/mobileai-pick-and-place \
                --dataset.episode_time_s=60 \
                --dataset.reset_time_s=15 \
                --dataset.num_episodes=2 \
                --dataset.push_to_hub=true \
                --dataset.single_task="Pick and place the object" \
                --dataset.num_image_writer_threads_per_camera=8 \
                --display_data=false

If using ``RealSense`` camera interface is causing fps issues, you can try using the ``OpenCV`` interface instead.
Make sure that you have configured the cameras correctly as described in :ref:`tutorials/lerobot/configuration:Camera Serial Number`.

Recording Configuration
=======================

When recording a dataset, you can specify command line arguments to customize the behavior:

- ``--dataset.repo_id`` (str): The dataset identifier. By convention, it should match the format ``{hf_username}/{dataset_name}`` (e.g. `lerobot/test`). (default: None)
- ``--dataset.single_task`` (str): A short but accurate description of the task performed during the recording (e.g. "Pick the Lego block and drop it in the box on the right."). (default: None)
- ``--dataset.root`` (str | Path | None): The root directory where the dataset will be stored (e.g. `'dataset/path'`). (default: None)
- ``--dataset.fps`` (int): The number of frames per second to record. (default: 30)
- ``--dataset.episode_time_s`` (int | float): The duration of data recording for each episode in seconds. (default: 60)
- ``--dataset.reset_time_s`` (int | float): The duration of the reset phase after each episode in seconds. (default: 60)
- ``--dataset.num_episodes`` (int): The number of episodes to record. (default: 50)
- ``--dataset.video`` (bool): Flag to encode frames in the dataset into video. (default: True)
- ``--dataset.push_to_hub`` (bool): Flag to upload the dataset to the Hugging Face Hub. (default: True)
- ``--dataset.private`` (bool): Flag to upload the dataset to a private repository on the Hugging Face Hub. (default: False)
- ``--dataset.tags`` (list[str]): A list of tags to add to the dataset on the Hub. (default: None)
- ``--dataset.num_image_writer_processes`` (int): The number of subprocesses handling the saving of frames as PNGs. Set to 0 to use threads only; set to ≥1 to use subprocesses, each using threads to write images. (default: 0)
- ``--dataset.num_image_writer_threads_per_camera`` (int): The number of threads writing frames as PNG images on disk, per camera. Too many threads might cause unstable teleoperation FPS due to main thread blocking; too few might cause low camera FPS. (default: 4)
- ``--dataset.video_encoding_batch_size`` (int): The number of episodes to record before batch encoding videos. Set to 1 for immediate encoding (default behavior), or higher for batched encoding. (default: 1)
- ``--dataset.rename_map`` (dict): A rename map for observations to override image and state keys. (default: {})
