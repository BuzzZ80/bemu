#include <cpuCore.hpp>

bool cpuCore::condCheck(byte condWord){
    ///Checks a 3 bit word and core flags to see if an instruction should be executed
    condWord = condWord << 2 | int(Z) << 1 | C;
    return condLut[condWord] && !H;
}

void cpuCore::mov(byte mOp1, byte mRs2, byte mRs1, word mem[4096]){
    ///handles executing move instructions
    byte selectMode = (mOp1 << 1) | (mRs2 >> 2);
    switch(selectMode){
        case 0:
            rsLoad(mRs2, Op, mem);
            break;
        case 1:
            rsLoad(mRs2, Op, mem);
            break;
        case 2:
            Mp = Op;
            rsLoad(mRs2 & 3, mem[Mp], mem);
            break;
        case 3:
            Mp = Op;
            rsLoad(mRs2 & 3, mem[Mp + Ix], mem);
            break;
        case 4:
            Mp = Op;
            rsFetch(mRs2 & 3, mem);
            mem[Mp] = Op & 0xFFF;
            break;
        case 5:
            Mp = Op;
            rsFetch(mRs2 & 3, mem);
            mem[Mp + Ix] = Op & 0xFFF;
        case 6:
            break;
        case 7:
            break;
    }
}

void cpuCore::alu(byte mOp1, byte mOp2, byte mRs1, word mem[4096]){
    ///handles executing the majority of instructions that use the ALU
    byte selectMode = (mOp1 << 3 | mOp2) & 0xF;
    switch(selectMode){
        case 0:
            Ac = Ac + Op;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 1:
            Ac = Ac + Op + C;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 2:
            Ac = Ac - Op;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 3:
            Ac = Ac - Op - C;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 4:
            Ac = Op - Ac;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 5:
            Ac = Op - Ac - C;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 6:
            Ac = (-1*Ac) - Op - 1;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 7:
            Ac = (-1*Ac) - Op - C - 1;
            C = (Ac & 0x1000) == 0x1000;
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 8:
            Ac = ~( Ac &  Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 9:
            Ac =  ( Ac &  Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 10:
            Ac = ~( Ac & ~Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 11:
            Ac =  ( Ac & ~Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 12:
            Ac = ~(~Ac &  Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 13:
            Ac =  (~Ac &  Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 14:
            Ac = ~(~Ac & ~Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
        case 15:
            Ac =  (~Ac & ~Op);
            Ac = Ac & 0xFFF;
            Z = Ac == 0;
            break;
    }
}

void cpuCore::msc(byte mOp1, byte mOp2, byte mRs1, word mem[4096]){
    ///handles the rest of the instructions
    byte selectMode = (mOp1 << 3 | mOp2) & 0xF;
    switch(selectMode){
        case 0:
            Pc = Op-1;
            break;
        case 1:
            H = 1;
            break;
        case 2:
            Sp = (Sp - 1) & 0xFFF;
            mem[Sp] = Pc;
            Pc = Op-1;
            break;
        case 3:
            Pc = mem[Sp];
            Sp = (Sp + 1) & 0xFFF;
            break;
        case 4:
            Op--;
            C = (Op & 0x1000) == 0x1000;
            Op = Op & 0xFFF;
            Z = Op == 0;
            if ((mRs1 > 3) && (mRs1 < 7)) Pc = (Pc - 1) & 0xFFF;
            rsLoad(mRs1, Op, mem);
            break;
        case 5:
            Op++;
            C = (Op & 0x1000) == 0x1000;
            Op = Op & 0xFFF;
            Z = Op == 0;
            if ((mRs1 > 3) && (mRs1 < 7)) Pc = (Pc - 1) & 0xFFF;
            rsLoad(mRs1, Op, mem);
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            Tp = Ac - Op;
            C = (Tp & 0x1000) == 0x1000;
            Tp = Tp & 0xFFF;
            Z = Tp == 0;
            break;
        case 9:
            Tp = Op - Ac;
            C = (Tp & 0x1000) == 0x1000;
            Tp = Tp & 0xFFF;
            Z = Tp == 0;
            break;
        case 10:
            Ac =  (Ac ^ Op) & 0xFFF;
            break;
        case 11:
            Ac = ~(Ac ^ Op) & 0xFFF;
            break;
        case 12:
            C = 0;
            break;
        case 13:
            Z = 0;
            break;
        case 14:
            C = 1;
            break;
        case 15:
            Z = 1;
            break;
    }
}

void cpuCore::reset() {
    ///Resets registers and flags
    Ac = Br = Ix = Sp = Pc = Tp = Op = Mp = 0;
    H = C = Z = 0;
}

void cpuCore::rsLoad(byte rs, word data, word mem[4096]){
    ///Loads a core's register
    data = data & 0xFFF;
    rs = rs & 7;
    switch(rs){
        case 0:
            Ac = data;
            break;
        case 1:
            Br = data;
            break;
        case 2:
            Ix = data;
            break;
        case 3:
            Sp = data;
            break;
        case 4:
            Mp = mem[Pc];
            mem[Mp] = data;
            break;
        case 5:
            Pc = (Pc + 1) & 0xFFF;
            Mp = mem[Pc];
            mem[Mp] = data;
            break;
        case 6:
            Pc = (Pc + 1) & 0xFFF;
            Mp = mem[Pc];
            mem[(Mp + Ix) & 0xFFF] = data;
            break;
        case 7:
            if(condCheck(cnd)){
                Sp = (Sp - 1) & 0xFFF;
            }
            mem[Sp] = data;
            break;
    }
}

void cpuCore::rsFetch(byte rs, word mem[4096]){
    ///Reads a register and loads it into Op
    rs = rs & 7;
    switch(rs){
        case 0:
            Op = Ac;
            break;
        case 1:
            Op = Br;
            break;
        case 2:
            Op = Ix;
            break;
        case 3:
            Op = Sp;
            break;
        case 4:
            Pc = (Pc + 1) & 0xFFF;
            Op = mem[Pc];
            break;
        case 5:
            Pc = (Pc + 1) & 0xFFF;
            Mp = mem[Pc];
            Op = mem[Mp];
            break;
        case 6:
            Pc = (Pc + 1) & 0xFFF;
            Mp = mem[Pc];
            Op = mem[(Mp + Ix) & 0xFFF];
            break;
        case 7:
            if(condCheck(cnd)){
                Sp = (Sp + 1) & 0xFFF;
            }
            Op = mem[Sp];
            break;
    }
}

void cpuCore::execute(word mem[4096]) {
    ///Executes a 12 bit instruction at mem[Pc]
    word inst = mem[Pc];
    cnd = (inst >> 9) & 7;
    op1 = (inst >> 6) & 7;
    op2 = (inst >> 3) & 7;
    rs1 =  inst & 7;

    rsFetch(rs1, mem);

    if(condCheck(cnd)){
        switch(op1 >> 1){
            case 0:
                mov(op1, op2, rs1, mem);
                break;
            case 1:
                mov(op1, op2, rs1, mem);
                break;
            case 2:
                alu(op1, op2, rs1, mem);
                break;
            case 3:
                msc(op1, op2, rs1, mem);
                break;
        }
    }
    else if((op1 < 4) & (op2 > 3) & (op2 < 7)){
        Pc = (Pc + 1) & 0xFFF;
    }
    if(!H){
        Pc = (Pc + 1) & 0xFFF;
    }
}

