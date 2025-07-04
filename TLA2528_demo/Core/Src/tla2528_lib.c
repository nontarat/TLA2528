/*
 * tla2528.c
 *
 *  Created on: Apr 1, 2025
 *      Author: user
 */


#include "tla2528_lib.h"
#define TLA2528_I2C_ADDRESS 0x10
/*
// ฟังก์ชันอ่านค่ารีจิสเตอร์
uint8_t TLA2528_Read_Register(uint8_t reg_address) {
	uint8_t reg_value = 0;
	HAL_StatusTypeDef status;

	// Use HAL I2C to read the register value
	status = HAL_I2C_Mem_Read(&hi2c5, TLA2528_I2C_ADDRESS << 1, reg_address,
	I2C_MEMADD_SIZE_8BIT, &reg_value, 1, 1000);

	// Check the read status
	if (status != HAL_OK) {
		// Print error message if read fails
		printf("I2C Read Error: 0x%02X\n", reg_address);
		return 0;  //  Return 0 if an error occurs
	}

	// Return the value read from the register
	return reg_value;
}

HAL_StatusTypeDef TLA2528_Write_Register(uint8_t reg_address, uint8_t reg_value) {
	HAL_StatusTypeDef status;

	// Use HAL I2C to write value to the register
	status = HAL_I2C_Mem_Write(&hi2c5, TLA2528_I2C_ADDRESS << 1, reg_address,
	I2C_MEMADD_SIZE_8BIT, &reg_value, 1, 1000);

	// Check the write status
	if (status != HAL_OK) {
		printf("I2C Write Error: 0x%02X\n", reg_address);
		return HAL_ERROR;  // Return error if write fails
	}

	return HAL_OK;
}


HAL_StatusTypeDef TLA2528_Read_Registers(uint8_t start_reg, uint8_t *buffer, uint16_t length) {
    HAL_StatusTypeDef status;

    // อ่านค่า length ไบต์ต่อเนื่องจาก start_reg
    status = HAL_I2C_Mem_Read(&hi2c5, TLA2528_I2C_ADDRESS << 1, start_reg,
                              I2C_MEMADD_SIZE_8BIT, buffer, length, 1000);

    if (status != HAL_OK) {
        printf("I2C Continuous Read Error: Start Register 0x%02X\n", start_reg);
    }

    return status;
}

HAL_StatusTypeDef TLA2528_Write_Registers(uint8_t start_reg, uint8_t *data, uint16_t length) {
    HAL_StatusTypeDef status;

    // เขียนค่า length ไบต์ต่อเนื่องจาก start_reg
    status = HAL_I2C_Mem_Write(&hi2c5, TLA2528_I2C_ADDRESS << 1, start_reg,
                               I2C_MEMADD_SIZE_8BIT, data, length, 1000);

    if (status != HAL_OK) {
        printf("I2C Continuous Write Error: Start Register 0x%02X\n", start_reg);
    }

    return status;
}
*/
HAL_StatusTypeDef TLA2528_Write_With_OPCODE(uint8_t reg_address, uint8_t reg_value) {
    uint8_t data[2];
    HAL_StatusTypeDef status;

    // สร้างข้อมูลที่จะส่งไป: OPCODE (0x08) + Register Address + Value
    data[0] = 0x08;  // OPCODE สำหรับ Single Register Write (0000 1000b)
    data[1] = reg_value;

    // เริ่มการส่งข้อมูล I2C
    status = HAL_I2C_Master_Transmit(&hi2c5, (TLA2528_I2C_ADDRESS << 1), data, 2, 1000);

    if (status != HAL_OK) {
        // ถ้ามีข้อผิดพลาด
        printf("I2C Write Error: 0x%02X\n", reg_address);
        return HAL_ERROR;  // ส่งค่าผิดพลาดกลับ
    }

    return HAL_OK;  // ส่งสำเร็จ
}

uint8_t TLA2528_Read_Register_With_OPCODE(uint8_t reg_address) {
    uint8_t reg_value = 0;
    HAL_StatusTypeDef status;
    uint8_t opcode = 0x10; // OPCODE สำหรับ Single Register Read (0001 0000b)
    uint8_t data[2]; // ที่อยู่รีจิสเตอร์และค่า

    // สร้างข้อมูลที่ส่งไปพร้อม OPCODE (Opcode + Register Address)
    data[0] = opcode;  // OPCODE
    data[1] = reg_address;  // ที่อยู่ของรีจิสเตอร์ที่ต้องการอ่าน

    // เริ่มส่งข้อมูลไปที่อุปกรณ์
    status = HAL_I2C_Master_Transmit(&hi2c5, TLA2528_I2C_ADDRESS << 1, data, 2, 1000);

    if (status != HAL_OK) {
        // Handle error
        printf("I2C Transmit Error: 0x%02X\n", reg_address);
        return 0;  // Error occurred, return 0
    }

    // หลังจากส่งข้อมูลไปแล้วให้ทำการอ่านค่าจากรีจิสเตอร์
    status = HAL_I2C_Master_Receive(&hi2c5, TLA2528_I2C_ADDRESS << 1, &reg_value, 1, 1000);

    // ตรวจสอบสถานะการอ่าน
    if (status != HAL_OK) {
        printf("I2C Read Error: 0x%02X\n", reg_address);
        return 0;  // Error occurred, return 0
    }

    // Return the value read from the register
    return reg_value;
}
void Print_Binary(uint8_t value)
{
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

void Read_Register_OPCODE(uint8_t reg_address)
{
	uint8_t reg_value = TLA2528_Read_Register_With_OPCODE(reg_address);
	 printf("Register 0x%02X Value: 0x%02X\n", reg_address, reg_value);
	    printf("Binary Representation: ");
	    Print_Binary(reg_value);
	    printf("\n");
}


// ฟังก์ชันอ่านค่าทุกรีจิสเตอร์
void Read_All_Registers(void) {
    // รายการของรีจิสเตอร์ที่ต้องการอ่าน
    uint8_t register_addresses[] = {
        0x00, // SYSTEM_STATUS_ADDRESS
        0x01, // GENERAL_CFG
        0x02, //DATA_CFG
		0x03, //OSR_CFG
		0x04, //OPMODE_CFG
		0x05, //PIN_CFG
		0x07, //GPIO_CFG
		0x09, //GPO_DRIVE_CFG
		0xB, //GPO_VALUE
		0xD, //GPI_VALUE
		0x10,//SEQUENCE_CFG
        0x11, // MANUAL_CH_SEL_ADDRESS
		0x12 //AUTO_SEQ_CH_SEL
    };

    int num_registers = sizeof(register_addresses) / sizeof(register_addresses[0]);

    for (int i = 0; i < num_registers; i++) {
        uint8_t reg_address = register_addresses[i];
        uint8_t reg_value = TLA2528_Read_Register_With_OPCODE(reg_address);

        // แสดงผลลัพธ์
        printf("Register 0x%02X Value: 0x%02X\n", reg_address, reg_value);
        printf("Binary Representation: ");
        Print_Binary(reg_value);
        printf("\n");

        HAL_Delay(200);  // ปล่อยเวลาให้กับ I2C
    }
}

uint16_t TLA2528_Read_ADC_SingleChannel(uint8_t channel) {
    if (channel > 7) return 0xFFFF; // ตรวจสอบว่าช่องถูกต้อง (0-7)

    uint8_t reg_value = channel & MANUAL_CHID_MASK; // ตั้งค่าช่อง
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c5, TLA2528_I2C_ADDRESS << 1,
                                                       (uint8_t[]){MANUAL_CH_SEL_ADDRESS, reg_value}, 2, 1000);
    HAL_Delay(5); // รอ ADC เปลี่ยนช่อง

    if (status != HAL_OK) {
        printf("I2C Write Error: 0x%02X\n", MANUAL_CH_SEL_ADDRESS);
        return 0xFFFF;
    }

    // รอให้ ADC ทำงานเสร็จ (ขึ้นอยู่กับความเร็ว I2C และอัตราการแปลงของ ADC)
    HAL_Delay(1);

    // อ่านค่าจาก ADC (ค่า 16-bit)
    uint8_t adc_data[2] = {0};
    status = HAL_I2C_Master_Receive(&hi2c5, TLA2528_I2C_ADDRESS << 1, adc_data, 2, 1000);

    if (status != HAL_OK) {
        printf("I2C Read Error: ADC Channel %u\n", channel);
        return 0xFFFF;
    }

    // รวมค่าจาก 2 ไบต์ให้เป็น 16-bit
    uint16_t adc_value = (adc_data[0] << 8) | adc_data[1];

    return adc_value;
}

void TLA2528_Read_ADC_AllChannels(uint16_t *adc_values) {
    for (uint8_t ch = 0; ch < 8; ch++) {
        adc_values[ch] = TLA2528_Read_ADC_SingleChannel(ch);
    }
}

float TLA2528_Convert_To_Voltage(uint16_t adc_value, float vref, uint8_t resolution) {
    uint16_t max_adc = (1 << resolution) - 1; // คำนวณค่ามากสุดของ ADC (2^resolution - 1)
    return ((float)adc_value * vref) / max_adc;
}


