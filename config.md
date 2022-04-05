* board selection: L4R5ZI-P
* rpi uart serial
	- Connectivity -> LPUART1
	- mode: async
	- flow control: CTS/RTS
	- baud: 115200
	- word length: 8 bits
	- nucleo pins
		- CTS: A6
		- RTS: B1
		- RX:  C0
		- TX:  C1
	- rpi pins (pin numbers not gpio numbers)
		- RTS: 11
		- CTS: 36
		- TX:   8
		- RX:  10
	- nucleo TX -> rpi RX
	- nucleo RX <- rpi TX
	- nucleo CTS A6 <- rpi RTS
	- nucleo RTS B1 -> rpi CTS
* GPS
	* Go to USART# (will be using USART3 in general for this instruction)
	* Set Mode of USART to Asynchronous and set baud rate to 9600 and enable Fifo Mode
	* Make sure that the Rx port from the GPS goes to the Tx port of the microcontroller and the Tx from GPS goes to Rx
	* Create a global variable of an array of "uint8_t" and then another array (for debugging) of "uint8_t in the main function (about size of 100)
	* Write the function in main (or whenever requesting GPS coordinates): HAL_StatusTypeDef GPSStatus = HAL_UART_Receive(&huart3, (uint8_t*) testData, 100, 5000);
	* HAL_UART_Receive(&huart#, global array of uint8_t, DATA_SIZE, time to get data)
	* look for $GPGGA in the array
	* Copy and paste the code for the GPS structure and function from the word document
	* Make sure it works in the field can't in the lab
	* nucleo pins
		- TX: D8
		- RX: D9