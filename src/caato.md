# caato

# Introduction*

The CAATO is a small, nimble Autonomous Ground Vehicle (AGV) developed off of the LOCUST platform with more powerful and accurate motors and a patented (TBC) trolley docking mechanism. This platform will be the first-ever robot to be deployed under the Robotic middleware framework RMF 2.0 program and is developed in joint collaboration with Changi Airport.

Checkout the CAATO repository for more information: [caato github](https://github.com/TRL-RMF/caato2)

## Operational WorkFlow

![download (1).png](./img/download_(1).png)

## Autonomy

### Introduction

The autonomy of the CAATO implementation consists of two main parts: navigation stack and computer vision intelligence.

A single board computer (SBC) and sagemaker is used to achieve the two parts. Currently both are running ROS Noetic with Ubuntu 20.04 setup.

### Architecture

Major components of the overall system is shown below in the figure.

- NUC
    - Perform mapping, localization, path planninig
- SageMaker
- Roboteq controller
    - Perform BLDC motors control (closed loop speed control, i.e. `cmd_vel`)
- Pixhawk IMU
    - Perform data fusion with odometry
    - GPS location if outdoor
- Hokuyo LiDAR
    - To perform 270 degree laser scan by combining one front-left LIDAR and one one right-rear LIDAR
- Ouster LiDAR (OS1-32)
    - To perform 360 degree laser scan needed to carry out mapping and localization.