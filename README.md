# CanFD node based on a PIC18 Microcontroller

Click here to access the full report [:page_with_curl:](https://github.com/oriolgarrobe/Can-FD-node/blob/master/TFG%20final%20-%20Oriol%20Garrobe.pdf)

Paper of the project can also be found in the UPC database [:books:](https://upcommons.upc.edu/handle/2117/177408?locale-attribute=en)

As part of a Bachelor thesis, in this repo there is the developement of a CAN node based on a PIC18 microcontroller and an MCP2517FD click. The system has been programmed using C language. 

* The development of the driver for this project is based on an existing solution from [Microchip](https://www.waveshare.com/wiki/Microchip_Datasheets#PIC18F4_Series)

* The source code that is used in this project is a variation of the MCP2517FD canfdspi API for a [PIC32MX470](https://www.microchip.com/wwwproducts/en/MCP2517FD)

## Review

On the first hand, the architecture of the electronic system as well as the CAN FD protocol is explained. Briefly it is shown how the CAN FD frames are, in order to leave it clear for the reader, as the main object of the project is to transfer data through the bus CAN. Therefore, the structure of the data is of great relevance. Also, the architecture of the electronic system as well as its components, both hardware and software, is detailed.

On the other hand, it is explained step by step how to implement the system. The PIC18 microcontroller family from Microchip includes an 8 bit CPU, whereas the MCP2517FD
CANFD controller is oriented to work with 4 byte word. From this regard, the main issue is to make these two devices compatible. It will be then, a good exercise to show how to link different devices with different bandwidths, with a method that is applicable to any device.

Finally, once the implementation is finished, several tests to prove that the system works properly are included. One of the main features of the CAN FD protocol is that it can transfer as well as receive data with different payloads and different baudrates, hence it will be demonstrated that the node is able to process any data frame in any rate set in the CAN FD protocol using a CAN FD sniffer. 


