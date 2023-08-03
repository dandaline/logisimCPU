# logisimCPU

A nother school project of a logisim 2.7.1 CPU simulation
- CISC Harward architecture, no pipeline
- assambler for this CPU
- testcode for ALU

## Setup
- MUST BE logisim 2.7.1 or lower - logisim evolution won't open the .circ file correctly
- download [logisim](http://www.cburch.com/logisim/download.html)
- [logisim for arch distros](https://aur.archlinux.org/packages/logisim)
- [logisim as snap package](https://snapcraft.io/logisim-snapcraft)
### Run existing number guessing game (guess.txt) - german but little text 
- run ./logisimCPU/CPU.circ with logisim
- Menu->Simulate->TickFrquency
- CTRL+K to run CTRL+R to reset
### Run custom programs
- compile asm.cpp with clang or g++
- write assambler code to a text file (ASSAMBLER is describes in ./asm/README.txt)
- run asm and input your file name -> this will generate a rom.txt, which you can insert into the rom of the cpu
- in logisim main, right click on ROM (memory at the top), load image...
- select rom.txt
- run as discussed before
