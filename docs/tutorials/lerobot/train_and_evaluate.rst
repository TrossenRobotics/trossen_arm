================================
Training and Evaluating a Policy
================================

Training a Policy
=================

To train a policy to control your robot, use the :guilabel:`python lerobot/scripts/train.py` script. A few arguments are required. Here is an example command:

.. code-block:: bash

   python lerobot/scripts/train.py \
     --dataset.repo_id=${HF_USER}/trossen_ai_bimanual_test \
     --policy.type=act \
     --output_dir=outputs/train/act_trossen_ai_bimanual_test \
     --job_name=act_trossen_ai_bimanual_test \
     --device=cuda \
     --wandb.enable=true

Explanation of the Command
--------------------------

#. We provided the dataset using ``--dataset.repo_id=${HF_USER}/trossen_ai_bimanual_test``.
#. We specified the policy with ``--policy.type=act``, which loads configurations from :guilabel:`configuration_act.py <../lerobot/common/policies/act/configuration_act.py>`.
   This policy will automatically adapt to the **number of motor states, motor actions, and cameras** recorded in your dataset.
#. We set ``--device=cuda`` to train on an **NVIDIA GPU**.
   If using Apple Silicon, you can replace it with ``--device=mps``.
#. We enabled **Weights & Biases** logging using ``--wandb.enable=true`` for visualizing training plots.
   This is optional, but if used, ensure you're logged in by running:

   .. code-block:: bash

      wandb login

**Training will take several hours.** Checkpoints will be saved in:
:guilabel:`outputs/train/act_trossen_ai_bimanual_test/checkpoints`.


Evaluating Your Policy
======================

You can use the ``record`` function from :guilabel:`lerobot/scripts/control_robot.py` but with a **policy checkpoint as input**.
Run the following command to record **10 evaluation episodes**:

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --control.type=record \
     --control.fps=30 \
     --control.single_task="Grasp a lego block and put it in the bin." \
     --control.repo_id=${HF_USER}/eval_act_trossen_ai_bimanual_test \
     --control.tags='["tutorial"]' \
     --control.warmup_time_s=5 \
     --control.episode_time_s=30 \
     --control.reset_time_s=30 \
     --control.num_episodes=10 \
     --control.push_to_hub=true \
     --control.policy.path=outputs/train/act_trossen_ai_bimanual_test/checkpoints/last/pretrained_model \
     --control.num_image_writer_processes=1

Key Differences from Training Data Recording
--------------------------------------------

#. Policy Checkpoint:

   - The command includes ``--control.policy.path``, which specifies the path to the trained policy checkpoint (e.g., :guilabel:`outputs/train/act_trossen_ai_bimanual_test/checkpoints/last/pretrained_model`).
   - If you uploaded the model checkpoint to **Hugging Face Hub**, you can also specify it as: :guilabel:`--control.policy.path=${HF_USER}/act_trossen_ai_bimanual_test`.

#. Dataset Naming Convention:

   - The dataset name now begins with ``eval_`` (e.g., ``${HF_USER}/eval_act_trossen_ai_bimanual_test``) to indicate that **this is an evaluation dataset**.

#. Image Writing Process:

   - We set ``--control.num_image_writer_processes=1`` instead of the default ``0``.
   - On some systems, using a **dedicated process** for writing images (from multiple cameras) allows achieving **a consistent 30 FPS during inference**.
   - You can experiment with different values of ``--control.num_image_writer_processes`` to optimize performance.
