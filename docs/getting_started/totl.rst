================
TOTL Workstation
================

Logging In
==========

The default username for the TOTL Workstation is ``trossen``.
The default password for the TOTL Workstation is ``trossen``.
For some older models, the default password may be ``trossen ai``, ``trossen-ai``, or ``TOTL``.
You can change this password after logging in for the first time.

.. note::

  It is highly recommended to change the default password to ensure the security of your workstation.

Installed Software
==================

The TOTL Workstation comes pre-installed with all the necessary software to get started with the Trossen AI robots and kits.
This includes:

- python3-dev
- python3-venv
- python3-pip
- miniconda3
- cmake
- git
- build-essential
- ros-build-essential
- ros-jazzy-desktop
- trossen_arm_ros
- librealsense2-utils
- librealsense2-dkms
- librealsense2-dev
- teensy_loader_cli
- trossen_ai_data_collection_ui
- and more...

Redeployment Instructions
=========================

If your Linux installation becomes unusable or support has provided you with a base system image, you can restore the entire system using the steps below.

.. important::

  This process will **completely erase** the selected internal drive and replace it with a fresh copy of the base Linux image.
  **All existing data on that drive will be permanently deleted.**

1. What You Need
----------------

Before you begin, make sure you have:

- The target PC you want to restore
- A USB boot drive (32 GB or larger) for the Ubuntu bootable USB
- A USB or external image drive (64 GB or larger) containing the system images provided `at this link`_:

    - ``system.img.xz`` (main system image)
    - ``efi.img.xz`` (EFI boot partition image)

- A Linux machine to create the bootable USB (Ubuntu recommended)
- Internet access is not required during redeployment
- No additional software is required beyond what is included with Ubuntu.

.. _`at this link`: https://drive.google.com/drive/folders/1BWVLTzBFHfuoIqlyIQhhCdUSSNOLms9X?usp=drive_link

2. Create the Ubuntu Bootable USB (on a Linux system)
------------------------------------------------------

#.  Download the Ubuntu Desktop ISO from: https://ubuntu.com/download
#.  Insert your USB boot drive (32 GB or larger drive).
#.  Open **Startup Disk Creator**:

    - Press **Super** / **Windows** key
    - Search for **Startup Disk Creator**

#.  In Startup Disk Creator:

    - Select the downloaded Ubuntu Desktop ISO
    - Select the correct USB device
    - Click **Make Startup Disk**

#.  Wait for completion, then safely remove the USB.

3. Boot the Target PC From the Ubuntu USB
------------------------------------------

#.  Insert:

    - The Ubuntu bootable USB
    - The USB/external image drive containing the image files

#.  Power on the PC and open the boot menu (Repeatedly press **DEL**)
#.  Select the Ubuntu USB, then proceed to save and exit
#.  When prompted, choose: **"Try Ubuntu"** (Do not install Ubuntu)

You are now running a live environment.

4.  Wipe and Recreate the Internal Drive (GNOME Disks)
------------------------------------------------------

#.  Open **Disks**:

    - Press **Super**
    - Search for **Disks**
    - Open **Disks** (GNOME Disks)

#.  In the left sidebar, select the internal SSD (Confirm by size — e.g., 2 TB NVMe)
#.  Click the **⋮** (three dots) menu → **Format Disk**
#.  Use these settings:

    - **Erase:** Don't overwrite existing data (Fast)
    - **Partitioning:** Compatible with modern systems and hard disks >2TB (GPT)

#.  Click **Format**

This removes all partitions and prepares the drive.

5. Create Required Partitions (GParted)
----------------------------------------

With the same drive selected, you are going to create 2 new partitions on the 2TB SSD (by clicking the plus button on the partition details screen)

**EFI Partition**

- Size: 2 GB
- Filesystem: FAT32
- Flags:

  - boot
  - esp

**Root (System) Partition**

- Size: Remaining space
- Filesystem: ext4

Click **Apply** to commit changes.

6. Restore the EFI Image (GNOME Disks)
---------------------------------------

#.  Select the EFI partition (small FAT32 partition)
#.  Click the settings button → **Restore Partition Image** (make sure this isn't the entire disk, just on the partition)
#.  Select: ``efi.img.xz``
#.  Confirm restore and wait for completion

7. Restore the System Image (GNOME Disks)
------------------------------------------

#.  Select the ext4 root partition (larger partition)
#.  Click the settings button → **Restore Partition Image** (make sure this isn't the entire disk, just on the partition)
#.  Select: ``system.img.xz``
#.  Confirm restore and wait (This may take several minutes)

8. Expand the Root Filesystem to Fill the Drive (GParted)
---------------------------------------------------------

#.  Open **GParted**
#.  Select the root (ext4) partition
#.  Click **Resize/Move**
#.  Drag the partition to use all remaining space
#.  Click **Apply**

This ensures the system uses the full SSD capacity.

9. Repair Bootloader (Terminal)
--------------------------------

#.  Open **Terminal**
#.  Run this code to mount and bind directories:

.. code-block:: bash

  sudo mount /dev/nvme0n1p2 /mnt
  sudo mkdir -p /mnt/boot/efi
  sudo mount /dev/nvme0n1p1 /mnt/boot/efi
  sudo mount --bind /dev /mnt/dev
  sudo mount --bind /proc /mnt/proc
  sudo mount --bind /sys /mnt/sys

10. Reboot Into the Restored System
------------------------------------

#.  Shut down the PC
#.  Remove:

    - Ubuntu USB
    - Image USB

#.  Power the PC back on

The system should now boot normally into the restored Linux environment.
