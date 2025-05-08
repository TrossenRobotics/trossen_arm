===================
Updates & Changelog
===================

This page documents major updates to the project, such as changes to dataset formats, codebase improvements, new features, and compatibility notes.

Trossen v1.0 Dataset Format
===========================

:date: 05-01-2025
:commit: `Interbotix/lerobot@8f72211 <https://github.com/Interbotix/lerobot/commit/8f7221114505e770f1f987b6cd909e0f4a323993>`_


We have introduced a new dataset format for the Trossen AI arms, which is now available in the :guilabel:`Interbotix\lerobot` repository.
This new datasets format allows for storing joints angles in ``radians``, which is the default for the Trossen AI arms.
We also have removed the scaling factor from the gripper joint angles, as it is no longer needed.
The  new dataset format allows us to have smaller and normalized joint angles, which is more efficient for training and inference.

The new dataset format is not compatible with the previous one, so you will need to convert your old datasets to the new format.
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
We add :guilabel:`trossen_subversion` to the dataset metadata to indicate that this dataset was created using ``Interbotix/lerobot`` 
This also allows us to run ``backward compatibility`` checks in the future.
The ``backward compatibility`` checks are run when you load a dataset using the ``Dataset`` class.
The checks will verify that the dataset is compatible with the current version of the code.
