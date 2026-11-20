The controller base has three M12 connectors on its rear panel.
From left to right, they are Ethernet, GPIO, and 24 V power.

.. image:: images/controller_base/controller_base_panel.svg
  :align: center
  :alt: Rear panel of the controller base with the screen, power switch, status LED, and the Ethernet, GPIO, and 24 V power connectors
  :width: 560px

The base also has the arm's power rocker switch, a status LED, and a touchscreen that can display and manage the arm's state and configuration.

.. list-table::
  :align: center
  :header-rows: 1

  * - Connector
    - Type on the base
    - Mating connector
  * - Ethernet
    - M12 X-coded, 8-pin, female
    - M12 X-coded, 8-pin, male, to RJ45 (IEC 61076-2-109).
      Standard Cat6A M12 to RJ45 Ethernet cables fit.
  * - GPIO
    - M12 A-coded, 6-pin, female
    - M12 A-coded, 6-pin, male, with contacts in positions 1, 2, 3, 4, 6, and 7 (IEC 61076-2-101).
  * - 24 V power
    - M12 L-coded, 4-pin, male
    - M12 L-coded, 4-pin, female (IEC 61076-2-111).

All drawings below show the front of each connector as seen on the base.

GPIO
----

.. image:: images/controller_base/gpio_connector.svg
  :align: center
  :alt: Front view of the GPIO M12 A-coded 6-pin female connector with numbered pins
  :width: 240px

.. list-table::
  :align: center
  :header-rows: 1

  * - Pin
    - Signal
  * - 1
    - Sync In
  * - 2
    - GND
  * - 3
    - Not used
  * - 4
    - GND
  * - 5
    - Not used
  * - 6
    - E-Stop In
  * - 7
    - E-Stop Out

24 V Power
----------

.. image:: images/controller_base/power_connector.svg
  :align: center
  :alt: Front view of the 24 V power M12 L-coded 4-pin male connector with numbered pins
  :width: 240px

.. list-table::
  :align: center
  :header-rows: 1

  * - Pin
    - Function
  * - 1
    - +24 V
  * - 2
    - GND
  * - 3
    - GND
  * - 4
    - +24 V
