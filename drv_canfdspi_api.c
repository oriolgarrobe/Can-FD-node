/*******************************************************************************
 Title: .

  Company:
    Microchip Technology Inc.

  File Name:
    drv_canfdspi_api.c

  Summary:
    .

  Description:
    .
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2016 Microchip Technology Inc. and its subsidiaries.  
You may use this software and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, 
COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER 
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED 
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, 
MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE 
WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
 *******************************************************************************/
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files

#include "drv_canfdspi_api.h"
#include "drv_canfdspi_register.h"
#include "drv_canfdspi_defines.h"
#include "drv_spi.h"
//#include "system_config.h"
#define true 1
#define false 0
#include<p18f4520.h>
#include "spi.h"
//#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Defines

#define CRCBASE    0xFFFF
#define CRCUPPER   1
unsigned char acc;
uint16_t a = 0;


// *****************************************************************************
// *****************************************************************************
// Section: Variables

//! SPI Transmit buffer
uint8_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! SPI Receive buffer
uint8_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! Reverse order of bits in byte
//const rom uint8_t BitReverseTable256[256];
const rom uint8_t BitReverseTable256[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

//! Look-up table for CRC calculation
//const rom uint16_t crc16_table[256];
const rom uint16_t crc16_table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};


// *****************************************************************************
// *****************************************************************************
// Section: Reset
/*
int8_t DRV_CANFDSPI_Reset2(CANFDSPI_MODULE_ID index)
{
    uint16_t spiTransferSize = 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) (cINSTRUCTION_RESET << 4);
    spiTransmitBuffer[1] = 0;

    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}
*/

int8_t DRV_CANFDSPI_Reset(CANFDSPI_MODULE_ID index)
{
    uint16_t spiTransferSize = 2;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) (cINSTRUCTION_RESET << 4);
    spiTransmitBuffer[1] = 0;

	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
	SPI_CS = 1; 

    return spiTransferError;
}


// *****************************************************************************
// *****************************************************************************
// Section: SPI Access Functions


int8_t DRV_CANFDSPI_ReadByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t *rxd)
{
	int8_t spiTransferError = 0;
	SPI_CS = 0;
    //uint16_t spiTransferSize = 3;
    

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

	// Update data
	*rxd = ReadSPI();
    //spiTransmitBuffer[2] = 0;

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    // Update data
    //*rxd = spiReceiveBuffer[2];
	SPI_CS = 1;

    return spiTransferError;
}



int8_t DRV_CANFDSPI_WriteByte(CANFDSPI_MODULE_ID index, uint16_t address, uint8_t txd)
{
	
    uint16_t spiTransferSize = 3;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    spiTransmitBuffer[2] = txd;
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);
	WriteSPI(spiTransmitBuffer[2]);

   // spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
	SPI_CS = 1;
    return spiTransferError;
}
int8_t DRV_CANFDSPI_ReadWord(CANFDSPI_MODULE_ID index, uint16_t address, uint32_t *rxd)
{
	
    uint8_t i;
    uint32_t x;
    uint16_t spiTransferSize = 6; //Modified 16/04/2019
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);


	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);
// Update data
    *rxd = 0;
    for (i = 2; i < 6; i++) {
        x = (uint32_t) ReadSPI();
        *rxd += x << ((i - 2)*8);
}
/*
    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 6; i++) {
        x = (uint32_t) spiReceiveBuffer[i];
        *rxd += x << ((i - 2)*8);
    }
*/
	SPI_CS = 1;
    return spiTransferError;
}



int8_t DRV_CANFDSPI_WriteWord(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t txd)
{
	
    uint8_t i;
    uint16_t spiTransferSize = 6;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

    // Split word into 4 bytes and add them to buffer
    for (i = 0; i < 4; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
		WriteSPI(spiTransmitBuffer[i + 2]);
    }

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
	
	SPI_CS = 1;
    return spiTransferError;
}


int8_t DRV_CANFDSPI_ReadHalfWord(CANFDSPI_MODULE_ID index, uint16_t address, uint16_t *rxd)
{	
	
    uint8_t i;
    uint32_t x;
    uint16_t spiTransferSize = 4;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);


    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 4; i++) {
        x = (uint32_t) ReadSPI();
        *rxd += x << ((i - 2)*8);
    }

	SPI_CS = 1;
    return spiTransferError;
}



int8_t DRV_CANFDSPI_WriteHalfWord(CANFDSPI_MODULE_ID index, uint16_t address,
        uint16_t txd)
{
	
    uint8_t i;
    uint16_t spiTransferSize = 4;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

    // Split word into 2 bytes and add them to buffer
    for (i = 0; i < 2; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
		WriteSPI(spiTransmitBuffer[i + 2]);
    }

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

	SPI_CS = 1;
    return spiTransferError;
}



/*
int8_t DRV_CANFDSPI_WriteByteSafe(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t txd)
{
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = 5;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    spiTransmitBuffer[2] = txd;

    // Add CRC
    crcResult = DRV_CANFDSPI_CalculateCRC16(spiTransmitBuffer, 3);
    spiTransmitBuffer[3] = (crcResult >> 8) & 0xFF;
    spiTransmitBuffer[4] = crcResult & 0xFF;

    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_WriteWordSafe(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t txd)
{
    uint8_t i;
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = 8;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);

    // Split word into 4 bytes and add them to buffer
    for (i = 0; i < 4; i++) {
        spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
    }

    // Add CRC
    crcResult = DRV_CANFDSPI_CalculateCRC16(spiTransmitBuffer, 6);
    spiTransmitBuffer[6] = (crcResult >> 8) & 0xFF;
    spiTransmitBuffer[7] = crcResult & 0xFF;

    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}

*/

int8_t DRV_CANFDSPI_ReadByteArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *rxd, uint16_t nBytes)
{
	
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

    // Clear data
/*
    for (i = 2; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }
*/
    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    // Update data
    for (i = 0; i < nBytes; i++) {
        rxd[i] = ReadSPI();
    }

	SPI_CS = 1; 
    return spiTransferError;
}



/*
int8_t DRV_CANFDSPI_ReadByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *rxd, uint16_t nBytes, bool fromRam, bool* crcIsCorrect)
{
    uint8_t i;
    uint16_t crcFromSpiSlave = 0;
    uint16_t crcAtController = 0;
    uint16_t spiTransferSize = nBytes + 5; //first two bytes for sending command & address, third for size, last two bytes for CRC
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ_CRC << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    if (fromRam) {
        spiTransmitBuffer[2] = nBytes >> 2;
    } else {
        spiTransmitBuffer[2] = nBytes;
    }

    // Clear data
    for (i = 3; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }

    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        return spiTransferError;
    }

    // Get CRC from controller
    crcFromSpiSlave = (uint16_t) (spiReceiveBuffer[spiTransferSize - 2] << 8) + (uint16_t) (spiReceiveBuffer[spiTransferSize - 1]);

    // Use the receive buffer to calculate CRC
    // First three bytes need to be command
    spiReceiveBuffer[0] = spiTransmitBuffer[0];
    spiReceiveBuffer[1] = spiTransmitBuffer[1];
    spiReceiveBuffer[2] = spiTransmitBuffer[2];
    crcAtController = DRV_CANFDSPI_CalculateCRC16(spiReceiveBuffer, nBytes + 3);

    // Compare CRC readings
    if (crcFromSpiSlave == crcAtController) {
        *crcIsCorrect = true;
    } else {
        *crcIsCorrect = false;
    }

    // Update data
    for (i = 0; i < nBytes; i++) {
        rxd[i] = spiReceiveBuffer[i + 3];
    }

    return spiTransferError;
}
*/

int8_t DRV_CANFDSPI_WriteByteArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *txd, uint16_t nBytes)
{
	
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);


    // Add data
    for (i = 2; i < spiTransferSize; i++) {
        //spiTransmitBuffer[i] = txd[i - 2];
		WriteSPI(txd[i - 2]);
    }

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

	SPI_CS = 1;
    return spiTransferError;
}



/*
int8_t DRV_CANFDSPI_WriteByteArrayWithCRC(CANFDSPI_MODULE_ID index, uint16_t address,
        uint8_t *txd, uint16_t nBytes, bool fromRam)
{
    uint16_t i;
    uint16_t crcResult = 0;
    uint16_t spiTransferSize = nBytes + 5;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_CRC << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
    if (fromRam) {
        spiTransmitBuffer[2] = nBytes >> 2;
    } else {
        spiTransmitBuffer[2] = nBytes;
    }

    // Add data
    for (i = 0; i < nBytes; i++) {
        spiTransmitBuffer[i + 3] = txd[i];
    }

    // Add CRC
    crcResult = DRV_CANFDSPI_CalculateCRC16(spiTransmitBuffer, spiTransferSize - 2);
    spiTransmitBuffer[spiTransferSize - 2] = (uint8_t) ((crcResult >> 8) & 0xFF);
    spiTransmitBuffer[spiTransferSize - 1] = (uint8_t) (crcResult & 0xFF);

    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

    return spiTransferError;
}
*/

int8_t DRV_CANFDSPI_ReadWordArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t *rxd, uint16_t nWords)
{
	
    uint16_t i, j, n;
    REG_t w;
    //uint16_t spiTransferSize = nWords * 4 + 2;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

//    // Clear data
//    for (i = 2; i < spiTransferSize; i++) {
//        spiTransmitBuffer[i] = 0;
//    }
//
//    spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
//    if (spiTransferError) {
//        return spiTransferError;
//    }
//
    // Convert Byte array to Word array
    n = 2;
    for (i = 0; i < nWords; i++) {
        w.word = 0;
        for (j = 0; j < 4; j++, n++) {
            w.byte[j] = ReadSPI();
        }
        rxd[i] = w.word;
    }

	SPI_CS = 1;
    return spiTransferError;
}



int8_t DRV_CANFDSPI_WriteWordArray(CANFDSPI_MODULE_ID index, uint16_t address,
        uint32_t *txd, uint16_t nWords)
{
	
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int8_t spiTransferError = 0;
	SPI_CS = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;
	WriteSPI(spiTransmitBuffer[0]);
	WriteSPI(spiTransmitBuffer[1]);

    // Convert ByteArray to word array
    n = 2;
    for (i = 0; i < nWords; i++) {
        w.word = txd[i];
        for (j = 0; j < 4; j++, n++) {
            spiTransmitBuffer[n] = w.byte[j];
			WriteSPI(spiTransmitBuffer[n]);
    
        }
    }

    //spiTransferError = DRV_SPI_TransferData(index, spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
	SPI_CS = 1;
    return spiTransferError;
}


// *****************************************************************************
// *****************************************************************************
// Section: Configuration


int8_t DRV_CANFDSPI_Configure(CANFDSPI_MODULE_ID index, CAN_CONFIG* config)
{
    REG_CiCON ciCon;
    int8_t spiTransferError = 0;

    ciCon.word = canControlResetValues[cREGADDR_CiCON / 4];

    ciCon.bF.DNetFilterCount = config->DNetFilterCount;
    ciCon.bF.IsoCrcEnable = config->IsoCrcEnable;
    ciCon.bF.ProtocolExceptionEventDisable = config->ProtocolExpectionEventDisable;
    ciCon.bF.WakeUpFilterEnable = config->WakeUpFilterEnable;
    ciCon.bF.WakeUpFilterTime = config->WakeUpFilterTime;
    ciCon.bF.BitRateSwitchDisable = config->BitRateSwitchDisable;
    ciCon.bF.RestrictReTxAttempts = config->RestrictReTxAttempts;
    ciCon.bF.EsiInGatewayMode = config->EsiInGatewayMode;
    ciCon.bF.SystemErrorToListenOnly = config->SystemErrorToListenOnly;
    ciCon.bF.StoreInTEF = config->StoreInTEF;
    ciCon.bF.TXQEnable = config->TXQEnable;
    ciCon.bF.TxBandWidthSharing = config->TxBandWidthSharing;

    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiCON, ciCon.word);
    if (spiTransferError) {
        return -1;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ConfigureObjectReset(CAN_CONFIG* config)
{
    REG_CiCON ciCon;
    ciCon.word = canControlResetValues[cREGADDR_CiCON / 4];

    config->DNetFilterCount = ciCon.bF.DNetFilterCount;
    config->IsoCrcEnable = ciCon.bF.IsoCrcEnable;
    config->ProtocolExpectionEventDisable = ciCon.bF.ProtocolExceptionEventDisable;
    config->WakeUpFilterEnable = ciCon.bF.WakeUpFilterEnable;
    config->WakeUpFilterTime = ciCon.bF.WakeUpFilterTime;
    config->BitRateSwitchDisable = ciCon.bF.BitRateSwitchDisable;
    config->RestrictReTxAttempts = ciCon.bF.RestrictReTxAttempts;
    config->EsiInGatewayMode = ciCon.bF.EsiInGatewayMode;
    config->SystemErrorToListenOnly = ciCon.bF.SystemErrorToListenOnly;
    config->StoreInTEF = ciCon.bF.StoreInTEF;
    config->TXQEnable = ciCon.bF.TXQEnable;
    config->TxBandWidthSharing = ciCon.bF.TxBandWidthSharing;

    return 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Operating mode

int8_t DRV_CANFDSPI_OperationModeSelect(CANFDSPI_MODULE_ID index,
        CAN_OPERATION_MODE opMode)	
{
    uint8_t d = 0;
    int8_t spiTransferError = 0;

    // Read
    spiTransferError = DRV_CANFDSPI_ReadByte(index, cREGADDR_CiCON + 3, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d &= ~0x07;
    d |= opMode;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, cREGADDR_CiCON + 3, d);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

// *****************************************************************************
// *****************************************************************************
// Section: Module Events



int8_t DRV_CANFDSPI_ModuleEventEnable(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiINTENABLE intEnables;

    // Read Interrupt Enables
    a = cREGADDR_CiINTENABLE;
    
    intEnables.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadHalfWord(index, a, &intEnables.word);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    intEnables.word |= (flags & CAN_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteHalfWord(index, a, intEnables.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitChannelEventEnable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT flags)
{
    REG_CiFIFOCON ciFifoCon;
	int8_t spiTransferError = 0;
    ////uint16_t a = 0;

    // Read Interrupt Enables
    a = (unsigned int) cREGADDR_CiFIFOCON;// + (channel * CiFIFO_OFFSET);
    
    ciFifoCon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] |= (flags & CAN_TX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_GpioModeConfigure(CANFDSPI_MODULE_ID index,
        GPIO_PIN_MODE gpio0, GPIO_PIN_MODE gpio1)
{
    REG_IOCON iocon;
	int8_t spiTransferError = 0;
    ////uint16_t a = 0;

    // Read
    a = cREGADDR_IOCON + 3;
    
    iocon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &iocon.byte[3]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    iocon.bF.PinMode0 = gpio0;
    iocon.bF.PinMode1 = gpio1;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, iocon.byte[3]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventDisable(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    ////uint16_t a = 0;
	REG_CiINTENABLE intEnables;

    // Read Interrupt Enables
    a = cREGADDR_CiINTENABLE;
    
    intEnables.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadHalfWord(index, a, &intEnables.word);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    intEnables.word &= ~(flags & CAN_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteHalfWord(index, a, intEnables.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventClear(CANFDSPI_MODULE_ID index,
        CAN_MODULE_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiINTFLAG intFlags;

    // Read Interrupt flags
    a = cREGADDR_CiINTFLAG;
    
    intFlags.word = 0;

    // Write 1 to all flags except the ones that we want to clear
    // Writing a 1 will not set the flag
    // Only writing a 0 will clear it
    // The flags are HS/C
    intFlags.word = CAN_ALL_EVENTS;
    intFlags.word &= ~flags;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteHalfWord(index, a, intFlags.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventRxCodeGet(CANFDSPI_MODULE_ID index,
        CAN_RXCODE* rxCode)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
    uint8_t rxCodeByte = 0;

    // Read
    a = cREGADDR_CiVEC + 3;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &rxCodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode data
    // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
    if ((rxCodeByte < CAN_RXCODE_TOTAL_CHANNELS) || (rxCodeByte == CAN_RXCODE_NO_INT)) {
        *rxCode = (CAN_RXCODE) rxCodeByte;
    } else {
        *rxCode = CAN_RXCODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventTxCodeGet(CANFDSPI_MODULE_ID index,
        CAN_TXCODE* txCode)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
    uint8_t txCodeByte = 0;

    // Read
    a = cREGADDR_CiVEC + 2;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &txCodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode data
    // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
    if ((txCodeByte < CAN_TXCODE_TOTAL_CHANNELS) || (txCodeByte == CAN_TXCODE_NO_INT)) {
        *txCode = (CAN_TXCODE) txCodeByte;
    } else {
        *txCode = CAN_TXCODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventFilterHitGet(CANFDSPI_MODULE_ID index,
        CAN_FILTER* filterHit)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
    uint8_t filterHitByte = 0;

    // Read
    a = cREGADDR_CiVEC + 1;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &filterHitByte);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *filterHit = (CAN_FILTER) filterHitByte;

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ModuleEventIcodeGet(CANFDSPI_MODULE_ID index,
        CAN_ICODE* icode)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
    uint8_t icodeByte = 0;

    // Read
    a = cREGADDR_CiVEC;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &icodeByte);
    if (spiTransferError) {
        return -1;
    }

    // Decode
    if ((icodeByte < CAN_ICODE_RESERVED) && ((icodeByte < CAN_ICODE_TOTAL_CHANNELS) || (icodeByte >= CAN_ICODE_NO_INT))) {
        *icode = (CAN_ICODE) icodeByte;
    } else {
        *icode = CAN_ICODE_RESERVED; // shouldn't get here
    }

    return spiTransferError;
}

////////////////////////TX////////////////////////

// *****************************************************************************
// *****************************************************************************
// Section: CAN Transmit


int8_t DRV_CANFDSPI_TransmitChannelLoad(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_MSGOBJ* txObj,
        uint8_t *txd, uint32_t txdNumBytes, bool flush)
{
//static unsigned char KK = 1;
    //uint16_t a = 0;
    uint32_t fifoReg[3];
    uint32_t dataBytesInObject;
    REG_CiFIFOCON ciFifoCon;
    REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError = 0; 
	uint8_t txBuffer[MAX_MSG_SIZE];
    uint8_t i;
	uint16_t n = 0;
    uint8_t j = 0;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_ReadWordArray(index, a, fifoReg, 3);

    if (spiTransferError) {
        return -1;
    }

    // Check that it is a transmit buffer
    ciFifoCon.word = fifoReg[0];
    if (!ciFifoCon.txBF.TxEnable) {
        return -2;
    }

    // Check that DLC is big enough for data
    dataBytesInObject = DRV_CANFDSPI_DlcToDataBytes((CAN_DLC) txObj->bF.ctrl.DLC);
    if (dataBytesInObject < txdNumBytes) {
        return -3;
    }

    // Get status
    ciFifoSta.word = fifoReg[1];

    // Get address
    ciFifoUa.word = fifoReg[2];


#ifdef USERADDRESS_TIMES_FOUR //ifdef
    a = 4 * ciFifoUa.bF.UserAddress;
#else
	
    a = ((uint16_t)ciFifoUa.bF.UserAddress2)<<8 | ciFifoUa.bF.UserAddress1;
#endif 


    a += cRAMADDR_START;

//	a = 0x400+(unsigned int)KK*(76);
//	KK++;

    txBuffer[0] = txObj->bF.id.SIDB;//txObj->byte[0]; 
    txBuffer[1] = ((txObj->bF.id.EIDA &0x1F) << 3) | (txObj->bF.id.SIDA);//txObj->byte[1];
    txBuffer[2] = ((txObj->bF.id.EIDB & 0x1F) << 3) | (txObj->bF.id.EIDA & 0xE0);//txObj->byte[2];
    txBuffer[3] = ((txObj->bF.id.SID1 << 2) | (txObj->bF.id.EIDC)<<3) | (txObj->bF.id.EIDB &0xE0);//txObj->byte[3];


//    txBuffer[0] = txObj->byte[0]; 
//    txBuffer[1] = txObj->byte[1];
//    txBuffer[2] = txObj->byte[2];
//    txBuffer[3] = txObj->byte[3];

    txBuffer[4] = txObj->byte[4];
    txBuffer[5] = txObj->byte[5];
    txBuffer[6] = txObj->byte[6];
    txBuffer[7] = txObj->byte[7];

    
    for (i = 0; i < txdNumBytes; i++) {
        txBuffer[i + 8] = txd[i];
    }

    // Make sure we write a multiple of 4 bytes to RAM
    

    if (txdNumBytes % 4) {
        // Need to add bytes
        n = 4 - (txdNumBytes % 4);
        i = txdNumBytes + 8;

        for (j = 0; j < n; j++) {
            txBuffer[i + 8 + j] = 0;
        }
    }
/*
for (i = 0; i < 76; i++) 
txBuffer[i] = 0xff;
a = 0x44c;
*/
    spiTransferError = DRV_CANFDSPI_WriteByteArray(index, a, txBuffer, txdNumBytes + 8 + n);
    if (spiTransferError) {
        return -4;
    }


//DRV_CANFDSPI_RamInit2(DRV_CANFDSPI_INDEX_0, 0xFF); 

    // Set UINC and TXREQ
    spiTransferError = DRV_CANFDSPI_TransmitChannelUpdate(index, channel, flush);
    if (spiTransferError) {
        return -5;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitChannelUpdate(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, bool flush)
{
    //uint16_t a = 0;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.word = 0;
    ciFifoCon.txBF.UINC = 1;

    // Set TXREQ
    if (flush) {
        ciFifoCon.txBF.TxRequest = 1;
    }

    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoCon.byte[1]);
    if (spiTransferError) {
        return -1;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitChannelEventGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;

    // Read Interrupt flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_TX_FIFO_EVENT) (ciFifoSta.byte[0] & CAN_TX_FIFO_ALL_EVENTS);

    return spiTransferError;
}

// DES D'AQUI

int8_t DRV_CANFDSPI_TefUpdate(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiTEFCON ciTefCon;

    // Set UINC
    a = cREGADDR_CiTEFCON + 1;
    
    ciTefCon.word = 0;
    ciTefCon.bF.UINC = 1;

    // Write byte
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciTefCon.byte[1]);

    return spiTransferError;
}


// Section: Transmit Event FIFO Events

int8_t DRV_CANFDSPI_TefMessageGet(CANFDSPI_MODULE_ID index,
        CAN_TEF_MSGOBJ* tefObj)
{
	REG_CiTEFCON ciTefCon;
	REG_CiTEFSTA ciTefSta;
	REG_CiFIFOUA ciTefUa;
	REG_t myReg;
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
    uint32_t fifoReg[3];
    uint8_t n = 0;
	uint8_t ba[12];

    // Get FIFO registers
    a = cREGADDR_CiTEFCON;

    spiTransferError = DRV_CANFDSPI_ReadWordArray(index, a, fifoReg, 3);
    if (spiTransferError) {
        return -1;
    }

    // Get control
    
    ciTefCon.word = fifoReg[0];

    // Get status
    
    ciTefSta.word = fifoReg[1];

    // Get address
    
    ciTefUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciTefUa.bF.UserAddress;
#else
    //a = ciTefUa.bF.UserAddress; 14/05/2019 18:43
	a = ((uint16_t)ciTefUa.bF.UserAddress2)<<8 | ciTefUa.bF.UserAddress1;
#endif
    a += cRAMADDR_START;

    // Number of bytes to read
    n = 8; // 8 header bytes

    if (ciTefCon.bF.TimeStampEnable) {
        n += 4; // Add 4 time stamp bytes
    }

    // Read rxObj using one access
    

    spiTransferError = DRV_CANFDSPI_ReadByteArray(index, a, ba, n);
    if (spiTransferError) {
        return -2;
    }

    // Assign message header
    

    myReg.byte[0] = ba[0];
    myReg.byte[1] = ba[1];
    myReg.byte[2] = ba[2];
    myReg.byte[3] = ba[3];
    tefObj->word[0] = myReg.word;

    myReg.byte[0] = ba[4];
    myReg.byte[1] = ba[5];
    myReg.byte[2] = ba[6];
    myReg.byte[3] = ba[7];
    tefObj->word[1] = myReg.word;

    if (ciTefCon.bF.TimeStampEnable) {
        myReg.byte[0] = ba[8];
        myReg.byte[1] = ba[9];
        myReg.byte[2] = ba[10];
        myReg.byte[3] = ba[11];
        tefObj->word[2] = myReg.word;
    } else {
        tefObj->word[2] = 0;
    }

    // Set UINC
    spiTransferError = DRV_CANFDSPI_TefUpdate(index);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TefEventGet(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;

    // Read Interrupt flags
    REG_CiTEFSTA ciTefSta;
    ciTefSta.word = 0;
    a = cREGADDR_CiTEFSTA;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciTefSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_TEF_FIFO_EVENT) (ciTefSta.byte[0] & CAN_TEF_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TefEventEnable(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiTEFCON ciTefCon;

    // Read Interrupt Enables
    a = cREGADDR_CiTEFCON;
    
    ciTefCon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciTefCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefCon.byte[0] |= (flags & CAN_TEF_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciTefCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TefEventDisable(CANFDSPI_MODULE_ID index,
        CAN_TEF_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiTEFCON ciTefCon;

    // Read Interrupt Enables
    a = cREGADDR_CiTEFCON;
    
    ciTefCon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciTefCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefCon.byte[0] &= ~(flags & CAN_TEF_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciTefCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TefEventOverflowClear(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;

    // Read Interrupt Flags
    REG_CiTEFSTA ciTefSta;
    ciTefSta.word = 0;
    a = cREGADDR_CiTEFSTA;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciTefSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciTefSta.byte[0] &= ~(CAN_TEF_FIFO_OVERFLOW_EVENT);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciTefSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitChannelEventAttemptClear(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
	REG_CiFIFOSTA ciFifoSta;
    int8_t spiTransferError = 0;
    //uint16_t a = 0;

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);
    
    ciFifoSta.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoSta.byte[0] &= ~CAN_TX_FIFO_ATTEMPTS_EXHAUSTED_EVENT;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}



//////////////////RX/////////////////////////

    


int8_t DRV_CANFDSPI_ReceiveChannelUpdate(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    //uint16_t a = 0;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError = 0;
    ciFifoCon.word = 0;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.rxBF.UINC = 1;

    // Write byte
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoCon.byte[1]);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveChannelEventGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT* flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiFIFOSTA ciFifoSta;

#ifdef CAN_TXQUEUE_IMPLEMENTED
    if (channel == CAN_TXQUEUE_CH0) return -100;
#endif

    // Read Interrupt flags
    
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Update data
    *flags = (CAN_RX_FIFO_EVENT) (ciFifoSta.byte[0] & CAN_RX_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveEventGet(CANFDSPI_MODULE_ID index, uint32_t* rxif)
{
    int8_t spiTransferError = 0;

    spiTransferError = DRV_CANFDSPI_ReadWord(index, cREGADDR_CiRXIF, rxif);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveEventOverflowGet(CANFDSPI_MODULE_ID index,
        uint32_t* rxovif)
{
    int8_t spiTransferError = 0;

    spiTransferError = DRV_CANFDSPI_ReadWord(index, cREGADDR_CiRXOVIF, rxovif);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveChannelEventEnable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiFIFOCON ciFifoCon;

#ifdef CAN_TXQUEUE_IMPLEMENTED
    if (channel == CAN_TXQUEUE_CH0) return -100;
#endif

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    
    ciFifoCon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] |= (flags & CAN_RX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveChannelEventDisable(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT flags)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiFIFOCON ciFifoCon;

#ifdef CAN_TXQUEUE_IMPLEMENTED
    if (channel == CAN_TXQUEUE_CH0) return -100;
#endif

    // Read Interrupt Enables
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
   
    ciFifoCon.word = 0;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoCon.byte[0] &= ~(flags & CAN_RX_FIFO_ALL_EVENTS);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoCon.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveChannelEventOverflowClear(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
	REG_CiFIFOSTA ciFifoSta;

#ifdef CAN_TXQUEUE_IMPLEMENTED
    if (channel == CAN_TXQUEUE_CH0) return -100;
#endif

    // Read Interrupt Flags
    
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    ciFifoSta.byte[0] &= ~(CAN_RX_FIFO_OVERFLOW_EVENT);

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, ciFifoSta.byte[0]);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveMessageGet(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_MSGOBJ* rxObj,
        uint8_t *rxd, uint8_t nBytes)
{
    uint8_t n = 0;
    uint8_t i = 0;
    //uint16_t a = 0;
    uint32_t fifoReg[3];
    REG_CiFIFOCON ciFifoCon;
    REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError = 0;
    uint8_t ba[MAX_MSG_SIZE];
   REG_t myReg;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_ReadWordArray(index, a, fifoReg, 3);
    if (spiTransferError) {
        return -1;
    }

    // Check that it is a receive buffer
    ciFifoCon.word = fifoReg[0];
    if (ciFifoCon.txBF.TxEnable) {
        return -2;
    }

    // Get Status
    ciFifoSta.word = fifoReg[1];

    // Get address
    ciFifoUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciFifoUa.bF.UserAddress;
#else
    //a = ciFifoUa.bF.UserAddress;
	a = ((uint16_t)ciFifoUa.bF.UserAddress2)<<8 | ciFifoUa.bF.UserAddress1;
#endif
    a += cRAMADDR_START;

    // Number of bytes to read
    n = nBytes + 8; // Add 8 header bytes

    if (ciFifoCon.rxBF.RxTimeStampEnable) {
        n += 4; // Add 4 time stamp bytes
    }

    // Make sure we read a multiple of 4 bytes from RAM
    if (n % 4) {
        n = n + 4 - (n % 4);
    }

    // Read rxObj using one access


    if (n > MAX_MSG_SIZE) {
        n = MAX_MSG_SIZE;
    }

    spiTransferError = DRV_CANFDSPI_ReadByteArray(index, a, ba, n);
    if (spiTransferError) {
        return -3;
    }

    // Assign message header
 



    //myReg.byte[0] = ba[0];
    //myReg.byte[1] = ba[1];
    //myReg.byte[2] = ba[2];
    //myReg.byte[3] = ba[3];

    myReg.byte[0] = ba[0];
    myReg.byte[1] = ba[1];
    myReg.byte[2] = ba[2];
     myReg.byte[3] = ba[3];
	
    rxObj->word[0] = myReg.word;

    myReg.byte[0] = ba[4];
    myReg.byte[1] = ba[5];
    myReg.byte[2] = ba[6];
    myReg.byte[3] = ba[7];
    rxObj->word[1] = myReg.word;

    if (ciFifoCon.rxBF.RxTimeStampEnable) {
        myReg.byte[0] = ba[8];
        myReg.byte[1] = ba[9];
        myReg.byte[2] = ba[10];
        myReg.byte[3] = ba[11];
        rxObj->word[2] = myReg.word;

        // Assign message data
        for (i = 0; i < nBytes; i++) {
            rxd[i] = ba[i + 12];
        }
    } else {
        rxObj->word[2] = 0;

        // Assign message data
        for (i = 0; i < nBytes; i++) {
            rxd[i] = ba[i + 8];
        }
    }

    // UINC channel
    spiTransferError = DRV_CANFDSPI_ReceiveChannelUpdate(index, channel);
    if (spiTransferError) {
        return -4;
    }

    return spiTransferError;
}







// Receive 


int8_t DRV_CANFDSPI_BitTimeConfigure(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode,
        CAN_SYSCLK_SPEED clk)
{
    int8_t spiTransferError = 0;

    // Decode clk
    switch (clk) {
        case CAN_SYSCLK_40M:
            spiTransferError = DRV_CANFDSPI_BitTimeConfigureNominal40MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = DRV_CANFDSPI_BitTimeConfigureData40MHz(index, bitTime, sspMode);
            break;
        case CAN_SYSCLK_20M:
            spiTransferError = DRV_CANFDSPI_BitTimeConfigureNominal20MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = DRV_CANFDSPI_BitTimeConfigureData20MHz(index, bitTime, sspMode);
            break;
        case CAN_SYSCLK_10M:
            spiTransferError = DRV_CANFDSPI_BitTimeConfigureNominal10MHz(index, bitTime);
            if (spiTransferError) return spiTransferError;

            spiTransferError = DRV_CANFDSPI_BitTimeConfigureData10MHz(index, bitTime, sspMode);
            break;
        default:
            spiTransferError = -1;
            break;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(CAN_RX_FIFO_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canControlResetValues[cREGADDR_CiFIFOCON / 4];

    config->FifoSize = ciFifoCon.rxBF.FifoSize;
    config->PayLoadSize = ciFifoCon.rxBF.PayLoadSize;
    config->RxTimeStampEnable = ciFifoCon.rxBF.RxTimeStampEnable;

    return 0;
}

int8_t DRV_CANFDSPI_ReceiveChannelConfigure(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_CONFIG* config)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;
 REG_CiFIFOCON ciFifoCon;

#ifdef CAN_TXQUEUE_IMPLEMENTED
    if (channel == CAN_TXQUEUE_CH0) {
        return -100;
    }
#endif

    // Setup FIFO
   
    ciFifoCon.word = canControlResetValues[cREGADDR_CiFIFOCON / 4];

    ciFifoCon.rxBF.TxEnable = 0;
    ciFifoCon.rxBF.FifoSize = config->FifoSize;
    ciFifoCon.rxBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.rxBF.RxTimeStampEnable = config->RxTimeStampEnable;

    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
}
int8_t DRV_CANFDSPI_TransmitChannelConfigure(CANFDSPI_MODULE_ID index,
        CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_CONFIG* config)
{
    int8_t spiTransferError = 0;
    //uint16_t a = 0;

    // Setup FIFO
    REG_CiFIFOCON ciFifoCon;
	//a = canControlResetValues[0x14];
	//a = canControlResetValues[0x16];
	//a = canControlResetValues[cREGADDR_CiFIFOCON / 4 ];
	//a = canControlResetValues[(unsigned int )cREGADDR_CiFIFOCON / 4 ];	
    ciFifoCon.word = canControlResetValues[(unsigned int)cREGADDR_CiFIFOCON / 4 ];
	//ciFifoCon.word = canControlResetValues[0x14];
    ciFifoCon.txBF.TxEnable = 1;
    ciFifoCon.txBF.FifoSize = config->FifoSize;
    ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.txBF.TxAttempts = config->TxAttempts;
    ciFifoCon.txBF.TxPriority = config->TxPriority;
    ciFifoCon.txBF.RTREnable = config->RTREnable;

    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = DRV_CANFDSPI_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitChannelConfigureObjectReset(CAN_TX_FIFO_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canControlResetValues[cREGADDR_CiFIFOCON / 4];

    config->RTREnable = ciFifoCon.txBF.RTREnable;
    config->TxPriority = ciFifoCon.txBF.TxPriority;
    config->TxAttempts = ciFifoCon.txBF.TxAttempts;
    config->FifoSize = ciFifoCon.txBF.FifoSize;
    config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;

    return 0;
}


// *****************************************************************************
// *****************************************************************************
// Section: Error Handling




// *****************************************************************************
// *****************************************************************************
// Section: ECC

int8_t DRV_CANFDSPI_EccEnable(CANFDSPI_MODULE_ID index)
{
    int8_t spiTransferError = 0;
    uint8_t d = 0;

    // Read
    spiTransferError = DRV_CANFDSPI_ReadByte(index, cREGADDR_ECCCON, &d);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    d |= 0x01;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, cREGADDR_ECCCON, d);
    if (spiTransferError) {
        return -2;
    }

    return 0;
}


// *****************************************************************************
// *****************************************************************************
// Section: CRC

int8_t DRV_CANFDSPI_RamInit(CANFDSPI_MODULE_ID index, uint8_t d)
{
    uint8_t txd[SPI_DEFAULT_BUFFER_LENGTH];
    uint32_t k;
    int8_t spiTransferError = 0;
 	//uint16_t 

	a = cRAMADDR_START;

    // Prepare data
    for (k = 0; k < SPI_DEFAULT_BUFFER_LENGTH; k++) {
        txd[k] = d;
    }

   

    for (k = 0; k < (cRAM_SIZE / SPI_DEFAULT_BUFFER_LENGTH); k++) {
        spiTransferError = DRV_CANFDSPI_WriteByteArray(index, a, txd, SPI_DEFAULT_BUFFER_LENGTH);
        if (spiTransferError) {
            return -1;
        }
        a += SPI_DEFAULT_BUFFER_LENGTH;
    }

    return spiTransferError;
}

/*
int8_t DRV_CANFDSPI_RamInit2(CANFDSPI_MODULE_ID index, uint8_t d)
{
    uint8_t txd[SPI_DEFAULT_BUFFER_LENGTH];
    uint32_t k;
    int8_t spiTransferError = 0;
 	//uint16_t 

	a = 0x44c;//cRAMADDR_START;

    // Prepare data
    for (k = 0; k < SPI_DEFAULT_BUFFER_LENGTH; k++) {
        txd[k] = d;
    }

   spiTransferError = DRV_CANFDSPI_WriteByteArray(index, a, txd, 76);

   
    return spiTransferError;
}

*/



// *****************************************************************************
// *****************************************************************************
// Section: Time Stamp




// *****************************************************************************
// *****************************************************************************
// Section: Oscillator and Bit Time

int8_t DRV_CANFDSPI_BitTimeConfigureData10MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;

uint32_t tdcValue = 0;
    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
    

    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
        case CAN_1000K_4M:
        case CAN_1000K_8M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 7;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 8;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
        case CAN_250K_4M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_BitTimeConfigureNominal10MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 14;
            ciNbtcfg.bF.TSEG2 = 3;
            ciNbtcfg.bF.SJW = 3;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 7;
            ciNbtcfg.bF.TSEG2 = 2;
            ciNbtcfg.bF.SJW = 2;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}
int8_t DRV_CANFDSPI_BitTimeConfigureData20MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;
 uint32_t tdcValue = 0;

    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
   
    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_1000K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_5M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 1;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 2;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
        case CAN_1000K_8M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 17;
            ciDbtcfg.bF.TSEG2 = 4;
            ciDbtcfg.bF.SJW = 4;
            // SSP
            ciTdc.bF.TDCOffset = 18;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
            //qDebug("Data Bitrate not feasible with this clock!");
            return -1;
            break;
        case CAN_250K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = tdcValue;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_BitTimeConfigureNominal20MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 14;
            ciNbtcfg.bF.TSEG2 = 3;
            ciNbtcfg.bF.SJW = 3;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 126;
            ciNbtcfg.bF.TSEG2 = 31;
            ciNbtcfg.bF.SJW = 31;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_BitTimeConfigureData40MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
    int8_t spiTransferError = 0;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;
uint32_t tdcValue = 0;

    //    sspMode;

    ciDbtcfg.word = canControlResetValues[cREGADDR_CiDBTCFG / 4];
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
    
    // Data Bit rate and SSP
    switch (bitTime) {
        case CAN_500K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_2M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_3M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_4M:
        case CAN_1000K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_5M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 4;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 5;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_6M7:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 3;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 4;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_500K_8M:
        case CAN_1000K_8M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 2;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 3;
            ciTdc.bF.TDCValue = 1;
            break;
        case CAN_500K_10M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 1;
            ciDbtcfg.bF.TSEG2 = 0;
            ciDbtcfg.bF.SJW = 0;
            // SSP
            ciTdc.bF.TDCOffset = 2;
            ciTdc.bF.TDCValue = 0;
            break;

        case CAN_250K_500K:
        case CAN_125K_500K:
            ciDbtcfg.bF.BRP = 1;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_833K:
            ciDbtcfg.bF.BRP = 1;
            ciDbtcfg.bF.TSEG1 = 17;
            ciDbtcfg.bF.TSEG2 = 4;
            ciDbtcfg.bF.SJW = 4;
            // SSP
            ciTdc.bF.TDCOffset = 18;
            ciTdc.bF.TDCValue = tdcValue;
            ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
            break;
        case CAN_250K_1M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 30;
            ciDbtcfg.bF.TSEG2 = 7;
            ciDbtcfg.bF.SJW = 7;
            // SSP
            ciTdc.bF.TDCOffset = 31;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_1M5:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 18;
            ciDbtcfg.bF.TSEG2 = 5;
            ciDbtcfg.bF.SJW = 5;
            // SSP
            ciTdc.bF.TDCOffset = 19;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_2M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 14;
            ciDbtcfg.bF.TSEG2 = 3;
            ciDbtcfg.bF.SJW = 3;
            // SSP
            ciTdc.bF.TDCOffset = 15;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_3M:
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 8;
            ciDbtcfg.bF.TSEG2 = 2;
            ciDbtcfg.bF.SJW = 2;
            // SSP
            ciTdc.bF.TDCOffset = 9;
            ciTdc.bF.TDCValue = tdcValue;
            break;
        case CAN_250K_4M:
            // Data BR
            ciDbtcfg.bF.BRP = 0;
            ciDbtcfg.bF.TSEG1 = 6;
            ciDbtcfg.bF.TSEG2 = 1;
            ciDbtcfg.bF.SJW = 1;
            // SSP
            ciTdc.bF.TDCOffset = 7;
            ciTdc.bF.TDCValue = tdcValue;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError) {
        return -2;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError) {
        return -3;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_BitTimeConfigureNominal40MHz(CANFDSPI_MODULE_ID index,
        CAN_BITTIME_SETUP bitTime)
{
    int8_t spiTransferError = 0;
    REG_CiNBTCFG ciNbtcfg;

    ciNbtcfg.word = canControlResetValues[cREGADDR_CiNBTCFG / 4];

    // Arbitration Bit rate
    switch (bitTime) {
            // All 500K
        case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_3M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 62;
            ciNbtcfg.bF.TSEG2 = 15;
            ciNbtcfg.bF.SJW = 15;
            break;

            // All 250K
        case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 126;
            ciNbtcfg.bF.TSEG2 = 31;
            ciNbtcfg.bF.SJW = 31;
            break;

        case CAN_1000K_4M:
        case CAN_1000K_8M:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 30;
            ciNbtcfg.bF.TSEG2 = 7;
            ciNbtcfg.bF.SJW = 7;
            break;

        case CAN_125K_500K:
            ciNbtcfg.bF.BRP = 0;
            ciNbtcfg.bF.TSEG1 = 254;
            ciNbtcfg.bF.TSEG2 = 63;
            ciNbtcfg.bF.SJW = 63;
            break;

        default:
            return -1;
            break;
    }

    // Write Bit time registers
    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiNBTCFG, ciNbtcfg.word);

    return spiTransferError;
}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO




// *****************************************************************************
// *****************************************************************************
// Section: Miscellaneous

uint32_t DRV_CANFDSPI_DlcToDataBytes(CAN_DLC dlc)
{
    uint32_t dataBytesInObject = 0;

    Nop();
    Nop();

    if (dlc < CAN_DLC_12) {
        dataBytesInObject = dlc;
    } else {
        switch (dlc) {
            case CAN_DLC_12:
                dataBytesInObject = 12;
                break;
            case CAN_DLC_16:
                dataBytesInObject = 16;
                break;
            case CAN_DLC_20:
                dataBytesInObject = 20;
                break;
            case CAN_DLC_24:
                dataBytesInObject = 24;
                break;
            case CAN_DLC_32:
                dataBytesInObject = 32;
                break;
            case CAN_DLC_48:
                dataBytesInObject = 48;
                break;
            case CAN_DLC_64:
                dataBytesInObject = 64;
                break;
            default:
                break;
        }
    }

    return dataBytesInObject;
}


/*
uint16_t DRV_CANFDSPI_CalculateCRC16(uint8_t* data, uint16_t size)
{
    uint16_t init = CRCBASE;
    uint8_t index;

    while (size-- != 0) {
        index = ((uint8_t*) & init)[CRCUPPER] ^ *data++;
        init = (init << 8) ^ crc16_table[index];
    }

    return init;
}
*/

CAN_DLC DRV_CANFDSPI_DataBytesToDlc(uint8_t n)
{
    CAN_DLC dlc = CAN_DLC_0;

    if (n <= 4) {
        dlc = CAN_DLC_4;
    } else if (n <= 8) {
        dlc = CAN_DLC_8;
    } else if (n <= 12) {
        dlc = CAN_DLC_12;
    } else if (n <= 16) {
        dlc = CAN_DLC_16;
    } else if (n <= 20) {
        dlc = CAN_DLC_20;
    } else if (n <= 24) {
        dlc = CAN_DLC_24;
    } else if (n <= 32) {
        dlc = CAN_DLC_32;
    } else if (n <= 48) {
        dlc = CAN_DLC_48;
    } else if (n <= 64) {
        dlc = CAN_DLC_64;
    }

    return dlc;
}



// Section: CAN Receive

int8_t DRV_CANFDSPI_FilterObjectConfigure(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_FILTEROBJ_ID* id)
{
    //uint16_t a = 0;
    REG_CiFLTOBJ fObj;
    int8_t spiTransferError = 0;
	uint32_t new_word;

/*
	new_word = id->SIDB;
	new_word = new_word | (((uint32_t)id->SIDA)<<8);
	new_word = new_word | (((uint32_t)id->EIDA)<<11);
//	new_word = new_word | (((uint32_t)id->EIDB)<<19);
//	new_word = new_word | (((uint32_t)id->EIDC)<<27);
//	new_word = new_word | (((uint32_t)id->SID1)<<29);
//	new_word = new_word | (((uint32_t)id->EXIDE)<<30);

	Nop();
*/

	new_word = id->SIDA;
	new_word = new_word | (((uint32_t)id->SIDB)<<8);
	new_word = new_word | (((uint32_t)id->EIDA)<<11);
	new_word = new_word | (((uint32_t)id->EIDB)<<19);
	new_word = new_word | (((uint32_t)id->EIDC)<<27);
	new_word = new_word | (((uint32_t)id->SID1)<<29);
	new_word = new_word | (((uint32_t)id->EXIDE)<<30);



    // Setup
	fObj.word = new_word;

    // Setup
    //fObj.word = 0;
    //fObj.bF = *id;
    a = cREGADDR_CiFLTOBJ + (filter * CiFILTER_OFFSET);

    spiTransferError = DRV_CANFDSPI_WriteWord(index, a, fObj.word);



    return spiTransferError;
}




int8_t DRV_CANFDSPI_FilterMaskConfigure(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_MASKOBJ_ID* mask)
{
    //uint16_t a = 0;
    REG_CiMASK mObj;
    int8_t spiTransferError = 0;
	uint32_t new_word;

	// Test word
	Nop();
	// Inverse process re-order word

	new_word = mask->MSIDA;
	new_word = new_word | ((uint32_t)mask->MSIDB<<8);
	new_word = new_word | ((uint32_t)mask->MEIDA)<<11;
	new_word = new_word | ((uint32_t)mask->MEIDB)<<19;
	new_word = new_word | ((uint32_t)mask->MEIDC)<<27;
	new_word = new_word | ((uint32_t)mask->MSID1)<<29;
//	new_word = new_word | ((uint32_t)mask->MIDE)<<30;
	Nop();

	// Setup
    mObj.word = new_word;

	
	//acc = ((((((((uint16_t)mObj.bF.MSIDA) & 0x1F4 <<3 | mObj.bF.MSIDB)<<11 | mObj.bF.MEIDA)<<19 | mObj.bF.MEIDB)<<27 | mObj.bF.MEIDC)<<29 |mObj.bF.MSID1)<<30 | mObj.bF.MIDE)<<31 | mObj.bF.unimplemented1; 
    // Setup
	//mObj.bF = acc;
    //mObj.word = 0;
    //mObj.bF = *mask;
    a = cREGADDR_CiMASK + (filter * CiFILTER_OFFSET);

    spiTransferError = DRV_CANFDSPI_WriteWord(index, a, mObj.word);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_FilterToFifoLink(CANFDSPI_MODULE_ID index,
        CAN_FILTER filter, CAN_FIFO_CHANNEL channel, bool enable)
{
    ////uint16_t a = 0;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Enable
    if (enable) {
        fCtrl.bF.Enable = 1;
    } else {
        fCtrl.bF.Enable = 0;
    }

    // Link
    fCtrl.bF.BufferPointer = channel;
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, fCtrl.byte);

    return spiTransferError;
}

/*
int8_t DRV_CANFDSPI_FilterEnable(CANFDSPI_MODULE_ID index, CAN_FILTER filter)
{
    ////uint16_t a = 0;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Read
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &fCtrl.byte);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    fCtrl.bF.Enable = 1;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, fCtrl.byte);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}
*/

int8_t DRV_CANFDSPI_FilterDisable(CANFDSPI_MODULE_ID index, CAN_FILTER filter)
{
    ////uint16_t a = 0;
    REG_CiFLTCON_BYTE fCtrl;
    int8_t spiTransferError = 0;

    // Read
    a = cREGADDR_CiFLTCON + filter;

    spiTransferError = DRV_CANFDSPI_ReadByte(index, a, &fCtrl.byte);
    if (spiTransferError) {
        return -1;
    }

    // Modify
    fCtrl.bF.Enable = 0;

    // Write
    spiTransferError = DRV_CANFDSPI_WriteByte(index, a, fCtrl.byte);
    if (spiTransferError) {
        return -2;
    }

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TefConfigure(CANFDSPI_MODULE_ID index, CAN_TEF_CONFIG* config)
{
    int8_t spiTransferError = 0;

    // Setup FIFO
    REG_CiTEFCON ciTefCon;
    ciTefCon.word = canControlResetValues[cREGADDR_CiTEFCON / 4];

    ciTefCon.bF.FifoSize = config->FifoSize;
    ciTefCon.bF.TimeStampEnable = config->TimeStampEnable;

    spiTransferError = DRV_CANFDSPI_WriteWord(index, cREGADDR_CiTEFCON, ciTefCon.word);

    return spiTransferError;
}

int8_t DRV_CANFDSPI_TransmitQueueConfigure(CANFDSPI_MODULE_ID index,
        CAN_TX_QUEUE_CONFIG* config)
{
#ifndef CAN_TXQUEUE_IMPLEMENTED
    config;
    return -100;
#else
    int8_t spiTransferError = 0;
    uint16_t a = 0;

    // Setup FIFO
    REG_CiTXQCON ciFifoCon;
    ciFifoCon.word = canControlResetValues[cREGADDR_CiFIFOCON / 4];

    ciFifoCon.txBF.TxEnable = 1;
    ciFifoCon.txBF.FifoSize = config->FifoSize;
    ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.txBF.TxAttempts = config->TxAttempts;
    ciFifoCon.txBF.TxPriority = config->TxPriority;

    a = cREGADDR_CiTXQCON;
    spiTransferError = DRV_CANFDSPI_WriteWord(index, a, ciFifoCon.word);

    return spiTransferError;
#endif    
}

int8_t DRV_CANFDSPI_TransmitQueueConfigureObjectReset(CAN_TX_QUEUE_CONFIG* config)
{
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canControlResetValues[cREGADDR_CiFIFOCON / 4];

    config->TxPriority = ciFifoCon.txBF.TxPriority;
    config->TxAttempts = ciFifoCon.txBF.TxAttempts;
    config->FifoSize = ciFifoCon.txBF.FifoSize;
    config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;

    return 0;
}