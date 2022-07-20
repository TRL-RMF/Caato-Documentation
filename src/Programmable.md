# Programmable Logic Controller (PLC)

A programmable logic controller (PLC) was added for the second iteration of CAATO. Currently, we are using an Allen Bradley Micro820 . We intend for the PLC to satisfy 3 main objectives.

1. Safety Layer - In the event the x86 computing layer experiences failure, the PLC will take over to bring the robot to a safe state. Two critical areas of concern are motion and battery power. For the former, all motion commands are forwarded through the PLC to ensure no additional movements are given in the event of emergency. For the latter, the flow of power from the battery and Xnergy charger are controlled from the PLC.
2. Actuator Control - The PLC’s power, ruggedness and expandable I/O provide advantages over a basic microcontroller. We intend for the PLC to take over control of the trolley lift arm, which is currently operated by an Arduino Uno.
3. Machine Interface and Status Monitoring - The PLC will act as a central control hub, monitoring the status of electronics and the individual voltage terminals. This information will be displayed on an LCD touchscreen for quick troubleshooting and testing.

[Software](https://www.notion.so/Software-1491aca9b447436388bce194a45310fa)