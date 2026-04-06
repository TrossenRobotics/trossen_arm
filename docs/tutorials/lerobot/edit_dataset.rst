================
Editing Datasets
================

.. note::

    Ongoing support for the LeRobot fork is deprecated.
    We recommend using the :doc:`plugin-based LeRobot integration <../lerobot_plugin>` instead.

    However, this integration is necessary for users who wish to use the :doc:`openpi integration <../openpi>`.

After recording episodes, you may need to clean up or combine datasets before training.
The ``edit_dataset`` script provides three operations: **merge**, **delete**, and **validate**.
All commands can be run via CLI flags or a JSON config file.

.. code-block:: bash

    python lerobot/scripts/edit_dataset.py <command> [options]
    # or
    python lerobot/scripts/edit_dataset.py --config config.json

All dataset paths use the ``${HF_USER}/dataset_name`` format (e.g. ``TrossenRoboticsCommunity/trossen_ai_solo_dataset``),
the same convention used by ``--control.repo_id`` when recording.
By default, relative paths are resolved from ``~/.cache/huggingface/lerobot/``.
See :ref:`tutorials/lerobot/edit_dataset:Path Resolution` for details on customizing this with ``--root``.

Merging Datasets
================

Combine two datasets into one.
Episodes from dataset A come first, followed by episodes from dataset B.
All episode indices, frame indices, parquet files, video files, and metadata are renumbered to form a consistent output dataset.

The dataset name (``repo_id`` in ``info.json``) is derived automatically from the output directory path.

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --dataset-a ${HF_USER}/trossen_ai_stationary_dataset_a \
                --dataset-b ${HF_USER}/trossen_ai_stationary_dataset_b \
                --output-dir ${HF_USER}/trossen_ai_stationary_merged \
                --task "pick and place"

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --dataset-a ${HF_USER}/trossen_ai_mobile_dataset_a \
                --dataset-b ${HF_USER}/trossen_ai_mobile_dataset_b \
                --output-dir ${HF_USER}/trossen_ai_mobile_merged \
                --task "pick and place"

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --dataset-a ${HF_USER}/trossen_ai_solo_dataset_a \
                --dataset-b ${HF_USER}/trossen_ai_solo_dataset_b \
                --output-dir ${HF_USER}/trossen_ai_solo_merged \
                --task "pick and place"

Merging from Different Roots
----------------------------

If the two datasets were recorded with different ``--control.root`` paths (e.g. on different machines or storage locations), use ``--root-a`` and ``--root-b`` to specify where each dataset lives:

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --root-a /data/lab1 \
                --root-b /data/lab2 \
                --dataset-a ${HF_USER}/trossen_ai_stationary_dataset \
                --dataset-b ${HF_USER}/trossen_ai_stationary_dataset \
                --output-dir ${HF_USER}/trossen_ai_stationary_merged

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --root-a /data/lab1 \
                --root-b /data/lab2 \
                --dataset-a ${HF_USER}/trossen_ai_mobile_dataset \
                --dataset-b ${HF_USER}/trossen_ai_mobile_dataset \
                --output-dir ${HF_USER}/trossen_ai_mobile_merged

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py merge \
                --root-a /data/lab1 \
                --root-b /data/lab2 \
                --dataset-a ${HF_USER}/trossen_ai_solo_dataset \
                --dataset-b ${HF_USER}/trossen_ai_solo_dataset \
                --output-dir ${HF_USER}/trossen_ai_solo_merged

You can also use a JSON config file:

.. code-block:: json

    {
        "command": "merge",
        "dataset_a": "<HF_USER>/trossen_ai_solo_dataset_a",
        "dataset_b": "<HF_USER>/trossen_ai_solo_dataset_b",
        "output_dir": "<HF_USER>/trossen_ai_solo_merged",
        "task": "pick and place"
    }

.. code-block:: bash

    python lerobot/scripts/edit_dataset.py --config merge_config.json

Merge Parameters
----------------

- ``--dataset-a`` (str): **Required.** Path to the first (base) dataset.
- ``--dataset-b`` (str): **Required.** Path to the second dataset to append.
- ``--output-dir`` (str): **Required.** Output directory for the merged dataset.
- ``--root`` (str): Root directory for resolving all relative paths (default: ``~/.cache/huggingface/lerobot``).
- ``--root-a`` (str): Root directory for dataset A (overrides ``--root`` for A only).
- ``--root-b`` (str): Root directory for dataset B (overrides ``--root`` for B only).
- ``--task`` (str): Override all episodes with this single task string. If omitted, original per-episode tasks from both datasets are preserved and merged.
- ``--robot-type`` (str): Override ``robot_type`` in ``info.json`` (defaults to dataset A's value).
- ``--fps`` (int): Override ``fps`` in ``info.json`` (defaults to dataset A's value).

.. warning::

    Both datasets must have identical feature schemas (same feature names, dtypes, and shapes).
    The tool will raise an error if they differ.

.. warning::

    If ``--task`` is used, all original per-episode task labels are replaced.
    This cannot be recovered from the merged output.

What Happens During a Merge
---------------------------

#. Both source datasets are validated for structural integrity.
#. A compatibility report is printed comparing robot type, cameras, arm DOF, action dimensions, episode lengths, FPS, joint names, and codecs. Mismatches are warnings (the merge still proceeds); feature dtype/shape conflicts are hard errors.
#. Episodes from dataset A are copied first (keeping their original order), then episodes from dataset B. All episodes are renumbered contiguously starting from 0.
#. For each episode, the parquet file is rewritten with updated ``episode_index``, ``index``, ``frame_index``, ``task_index``, and video/image path columns. Video and image files are copied to the correct chunk directories.
#. Per-episode statistics (``episodes_stats.jsonl``) are recomputed, and aggregated statistics (``stats.json``) are derived from them.
#. The merged output is validated and a summary report is printed.

Source datasets are never modified. Only the output directory is written.

Deleting Episodes
=================

Remove specific episodes from a dataset.
Remaining episodes are renumbered contiguously, and all associated files (parquet, videos, images) and metadata are updated.

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            # Write to a new output directory (source is preserved)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_stationary_dataset \
                --episodes 2 5 7 \
                --output-dir ${HF_USER}/trossen_ai_stationary_trimmed

            # Edit in-place (source is overwritten)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_stationary_dataset \
                --episodes 2 5 7

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            # Write to a new output directory (source is preserved)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_mobile_dataset \
                --episodes 2 5 7 \
                --output-dir ${HF_USER}/trossen_ai_mobile_trimmed

            # Edit in-place (source is overwritten)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_mobile_dataset \
                --episodes 2 5 7

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            # Write to a new output directory (source is preserved)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_solo_dataset \
                --episodes 2 5 7 \
                --output-dir ${HF_USER}/trossen_ai_solo_trimmed

            # Edit in-place (source is overwritten)
            python lerobot/scripts/edit_dataset.py delete \
                --dataset-dir ${HF_USER}/trossen_ai_solo_dataset \
                --episodes 2 5 7

You can also use a JSON config file:

.. code-block:: json

    {
        "command": "delete",
        "dataset_dir": "<HF_USER>/trossen_ai_solo_dataset",
        "episodes": [1, 3, 5],
        "output_dir": "<HF_USER>/trossen_ai_solo_trimmed"
    }

.. code-block:: bash

    python lerobot/scripts/edit_dataset.py --config delete_config.json

Delete Parameters
-----------------

- ``--dataset-dir`` (str): **Required.** Path to the source dataset.
- ``--episodes`` (list[int]): **Required.** Episode indices to delete (space-separated on CLI, array in JSON).
- ``--output-dir`` (str): Output directory. If omitted, the dataset is edited in-place.
- ``--root`` (str): Root directory for resolving relative paths (default: ``~/.cache/huggingface/lerobot``).

.. warning::

    When ``--output-dir`` is omitted, the operation is **in-place**.
    The original dataset is overwritten and cannot be recovered.
    A backup is kept until validation passes -- if validation fails, the backup location is printed for recovery.

What Happens During a Delete
----------------------------

#. The source dataset is validated.
#. Specified episodes are removed. Remaining episodes are renumbered to 0, 1, 2, ... preserving their original order.
#. Tasks that are no longer referenced by any remaining episode are pruned from ``tasks.jsonl``.
#. Per-episode statistics are recomputed and aggregated statistics are rederived.
#. The output is validated and a summary report is printed.

Validating a Dataset
====================

Check a dataset's structural integrity against its own metadata.
Reports all errors and warnings without modifying any files.

.. tabs::

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py validate \
                --dataset-dir ${HF_USER}/trossen_ai_stationary_dataset

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py validate \
                --dataset-dir ${HF_USER}/trossen_ai_mobile_dataset

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            python lerobot/scripts/edit_dataset.py validate \
                --dataset-dir ${HF_USER}/trossen_ai_solo_dataset

Validate Parameters
-------------------

- ``--dataset-dir`` (str): **Required.** Path to the dataset to validate.
- ``--root`` (str): Root directory for resolving relative paths (default: ``~/.cache/huggingface/lerobot``).

The following checks are performed:

- Meta files exist (``info.json``, ``episodes.jsonl``, ``tasks.jsonl``)
- ``info.json`` totals match actual file/entry counts
- Episode indices are contiguous (0, 1, 2, ..., N-1)
- Parquet files exist for every episode with correct row counts and column values
- Video and image files exist for all episodes
- Task references are consistent between ``episodes.jsonl`` and ``tasks.jsonl``
- Statistics entries are present for all episodes with required fields

The command exits with code 0 on success, code 1 on failure.

Path Resolution
===============

All dataset paths can be absolute or relative. Relative paths are resolved against a root directory.

.. list-table::
   :header-rows: 1
   :widths: 15 30 55

   * - Flag
     - Applies to
     - Default
   * - ``--root``
     - All paths (inputs, output)
     - ``~/.cache/huggingface/lerobot`` (``HF_LEROBOT_HOME``)
   * - ``--root-a``
     - Dataset A only (merge)
     - Falls back to ``--root``
   * - ``--root-b``
     - Dataset B only (merge)
     - Falls back to ``--root``

For merge, ``--dataset-a`` resolves against ``--root-a`` if set, else ``--root``;
``--dataset-b`` resolves against ``--root-b`` if set, else ``--root``;
``--output-dir`` always resolves against ``--root``.
Absolute paths bypass root resolution entirely.

.. code-block:: bash

    # Default: all paths resolve under ~/.cache/huggingface/lerobot
    python lerobot/scripts/edit_dataset.py merge \
        --dataset-a ${HF_USER}/dataset_a \
        --dataset-b ${HF_USER}/dataset_b \
        --output-dir ${HF_USER}/merged
    # -> ~/.cache/huggingface/lerobot/${HF_USER}/dataset_a
    # -> ~/.cache/huggingface/lerobot/${HF_USER}/dataset_b
    # -> ~/.cache/huggingface/lerobot/${HF_USER}/merged

    # Per-dataset roots with a separate output root
    python lerobot/scripts/edit_dataset.py merge \
        --root /data/output \
        --root-a /data/lab1 \
        --root-b /data/lab2 \
        --dataset-a ${HF_USER}/dataset_a \
        --dataset-b ${HF_USER}/dataset_b \
        --output-dir ${HF_USER}/merged
    # -> /data/lab1/${HF_USER}/dataset_a
    # -> /data/lab2/${HF_USER}/dataset_b
    # -> /data/output/${HF_USER}/merged

JSON config supports ``root``, ``root_a``, and ``root_b`` the same way.
