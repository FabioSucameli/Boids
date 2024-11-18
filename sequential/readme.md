# Boids Simulation

This project simulates the **Boids Algorithm** using **C++** and **SFML** for graphical rendering. The algorithm implementation is **sequential**, meaning all boid updates are processed one after the other without parallelism. The current CMake configuration is designed specifically for **Windows systems**. Below are the setup, usage, and build instructions.

---

## Prerequisites

To build and run the project, ensure you have the following:
- **C++ Compiler**: Compatible with C++11 or higher.
- **CMake**: Installed and added to your system's PATH.
- **SFML**: Download the Windows version from the [official SFML website](https://www.sfml-dev.org/download.php).

---

## Setup Instructions

1. **Download SFML**:
   - Download SFML for Windows.

2. **Place SFML in the Project Directory**:
   - Extract the SFML archive.
   - Copy the entire `SFML` folder into the root of the project directory.
   - Ensure the folder includes the `include` and `lib` directories.

3. **Directory Structure**:
   After placing SFML, the project directory should look like this:
   ```plaintext
   Boids/
   ├── CMakeLists.txt
   ├── main.cpp
   ├── boids.cpp
   ├── boids.h
   ├── SFML/
       ├── include/
       ├── lib/
       └── lib/cmake/SFML/
