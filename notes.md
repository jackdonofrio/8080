### Interrupts
From Emu101:

> Interrupts are designed to let processors handle time-sensitive tasks like I/O. The processor can be running a program, and when the interrupt pin on the CPU gets triggered, the CPU will stop executing the current program and handle the interrupt

#### Handling interrupts

1. > The interrupt source (external to CPU) will set the CPU's interrupt pin
2. > When the CPU acknowledges the interrupt, the interrupt source can put any opcode on the bus and the CPU will see it (most of the time RST)
3. > The CPU executes that instruction, often an RST. It pushes the current PC on the stack.
4. > The code the RST calls handles the interrupt