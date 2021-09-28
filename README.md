## General Info
A 2-person project themed as the detection aspect of a missile defence platform, where nearby objects would be detected and displayed on a screen. This project is comprised of two main portions: a spinning portion at the top of the platform (spun so a sensor can detect objects in a 360° sweep) and a stationary portion at the base of the platform where objects are shown on a screen. Each portion had its own microcontroller (C2000 Piccolo board with a Real Time Operating System (RTOS)), and this repo contains code for the stationary portion as it was the section I worked on.

As a brief overview the project, a LIDAR sensor is spun around to detect objects in a 360° sweep. The data measured was intended to be sent wirelessly to the stationary C2000 board, however we could not get the RX/TX module we purchased for the project to work so we transmitted data manually via a wired connection during the project demo. While the LIDAR sensor measured object distance, an encoder on the motor that spins the platform lets us know which direction that data was measured at. To get a sense of the 0° position, an IR LED/Diode was used (LED on the spinning portion, diode on the stationary portion). As the platform spins, the LED lines up with the diode once every 360°. When they line up, the voltage in a comparator circuit attached to the diode would change, which would then be detected by the C2000 board, allowing us to define where 0° is.

The stationary portion of the project reads the encoder and combines it with the LIDAR data to get a coordinate point. This point is then sent to a screen so the user can see detected objects and their relative distance from the platform, with data being refreshed each sweep of the sensor. The screen interface was themed like a stereotypical "sonar sweep" in naval ships. 

The RTOS component of this project makes it so different threads are run and interrupt each other according to priority. For the stationary portion, the following threads are used (with top being highest priority):
Thread | Description | Pend/Post Operations
------ | ----------- | --------------------
HWI_0: encoder_Fxn (Interrupt # = 35) | Set GPIO7 (CPU measurement pin) low. Triggers each motor encoder pulse. Increments the angle counter and rolls it over if it goes over 360 degrees (roll-over condition is for if IR system that trips HWI_1 does not work correctly). | Post(clear_Sem) for each angle (to clear any point drawn at that angle during the previous sweep).
HWI_1: IR_Fxn (Interrupt # = 36) | Set GPIO7 (CPU measurement pin) low. Reset angle of motor to zero when motor makes ~360° sweep (trips when IR LED allows IR diode to increase voltage of input pin, creating a pulse) | None.
SWI_0: polar_to_cart_Fxn (Priority 0) | Set GPIO7 (CPU measurement pin) low. Trigger when new distance data is inputted (either manually from IDLE, or from a communication interrupt when new data enters the buffer). Converts polar coordinates (distance and angle) into Cartesian coordinates and stores it in a buffer. Selects correct TSK to draw or redraw the point on the screen. | Post(draw_Sem) after the coordinate conversion, and if no data was written to that angle during the same sweep. Post(redraw_Sem)) after the coordinate conversion, and if prior data was written to that angle during the same sweep (i.e. data comes in fast enough that a second measurement was given for the same angle, therefore update point position).
TSK_0: clear_point_Fxn (Priority 1) | Set GPIO7 (CPU measurement pin) low. Removes a point from the screen that was recorded during last sweep. Updates the “clear_index” which is used to access the data you want to clear from the screen (“clear_index” follows “array_index”, so if HWI_0 runs consecutively, this TSK will continue clearing points until it catches back up to “array_index”). | Pend(clear_Sem). Pend(lock_Sem) before TSK’s operation (esp. SPI communication) to prevent other TSK’s from accessing the SPI bus before this TSK is done with it. Post(lock_Sem) after it is done. (lock_Sem = 1 initially).
TSK_1: draw_point_Fxn (Priority 1) | Set GPIO7 (CPU measurement pin) low. Adds a point to the screen for the first distance measurement of the current motor angle. Pend(draw_Sem). Pend(lock_Sem) before TSK’s operation (esp. SPI communication) to prevent other TSK’s from accessing the SPI bus before this TSK is done with it. Post(lock_Sem) after it is done. (lock_Sem = 1 initially).
TSK_2: redraw_point_Fxn (Priority 1) | Set GPIO7 (CPU measurement pin) low. For the same motor angle of the current sweep, removes the previous distance measurement point from the screen before drawing the new distance measurement point (i.e. if the LIDAR is stationary or measurements are fast enough that > 1 come in for the same angle in the current sweep, update point on the screen). | Pend(redraw_Sem). Pend(lock_Sem) before TSK’s operation (esp. SPI communication) to prevent other TSK’s from accessing the SPI bus before this TSK is done with it. Post(lock_Sem) after it is done. (lock_Sem = 1 initially).
IDLE | Set GPIO7 (CPU measurement pin) high. Wait for user to input sample distance data manually (through the “Expressions” watch list in Debugging mode) for testing purposes. Waits for SCI buffer to be filled with distance data from LIDAR from “spinning module”. | Post(SWI_0) when test data is manually entered through “Expressions” watch list in Debug mode, OR if there is data received in the SCI buffer.

## Technologies
C was utilized for programming in this project. The library for communicating with the 128x128 pixel SPI screen was adapted from a repo made by Matevž Marš (https://github.com/matevzmars/ST7735R).

## Content
Important project files:

 Top level of project folder: 
├── DeviceInit_18Nov2018.c				# initialization information for C2000 board
├── F2802x_GlobalVariableDefs.c			# global variables for various registers for board
├── main_file.c							# file that gets run during program execution. Contains logic for handling input data and where to write to screen
├── spi_screen.c						# SPI screen library modified to work with this project
├── spi_screen.h						# header file for SPI screen library (modified to work with this project)
└── README.md
