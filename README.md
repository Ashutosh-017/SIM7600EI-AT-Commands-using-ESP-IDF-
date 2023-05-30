# SIM7600EI-AT-Commands-using-ESP-IDF-
Sending AT commands to SIMCOM7600EI for getting location using ESP IDF

UART Communication Example
=========================

This repository contains an example code for UART communication with SIM7600EI module using an ESP32 microcontroller. The code demonstrates how to initialize the UART interface, send AT commands, and parse responses from the module.

Getting Started
---------------

These instructions will help you get the code up and running on your development board.

### Prerequisites

- ESP32 development board
- ESP-IDF development framework


Usage
-----

The `app_main` function in `main.c` serves as the entry point of the program. It demonstrates the following steps:

1. UART initialization: `uart_init()` initializes the UART interface with the specified configuration.
2. Module Initialization: `module_init()` sends an AT command to the module to check if it responds with "OK" to confirm successful initialization.
3. SIM Registration Check: `check_SIM_reg()` sends an AT command to the module to check the SIM registration status with the network.
4. Mode Change: `changing_mode()` sends an AT command to the module to change the mode of operation.
5. GPS Enablement: `Enable_GPS()` sends an AT command to the module to enable GPS functionality.
6. Location Information Retrieval: `location_INFO()` sends an AT command to the module to retrieve location information.

Contributing
------------

Contributions are welcome! If you have any improvements or bug fixes, please submit a pull request.
.

