=========
Visualize
=========

If you uploaded your dataset to the Hugging Face Hub using ``--control.push_to_hub=true``, you can `visualize your dataset online <https://huggingface.co/spaces/lerobot/visualize_dataset>`_
by copying and pasting your repository ID, which can be obtained with:

.. code-block:: bash

   echo ${HF_USER}/trossen_ai_bimanual_test

Local Visualization
===================

If you didn't upload the dataset (i.e., you used ``--control.push_to_hub=false``), you can still visualize it locally with:

.. code-block:: bash

   python lerobot/scripts/visualize_dataset_html.py \
     --repo-id ${HF_USER}/trossen_ai_bimanual_test