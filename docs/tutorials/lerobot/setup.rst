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

      cd ~/lerobot && pip install --no-binary=av -e ".[trossen_ai]"

   .. note::

    If you encounter build errors, you may need to install additional dependencies ``cmake``, ``build-essential``, and ``ffmpeg libs``.
    On Linux, run: 
    
    .. code-block::
      
      sudo apt-get install cmake build-essential python3-dev pkg-config libavformat-dev libavcodec-dev libavdevice-dev libavutil-dev libswscale-dev libswresample-dev libavfilter-dev pkg-config

    For other systems, see: `Compiling PyAV <https://pyav.org/docs/develop/overview/installation.html#bring-your-own-ffmpeg>`_

#. For Linux only (not Mac), install extra dependencies for recording datasets:

   .. code-block:: bash

      conda install -y -c conda-forge 'ffmpeg>=7.0'


.. note::
   
   Installing ``ffmpeg>=7.0`` using the above command usually provides ``ffmpeg 7.X`` compiled with the ``libsvtav1`` encoder.  
   If ``libsvtav1`` is **not** available on your system (you can verify by running ``ffmpeg -encoders``), you have two options:

   - **Any platform**: Install a specific version of FFmpeg with conda:
      
      .. code-block:: bash

         conda install ffmpeg=7.1.1 -c conda-forge

   - **Linux only**: Manually install FFmpeg build dependencies and compile FFmpeg from source with `libsvtav1` support:  
      Refer to the official guides below:
      
      - `FFmpeg build dependencies <https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu#GettheDependencies>`_  
      - `Compile FFmpeg with libsvtav1 <https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu#libsvtav1>`_

   After installation, confirm you're using the correct FFmpeg binary with:

   .. code-block:: bash

      which ffmpeg