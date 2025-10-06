===============
Record Episodes
===============

Once you're familiar with teleoperation, you can record your first dataset with **Trossen AI**.

Logging into Hugging Face
=========================

If you want to use **Hugging Face Hub features** for uploading your dataset and you haven't done so before, make sure you've logged in using a **write-access token**, which can be generated from the `Hugging Face settings <https://huggingface.co/settings/tokens>`_.

.. code-block:: bash

   huggingface-cli login --token ${HUGGINGFACE_TOKEN} --add-to-git-credential

Store your Hugging Face repository name in a variable:

.. code-block:: bash

   HF_USER=$(huggingface-cli whoami | head -n 1)
   echo $HF_USER

Recording and Uploading a Dataset
=================================

Record **2 episodes** and upload your dataset to the **Hugging Face Hub**:

.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash
         :emphasize-lines: 12,13

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_stationary \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Stationary." \
         --control.repo_id=${HF_USER}/trossen_ai_stationary_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true
   
   .. group-tab:: Trossen AI Mobile

      The Trossen AI Mobile robot supports two teleoperation modes.
      In the first mode, with motor **torque disabled**, you can manually push the base to collect data.
      In the second mode, with motor **torque enabled**, you can control the robot using the Remote Control.
      For details on using the Remote Control, refer to the `Remote Control Operation <https://docs.trossenrobotics.com/slate_docs/operation/rc_controller.html>`_.
      
      .. note::
         By default, ``enable_motor_torque`` is set to ``false`` on the Trossen AI Mobile robot.
         To enable remote control, set it to ``true`` by adding ``--robot.enable_motor_torque=true`` to the command line.
         If you'd like to change the default behavior, you can modify the ``lerobot/common/robot_devices/robots/configs.py`` file.

      .. tabs::

         .. tab:: Trossen AI Mobile with Torque Disabled

            .. code-block:: bash
               :emphasize-lines: 12,13

               python lerobot/scripts/control_robot.py \
               --robot.type=trossen_ai_mobile \
               --robot.max_relative_target=null \
               --control.type=record \
               --control.fps=30 \
               --control.single_task="Test recording episode using Trossen AI Mobile." \
               --control.repo_id=${HF_USER}/trossen_ai_mobile_test \
               --control.tags='["tutorial"]' \
               --control.warmup_time_s=5 \
               --control.episode_time_s=30 \
               --control.reset_time_s=30 \
               --control.num_episodes=2 \
               --control.push_to_hub=true

         .. tab:: Trossen AI Mobile with Torque Enabled

            .. code-block:: bash
               :emphasize-lines: 12,13,14

               python lerobot/scripts/control_robot.py \
               --robot.type=trossen_ai_mobile \
               --robot.max_relative_target=null \
               --control.type=record \
               --control.fps=30 \
               --control.single_task="Test recording episode using Trossen AI Mobile." \
               --control.repo_id=${HF_USER}/trossen_ai_mobile_test \
               --control.tags='["tutorial"]' \
               --control.warmup_time_s=5 \
               --control.episode_time_s=30 \
               --control.reset_time_s=30 \
               --control.num_episodes=2 \
               --control.push_to_hub=true \
               --robot.enable_motor_torque=true



   .. group-tab:: Trossen AI Solo
      
      .. code-block:: bash
         :emphasize-lines: 12,13

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_solo \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Solo." \
         --control.repo_id=${HF_USER}/trossen_ai_solo_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true

.. note::

   The units for each joint are as follows:

   - **Joints 0-5**: Radians
   - **Joint 6 (Gripper)**: Millimeters (mm)

.. warning::

   We have introdced a new change in how the values are saved in the dataset.
   The values are now saved in the dataset as **radians** for all joints and no scaling is applied for the gripper.
   If you are using a **previous version** of the dataset, the values for joints 0-5 will be in **degrees** and a scaling of 10000 will be applied to gripper.
   Check  :ref:`tutorials/lerobot/changelog:Trossen v1.0 Dataset Format` before using datasets from previous versions.

.. note::

    The video encoding process can be resource-intensive.
    This can cause longer wait times between episodes, especially if you are recording at longer episode lengths.
    To mitigate this, consider adjusting the ``save_interval`` parameter to save data less frequently or at the end of the entire session.

Handling Camera FPS Issues
==========================

**Note:** If the camera FPS is **unstable**, consider increasing the number of **image writers per thread** or **disable camera display**.


.. tabs::
    
   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash
         :emphasize-lines: 14,15

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_stationary\
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Stationary." \
         --control.repo_id=${HF_USER}/trossen_ai_stationary_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false
   
   .. group-tab:: Trossen AI Mobile
      
      .. code-block:: bash
         :emphasize-lines: 14,15

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_mobile \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Mobile." \
         --control.repo_id=${HF_USER}/trossen_ai_mobile_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false
   
   .. group-tab:: Trossen AI Solo
      
      .. code-block:: bash
         :emphasize-lines: 14,15

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_solo \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Solo." \
         --control.repo_id=${HF_USER}/trossen_ai_solo_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false


If using ``IntelRealsense`` camera interface is causing fps issues, you can try using the ``OpenCV`` interface instead.
Make sure that you have configured the cameras correctly as described in :ref:`tutorials/lerobot/configuration:Camera Serial Number`.

.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash
         :emphasize-lines: 16

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_stationary \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Stationary." \
         --control.repo_id=${HF_USER}/trossen_ai_stationary_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false \
         --camera.interface_type=opencv
   
   .. group-tab:: Trossen AI Mobile
      
      .. code-block:: bash
         :emphasize-lines: 16

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_mobile \
         --robot.max_relative_target=null \
         --control.type=record \
         --control.fps=30 \
         --control.single_task="Test recording episode using Trossen AI Mobile." \
         --control.repo_id=${HF_USER}/trossen_ai_mobile_test \
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false \
         --camera.interface_type=opencv
   
   .. group-tab:: Trossen AI Solo
      
      .. code-block:: bash
         :emphasize-lines: 16

         python lerobot/scripts/control_robot.py \
         --robot.type=trossen_ai_solo\
         --robot.max_relative_target=null\
         --control.type=record\
         --control.fps=30\
         --control.single_task="Test recording episode using Trossen AI Solo."\
         --control.repo_id=${HF_USER}/trossen_ai_solo_test\
         --control.tags='["tutorial"]' \
         --control.warmup_time_s=5 \
         --control.episode_time_s=30 \
         --control.reset_time_s=30 \
         --control.num_episodes=2 \
         --control.push_to_hub=true \
         --control.num_image_writer_threads_per_camera=8 \
         --control.display_cameras=false \
         --camera.interface_type=opencv

Recording Configuration
=======================

When recording a dataset, you can specify command line arguments to customize the behavior:

- ``--control.fps`` (int): The number of frames per second to record. By default, uses the policy fps.
- ``--control.single_task`` (str): The task description for the episode (e.g. "Pick the Lego block and drop it in the box on the right.").
- ``--control.repo_id`` (str): The repository ID to upload the dataset to. By convention, it should match '{hf_username}/{dataset_name}' (e.g. `lerobot/test`).
- ``--control.tags`` (list[str]): The tags to add to the dataset.
- ``--control.warmup_time_s`` (int | float): The duration of the warm-up phase in seconds. It allows the robot devices to warm up and synchronize.
- ``--control.episode_time_s`` (int | float): The duration of the episode in seconds.
- ``--control.reset_time_s`` (int | float): The duration of the reset phase in seconds.
- ``--control.num_episodes`` (int): The number of episodes to record.
- ``--control.push_to_hub`` (bool): Flag to upload the dataset to the Hugging Face Hub.
- ``--control.num_image_writer_threads_per_camera`` (int): The number of image writer threads per camera. Too many threads might cause unstable teleoperation fps due to the main thread being blocked. Not enough threads might cause low camera fps.
- ``--control.root`` (str | Path | None): The root directory to save the dataset to (e.g. 'dataset/path').
- ``--control.device`` (str | None): The device to use for computation (e.g. 'cuda', 'cpu', 'mps').
- ``--control.use_amp`` (bool | None): Flag to use Automatic Mixed Precision (AMP) for training and evaluation.
- ``--control.video`` (bool): Flag to encode frames in the dataset into video.
- ``--control.run_compute_stats`` (bool): Flag to run the computation of the data statistics at the end of data collection.
- ``--control.private`` (bool): Flag to upload the dataset to a private repository on the Hugging Face Hub.
- ``--control.num_image_writer_processes`` (int): The number of subprocesses handling the saving of frames as PNGs.
- ``--control.display_cameras`` (bool): Flag to display all cameras on screen.
- ``--control.play_sounds`` (bool): Flag to use vocal synthesis to read events.
- ``--control.resume`` (bool): Flag to resume recording on an existing dataset.
- ``--control.local_files_only`` (bool): Flag to use local files only.
- ``--control.save_interval`` (int): Interval in episodes at which to encode images to video and save data to disk.
   For example, if set to 5, data will be saved every 5 episodes.
   Default is 1 (save after every episode).
   Setting save interval to ``-1``, ``0``, or ``> total number of episodes`` will only save data at the end of the entire data collection session.