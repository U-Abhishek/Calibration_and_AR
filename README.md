# Calibration_and_AR

#### Video Demo

Main Video Demo for this project: 

For more videos on this project [click here.]()

#### Team Members
- Aaron Pan
- Abhishek Uddaraju
  
#### Operating System Used: _Windows_
#### IDE: _VScode_
#### Time Travel days: 2


## Table of Contents
1. [Intoduction](#introduction)
2. [Installation](#installation)
3. [Usage](#usage)


## Introduction
This project introduces techniques of camera calibration and virtual object generation in OpenCV. The goal of camera calibration is to correct for any radial and tangential distortion in the image first before object generation. Calibration and calculating camera pose are both done using a chessboard and OpenCV function. Once completed, virtual objects can be generated and placed in the scene.

## Installation  

### 1. Cloning the project
```
$ git clone https://github.com/U-Abhishek/Calibration_and_AR.git
$ cd /project location
```

### 2. Install C++ Compiler

You need a C++ compiler installed on your system to build and run this project. If you don't have one installed, follow the instructions below:

- **Windows**: Install MinGW or Visual Studio with C++ development tools.
- **Mac**: Install Xcode command-line tools or GCC through Homebrew.
- **Linux**: Install GCC through your package manager (e.g., `sudo apt install g++` on Ubuntu).

### 3. Setup Visual Studio Code

Visual Studio Code is a lightweight but powerful source code editor that runs on your desktop. To set up Visual Studio Code for C++ development:

1. [Download](https://code.visualstudio.com/) and install Visual Studio Code.
2. Install the C/C++ extension by Microsoft from the Extensions view (`Ctrl+Shift+X`).
3. Open your project folder in Visual Studio Code.


### 4. OpenCV Installation (Pre-configured OpenCV for Python and C++ from GitHub releases)
1. Download opencv-4.5.1-vc14_vc15.exe from the latest release: 
   https://github.com/opencv/opencv/releases/tag/4.5.1
1. unpack to `C:\opencv`
1. edit environment variables. Win search "Environment variables"
1. append to Path the path to OpenCV: `C:\opencv\bin`
1. restart computer
1. open cmd
1. `echo %Path%` output should contain a new path

## Usage

### 1) Command line arguments required to run the **Calibration Executable**
```
$ cd directory_of_executable
$ .\calibration.exe
// Enter the .yaml filename in the terminal "Make sure you place the YAML file in {executable location}/../../data/calibration"
```

### 2) Keys required to calibrate the camera
| Key      | Description                          |
|----------|--------------------------------------|
| s        | Select the frame for calibration           |
| c        | Generates calibration matrix and saves it to YAML                |

### 3) Command line arguments required to run the ** AR Executable**
```
$ cd directory_of_executable
$ .\AugmentedReality.exe
// Enter the .yaml filename in the terminal "Make sure you place the YAML file in {executable location}/../../data/calibration"
```

### 4) Key presses for performing different tasks 

| Task Key | Task Description                      |
|----------|--------------------------------------|
| 5        | Draw corners of the board            |
| 6        | Draw a simple object                 |
| 7        | Detect Harris corners                |
| c        | Expand or contract a circle          |
| p        | Overlay painting onto a chessboard   |
| o        | Default: Do not overlay anything     |
| a        | Perform task specified by `key`      |

