#ifndef __BIT_H
#define __BIT_H

#include <stdint.h>
#include <cmsis_compiler.h>


int Set_Bit(uint32_t *num, uint8_t bit_position, uint8_t value);
int Toggle_Bit(uint32_t *num, uint8_t bit_position);
int Combine_Bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint32_t *result);
int Split_uint32(uint32_t num, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3, uint8_t *byte4);
void Loop_Test(void);
void Bit_Test(void);

#endif // BIT_H
