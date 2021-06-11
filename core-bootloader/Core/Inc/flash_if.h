#ifndef INC_FLASH_IF_H_
#define INC_FLASH_IF_H_

uint16_t Flash_If_Init(void);
uint16_t Flash_If_Erase(uint32_t Add);
uint16_t Flash_If_Write(uint8_t *src, uint8_t *dest, uint32_t Len);
uint8_t *Flash_If_Read(uint8_t *src, uint8_t *dest, uint32_t Len);
uint16_t Flash_If_DeInit(void);

#endif /* INC_FLASH_IF_H_ */
