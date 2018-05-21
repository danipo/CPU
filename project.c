//// Latest Upload by: Daniel
//// 4-21 8:00 PM
// Changes: Cleaned up some of the current code, Fixed ALU functions
#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //perform ALU operation based on ALUControl
    //record result and zero
    switch(ALUControl){
        case 0b000:
            //add
            *ALUresult = A + B;

            /////// Debugging
            printf("result = %d + %d = %d\n", A, B, *ALUresult);
            break;

        case 0b001:
            //subtract
            *ALUresult = A - B;
            break;

        case 0b010:
            //slt, return 1 if true. 0 otherwise
            if ((int)A < (int)B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        case 0b011:
            //slt (unsigned)
            if (A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        case 0b100:
            //AND
            *ALUresult = A & B;
            break;

        case 0b101:
            //OR
            *ALUresult = A | B;
            break;

        case 0b110:
            //shift B left 16 bits
            *ALUresult = B << 16;
            break;

        case 0b111:
            //NOT A
            *ALUresult = ~A;
            break;

        *Zero = (!*ALUresult) ? 1:0;
    }

}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(PC%4 != 0)
        return 1;

    *instruction = Mem[PC >> 2];

    if(*instruction == 0)
        return 1;

    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //divides parts from instruction

    *op = (instruction & 0xFC000000) >> 26;

    //R-Type
    *r1 = (instruction & 0x3E00000) >> 21;
    *r2 = (instruction & 0x1F0000) >> 16;
    *r3 = (instruction & 0xF800) >> 11;
    *funct = (instruction & 0x3F);

    //I-Type variations
    *offset = (instruction & 0xFFFF);

    //J-Type variations
    *jsec = (instruction & 0x3FFFFFF);
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    //set control signals from op

    switch(op)
    {
    case 0b000000: //R-Type Instructions (add, sub, slt, sltu, AND, OR)
        controls->ALUOp = 0b111;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
        break;
    case 0b001000: //addi
        controls->ALUOp = 0b000;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b100011: //lw
        controls->ALUOp = 0b000;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b101011: //sw
        controls->ALUOp = 0b000;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 1;
        controls->RegDst = 0;
        controls->RegWrite = 0;
        break;
    case 0b000010: //jump
        controls->ALUOp = 0b000;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 0;
        break;
    case 0b000100: //beq
        controls->ALUOp = 0b001;
        controls->ALUSrc = 0;
        controls->Branch = 1;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 0;
        break;
    case 0b000101: //bne
        controls->ALUOp = 0b001;
        controls->ALUSrc = 0;
        controls->Branch = 1;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 0;
        break;
    case 0b001100: //andi
        controls->ALUOp = 0b100;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b001101: //ori
        controls->ALUOp = 0b101;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b001111: //lui
        controls->ALUOp = 0b000;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b001010: //slti
        controls->ALUOp = 0b010;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;
    case 0b001011: //sltiu
        controls->ALUOp = 0b011;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
        break;

    default:
        return 1;
    }

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value){


    if ((offset >> 15) == 1)
    {
        *extended_value = offset | 0xffff0000;
    }

    else
    {
        *extended_value = offset | 0x00000000;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // ALUSrc
    // asserted - second ALU operand is sign extended lower 16 bits of instruction
    // deasserted - second ALU operand comes from second register file output

    if (ALUSrc == 1)
        ALU(data1,extended_value,ALUOp,ALUresult, Zero);

    // check ALUOp
    // check for halt condition
    // check for don't care condition

    ////////// Debugging
    printf("ALUOp is %d\n", ALUOp);
    printf("Funct is %d\n", funct);
    printf("data1 = %d\n", data1);
    printf("data2 = %d\n", data2);
    /////////////

    if (ALUOp == 0b111)
    {
            // we know it is an r-type
            // check funct
            switch(funct)
            {
                case 0b100000:
                    // add
                    ALU(data1, data2, 0b000, ALUresult, Zero);
                    break;
                case 0b100010:
                    // sub
                    ALU(data1, data2, 0b001, ALUresult, Zero);
                    break;
                case 0b100100:
                    // and
                    ALU(data1, data2, 0b100, ALUresult, Zero);
                    break;
                case 0b100101:
                    // or
                    ALU(data1, data2, 0b101, ALUresult, Zero);
                    break;
                case 0b101010:
                    // slt
                    ALU(data1, data2, 0b010, ALUresult, Zero);
                    break;
                default:
                   return 1;
                   break;
            }
    }

    else if (ALUOp == 1)
        ALU(data1,data2,ALUOp,ALUresult,Zero);

    // return 1 if halt condition occurs
    // return 0 otherwise
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

    if(MemRead == 1)
    {
        if( ALUresult < 65536 && (ALUresult % 4) == 0)
        *memdata = Mem[ALUresult >> 2];

        else
        return 1;
    }


    if(MemWrite == 1)
    {
        if( ALUresult < 65536 && (ALUresult % 4) == 0)
        Mem[ALUresult >> 2] = data2;

        else
        return 1;
    }

return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == 1 && MemtoReg==1)
    {
        //data is coming from memdata
        // RegDst value decides the destination register (r2 or r3)
        if(RegDst == 0)
        {
            Reg[r2] = memdata;
        }
        else
        {
            Reg[r3] = memdata;
        }
    }

    if(RegWrite == 1 && MemtoReg==0)
    {
        //data is coming from ALU
        // RegDst value decides the destination register (r2 or r3)
         if(RegDst == 0)
        {
            Reg[r2] = ALUresult;
        }
        else
        {
            Reg[r3] = ALUresult;
        }

    }

return;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4;

    if(Jump == 1)
    {
    *PC = (*PC & 0xf0000000) | (jsec << 2);
    }
    if(Branch == 1 && Zero == 1)
    {
    *PC = *PC + (extended_value << 2);
    }
}
