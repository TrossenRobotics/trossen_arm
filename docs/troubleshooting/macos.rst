================
Running on macOS
================

The driver supports macOS on arm64 (see :ref:`getting_started/software_setup:Supported Platforms`), but several
parts of the surrounding toolchain — the Data Collection UI, the RealSense SDK, and LeRobot's video
decoding — assume Linux. This page collects the macOS-specific issues and their fixes so a new setup can
get to a working teleoperation and training loop without rediscovering each one.

.. contents::
    :local:
    :depth: 1

Host Network Setup
==================

The Arm Controller needs the host on the same subnet, which usually means giving a USB Ethernet adapter a
static address such as ``192.168.1.1``. On macOS, also setting a **Router** on that service can make it the
default route for all traffic, which silently breaks internet access on the host:

.. code-block:: bash

    route -n get default        # shows the USB Ethernet interface instead of Wi-Fi

The arm network has no gateway, so leave the router field empty. If macOS still prefers the adapter, lower
its priority so Wi-Fi ranks above it:

.. code-block:: bash

    networksetup -listnetworkserviceorder
    networksetup -ordernetworkservices "Wi-Fi" "USB 10/100/1000 LAN" ...

Verify afterwards that ``route -n get default`` points at your normal interface while the arm stays
reachable with ``ping 192.168.1.2``.

Driver and Firmware Version Match
=================================

``pip install trossen-arm`` installs the newest release, which may be ahead of the firmware on your Arm
Controller. The major and minor versions must match exactly or ``configure()`` raises:

.. code-block:: text

    LogicError: The major and minor versions of the driver and controller firmware must match.

Check the firmware first, then pin the driver to it:

.. code-block:: bash

    python3 demos/python/arm_discovery.py     # reports firmware per arm
    pip install "trossen-arm==1.9.3"          # match what discover reported

.. warning::

    If you manage the environment with ``uv``, a bare ``uv pip install`` pin is reverted by the next
    ``uv run``, which re-syncs the environment to the lockfile. Use ``uv add "trossen_arm==<version>"`` so the
    pin is recorded, or pass ``uv run --no-sync``.

Cameras: Use the OpenCV Interface
=================================

The RealSense SDK is unreliable on macOS. ``pyrealsense2-macosx`` enumerates devices but raises on the first
device handle access:

.. code-block:: text

    RuntimeError: failed to set power state

This is not a power or cabling problem — it reproduces after replugging and across USB ports. Use the
OpenCV interface instead, which works normally:

.. code-block:: yaml

    camera_interface: 'opencv'

Find the indices with ``lerobot-find-cameras opencv`` and confirm which index is which physical camera by
checking the saved frames in ``outputs/captured_images``.

.. note::

    AVFoundation camera indices are **not stable across replugging**. Moving a camera to a different USB port
    can change its index, and can also change which stream a RealSense device exposes first — an index may
    return the infrared stream rather than colour. Re-run ``lerobot-find-cameras`` and re-check the captured
    frames after any cable change.

Trossen AI Data Collection UI
=============================

The UI package itself is pure Python and installs on macOS, but its setup and one of its runtime paths are
Linux-only.

Post-install script
-------------------

``trossen_ai_data_collection_ui_post_install`` calls ``sudo apt-get`` and writes a ``.desktop`` entry, so it
fails on macOS. Perform its steps manually instead:

.. code-block:: bash

    brew install ffmpeg pkg-config
    git clone -b trossen-ai https://github.com/Interbotix/lerobot.git ~/.lerobot_trossen_ai_data_collection_ui
    cd ~/.lerobot_trossen_ai_data_collection_ui && pip install -e .
    pip install PySide6 pynput "trossen-arm==<your firmware version>"

Omit the ``--no-binary=av`` flag the script uses. It builds PyAV against system FFmpeg, and the prebuilt
wheel — which bundles its own FFmpeg — avoids a compile that fails against newer Homebrew FFmpeg releases.

Unavailable dependencies
------------------------

The ``[trossen_ai]`` extra pulls ``pyrealsense2`` and ``trossen-slate``, neither of which publishes a macOS
arm64 wheel. Install the base package without that extra. Two consequences:

-   ``pyrealsense2`` is imported inside functions that only run under ``camera_interface: 'intel_realsense'``,
    so it is harmless when using the OpenCV interface. If the UI needs the module importable, install
    ``pyrealsense2-macosx``, which provides the same module name.
-   ``trossen_slate`` is imported at **module scope** in
    ``lerobot/common/robot_devices/robots/trossen_ai_mobile.py``. Because the UI imports that module
    unconditionally, its absence blocks startup even for Solo and Stationary kits. Moving the import inside
    ``TrossenAIMobile.__init__`` resolves it, since only an actual SLATE base needs the driver.

Camera hardware reset
---------------------

``hardware_reset_cameras()`` calls ``rs.context().query_devices()`` regardless of the configured
``camera_interface``, and it runs automatically when a recording session or dry run starts. On macOS that
call **segfaults the application** rather than raising, so the UI dies at the start of every session even
when configured for OpenCV cameras. Gate the reset on the interface actually in use:

.. code-block:: python

    if camera_interface != "intel_realsense":
        return   # a hardware reset only applies to RealSense devices

Video Decoding: FFmpeg Version
==============================

``torchcodec`` — which LeRobot uses to read episode video — supports FFmpeg 4 through 7. Homebrew's default
``ffmpeg`` formula is newer, so loading a dataset fails with:

.. code-block:: text

    RuntimeError: Could not load libtorchcodec ...
    OSError: libavutil.59.dylib: cannot open shared object file

Install a supported FFmpeg alongside the current one and point the dynamic loader at it:

.. code-block:: bash

    brew install ffmpeg@7
    export DYLD_FALLBACK_LIBRARY_PATH=/opt/homebrew/opt/ffmpeg@7/lib

``ffmpeg@7`` is keg-only, so it does not shadow your existing installation. Add the export to your shell
profile — without it, any local dataset work (visualisation, replay, policy rollout) fails at the first
frame decode.

Not macOS-Specific, but Easy to Hit
===================================

These affect every platform and commonly surface when moving from data collection to training.

Install the training extras
---------------------------

``lerobot[smolvla]`` does not include PyAV or torchcodec, so datasets cannot be decoded:

.. code-block:: text

    ImportError: 'av' is required but not installed

Install ``lerobot[smolvla,training]``, which pulls in the ``dataset`` extra.

Datasets need a codebase version tag
------------------------------------

A dataset uploaded with ``HfApi.upload_folder`` rather than LeRobot's own ``push_to_hub`` is missing the git
tag LeRobot resolves against, and training fails with:

.. code-block:: text

    RevisionNotFoundError: Your dataset must be tagged with a codebase version.

Add the tag matching ``codebase_version`` in ``meta/info.json``:

.. code-block:: python

    from huggingface_hub import HfApi
    HfApi().create_tag("<user>/<dataset>", tag="v3.0", repo_type="dataset")

Camera names when finetuning SmolVLA
------------------------------------

``lerobot/smolvla_base`` was pretrained with cameras named ``camera1``/``camera2``/``camera3``, while Trossen
robot configurations use descriptive names such as ``cam_main`` and ``cam_wrist``. Training aborts at policy
construction without a mapping:

.. code-block:: bash

    --rename_map='{"observation.images.cam_main": "observation.images.camera1",
                   "observation.images.cam_wrist": "observation.images.camera2"}'

The same mapping is required again at rollout. ACT does not need it, because it builds its configuration from
the dataset and already expects the dataset's camera names.

Other training flags
--------------------

-   ``--policy.push_to_hub=false`` is required unless a model repository id is also supplied, or configuration
    validation refuses to start.
-   Rollout datasets must be named with a ``rollout_`` prefix, which keeps policy-generated episodes from being
    mistaken for human demonstrations.

Memory on Apple silicon
-----------------------

Unified memory is shared with the GPU, so a large policy can exhaust it and fall back to swapping. On a 32 GB
machine, finetuning a 450M-parameter policy at batch size 8 with two camera streams drove roughly 13 GB of
swap, and step time degraded from about 2 s to over 60 s while the process sat mostly idle waiting on paging.
If step time grows steadily while the reported ``updt_s`` stays normal, check ``sysctl vm.swapusage`` and
reduce ``--batch_size``.
