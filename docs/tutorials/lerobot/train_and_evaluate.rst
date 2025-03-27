================================
Training and Evaluating a Policy
================================

Training a Policy
=================

To train a policy to control your robot, use the :guilabel:`train.py` script.
A few arguments are required.
Here is an example command:

.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

         python lerobot/scripts/train.py \
           --dataset.repo_id=${HF_USER}/trossen_ai_stationary_test \
           --policy.type=act \
           --output_dir=outputs/train/act_trossen_ai_stationary_test \
           --job_name=act_trossen_ai_stationary_test \
           --device=cuda \
           --wandb.enable=true
   
   .. group-tab:: Trossen AI Mobile

      .. code-block:: bash

         python lerobot/scripts/train.py \
           --dataset.repo_id=${HF_USER}/trossen_ai_mobile_test \
           --policy.type=act \
           --output_dir=outputs/train/act_trossen_ai_mobile_test \
           --job_name=act_trossen_ai_mobile_test \
           --device=cuda \
           --wandb.enable=true

   .. group-tab:: Trossen AI Solo

      .. code-block:: bash

         python lerobot/scripts/train.py \
           --dataset.repo_id=${HF_USER}/trossen_ai_solo_test \
           --policy.type=act \
           --output_dir=outputs/train/act_trossen_ai_solo_test \
           --job_name=act_trossen_ai_solo_test \
           --device=cuda \
           --wandb.enable=true

Explanation of the Command
--------------------------

#. We provided the dataset using ``--dataset.repo_id=${HF_USER}/trossen_ai_xxxxxxx_test``.
#. We specified the policy with ``--policy.type=act``, which loads configurations from `configuration_act.py <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/policies/act/configuration_act.py>`_.
   This policy will automatically adapt to the **number of motor states, motor actions, and cameras** recorded in your dataset.
#. We set ``--device=cuda`` to train on an **NVIDIA GPU**.
   If using Apple Silicon, you can replace it with ``--device=mps``.
#. We enabled **Weights & Biases** logging using ``--wandb.enable=true`` for visualizing training plots.
   This is optional, but if used, ensure you're logged in by running:

   .. code-block:: bash

      wandb login

.. note::

   **Training will take several hours.** Checkpoints will be saved in:
   :guilabel:`outputs/train/act_trossen_ai_xxxxx_test/checkpoints`.


Training Pipeline Configuration
-------------------------------

The training pipeline can be configured using the following parameters:

- ``--dataset``: Configuration for the dataset.
- ``--env``: Configuration for the environment. Can be ``None``.
- ``--policy``: Configuration for the pre-trained policy. Can be ``None``.
- ``--output_dir``: Directory to save all run outputs. If another training session is run with the same value, its contents will be overwritten unless ``resume`` is set to true.
- ``--job_name``: Name of the job. Can be ``None``.
- ``--resume``: Set to true to resume a previous run. Ensure ``output_dir`` is the directory of an existing run with at least one checkpoint.
- ``--device``: Device to use for training (e.g., ``cuda``, ``cpu``, ``mps``).
- ``--use_amp``: Determines whether to use Automatic Mixed Precision (AMP) for training and evaluation.
- ``--seed``: Seed for training and evaluation environments.
- ``--num_workers``: Number of workers for the dataloader.
- ``--batch_size``: Batch size for training.
- ``--eval_freq``: Frequency of evaluation during training.
- ``--log_freq``: Frequency of logging during training.
- ``--save_checkpoint``: Whether to save checkpoints during training.
- ``--save_freq``: Frequency of saving checkpoints.
- ``--offline``: Configuration for offline training.
- ``--online``: Configuration for online training.
- ``--use_policy_training_preset``: Whether to use policy training preset.
- ``--optimizer``: Configuration for the optimizer. Can be ``None``.
- ``--scheduler``: Configuration for the learning rate scheduler. Can be ``None``.
- ``--eval``: Configuration for evaluation.
- ``--wandb``: Configuration for Weights & Biases logging.

Evaluating Your Policy
======================

You can use the ``record`` function from :guilabel:`lerobot/scripts/control_robot.py` but with a **policy checkpoint as input**.
Run the following command to record **10 evaluation episodes**:

.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

         python lerobot/scripts/control_robot.py \
           --robot.type=trossen_ai_stationary \
           --control.type=record \
           --control.fps=30 \
           --control.single_task="Recording evaluation episode using Trossen AI Stationary." \
           --control.repo_id=${HF_USER}/eval_act_trossen_ai_stationary_test \
           --control.tags='["tutorial"]' \
           --control.warmup_time_s=5 \
           --control.episode_time_s=30 \
           --control.reset_time_s=30 \
           --control.num_episodes=10 \
           --control.push_to_hub=true \
           --control.policy.path=outputs/train/act_trossen_ai_stationary_test/checkpoints/last/pretrained_model \
           --control.num_image_writer_processes=1
   
   .. group-tab:: Trossen AI Mobile

      .. code-block:: bash

         python lerobot/scripts/control_robot.py \
           --robot.type=trossen_ai_mobile \
           --control.type=record \
           --control.fps=30 \
           --control.single_task="Recording evaluation episode using Trossen AI Mobile." \
           --control.repo_id=${HF_USER}/eval_act_trossen_ai_mobile_test \
           --control.tags='["tutorial"]' \
           --control.warmup_time_s=5 \
           --control.episode_time_s=30 \
           --control.reset_time_s=30 \
           --control.num_episodes=10 \
           --control.push_to_hub=true \
           --control.policy.path=outputs/train/act_trossen_ai_mobile_test/checkpoints/last/pretrained_model \
           --control.num_image_writer_processes=1 \
           --robot.enable_motor_torque=true

   .. group-tab:: Trossen AI Solo

      .. code-block:: bash

         python lerobot/scripts/control_robot.py \
           --robot.type=trossen_ai_solo \
           --control.type=record \
           --control.fps=30 \
           --control.single_task="Recording evaluation episode using Trossen AI Solo." \
           --control.repo_id=${HF_USER}/eval_act_trossen_ai_solo_test \
           --control.tags='["tutorial"]' \
           --control.warmup_time_s=5 \
           --control.episode_time_s=30 \
           --control.reset_time_s=30 \
           --control.num_episodes=10 \
           --control.push_to_hub=true \
           --control.policy.path=outputs/train/act_trossen_ai_solo_test/checkpoints/last/pretrained_model \
           --control.num_image_writer_processes=1

Key Differences from Training Data Recording
--------------------------------------------

#. Policy Checkpoint:

   - The command includes ``--control.policy.path``, which specifies the path to the trained policy checkpoint (e.g., :guilabel:`outputs/train/act_trossen_ai_xxxxx_test/checkpoints/last/pretrained_model`).
   - If you uploaded the model checkpoint to **Hugging Face Hub**, you can also specify it as: :guilabel:`--control.policy.path=${HF_USER}/act_trossen_ai_xxxxx_test`.

#. Dataset Naming Convention:

   - The dataset name now begins with ``eval_`` (e.g., ``${HF_USER}/eval_act_trossen_ai_xxxxx_test``) to indicate that **this is an evaluation dataset**.

#. Image Writing Process:

   - We set ``--control.num_image_writer_processes=1`` instead of the default ``0``.
   - On some systems, using a **dedicated process** for writing images (from multiple cameras) allows achieving **a consistent 30 FPS during inference**.
   - You can experiment with different values of ``--control.num_image_writer_processes`` to optimize performance.
