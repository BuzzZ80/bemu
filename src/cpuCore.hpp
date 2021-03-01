#ifndef CPUCORE_H
#define CPUCORE_H

#include <cstdint>

typedef bool flag; //for 1 bit flags like Z and C
typedef uint8_t byte; //occasionally used for small values
typedef uint16_t word; //for 12 bit registers like Ac and Br

class cpuCore{
    /// W-4096/0 core
    public:
    word Ac; //Accumulator
    word Br; //General purpose register
    word Ix; //Index register
    word Sp; //Stack pointer
    word Pc; //Program Counter

    word Tp; //Temp register
    word Op; //Operand register
    word Mp; //Memory address pointer

    flag H;//Halt flag
    flag C;//Carry flag
    flag Z;  //Zero flag

    private:
    byte cnd; //Condition (most significant)
    byte op1; //Operation code 1
    byte op2; //Operation code 2
    byte rs1; //Register 1

    bool condCheck(byte condWord);

    const bool condLut[32] = {
        //takes condition word and flags, then returns 1 if the instruction should be executed
        1,1,1,1,0,1,0,1,
        0,0,1,1,0,0,0,1,
        1,1,1,1,1,0,1,0,
        1,1,0,0,1,0,0,0
    };

    void mov(byte mOp1, byte mRs2, byte mRs1, word mem[4096]);

    void alu(byte mOp1, byte mOp2, byte mRs1, word mem[4096]);

    void msc(byte mOp1, byte mOp2, byte mRs1, word mem[4096]);

    public:
    void reset();

    void rsLoad(byte rs, word data, word mem[4096]);

    void rsFetch(byte rs, word mem[4096]);

    void execute(word mem[4096]);
};

#endif // CPUCORE_H
