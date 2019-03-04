#Cedric Nicolas and Qua'on Thomas
#Operating Systems - Project 2
#Memory Simulation

# Reqired Files: main.c LinkedList.c LinkedList.h <trace-file>
1. Ensure the necessary trace files are in the directory
2. Compile the project using the "make" command
3. Run File: "memsim <tracefile> <nframes> <lru|fifo|vms> <debug|quiet>"

Sample Output Below:

[thomasq@c4lab02 operating-systems-project-2]$ ./memsim swim.trace 128 fifo quiet
total memory frames: 128
events in trace: 1000000
total disk reads: 1674
total disk writes: 3213
[thomasq@c4lab02 operating-systems-project-2]$ ./memsim swim.trace 128 lru quiet
total memory frames: 128
events in trace: 1000000
total disk reads: 1674
total disk writes: 2597
[thomasq@c4lab02 operating-systems-project-2]$ ./memsim swim.trace 128 vms quiet
total memory frames: 128
events in trace: 1000000
total disk reads: 1674
total disk writes: 2896

