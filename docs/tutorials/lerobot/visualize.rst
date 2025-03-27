=========
Visualize
=========

Remote Visualization
====================

If you uploaded your dataset to the Hugging Face Hub using ``--control.push_to_hub=true``, you can `visualize your dataset online <https://huggingface.co/spaces/lerobot/visualize_dataset>`_.
To do so, copy and paste your repository ID into the provided field. Your repository ID follows the format:

.. code-block:: bash

   <huggingface-username>/<dataset-id>

You can retrieve your repository ID by running the following command in your terminal:

.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

         echo ${HF_USER}/trossen_ai_stationary_test

   .. group-tab:: Trossen AI Mobile
      
      .. code-block:: bash

         echo ${HF_USER}/trossen_ai_mobile_test
   
   .. group-tab:: Trossen AI Solo
      
      .. code-block:: bash

         echo ${HF_USER}/trossen_ai_solo_test

Here, **${HF_USER}** represents your **Hugging Face username**, and **trossen_ai_xxxxxxx_test** is the **dataset ID** you provided during the upload.


Local Visualization
===================

If you didn't upload the dataset (i.e., you used ``--control.push_to_hub=false``), you can still visualize it locally with:


.. tabs::

   .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

         python lerobot/scripts/visualize_dataset_html.py \
           --repo-id ${HF_USER}/trossen_ai_stationary_test \
           --local-files-only 1

   .. group-tab:: Trossen AI Mobile
      
      .. code-block:: bash

         python lerobot/scripts/visualize_dataset_html.py \
           --repo-id ${HF_USER}/trossen_ai_mobile_test \
           --local-files-only 1
   
   .. group-tab:: Trossen AI Solo
      
      .. code-block:: bash

         python lerobot/scripts/visualize_dataset_html.py \
           --repo-id ${HF_USER}/trossen_ai_solo_test \
           --local-files-only 1
