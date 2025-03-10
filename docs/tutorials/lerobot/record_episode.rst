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

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --robot.max_relative_target=null \
     --control.type=record \
     --control.fps=30 \
     --control.single_task="Grasp a lego block and put it in the bin." \
     --control.repo_id=${HF_USER}/trossen_ai_bimanual_test \
     --control.tags='["tutorial"]' \
     --control.warmup_time_s=5 \
     --control.episode_time_s=30 \
     --control.reset_time_s=30 \
     --control.num_episodes=2 \
     --control.push_to_hub=true

Handling Camera FPS Issues
==========================

**Note:** If the camera FPS is **unstable**, consider increasing the number of **image writers per thread**.

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --robot.max_relative_target=null \
     --control.type=record \
     --control.fps=30 \
     --control.single_task="Grasp a lego block and put it in the bin." \
     --control.repo_id=${HF_USER}/trossen_ai_bimanual_test \
     --control.tags='["tutorial"]' \
     --control.warmup_time_s=5 \
     --control.episode_time_s=30 \
     --control.reset_time_s=30 \
     --control.num_episodes=2 \
     --control.push_to_hub=true \
     --control.num_image_writer_threads_per_camera=8



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
