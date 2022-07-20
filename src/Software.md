# Software

## General Operability & Overview

- For Caato2 to carry out its tasks using ROS, certain commands are needed to be executed
- Various packages are available to be implemented onto Caato2
    - Git Clone from [GitHub - CAATO](https://github.com/TRL-RMF/caato2)
        

## Packages

1. caato2_bringup
    - 
2. caato2_navigation
    - 
3. caato2_gazebo
4. AMCL3d
    - **Adaptive Monte-Carlo Localization in 3D** is a particle filter that allows you to estimate where a robot is within a 3D environment without using GPS, for example in indoor environments.

## Launch Files

1. Caato2_bringup
    - Launches certain relevant nodes which are needed for basic operation and functionality
        - Roboteq Differential Driver - Control the motors
        - Lasers - Activates the lasers used by Caato2
        - Teleop - Enable manual control of Caato2
        - DS4 Driver - Enable control of Caato2 using a remote joystick
2. Caato2_navigation
    - Launches relevant stacks needed for navigation
        - AMCL
        - DWA
        - EBAND (Preferred)
        - TEB
3. (Cartographer) (2D & 3D)
    - 
4. Xnergy charger
    - roslaunch xnergy_charger_rcu xnergy_charger_modbus.launch
        - Launches the modbus communication between the charger and the RCU
    - rosservice call /xnergy_charger_rcu/start_charging
        - Initiates the charging process

## RViz

- RViz is a 3D visualization software tool for robots, sensors, and algorithms. It enables you to see the robot’s perception of its world (real or simulated). The purpose of RViz is to enable you to visualize the state of a robot. It uses sensor data to try to create an accurate depiction of what is going on in the robot’s environment

## Gazebo

- Gazebo is a 3D robot simulator. Its objective is to simulate a robot, giving you a close substitute to how your robot would behave in a real-world physical environment. It can compute the impact of forces (such as gravity)