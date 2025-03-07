==========================
LeRobot Installation Guide
==========================

Follow the `Trossen Robotics documentation <https://docs.trossenrobotics.com/trossen_arm/main/getting_started/hardware_setup.html>`_
for setting up the hardware.

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

      pip install torchvision==0.20.1
      conda install -y -c conda-forge ffmpeg
      pip uninstall -y opencv-python
      conda install -y -c conda-forge "opencv>=4.10.0"

Troubleshooting
===============

If you encounter the following error:

.. code-block:: bash

   ImportError: /xxx/xxx/xxx/envs/lerobot/lib/python3.10/site-packages/cv2/python-3.10/../../../.././libtiff.so.6: undefined symbol: jpeg12_write_raw_data, version LIBJPEG_8.0

Below are the known system-specific solutions:

System 76 Serval Workstation (`serw13`) & Dell Precision 7670
-------------------------------------------------------------

   .. code-block:: bash

      conda install pytorch==2.5.1=cpu_openblas_py310ha613aac_2 -y
      conda install torchvision==0.21.0 -y

HP Workstations
---------------

   .. code-block:: bash

      pip install torch==2.5.1+cu121 torchvision==0.20.1+cu121 torchaudio==2.5.1+cu121 --index-url https://download.pytorch.org/whl/cu121
