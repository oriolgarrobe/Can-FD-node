// A simple template in C language
// Manuel Moreno-Eguilaz, BarcelonaTech

#include<p18f4520.h>	// MCU Registers

#include "drv_canfdspi_api.h"
#include "drv_canfdspi_defines.h"
#include "drv_spi.h"
#include "drv_canfdspi_register.h"
#include "main.h"
#include "spi.h"
#include "stdlib.h"
// Global variables

unsigned char i;
unsigned int address;
unsigned char v_data[4];

uint8_t length;
bool good = false;

// Transmit objects
CAN_TX_FIFO_CONFIG txConfig;
CAN_TX_FIFO_EVENT txFlags;
CAN_TEF_FIFO_EVENT tefFlags;
CAN_TX_MSGOBJ txObj;
uint8_t txd[MAX_DATA_BYTES]; //MAX_DATA_BYTES 64, tengo que poner aquí 64 bytes? 

// Receive objects
CAN_RX_FIFO_CONFIG rxConfig;
REG_CiFLTOBJ fObj;
REG_CiMASK mObj;
CAN_RX_FIFO_EVENT rxFlags;
CAN_RX_MSGOBJ rxObj;
uint8_t rxd[MAX_DATA_BYTES];
CAN_TEF_MSGOBJ tefObj;
CAN_TEF_CONFIG tefConfig;
//CAN_TX_QUEUE_CONFIG txqConfig;

bool ramInitialized = 0;

CAN_CONFIG config;
CAN_OPERATION_MODE opMode;

CAN_BITTIME_SETUP selectedBitTime = CAN_250K_2M;//CAN_500K_2M;
uint8_t ip[64] = {0x01,0x44,0x33,0x55};
uint32_t iprx[16];



void main(void)
{
int8_t code;
uint8_t n;
uint8_t error;
uint32_t value=0x00;
uint16_t address;
uint8_t valuebyte=0x33;
uint8_t var=0x55;

int i;
uint8_t length;
uint32_t id;
REG_CiCON reg;
		// Variable initialization
		i = 0;
/*
TRISBbits.TRISB0 = 0;
while(1)
{
LATBbits.LATB0 = !LATBbits.LATB0;
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();
for(i=0;i<250;i++) Nop();

}
*/




SPI_CS = 1;
CONFIG_SPI_CS = 0; // output
DRV_SPI_Initialize();


	address = 0x010;
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteHalfWord(0, address, 0x5A5A);
	DRV_CANFDSPI_WriteHalfWord(0, 0x012, 0xAAAA);
	DRV_CANFDSPI_ReadWord(0, address, &value);
	Nop();


/*

	for (i=0; i<64; i++ ) 
	ip[i]=64-i;

	address = 0x401;
	
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteByteArray(0, address, ip, 64);	
	DRV_CANFDSPI_ReadWordArray(0, address, iprx, 16);
	Nop();

	
	//Test 4
	
	//Test WriteHalfWord & ReadHalfWord

	address = 0x010;
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteHalfWord(0, address, 0x5A5A);
	DRV_CANFDSPI_WriteHalfWord(0, 0x012, 0xAAAA);
	DRV_CANFDSPI_ReadWord(0, address, &value);
	Nop();



	//Test WriteWord & ReadWord


	address = 0x400;
	DRV_CANFDSPI_WriteWord(0, address, 0x11223344);
	DRV_CANFDSPI_ReadWord(0, address, &value);
	Nop();



	//Test WriteByte & ReadByte

	//Test 1

	address = 0x010;
	
	DRV_CANFDSPI_Reset(0);	
	DRV_CANFDSPI_WriteByte(0, address, 0x5A);
	DRV_CANFDSPI_ReadByte(0, address, &value);
	Nop();

	//Test 2

	address = 0x010;
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteByte(0, 0x010, 0x55);
	DRV_CANFDSPI_WriteByte(0, 0x011, 0x55);
	DRV_CANFDSPI_WriteByte(0, 0x012, 0x55);
	DRV_CANFDSPI_WriteByte(0, 0x013, 0x55);
	DRV_CANFDSPI_ReadWord(0, address, &value);
	Nop();
	


	//Test WriteHalfWord & ReadHalfWord



	//Test 1

	address = 0x010;
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteHalfWord(0, address, 0x5A5A);
	DRV_CANFDSPI_ReadHalfWord(0, address, &value);
	Nop();

	//Test 2

	address = 0x010;
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteHalfWord(0, address, 0x5555);
	DRV_CANFDSPI_WriteHalfWord(0, 0x012, 0xAAAA);	
	DRV_CANFDSPI_ReadWord(0, address, &value);
	Nop();
	
	
	


	//Test WriteByteArray & ReadByteArray
	
	address = 0x400;
	
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_WriteByteArray(0, address, ip, 4);	
	DRV_CANFDSPI_ReadByteArray(0, address, iprx, 4);
	Nop();
	

SPI_CS = 0;
address = 0xE00;
WriteSPI((uint8_t)((cINSTRUCTION_READ<<4) + ((address>>8) & 0xF)));
WriteSPI((uint8_t)(address & 0xFF));
v_data[0] = ReadSPI();
v_data[1] = ReadSPI();
v_data[2] = ReadSPI();
v_data[3] = ReadSPI();
SPI_CS = 1;

*/
//while(1)
//{
//	SPI_CS = 0;
//	error = DRV_CANFDSPI_ReadWord(0,0xE00,&value);
//	SPI_CS = 1;
//	Nop();
//}
//Nop();
//



//while(1);


// Reset device
    DRV_CANFDSPI_Reset(0);

    // Enable ECC and initialize RAM
    DRV_CANFDSPI_EccEnable(DRV_CANFDSPI_INDEX_0);

    if (!ramInitialized) {
        DRV_CANFDSPI_RamInit(DRV_CANFDSPI_INDEX_0, 0x00); //0xff);
	//DRV_CANFDSPI_RamInit2(DRV_CANFDSPI_INDEX_0, 0xFF); //0xff);
        ramInitialized = true;
    }

	address = 0xBF4;
/*	
	DRV_CANFDSPI_Reset(0);
	DRV_CANFDSPI_ReadByteArray(0, address, iprx, 16);
	Nop();
*/
    // Configure device
    DRV_CANFDSPI_ConfigureObjectReset(&config);
    config.IsoCrcEnable = ISO_CRC;
    config.StoreInTEF = 0;
	config.TXQEnable = 0;
    DRV_CANFDSPI_Configure(0, &config);


/*
    // Setup TX FIFO
	// Original test
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig);
    txConfig.FifoSize = 0; //7;
    txConfig.PayLoadSize = CAN_PLSIZE_64;
    txConfig.TxPriority = 0;
//	txConfig.TxEnable = 1; 
    DRV_CANFDSPI_TransmitChannelConfigure(0, APP_TX_FIFO, &txConfig);
*/
	// Test 1 TXFIFO
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig);
    txConfig.FifoSize = 0;//7;
    txConfig.PayLoadSize = CAN_PLSIZE_64;
    txConfig.TxPriority = 0;
//	txConfig.TxEnable = 1; 
    DRV_CANFDSPI_TransmitChannelConfigure(0, APP_TX_FIFO, &txConfig);



	



/*

    // Setup RX FIFO
	// Original test
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig);
    rxConfig.FifoSize = 0;
    rxConfig.PayLoadSize = CAN_PLSIZE_64;
	rxConfig.RxTimeStampEnable = 1; 
    DRV_CANFDSPI_ReceiveChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxConfig);

*/

	// Test 1 RXFIFO
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig);
    rxConfig.FifoSize = 15;
    rxConfig.PayLoadSize = CAN_PLSIZE_64;
	rxConfig.RxTimeStampEnable = 1; 
    DRV_CANFDSPI_ReceiveChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxConfig);


	//Setup TEF Configuration
	tefConfig.FifoSize = 11; 
	tefConfig.TimeStampEnable = 1;
	DRV_CANFDSPI_TefConfigure(0, &tefConfig); 

	//Setup TXQ Configuration
//	DRV_CANFDSPI_TransmitQueueConfigureObjectReset(&txqConfig);
//    txqConfig.TxPriority = 1;
//    txqConfig.FifoSize = 7;
//    txqConfig.PayLoadSize = CAN_PLSIZE_32;
//    DRV_CANFDSPI_TransmitQueueConfigure(DRV_CANFDSPI_INDEX_0, &txqConfig);
//



	



	DRV_CANFDSPI_FilterDisable(0, CAN_FILTER0);

    // Setup RX Filter
    fObj.word = 0;
    //fObj.bF.SID = 0xda;
	fObj.bF.SIDA = (0x400>>8) & (0x07); //Original
	fObj.bF.SIDB = 0x400 & (0xff) ;//Original
	//txObj.bF.id.SID1 = 0x300 >> 3 ;
	//txObj.bF.id.SID2 = 0x300 & (0x03); 
    fObj.bF.EXIDE = 0; //0;
    //fObj.bF.EID = 0x00;
	fObj.bF.EIDA = 0x00;
	fObj.bF.EIDB = 0x00;
	fObj.bF.EIDC = 0x00;

    DRV_CANFDSPI_FilterObjectConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &fObj.bF);

    // Setup RX Mask
    mObj.word = 0;
    //mObj.bF.MSID = 0x0;
	mObj.bF.MSIDA = 0x00;
	mObj.bF.MSIDB = 0x00;
    mObj.bF.MIDE = 1; // Only allow standard IDs
    //mObj.bF.MEID = 0x0;
	mObj.bF.MEIDA = 0x0;
	mObj.bF.MEIDB = 0x0;
    DRV_CANFDSPI_FilterMaskConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &mObj.bF);

    // Link FIFO and Filter
    DRV_CANFDSPI_FilterToFifoLink(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, APP_RX_FIFO, true);



    // Setup Bit Time
    DRV_CANFDSPI_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);



    // Setup Transmit and Receive Interrupts
////    DRV_CANFDSPI_GpioModeConfigure(DRV_CANFDSPI_INDEX_0, GPIO_MODE_INT, GPIO_MODE_INT);
//    DRV_CANFDSPI_TransmitChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
//    DRV_CANFDSPI_ReceiveChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
//    DRV_CANFDSPI_ModuleEventEnable(DRV_CANFDSPI_INDEX_0, CAN_TX_EVENT | CAN_RX_EVENT);
//
    // Select Normal Mode
    //DRV_CANFDSPI_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_NORMAL_MODE);
	DRV_CANFDSPI_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_EXTERNAL_LOOPBACK_MODE);





	while(1)	// An infinite Loop
	{

			//TRANSMIT USING FIFO STATUS
	
            // Configure transmit message
            txObj.word[0] = 0;
            txObj.word[1] = 0;

			//id = rand() & 0x7FF;
			id = 0x400; 
			txObj.bF.id.SIDA = (id >> 8)&0x07 ; //Original
			txObj.bF.id.SIDB = id & (0xFF); //Original
			//txObj.bF.id.SIDA = (0x500 >> 8)&0x07 ; //Original
			//txObj.bF.id.SIDB = 0x500 & (0xFF); //Original
			txObj.bF.id.EIDA = 0;
			txObj.bF.id.EIDB = 0;
			txObj.bF.id.EIDC = 0;

            txObj.bF.ctrl.BRS = 1;
            txObj.bF.ctrl.DLC = CAN_DLC_64;
            //txObj.bF.ctrl.DLC = rand() & 0x0F;//CAN_DLC_64;
			txObj.bF.ctrl.FDF = 1;
            txObj.bF.ctrl.IDE = 0;

            txObj.bF.ctrl.SEQ = 0; //// Sequence: doesn't get transmitted, but will be stored in TEF. Can be read afterwards with Read TEF

            // Configure message data
            
            for (i = 0; i < MAX_DATA_BYTES; i++) 
			{
				//txd[i] = rand() & 0xFF;//data;
				//txd[i] = i;
				//txd[i] = 0xFF;
				txd[i] = 64-i; 
				rxd[i] = 0;
			}


		//TX
		DRV_CANFDSPI_TransmitChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txFlags);
 		while (!(txFlags & CAN_TX_FIFO_NOT_FULL_EVENT));
		n = DRV_CANFDSPI_DlcToDataBytes(txObj.bF.ctrl.DLC);
		code = DRV_CANFDSPI_TransmitChannelLoad(0, APP_TX_FIFO, &txObj, txd, n, true); 
			// End of infinite loop
		//RX
		DRV_CANFDSPI_ReceiveChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxFlags);
		if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) 
		{
			DRV_CANFDSPI_ReceiveMessageGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxObj, rxd, MAX_DATA_BYTES);
			Nop();
			Nop();
			Nop();
			Nop();
		}	
	}
}
	



////Això: 13/05/2019
//
//            // RAM ACCES TEST
//
//
//            // Verify read/write with different access length
//            // Note: RAM can only be accessed in multiples of 4 bytes
//            for (length = 4; length <= MAX_DATA_BYTES; length += 4) {
//                for (i = 0; i < length; i++) {
//                    txd[i] = rand() & 0xff;
//                    rxd[i] = 0xff;
//                }
//
//                // Write data to RAM
//                DRV_CANFDSPI_WriteByteArray(DRV_CANFDSPI_INDEX_0, cRAMADDR_START, txd, length);
//
//                // Read data back from RAM
//                DRV_CANFDSPI_ReadByteArray(DRV_CANFDSPI_INDEX_0, cRAMADDR_START, rxd, length);
//
//                // Verify
//                for (i = 0; i < length; i++) {
//                    good = txd[i] == rxd[i];
//
//                    if (!good) {
//                        Nop();
//                        Nop();
//
//                        // Data mismatch
//                    }
//                }
//            }
//        
//
//    
//            // REGISTER ACCES TEST
//
//            // Verify read/write with different access length
//            // Note: registers can be accessed in multiples of bytes
//            for (length = 1; length <= MAX_DATA_BYTES; length++) {
//                for (i = 0; i < length; i++) {
//                    txd[i] = rand() & 0x7f; // Bit 31 of Filter objects is not implemented
//                    rxd[i] = 0xff;
//                }
//                // Write data to registers
//                DRV_CANFDSPI_WriteByteArray(DRV_CANFDSPI_INDEX_0, cREGADDR_CiFLTOBJ, txd, length);
//
//                // Read data back from registers
//                DRV_CANFDSPI_ReadByteArray(DRV_CANFDSPI_INDEX_0, cREGADDR_CiFLTOBJ, rxd, length);
//
//                // Verify
//
//                for (i = 0; i < length; i++) {
//                    good = txd[i] == rxd[i];
//
//                    if (!good) {
//                        Nop();
//                        Nop();
//
//                        // Data mismatch
//                    }
//                }
//            }
//
//            Nop();
//            Nop();
//
//
//		// READ TEF MESSAGE
//
//            // TEF Object
//
//
//            //Check that TEF is not empty
//
//            DRV_CANFDSPI_TefEventGet(DRV_CANFDSPI_INDEX_0, &tefFlags);
//
//            if (tefFlags & CAN_TEF_FIFO_NOT_EMPTY_EVENT) {
//                // Read message and UINC
//                DRV_CANFDSPI_TefMessageGet(DRV_CANFDSPI_INDEX_0, &tefObj);
//
//                // Process message
//                Nop();
//                Nop();
//                id = tefObj.bF.id.EID1;
//			}
//
//            Nop();
//			Nop();
//}

