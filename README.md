# RTLS

## Table of Contents
- 1. [Introduction](#introduction)
- 2. [Building](#building)
  - 1. [Windows](#windows)
  - 2. [Linux / Raspberry Pi](#linux--raspberry-pi)
- 3. [Testing](#testing)
- 4. [Project Structure](#project-structure)
  - 1. [src](#src)
  - 2. [tests](#tests)
  - 3. [thirdparty](#thirdparty)
  - 4. [web](#web)

## Introduction
A Real-Time Locating System, made for the final year project at Swinburne University of Technology. The project uses the Decawave MDEK1001 kit, alongside custom-built algorithms, to locate a tag in real-time. 

## Building

### Windows
First, ensure that you have the following prerequisites:
 - [CMake](https://cmake.org/download/)
 - [Visual Studio 2022](https://visualstudio.microsoft.com/vs/whatsnew/)

Next, download/clone the repository and open the `rtls` directory in the command line.
This can be done by running the following commands:
```
> git clone https://github.com/SEP-46/rtls
> cd rtls
```

Now the project must be created using CMake:
```
> md build && cd build
> cmake ..
```

This should create a `build/` folder with a file named `rtls.sln` inside it.
The final step is to open `rtls.sln` with Visual Studio 2022 and run `Build > Build Solution`.

The executables will be placed in the `build/bin/Debug/` folder.

### Linux / Raspberry Pi
First, ensure that you have the following prerequisites:
 - [CMake](https://cmake.org/download/) (can also be installed from package manager)
 - Make / GCC (`sudo apt install build-essential`)

Next, download/clone the repository and open the `rtls` directory.
This can be done by running the following commands:
```
$ git clone https://github.com/SEP-46/rtls
$ cd rtls
```

Now the project must be created using CMake:
```
$ mkdir build && cd build
$ cmake ..
```

Finally, the project can be built by using `make`:
```
$ make
```

The executables will be placed in the `build/bin/` folder.

## Testing
The project contains various unit tests that can be used to ensure the validity of the system. To run these tests, simply follow the [build instructions](#building), then run the `rtls-tests` program.

## Project Structure
The project is split into various folders, which each make up a separate module in the system.

### src
This folder contains the core RTLS program, written in C++. Examples of things this program does are:
 - Communicating with the tag
 - Outputting data to multiple interfaces (UART/analog/websockets)
 - Create a web server with a REST API
 - Implements multiple trilateration algorithms
The code here will be compiled into the `rtls-main` program.

| File                               | Description                                                                                              |
| ---------------------------------- | -------------------------------------------------------------------------------------------------------- |
| aabb.h                             | Utility class for an Axis-Aligned Bounding Box (AABB)                                                    |
| vector.h                           | Utility class for a 3D vector                                                                            |
| matrix.h                           | Utility class for matrices of any dimensions                                                             |
| mathlib.h                          | Utility file with miscellaneous math functions / types                                                   |
| util.h/.cpp                        | Utility file with miscellaneous functions / types                                                        |
| optparse.h                         | Utility class for parsing command line options                                                           |
| config.h/.cpp                      | Code for loading/storing saved configuration of the RTLS (see config.json)                               |
| settings.h/.cpp                    | Code for reading all of the command line options                                                         |
| lmh_serial.h/.cpp                  | Low-level Module Handshake code for DWM API, handles sending/receiving raw data with tag                 |
| main.cpp                           | Entry point for the RTLS program, handles initialisation of top-level systems and loops                  |
| rtls.h/.cpp                        | Top level class for the RTLS system, handles the entire flow of getting distances and running algorithms |
| communication_interface.h          | Interface for any output interface, which outputs the position of the tag                                |
| analog_interface.h/.cpp            | The code for outputting tag position to analog output w/ Raspberry Pi DAC HAT                            |
| uart_interface.h/.cpp              | The code for outputting tag position to UART interface                                                   |
| websocket_interface.h/.cpp         | The code for outputting tag position to WebSocket interface                                              |
| restapi.h/.cpp                     | Contains web server code, with all of the REST API endpoints                                             |
| trilaterationsolver.h              | Common interface for all the trilateraion solvers / algorithms                                           |
| trilaterationsolver_basic.h/.cpp   | Implementation for the basic trilateration algorithm                                                     |
| trilaterationsolver_ekf.h/.cpp     | Implementation for the EKF trilateration algorithm                                                       |
| trilaterationsolver_builtin.h/.cpp | Implementation for the built-in trilateration algorithm                                                  |
| trilaterationsolver_builtin.h/.cpp | Implementation for the built-in trilateration algorithm                                                  |
| velocity_output.h/.cpp             | Code that converts location data into a velocity output                                                  |
| node.h                             | Contains definitions that are common to all "nodes" (tags/anchors)                                       |
| uwb_anchor.h/.cpp                  | Utility class for keeping track of connected anchors and their measurements                              |
| tag.h                              | Interface for a "tag" device that provides distance measurement data                                     |
| uwb_tag.h/.cpp                     | Implemntation of the tag interface that uses the actual UWB device to get measurements                   |
| mock_tag.h/.cpp                    | Implemntation of the tag interface that simulates a device moving in a circle                            |
| mock_tag.h/.cpp                    | Implemntation of the tag interface that simulates a device moving in a circle                            |

### tests
The `tests/` folder contains all of the unit tests, used to ensure the validity of the RTLS program. We use the `googletest` framework to implement unit testing in C++.
The code here will be compiled into the `rtls-tests` program.

### thirdparty
The `thirdparty/` folder contains all of the third-party dependencies that the RTLS program relies on.

| Library       | Description         |
| ------------- | ------------------- |
| [IXWebSocket](https://github.com/machinezone/IXWebSocket) | A lightweight C++ websockets library, used for implementing the websockets output interface |
| [abelectronics](https://github.com/abelectronicsuk/ABElectronics_CPP_Libraries) | Used for the ADCDACPi library, allowing us to interface with the [ADC-DAC Pi Zero](https://www.abelectronics.co.uk/p/74/adc-dac-pi-zero-raspberry-pi-adc-and-dac-expansion-board) HAT device |
| [cpp-httplib](https://github.com/yhirose/cpp-httplib) | A header-only C++ HTTP library, used for implementing the web server & REST API functionality |
| [dwm](https://www.qorvo.com/products/p/MDEK1001#documents) | A slightly modified version of the dwm_api files provided by Qorvo as part of the `DWM1001C Software and Documentation Pack`, allowing for communication with the tag device |
| [googletest](https://github.com/google/googletest) | The Google C++ testing framework, used for our unit tests |
| [nlohmann](https://github.com/nlohmann/json) | A C++ header-only library for JSON functionality, used for the REST API and websockets output interface |
| [serial](https://github.com/wjwwood/serial) | A cross-platform library allowing for serial port communication, used for the UART output interface and for communicating with the tag device |



### web
This folder contains all files and programs for the running of the web client.
Languages include HTML, CSS, and JavaScript.

| File                               | Description                                                          |
| ---------------------------------- | -------------------------------------------------------------------- |
| index.html                         | Layout for the main page                                             |
| css/styles.css                     | Styling for the main page                                            |
| scripts\output_data.js             | Contains code to initiate data interfaces and direct IO operations   |
