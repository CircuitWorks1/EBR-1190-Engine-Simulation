# EBR-1190-Engine-Simulation
This is an engine sensor simulator designed for the EBR 1190rx/sx to diagnose a failed ECM. The project was started to assist with the reverse engineering of the CANbus system. The program focuses on simulating 2 main sensors. The crankshaft sensor(RPM) and the speed sensor. The crankshaft sensor is vital to the simulation, as the ECM will not acknowledge other input data, unless there is proper crankshaft data input to the ECM. The crankshaft sensor sits on the outside of a tone ring which is fixed to the stator magneto. The tone ring consist of 34 steps, then a timing gap the equivalent to 2 steps. To acomplish simulating this waveform, I took the frequency input by the user, then calculated the time piriod to execute both 34 wavecycles, and 36 wavecycles.
example:
rpmTime = 1 / rpmFreq * 1000000 * 34
and
rpmRestart = 1 / rpmFreq * 1000000 * 36

A non blocking delay was then created to loop the wave form until it was looped 34 times(the equivalent to rpmTime in microseconds).
