#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include<errno.h>

//#define DEBUG

#ifdef DEBUG
#define LOG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...) (void)0
#endif


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please provide a path for the binary you wish to decode. Usage: ./sim8086 <binary file path>\n");
        return -1;
    }

    FILE* bin_file = fopen(argv[1], "rb");

    if (!bin_file) {
        perror("Error opening file ");
        return -1;
    }

    if (fseek(bin_file, 0L, SEEK_END) == -1) {
        perror("Error seeking to end of file ");
        return -1;
    }

    long file_sz = ftell(bin_file);
    if (file_sz < 0) {
        perror("Error getting file size");
        return -1;
    } else if (file_sz == 0) {
        printf("Empty file provided. The program will now close.\n");
        return -1;
    }

    rewind(bin_file);

    unsigned char* buf = malloc(file_sz);
    if (!buf) {
        perror("Unable to allocate memory");
        return -1;
    }
    memset(buf, 0, file_sz);

    size_t items_read = fread(buf, file_sz, 1, bin_file);
    assert(items_read == 1); 
    assert(file_sz * (long)items_read == file_sz);

    char* reg_table[16] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };  

    printf("bits 16\n\n");

    for (int i = 0; i < file_sz - 1; i = i + 2) {
        LOG("%x ", buf[i]); 
        unsigned char opcode = buf[i] & 0b11111100;
        unsigned char d = buf[i] & 0b00000010;
        unsigned char w = buf[i] & 0b00000001;

        //unsigned char mod = buf[1] & 0b11000000;
        unsigned char reg = buf[i + 1] & 0b00111000;
        unsigned char r_w = buf[i + 1] & 0b00000111;

        LOG("OPCODE: %x\n", opcode);
        LOG("D: %x\n", d);
        LOG("W: %x\n", w);
        LOG("mod: %x\n", mod);
        LOG("reg: %x\n", reg);
        LOG("r_w: %x\n", r_w);
        LOG("\n");

        if ((opcode & 0b10001000) == 0b10001000) {
            printf("mov ");
        }

        char* rw_reg = reg_table[w << 3 | r_w];
        char* reg_reg = reg_table[reg >> 3 | w << 3];
        if (d) {
            printf("%s, %s\n", reg_reg, rw_reg); 
        } else {
            printf("%s, %s\n", rw_reg, reg_reg); 
        }
    }

    return 0;
}
