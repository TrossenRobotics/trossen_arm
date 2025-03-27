==========================
LeRobot Installation Guide
==========================

Install LeRobot
===============

On your computer:

#. Install Miniconda:

   We use Miniconda to manage Python environments and dependencies.
   To install Miniconda, download the installer for your Linux operating system from the `Miniconda Installation Guide <https://www.anaconda.com/docs/getting-started/miniconda/install#quickstart-install-instructions>`_.

#. Create and activate a fresh Conda environment for LeRobot:

   .. code-block:: bash

      conda create -y -n lerobot python=3.10 && conda activate lerobot

#. Clone LeRobot:

   .. code-block:: bash

      git clone -b trossen-ai https://github.com/Interbotix/lerobot.git ~/lerobot

#. Install LeRobot with dependencies for the Trossen AI arms (`trossen-arm`) and cameras (`intelrealsense`):

   .. code-block:: bash

      cd ~/lerobot && pip install -e ".[trossen_ai]"

#. For Linux only (not Mac), install extra dependencies for recording datasets:

   .. code-block:: bash

      conda install -y -c conda-forge ffmpeg
      pip uninstall -y opencv-python
      conda install -y -c conda-forge "opencv>=4.10.0"

