# CAATO Arm

<aside>
💡 Create a new page and select `Product Spec` ****from the list of template options to automatically generate the format below.

</aside>

# Hardware

## Photomicro Sensor

We used 2 pairs of photomicro sensors, one to detect if the arm is lifted to the top, and one to detect if the arm is at the bottom. 

Each pair consists of an emitter and a detector. When the arm is moved into the top or bottom position, it will block the laser beam coming from the respective emitter, and its corresponding detector will detect a decrease in optical energy. An output signal will then be sent to the arduino microcontroller. 

There are 4 wires connected to each sensor. 

- **Brown:** live
- **Black:** output
- **Blue:** ground
- **White:** not in use

## Breakbeam Sensor

2 pairs of breakbeam sensors were used to detect the docking status of the trolley. They work in a very similar fashion to the photomicro sensors mentioned in the previous paragraph. 

If the trolley is properly engaged by the arm, the laser beam from the emitter will be blocked and cannot reach the detector. The sensors will then output a “FALSE” value. 

We placed one pair of breakbeam sensors on each side of the arm. In order for the trolley to be considered successfully docked, the breakbeam sensors on both sides must return a “FALSE” value. If the laser beam from only one, or neither, pair of breakbeam sensors is blocked, the trolley is considered incorrectly docked, and the arm will be lowered back to the bottom position for the robot to re-attempt the docking action.

There are 3 wires connected to each detector (but only 2 for each transmitter, which lacks a signal wire).

- **Black:** ground
- **Red:** live
- **White:** signal (see “Arduino” section below for wiring information)

## Arduino

The arduino is used to check the output from the sensors and provide input to the dc motor driver.

In our current implementation of CAATO2, the wiring of the various signal pins is as follows:

- **4:** Top photomicro sensors
- **6:** Bottom photmicro sensors
- **7:** Breakbeam sensor 1
- **12:** Breakbeam sensor 2

## DC Motor Driver

The motor driver is used to check inputs from the arduino and provide output to the linear motor. We give a 24V input to the dc motor driver. We power the arduino through the 5v regulator from the dc motor driver.

## Stripboard

We used the stripboard to connect the sensors to the arduino.

After seeing all the codes, all the pinouts will make much more sense.

---

# Software

check out the repository for caato arm at https://github.com/TRL-RMF/bb_ros