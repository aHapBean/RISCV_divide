
#ifndef BUFFER_STAGES_HPP
#define BUFFEr_StAGES_HPP
#include<bits/stdc++.h>

#define funct3B 0b00000000000000000111000000000000   //define funct 3
//0b00000000'00000000'01110000'00000000

using u32 = unsigned int;
using u8  = unsigned char;

enum OPflag {
    LUI,
    AUIPC,      //add uppper imm to PC
    JAL,        //jump and link
    JALR,       //jump and link reg
    BEQ,        //branch equal  

    BNE,        
    BLT,
    BGE,
    BLTU,
    BGEU,
    
    LB,
    LH,
    LW,
    LBU,
    LHU,

    SB,
    SH,
    SW,
    ADDI,
    SLTI,

    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,

    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,

    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,

    OR,
    AND
};

extern u8 mem[500000];
extern u32 reg[32];
extern u32 PC;//������������ 
namespace STAGE{

    struct IF_ID_buffer {
        u32 code,PC;
    }IF_ID;

    struct ID_EX_buffer {   //ID give the value to it
        u32 rd,rs1,rs2;     //���
        u32 PC;                 
        u32 regd,reg1,reg2,imm,shamt; //������
        OPflag opflag;      //OPflag
        ID_EX_buffer(){rs1 = rs2 = rd = imm = 0;}
    }ID_EX;

    struct EX_MEM_buffer {
        u32 esc_flag;
        u32 rd,rs1,rs2;     //TO delete !     
        u32 regd,PC,imm,reg2;
        u32 ld_dest,st_dest,ld_flag,st_flag;//���ǣ����Ƿ����ڴ��д����
        OPflag opflag;
    }EX_MEM;

    struct MEM_WB_buffer{
        OPflag opflag;
        u32 regd,rd,PC;
        u32 esc_flag;
    }MEM_WB;

void printOP(OPflag flag);
void printREG(u32 rd);
void printID_EX_Buffer(STAGE::ID_EX_buffer &t);

void IF(){
    //in the mem the info is stored in the form of byte  |u8 !
    u32 code = 0b0;
	IF_ID.code = 0b0;
    for(int i = 0;i < 4; ++i){
        code |= (mem[PC + 3 - i] & 0b11111111);//�����ڲ���ʵ���Ƕ�������ʽ�ļ򵥸�ֵ��16���������ڴ�Ҳ��0 1 ��ʽ����
        if(i != 3)code <<= 8;       //     ���������ĴΣ���Ϊ��һ�β����ƣ��� 
    }//���
    PC += 4;
    IF_ID.PC = PC;
    IF_ID.code = code;
}


    /*Instuction_Decode*/
void ID(){
	u32 funct3 = 0b0,funct7 = 0b0,code = IF_ID.code;
    u32 opcode = (code & 0b1111111);
    OPflag opflag;

    //give value to the opflag
    switch (opcode) {
        case 0b0110111:
            opflag = LUI;break;
        case 0b0010111:
            opflag = AUIPC;break;
        case 0b1101111:
            opflag = JAL;break;
        case 0b1100111://it has a funct3 �ô�����������������
			opflag = JALR;break;
        case 0b1100011:
            funct3 = (code & (funct3B));// funct3
            funct3 >>= 12;//?
            switch (funct3) {
                case 0b000:
                    opflag = BEQ;break;
                case 0b001:
                    opflag = BNE;break;
                case 0b100:
                    opflag = BLT;break;
                case 0b101:
                    opflag = BGE;break;
                case 0b110:
                    opflag = BLTU;break;
                case 0b111:
                    opflag = BGEU;break;
            }
            break;
        case 0b0000011:
            funct3 = (code & funct3B);// funct3
            funct3 >>= 12;
            switch (funct3) {
                case 0b000:
                    opflag = LB;break;
                case 0b001:
                    opflag = LH;break;
                case 0b010:
                    opflag = LW;break;
                case 0b100:
                    opflag = LBU;break;
                case 0b101:
                    opflag = LHU;break;
            }
            break;
        case 0b0100011:
            funct3 = (code & funct3B);// funct3
            funct3 >>= 12;
            switch (funct3) {
                case 0b000:
                    opflag = SB;break;
                case 0b001:
                    opflag = SH;break;
                case 0b010:
                    opflag = SW;break;
            }
            break;
        case 0b0010011:
            funct3 = (code & funct3B);// funct3
            funct3 >>= 12;
            switch (funct3) {
                case 0b000:
                    opflag = ADDI;break;
                case 0b010:
                    opflag = SLTI;break;
                case 0b011:
                    opflag = SLTIU;break;
                case 0b100:
                    opflag = XORI;break;
                case 0b110:
                    opflag = ORI;break;
                case 0b111:
                    opflag = ANDI;break;
                case 0b001:
                    opflag = SLLI;break;//it has funct7
                case 0b101:
                    u32 funct7 = (code & 0b11111110000000000000000000000000);//0b11111110'00000000'00000000'00000000
                    funct7 >>= 25;
                    switch (funct7) {
                        case 0b0000000:
                            opflag = SRLI;break;
                        case 0b0100000:
                            opflag = SRAI;break;
                    }
                    break;
            }
            break;
        case 0b0110011:
            funct3 = (code & funct3B);// funct3
            funct3 >>= 12;
            switch(funct3){
                case 0b000:
                    funct7 = (code & 0b11111110000000000000000000000000);
                        funct7 >>= 25;
                        switch (funct7) {
                            case 0b0000000:
                                opflag = ADD;break;
                            case 0b0100000:
                                opflag = SUB;break;
                        }
                        break;
                case 0b001:
                    opflag = SLL;break;
                case 0b010:
                    opflag = SLT;break;
                case 0b011:
                    opflag = SLTU;break;
                case 0b100:
                    opflag = XOR;break;
                case 0b101:
                    funct7 = (code & 0b11111110000000000000000000000000);
                        funct7 >>= 25;
                        switch (funct7) {
                            case 0b0000000:
                                opflag = SRL;break;
                            case 0b0100000:
                                opflag = SRA;break;
                        }
                        break;
                case 0b110:
                    opflag = OR;break;
                case 0b111:
                    opflag = AND;break;
            }
            break;
    }
    
    printOP(opflag);

    u32 rd = 0b0,rs1 = 0b0,rs2 = 0b0,imm = 0b0,flag = 0b0,shamt = 0b0;

    //���ֽ���
    switch (opflag) {

//U
        case LUI:case AUIPC:
            imm = (code & 0b11111111111111111111000000000000);		//0b11111111'11111111'11110000'00000000
            // ��չ
            rd  = (code & 0b00000000000000000000111110000000);		//0b00000000'00000000'00001111'10000000
            rd >>= 7;
            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;

//UJ
        case JAL:
        	char *tp;
            u32 imm20,imm10_1,imm11,imm19_12;
            imm20    = (code & 0b10000000000000000000000000000000);	//0b10000000'00000000'00000000'00000000
			imm20   >>= (31 - 20);
			//printf("imm_binary: %s\n\n",itoa(imm20,tp,2));
            
            imm10_1  = (code & 0b01111111111000000000000000000000);	//0b01111111'11100000'00000000'00000000
			imm10_1 >>= (30 - 10);
			//printf("imm_binary: %s\n\n",itoa(imm10_1,tp,2));

            imm11    = (code & 0b00000000000100000000000000000000);	//0b00000000'00010000'00000000'00000000
            
			imm11 >>= (20 - 11);
			//printf("imm_binary: %s\n\n",itoa(imm11,tp,2));

            imm19_12 = (code & 0b00000000000011111111000000000000);	//0b00000000'00001111'11110000'00000000
			//printf("imm_binary: %s\n\n",itoa(imm19_12,tp,2));
			
            rd       = (code & 0b00000000000000000000111110000000);	//0b00000000'00000000'00001111'10000000
            rd >>= 7;
            
            imm |= imm20;imm |= imm10_1;imm |= imm11;imm |= imm19_12;

            flag = (code & 0b10000000000000000000000000000000);		//0b10000000'00000000'00000000'00000000
            if(flag != 0b0){//������չ
                imm |= (0b11111111111100000000000000000000);		//0b11111111'11110000'00000000'00000000
            }
            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;

//SB
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
            rs1 = (code & 0b00000000000011111000000000000000);		//0b00000000'00001111'10000000'00000000
            rs1 >>= 15;
            rs2 = (code & 0b00000001111100000000000000000000);		//0b00000001'11110000'00000000'00000000
            rs2 >>= 20;

            u32 imm12,imm10_5,imm4_1;//imm11;
            imm12   = (code & 0b10000000000000000000000000000000);	//0b10000000'00000000'00000000'00000000
            imm12 >>= (31 - 12);//31 -> 12

            imm10_5 = (code & 0b01111110000000000000000000000000);	//0b01111110'00000000'00000000'00000000
            imm10_5 >>= (30 - 10);
            
            imm4_1  = (code & 0b00000000000000000000111100000000);	//0b00000000'00000000'00001111'00000000
            imm4_1 >>= (8 - 1);

            imm11   = (code & 0b00000000000000000000000010000000);	//0b00000000'00000000'00000000'10000000
            imm11 <<= (11 - 7);

            imm = 0b0;
            imm |= imm4_1;imm |= imm10_5;imm |= imm11;imm |= imm12;
            flag = (code & 0b10000000000000000000000000000000);		//0b10000000'00000000'00000000'00000000
            if(flag != 0){//��12λ��ʼ������չ
                imm |= (0b11111111111111111110000000000000);		//0b11111111'11111111'11100000'00000000
            }
            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;

//I
        case JALR:case LB:case LH:case LW:case LBU:case LHU:case ADDI:
        case SLTI:case SLTIU:case XORI:case ORI:case ANDI: 
            imm  = (code & 0b11111111111100000000000000000000);		//0b11111111'11110000'00000000'00000000
            flag = (code & 0b10000000000000000000000000000000);	//0b10000000'00000000'00000000'00000000
            imm >>= 20;//???change 6.23
            if(flag != 0b0){//��20λ����λ��չ1
            //change Here
                imm |= (0b11111111111111111111000000000000);		//0b11111111'11111111'11110000'00000000
            }
            rd  = (code & 0b00000000000000000000111110000000);		//0b00000000'00000000'00001111'10000000
            rd >>= 7;//

            rs1 = (code & 0b00000000000011111000000000000000);		//0b00000000'00001111'10000000'00000000
            rs1 >>= 15;

            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;

//S
        case SB:case SH:case SW://TODO
            u32 t1,t2;
            t1 = (code & 0b11111110000000000000000000000000);		//0b11111110'00000000'00000000'00000000
            t2 = (code & 0b00000000000000000000111110000000);		//0b00000000'00000000'00001111'10000000
            t2 >>= 7;
            t1 >>= (25 - 5);

            t1 |= t2;
            imm = t1;

            flag = (code & 0b10000000000000000000000000000000);	//0b10000000'00000000'00000000'00000000
            if(flag != 0b0){//������չ
                imm |= (0b11111111111111111111000000000000);		//0b11111111'11111111'11110000'00000000
            }//��չ20λ����������չ19λ��

            rs1 = (code & 0b00000000000011111000000000000000);		//0b00000000'00001111'10000000'00000000
            rs1 >>= 15;

            rs2 = (code & 0b00000001111100000000000000000000);		//0b00000001'11110000'00000000'00000000
            rs2 >>= 20;
            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;

//R
        case SLLI:case SRLI:case SRAI:case ADD:case SUB:case SLL:case SLT:
        case SLTU:case XOR:case SRL:case SRA:case OR:case AND:
            rs1 = (code & 0b00000000000011111000000000000000);		//0b00000000'00001111'10000000'00000000
            rs1 >>= 15;

            rs2 = (code & 0b00000001111100000000000000000000);		//0b00000001'11110000'00000000'00000000
            rs2 >>= 20;

            if(opflag == SLLI || opflag == SRLI || opflag == SRAI){
                shamt = rs2;                                        //ע�⿴shamt�ɷ���rs2����
            }
            rd  = (code & 0b00000000000000000000111110000000);		//0b00000000'00000000'00001111'10000000
            rd >>= 7;
            //std::cout<<std::endl<<"imm:"<<imm<<std::endl;
            break;
    }

    //��ȷ�Լ���
    //����ط�Ӧ���и�shamt�Ŷԣ�������

    ID_EX.opflag = opflag;
    ID_EX.imm = imm;
    ID_EX.rd = rd;
    ID_EX.rs1 = rs1;
    ID_EX.rs2 = rs2;    //Ϊ��data hazard��׼��
    ID_EX.opflag = opflag;
    ID_EX.shamt = shamt;

    ID_EX.regd = reg[rd];
    ID_EX.reg1 = reg[rs1];
    ID_EX.reg2 = reg[rs2];
    
    ID_EX.PC = IF_ID.PC;
    printID_EX_Buffer(ID_EX);
}

    /*Execute*/
void EX(){  //��ȫ��PC���в���
    //ID_EX_buffer
    OPflag opflag = ID_EX.opflag;
    u32 rd = ID_EX.rd,
        rs1 = ID_EX.rs1,
        rs2 = ID_EX.rs2,
        shamt = ID_EX.shamt,
        imm = ID_EX.imm,
        regd = ID_EX.regd,
        reg1 = ID_EX.reg1,
        reg2 = ID_EX.reg2;
    u32 ld_dest = 0b0,st_dest = 0b0,ld_flag = 0,st_flag = 0;
    EX_MEM.esc_flag = 0;

    switch (opflag) {
        case LUI:       
            //fdb
            regd = imm;break;
        case AUIPC:      
            //fdb
            regd = (PC - 4) + imm;//  change ! PC -= 4;PC += imm;break;
            break;          //change !
        case JAL:       
            //fdb
            PC -= 4;regd = PC + 4;PC += imm;break;          //��һ����ַ���ӵ�regd�У����㾲̬Ԥ���Ѿ���Ĭ��+4 ����YC ! !
        case JALR:      
            //fdb
            PC -= 4;regd = PC + 4;PC = reg1 + imm;          //���PC��ȫ��PC!
			break;            //*
        case BEQ:
            //fdb
            if((reg1) == (reg2))PC = PC - 4 + imm;break;    //*
        case BNE:
            //fdb
            if((reg1) != (reg2))PC = PC - 4 + imm;break;    //*
        case BLT:
            //fdb
            if((int)reg1 < (int)reg2)PC = PC - 4 + imm;break;       //*
        case BGE:
            //fdb
            if((int)reg1 >= (int)reg2)PC = PC - 4 + imm;break;      //*
        case BLTU:
            //fdb
            if(reg1 < reg2)PC = PC - 4 + imm;break;         //*
        case BGEU:
            //fdb
            if(reg1 >= reg2)PC = PC - 4 + imm;break;        //*change the PC

        case LB:case LH:case LW:case LBU:case LHU:
            //fdb
            ld_dest = 0b0;ld_dest += (u32)(imm);ld_dest += reg1;    //���imm��Ҫ������
            ld_flag = 1;
            break;

        case SB:case SH:case SW:
            //fdb
            st_dest = reg1 + imm;
            st_flag = 1;
            break;

        case ADDI:
            //load imm to rd !
            //fdb
            if(imm == 255 && rd == 10){
                EX_MEM.esc_flag = 1;break;
            }
            regd = reg1 + imm;
            break;

        case SLTI:      //set less than imm
            //fdb
            if((int)reg1 < (int)imm)regd = 1;
            else regd = 0;
            break;

        case SLTIU:     //set less than imm unsigned
            //fdb
            if(reg1 < imm)regd = 1;
            else regd = 0;
            break;

        case XORI:
            //fdb
            regd = reg1 ^ imm;
            break;
        case ORI:
            //fdb
            regd = reg1 | imm;
            break;
        case ANDI:
            //fdb
            regd = reg1 & imm;
            break;
            
        case SLLI:      //shift left logical imm   
            //fdb
            regd = reg1 << shamt;
            break;      //����ط��и�ʲôҪ�����ƶ����ԣ������ƶ�
        case SRLI:
            //fdb
            regd = reg1 >> shamt;
            break;
//??
        case SRAI:		//change here
            //fdb
            regd = (u32)((int)reg1 >> shamt);//
            break;
        case ADD:
            //fdb
            regd = reg1 + reg2;
            break;
        case SUB:
            //fdb
            regd = reg1 - reg2;//???����Ҫ���𣿣�
            break;
        case SLL:       //shift left locical
            //fdb
            regd = reg1 << reg2;
            break;
        case SLT:       //set less than
            //fdb
            regd = ((int)reg1 < (int)reg2) ? 1 : 0;
            break;
        case SLTU:
            //fdb
            regd = (reg1 < reg2) ? 1 : 0;
            break;
        case XOR:
            //fdb
            regd = reg1 ^ reg2;
            break;
        case SRL:       //shift right locical
            //fdb
            regd = reg1 >> reg2;
            break;
        case OR:
            //fdb
            regd = reg1 | reg2;
            break;
        case AND:
            //fdb
            regd = reg1 & reg2;
            break;
    }
    
	EX_MEM.ld_dest = ld_dest;EX_MEM.st_flag = st_flag;              //FUCKKKKKKKKKKKKKKKKKKK
    EX_MEM.ld_flag = ld_flag;EX_MEM.st_dest = st_dest;
    EX_MEM.regd = regd;EX_MEM.opflag = opflag;EX_MEM.rd = rd;
    EX_MEM.rs1 = rs1;EX_MEM.rs2 = rs2;//no opcode ! 
    EX_MEM.reg2 = reg2;
    EX_MEM.PC = PC;           //??**
}

   /*Memrory access*/
void MEM(){// L S
    OPflag opflag = EX_MEM.opflag;
    u32 ld_dest = EX_MEM.ld_dest,
        ld_flag = EX_MEM.ld_flag,
        st_dest = EX_MEM.st_dest,
        st_flag = EX_MEM.st_flag,
        regd = EX_MEM.regd,//����������regd����������Ҳ����û������
        rd = EX_MEM.rd,
        reg2 = EX_MEM.reg2,
        PC = EX_MEM.PC;     //*
    u32 tmp;
    
    if(ld_flag){
        //���ڴ�
        switch (opflag) {
            case LB:    //load binary
                regd = (u32)mem[ld_dest];        //?????u8 -> u32��Ҫ������
                if(((u32)mem[ld_dest] & 0b10000000)){
                    regd |= 0b11111111111111111111111100000000;//0b11111111'11111111'11111111'00000000;
                }//����λ��չ
                break;

            case LH:    //load halfword
                regd = (u32)mem[ld_dest + 1];regd <<= 8;
                regd |= (u32)mem[ld_dest];
                if(((u32)regd & 0b1000000000000000)){//two byte
                    regd |= 0b11111111111111110000000000000000;//0b11111111'11111111'00000000'00000000;
                }//����λ��չ
                break;

            case LW:
                regd = 0b0;
                for(int i = 0;i < 4; ++i){//�ұߵ������� 
                    regd |= mem[ld_dest + 3 - i];       
                    if(i != 3)regd <<= 8;
                }
                break;

            case LBU:
                regd = (u32)mem[ld_dest];
                break;

            case LHU://�ұߵ������� 
                regd = (u32)mem[ld_dest + 1];regd <<= 8;
                regd |= (u32)mem[ld_dest];
                break;
        }
    }
	if(st_flag) {
        switch (opflag) {
            case SB:
                tmp = 0b0;tmp = (reg2 & 0b11111111);//ȡ���λ
                mem[st_dest] = (u8)tmp;
                break;

            case SH:
                tmp = 0b0;tmp = (reg2 & 0b1111111111111111);//ȡ��16λ
                //�ұ߷����ڴ����
				mem[st_dest] = (u8)(tmp);tmp >>= 8;
                mem[st_dest + 1] = (u8)(tmp);                        //????here ????
                break;
                
            case SW:
                tmp = 0b0;tmp = (reg2);             //ȡ��16λ
                for(int i = 0;i < 4; ++i){
                    mem[st_dest + i] = (u8)(tmp);
                    tmp >>= 8;
                }
                break;
        }
    }

    MEM_WB.esc_flag = EX_MEM.esc_flag;
    MEM_WB.opflag = opflag;
    MEM_WB.PC = PC;
    MEM_WB.rd = rd;
    MEM_WB.regd = regd;
}

    /*write back to the register*/
void WB(){
    OPflag opflag = MEM_WB.opflag;
    u32 regd = MEM_WB.regd,
        rd   = MEM_WB.rd,
        esc_flag = MEM_WB.esc_flag;
    switch (opflag) {
        case BEQ:case BNE:case BLT:case BGE:case BLTU:		//AUIPC!!! 
        case BGEU:case SB:case SH:case SW:
            break;
        default:
            if (esc_flag) {     //PC�����Ժ��ǲ��ǲ���PC += 4;���У�
                //reg[rd] = regd;//???��Ҫ�洢���� no need to store
                //actually it doesn't matters
                printf("%d\n",(regd & 255u));
                return ;
            }
            reg[rd] = regd;
    }
    reg[0] = 0;//       big fault!
}

void printOP(OPflag flag){
	return ;
    switch (flag) {
        case 0:std::cout<<"LUI"<<std::endl;break;
        case 1:std::cout<<"AUIPC"<<std::endl;break;
        case 2:std::cout<<"JAL"<<std::endl;break;
        case 3:std::cout<<"JALR"<<std::endl;break;
        case 4:std::cout<<"BEQ"<<std::endl;break;
        case 5:std::cout<<"BNE"<<std::endl;break;
        case 6:std::cout<<"BLT"<<std::endl;break;
        case 7:std::cout<<"BGE"<<std::endl;break;
        case 8:std::cout<<"BLTU"<<std::endl;break;
        case 9:std::cout<<"BGEU"<<std::endl;break;
        case 10:std::cout<<"LB"<<std::endl;break;
        case 11:std::cout<<"LH"<<std::endl;break;
        case 12:std::cout<<"LW"<<std::endl;break;
        case 13:std::cout<<"LBU"<<std::endl;break;
        case 14:std::cout<<"LHU"<<std::endl;break;
        case 15:std::cout<<"SB"<<std::endl;break;
        case 16:std::cout<<"SH"<<std::endl;break;
        case 17:std::cout<<"SW"<<std::endl;break;
        case 18:std::cout<<"ADDI"<<std::endl;break;
        case 19:std::cout<<"SLTI"<<std::endl;break;
        case 20:std::cout<<"SLTIU"<<std::endl;break;
        case 21:std::cout<<"XORI"<<std::endl;break;
        case 22:std::cout<<"ORI"<<std::endl;break;
        case 23:std::cout<<"ANDI"<<std::endl;break;
        case 24:std::cout<<"SLLI"<<std::endl;break;
        case 25:std::cout<<"SRLI"<<std::endl;break;
        case 26:std::cout<<"SRAI"<<std::endl;break;
        case 27:std::cout<<"ADD"<<std::endl;break;
        case 28:std::cout<<"SUB"<<std::endl;break;
        case 29:std::cout<<"SLL"<<std::endl;break;
        case 30:std::cout<<"SLT"<<std::endl;break;
        case 31:std::cout<<"SLTU"<<std::endl;break;
        case 32:std::cout<<"XOR"<<std::endl;break;
        case 33:std::cout<<"SRL"<<std::endl;break;
        case 34:std::cout<<"SRA"<<std::endl;break;
        case 35:std::cout<<"OR"<<std::endl;break;
        case 36:std::cout<<"AND"<<std::endl;break;
    }
}

void printREG(u32 rd){
	return ;
    switch (rd) {
        case 0:std::cout<<"zero";break;
        case 1:std::cout<<"ra";break;
        case 2:std::cout<<"sp";break;
        case 3:std::cout<<"qp";break;
        case 4:std::cout<<"tp";break;
        case 5:std::cout<<"t0";break;
        case 6:std::cout<<"t1";break;
        case 7:std::cout<<"t2";break;
        case 8:std::cout<<"s0/fp";break;
        case 9:std::cout<<"s1";break;
        case 10:std::cout<<"a0";break;
        case 11:std::cout<<"a1";break;
        case 12:std::cout<<"a2";break;
        case 13:std::cout<<"a3";break;
        case 14:std::cout<<"a4";break;
        case 15:std::cout<<"a5";break;
        case 16:std::cout<<"a6";break;
        case 17:std::cout<<"a7";break;
        case 18:std::cout<<"s2";break;
        case 19:std::cout<<"s3";break;
        case 20:std::cout<<"s4";break;
        case 21:std::cout<<"s5";break;
        case 22:std::cout<<"s6";break;
        case 23:std::cout<<"s7";break;
        case 24:std::cout<<"s8";break;
        case 25:std::cout<<"s9";break;
        case 26:std::cout<<"s10";break;
        case 27:std::cout<<"s11";break;
        case 28:std::cout<<"t3";break;
        case 29:std::cout<<"t4";break;
        case 30:std::cout<<"t5";break;
        case 31:std::cout<<"t6";break;
    }
}

void printID_EX_Buffer(STAGE::ID_EX_buffer &t){
	return ;
    printf("imm: %d rd: %d rs1: %d rs2 %d\n",t.imm,t.rd,t.rs1,t.rs2);
    printf("�Ĵ������ rd:");
    printREG(t.rd);
    printf(" ");
    printf("rs1: ");
    printREG(t.rs1);
    printf(" ");
    printf("rs2 ");
    printREG(t.rs2);
    puts(" ");
    char tp[32];
    //printf("imm_binary: %s\n\n",itoa(t.imm,tp,2));
}

}

#endif //BUFFER_STAGES_HPP
