#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <fcntl.h>

#include <cpuCore.hpp> 

cpuCore myCpu;

word *ram;

void printStatus(cpuCore cpu, word mem[4096]){
	///prints registers, flags, and the instruction that will be executed
	std::cout.fill('0');
	std::cout << "Pc: " << std::setw(4) << std::oct << cpu.Pc;
	std::cout << "    Z: " << cpu.Z << std::endl;
	std::cout << "Ac: " << std::setw(4)<<std::oct<<cpu.Ac;
	std::cout << "    C: " << cpu.C << std::endl;
	std::cout << "Br: " << std::setw(4) << std::oct << cpu.Br;
	std::cout << "    H: " << cpu.H << std::endl;
	std::cout << "Ix: " << std::setw(4) << std::oct << cpu.Ix << std::endl;
	std::cout << "Sp: " << std::setw(4) << std::oct << cpu.Sp;
	std::cout << " Inst: " << std::setw(4) << std::oct << mem[cpu.Pc] << std::endl;
	std::cout << "---------------------"<<std::endl;
}

int main(int argc, char *argv[]) {
	bool showRegs = false;
	bool showRegsFinl = false;	
	bool showA = false;
	bool showAFinl = false;
	bool intOut = false;
	int c;
	while ((c = getopt (argc, argv, "raoRAh")) != -1){
		switch (c) {
		case 'r':
			showRegsFinl = true;
			break;
		case 'a':
			showAFinl = true;
			break;
		case 'o':
			intOut = true;
			break;
		case 'R':
			showRegs = true;
			break;
		case 'A':
			showA = true;
			break;
		case 'h':
			{
				std::ifstream hf("help.txt");
				if (hf.is_open()){
		        std::cout << hf.rdbuf() << std::endl;
					hf.close();
				}
				else{
					std::cout << "help.txt not found!" << std::endl;
				}
			}
			return 0;
			break;
		}
		
	}

	if((argc < 2) || (argv[argc-1][0] == '-')){
		std::cout << "No file given" << std::endl;
		return 1;
	}

	int fd = open(argv[argc-1], O_RDONLY); //get input file
	ram = static_cast<word*> (mmap( //memory map the input file
		NULL, 4096*2,
		PROT_READ | PROT_WRITE, MAP_PRIVATE,
		fd, 0
	));
	
	close(fd); //close the file

	myCpu.reset(); //resets the CPU
	
	while (!myCpu.H) { //execute instructions while the CPU isn't halted
		if ((ram[04010] == 1) && intOut) std::cout << 
			uint32_t(ram[04005] << 12) +
			uint32_t(ram[04004])<<
			std::endl;

		if (showRegs) printStatus(myCpu, ram);
		else if (showA) std::cout << std::oct << myCpu.Ac << std::endl;
		myCpu.execute(ram); //executes the next instruction
	}

	if (showRegsFinl) {
		std::cout << "Final CPU State:" << std::endl;
		printStatus(myCpu, ram); //prints the CPUs state after the halt
	}
	else if (showAFinl) {
		std::cout << "Final Ac Value:" << std::endl;
		std::cout << std::oct << myCpu.Ac << std::endl;
	}

	munmap(ram, 4096*2); //un-memory-map the file
	return 0;
}
