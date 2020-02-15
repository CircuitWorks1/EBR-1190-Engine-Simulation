# EBR-1190-Engine-Simulation
This is an engine sensor simulator, designed for the EBR 1190rx/sx, to diagnose a failed ECM. The project was started to assist with the reverse engineering of the CANbus system. The program focuses on simulating 2 main sensors. The crankshaft sensor(RPM) and the speed sensor. The signals are simulated from a frequency value, input into the serial monitor. Any value less than 1000 will be used to simulate the speed sensor, and any value greater than 1000 will be used to simulate the crankshaft sensor. The crankshaft sensor is vital to the simulation, as the ECM will not acknowledge other input data, unless there is proper crankshaft data input to the ECM. The crankshaft sensor sits on the outside of a tone ring which is fixed to the stator magneto. The tone ring consist of 34 steps, then a timing gap the equivalent to 2 steps. To acomplish simulating this waveform, I took the frequency, input by the user, begin a PWM tone at the given frequency, then calculated the time piriod to execute both 34 wavecycles, and 36 wavecycles.

example:

rpmTime = 1 / rpmFreq * 1000000 * 34

and

rpmRestart = 1 / rpmFreq * 1000000 * 36

A non-blocking delay was then created to loop the waveform until it was looped 34 times(the equivalent to rpmTime in microseconds). Then came the timing gap. If the non-blocking delay counter was between rpmTime and rpmRestart, we would loop with no PWM tone. Once the non-blocking delay counter had reached a value > rpmRestart, the counter is restarted allowing the 34 wavecycles to start again.

Now we focus on generating the wave used to simulate the speed sensor output. The speed sensor simulation is a bit more basic because there is no timing gap. To simulate the speed sensor waveform we will take the given frequency, that was input by the user and rapidly turn on and off one of the pins. This will generate a squarewave at the given frequency.

First we find out the period to complete 1 wavecycle in microseconds

speedTime = 1 / speedFreq * 1000000

We can then devide that by 2, to find out how long the pin should remain in each state. We control this action with another non-blocking delay. 

If delay timer < speedTime/2 

pin off

If delay timer > speedTime/2

pin on

If delay timer > speedTime

restart delay timer
