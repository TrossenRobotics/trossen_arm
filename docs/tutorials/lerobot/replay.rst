====================
Replaying an Episode
====================

Now try to replay the first recorded episode on your robot:

.. code-block:: bash

   python lerobot/scripts/control_robot.py \
     --robot.type=trossen_ai_bimanual \
     --robot.max_relative_target=null \
     --control.type=replay \
     --control.fps=30 \
     --control.repo_id=${HF_USER}/trossen_ai_bimanual_test \
     --control.episode=0