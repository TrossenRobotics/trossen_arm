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