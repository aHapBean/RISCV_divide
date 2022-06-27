//decided by Path !
//namely the Path is responsible for find the include lib
//cpu.hpp

#ifndef CPU_HPP
#define CPU_HPP

//#include <bits/stdc++.h>
#include "buffer_stages.hpp"

#define fdb std::cout<<"FUCK"<<std::endl;
using u32 = unsigned int;
using u8  = unsigned char;


//建立五个pipeline,然后流水执行？
//对每一个过程传入对象，可以有一个计数的单位！
//有道理，独立出一个stage hpp !
//根据示意图更改buffer的值，然后上OJ测试


u8 mem[500000];
u32 reg[32];
u32 PC;
//int 和进制扯不上关系
class cpu{
//buffer
public:
    //u8 mem[500000];
private:
    //u32 reg[32];
    //u32 PC;
    u32 sz;
public:
cpu(){
    memset(mem,0,sizeof(mem));
    memset(reg,0,sizeof(reg));
    PC = 0;
    sz = 0;
}

~cpu(){
	//delete mem
}

void init(std::istream &in){
    std::string s = "";
    u32 ptr = 0;
    in>>s;
    while(!in.fail() && s != "###"){
        if(s == "###")break;
        if(s[0] == '@'){
        	char *tmp;
            ptr = strtoul(s.substr(1,8).c_str(),&tmp,16);
        } else {
        	char *tmp;
            mem[ptr] = strtoul(s.c_str(),&tmp,16);//自然分界读写可以
            //printf("[%s,%d] \n", s.c_str(), mem[ptr]);
			ptr++;
        }
        in>>s;
    }
    sz = ptr;
}

void DEBUGrun(){
	while(1){
		STAGE::IF();
		STAGE::ID();
        STAGE::EX();
        STAGE::MEM();
        STAGE::WB();
        if(STAGE::MEM_WB.esc_flag != 0)break;
	}
}

/*
void pipeRUN(){
    while(1){
        WB(1 2 3 4 5 1);
       MEM(2 3 4 5 1 2);
        if(MEM_WB.esc_flag != 0)break;
        EX(3 4 5 1 2 3);
        ID(4 5 1 2 3 4);
        IF(5 1 2 3 4 5);
        //对buffer动手
    }
}
*/

};
#endif //CPP_HPP

