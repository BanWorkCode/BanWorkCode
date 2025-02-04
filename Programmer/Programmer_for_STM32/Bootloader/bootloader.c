#include "bootloader.h"
#include "stdio.h"
#include "string.h"
#include "displayMenu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sd_card.h"
#include "usart.h"
#include "cmsis_os.h"

//
//
//=====================================================================================================================================
//
/*


		= {{, , },{,,},{,,}};
		= {, , };
*/
//добавить static
_Bool sectorNumbersForErase[MAX_NUMBER_OF_SECTORS];


PROGRAMMER_MemoryAllocated_t  id410[1] = {0x08000000, 0x80, 0x400};
PROGRAMMER_MemoryAllocated_t  id411[3] = {{0x08000000, 0x4, 0x4000},{0x08010000,0x1,0x10000},{0x08020000,0x7,0x20000}};
PROGRAMMER_MemoryAllocated_t  id412[1] = {0x08000000, 0x20, 0x400};
PROGRAMMER_MemoryAllocated_t  id413[3] = {{0x08000000, 0x4, 0x4000},{0x08010000,0x1,0x10000},{0x08020000,0x7,0x20000}};
//<FlashSize address="0x1FFFF7CC" default="0x100000"/>
PROGRAMMER_MemoryAllocated_t  id414[1] = {0x08000000, 0x100, 0x800};
//PROGRAMMER_MemoryAllocated_t  id415
//<FlashSize address="0x1FFF75E0" default="0x100000"/>


static PROGRAMMER_Device_t stm32[NUMBER_OF_DEVICES] =
{
  {0x440, 0x21, "STM32F05xxx and STM32F030x8"},
  {0x444, 0x10, "STM32F03xx4/6"},
  {0x442, 0x52, "STM32F030xC"},
  {0x445, 0xA1, "STM32F04xxx"},
  {0x445, 0xA2, "STM32F070x6"},
  {0x448, 0xA2, "STM32F070xB"},
  {0x448, 0xA1, "STM32F071xx/072xx"},
  {0x442, 0x50, "STM32F09xxx"},
  {0x412, 0x00, "STM32F10xxx, Low-density"},
  {0x410, 0x00, "STM32F10xxx, Medium-density",1,id410},
  {0x414, 0x00, "STM32F10xxx, High-density"},
  {0x420, 0x10, "STM32F10xxx, Medium-density value line"},
  {0x428, 0x10, "STM32F10xxx, High-density value line"},
  {0x418, 0x00, "STM32F105xx/107xx"},
  {0x430, 0x21, "STM32F10xxx XL-density"},
  {0x411, 0x20, "STM32F2xxxx"},//BID=1FFF77DE //30686
  {0x411, 0x33, "STM32F2xxxx"},
  {0x432, 0x41, "STM32F373xx"},
  {0x432, 0x50, "STM32F378xx"},
  {0x422, 0x41, "STM32F302xB(C)/303xB(C)"},
  {0x422, 0x50, "STM32F358xx"},
  {0x439, 0x40, "STM32F301xx/302x4(6/8)"},
  {0x439, 0x50, "STM32F318xx"},
  {0x438, 0x50, "STM32F303x4(6/8)/334xx/328xx"},
  {0x446, 0x40, "STM32F302xD(E)/303xD(E)"},
  {0x446, 0x50, "STM32F398xx"},
  {0x413, 0x31, "STM32F40xxx/41xxx"},//
  {0x413, 0x90, "STM32F40xxx/41xxx",3,id413},//-
  {0x419, 0x70, "STM32F42xxx/43xxx"},//
  {0x419, 0x91, "STM32F42xxx/43xxx"},//-
  {0x423, 0xD1, "STM32F401xB(C)"},//
  {0x433, 0xD1, "STM32F401xD(E)"},//BID=1FFF76DE //30430
  {0x458, 0xB1, "STM32F410xx"},//
  {0x431, 0xD0, "STM32F411xx"},//
  {0x441, 0x90, "STM32F412xx"},//
  {0x421, 0x90, "STM32F446xx"},//
  {0x434, 0x90, "STM32F469xx/479xx"},//
  {0x463, 0x90, "STM32F413xx/423xx"},
  {0x452, 0x90, "STM32F72xxx/73xxx"},
  {0x449, 0x70, "STM32F74xxx/75xxx"},
  {0x449, 0x90, "STM32F74xxx/75xxx"},
  {0x451, 0x93, "STM32F76xxx/77xxx"},
  {0x466, 0x52, "STM32G03xxx/04xxx"},
  {0x460, 0xB2, "STM32G07xxx/08xxx"},
  {0x468, 0xD4, "STM32G431xx/441xx"},
  {0x469, 0xD5, "STM32G47xxx/48xxx"},
  {0x483, 0x91, "STM32H72xxx/73xxx"},
  {0x450, 0x90, "STM32H74xxx/75xxx"},
  {0x480, 0x90, "STM32H7A3xx/B3xx"},
  {0x457, 0xC3, "STM32L01xxx/02xxx"},
  {0x425, 0xC0, "STM32L031xx/041xx"},
  {0x417, 0xC0, "STM32L05xxx/06xxx"},
  {0x447, 0x41, "STM32L07xxx/08xxx"},
  {0x447, 0xB2, "STM32L07xxx/08xxx"},
  {0x416, 0x20, "STM32L1xxx6(8/B)"},
  {0x429, 0x20, "STM32L1xxx6(8/B)A"},
  {0x427, 0x40, "STM32L1xxxC"},
  {0x436, 0x45, "STM32L1xxxD"},
  {0x437, 0x40, "STM32L1xxxE"},
  {0x464, 0xD1, "STM32L412xx/422xx"},
  {0x435, 0x91, "STM32L43xxx/44xxx"},
  {0x462, 0x92, "STM32L45xxx/46xxx"},
  {0x415, 0xA3, "STM32L47xxx/48xxx"},
  {0x415, 0x92, "STM32L47xxx/48xxx"},
  {0x461, 0x93, "STM32L496xx/4A6xx"},
  {0x470, 0x95, "STM32L4Rxx/4Sxx"},
  {0x471, 0x90, "STM32L4P5xx/Q5xx"},
  {0x472, 0x92, "STM32L552xx/562xx"},
  {0x495, 0xD5, "STM32WB30xx/35xx/50xx/WB55xx"},
  {0x497, 0xC3, "STM32WLE5xx/WL55xx"}
};






/*
<DeviceID>0x410</DeviceID>
<Parameters address="0x08000000" name="sector0" occurence="0x80" size="0x400"/>


<DeviceID>0x413</DeviceID>
			<Field>
							<Parameters address="0x08000000" name="sector0" occurence="0x4" size="0x4000"/>
						</Field>
						<Field>
							<Parameters address="0x08010000" name="sector4" occurence="0x1" size="0x10000"/>
						</Field>
						<Field>
							<Parameters address="0x08020000" name="sector5" occurence="0x7" size="0x20000"/>
						</Field>

*/
//
//
static PROGRAMMER_DeviceData_t g_deviceData;
PROGRAMMER_DeviceData_t* g_pConnectedDeviceData = &g_deviceData;
//
//
//=====================================================================================================================================
//
static inline uint8_t PROGRAMMER_SumToByte ( char c )
{
  return ( uint8_t ) ( c <= '9' ? c - '0' : c - 'A' + 10 );
}
//
//
//=====================================================================================================================================
//
static void PROGRAMMER_ReseteTarget ( uint16_t delay )
{
  GPIOA->BSRR = GPIO_BSRR_BR4; //Resets the corresponding ODRx bit
  HAL_Delay ( 200 ); //osDelay(10);
  GPIOA->BSRR = GPIO_BSRR_BS4; //Sets the corresponding ODRx bit
  HAL_Delay ( delay ); //osDelay(10)
	


}
//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_InitTarget ( void )
{
  uint8_t command = 0x7F;
  if ( HAL_UART_Transmit ( &huart2, &command, 1, 100 ) )
    {
      return 1;
    }
  uint8_t returnedValue;
  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
    {
      return 1;
    }
  if ( returnedValue == 0x79 )
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_GetSupportedCommands ( void )
{
  uint8_t command[2] = {0x00, 0xFF};
  if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
    {
      return 1;
    }
  uint8_t returnedValue[15];
  if ( HAL_UART_Receive ( &huart2, returnedValue, 15, 100 ) )
    {
      return 1;
    }
  if ( returnedValue[0] == 0x79 && returnedValue[1] == 0x0b && returnedValue[14] == 0x79 )
    {
      for ( int i = 0; i < 12; i++ )
        {
          * ( ( uint8_t* ) g_pConnectedDeviceData + i ) = returnedValue[i + 2];
        }
      return 0;
    }
  else
    {
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_GetIDCommand ( void )
{
  uint8_t command[2] = {g_deviceData.getID, 0xFF - g_deviceData.getID};
  if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
    {
      return 1;
    }
  uint8_t returnedValue[5];
  if ( HAL_UART_Receive ( &huart2, returnedValue, 5, 100 ) )
    {
      return 1;
    }
  if ( returnedValue[0] == 0x79 && returnedValue[1] == 0x01 && returnedValue[4] == 0x79 )
    {
      g_deviceData.targetPID = 	( ( int16_t ) returnedValue[2] ) << 8 | ( int16_t ) returnedValue[3];
      return 0;
    }
  else
    {
      return 1;
    }
}

//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_GetCurrentDevice ( void )
{
  for ( uint8_t i = 0; i < NUMBER_OF_DEVICES; i++ )
    {
      if ( g_deviceData.targetPID == stm32[i].targetPID )
        {
          g_deviceData.currentDevice = &stm32[i];
          return 0;
        }
    }
  return 1;
}
//
//
//=====================================================================================================================================
//
void* PROGRAMMER_InitBootloader ( void* currentItem )
{
  _Bool status = 0;
  //длительность задержки зависит от МК, описана в AN2606
  PROGRAMMER_ReseteTarget ( 100 );
  if ( PROGRAMMER_InitTarget() )
    {
      status = 1;
    }
  if ( status == 1 )
    {
      PROGRAMMER_ReseteTarget ( 2600 );
      if ( PROGRAMMER_InitTarget() )
        {
          status = 1;
        }
      else
        {
          status = 0;
        }
    }
  if ( status == 0 )
    {
      if ( PROGRAMMER_GetSupportedCommands() )
        {
          status = 1;
        }
    }
  if ( status == 0 )
    {
      if ( PROGRAMMER_GetIDCommand() )
        {
          status = 1;
        }
    }
//  if ( status == 0 )
//    {
//      if ( PROGRAMMER_GetTargetName() )
//        {
//          status = 1;
//        }
//    }
  if ( status == 0 )
    {
      if ( PROGRAMMER_GetCurrentDevice() )
        {
          status = 1;
        }
    }
  if ( status )
    {
      g_infoMessage =  "ERROR: Initializing the bootloader!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      //g_infoMessage = g_deviceData.targetName;
      g_infoMessage = g_deviceData.currentDevice->targetName;
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_FullEraseMemory(void){

  uint8_t command[2] = {g_deviceData.eraseCommand, 0xFF - g_deviceData.eraseCommand};
  if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
    {
      return 1;
    }
  uint8_t returnedValue = 0x00;
  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
    {
      return 1;
    }
  if ( returnedValue == 0x79 )
    {
      command[0] = 0xFF;
      command[1] = 0x00;
      if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
        {
          return 1;
        }
      returnedValue = 0x00;
      if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 10000 ) )
        {
          return 1;
        }
				
				
    }
  else
    {
      return 1;	
    }
		
		
		  if ( returnedValue == 0x79 )
    {
		return 0;	
		}
		  else
    {
      return 1;	
    }

}



//
//
//=====================================================================================================================================
//

static _Bool PROGRAMMER_EraseMemorySectors ( void )
{
 
  uint8_t command[257] = {g_deviceData.eraseCommand, 0xFF - g_deviceData.eraseCommand};
  if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
    {
      return 1;	
    }
  uint8_t returnedValue = 0x00;
  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
    {
      return 1;	
    }
  if ( returnedValue == 0x79 )
    {
//			command[0] = 0x00; //N-1(1-1=0)
//      command[1] = 0x01;
//			command[2] = 0x01;  //Checksum  XOR
      //определяем сколько страниц надо отчистить
      uint16_t numberOfSectorsToBeErased =0;
      uint16_t currentPosition =1;
      for ( uint16_t i=0; i<MAX_NUMBER_OF_SECTORS; i++ )
        {
          if ( sectorNumbersForErase[i]==1 )
            {
              numberOfSectorsToBeErased++;
              command[currentPosition]=	i;
              currentPosition++;
            }
        }
      command[0] = numberOfSectorsToBeErased-1;
      for ( uint16_t i=0; i< ( numberOfSectorsToBeErased+1 ); i++ )
        {
          command[currentPosition]^=command[i];
        }
      /*
      command[0] = 0x01; //N-1(1-1=0)
      command[1] = 0x01;
      command[2] = 0x02;
      command[3] = 0x02; //Checksum  XOR

      */
      if ( HAL_UART_Transmit ( &huart2, command, numberOfSectorsToBeErased+2, 100 ) )
        {
          return 1;	
        }
      returnedValue = 0x00;
      if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 10000 ) )
        {
         return 1;	
        }
    }
  else
    {
      return 1;
    }
  if (  returnedValue == 0x79 )
    {
			return 0;
      

    }
  else
    {
     return 1;
    }
  
}
//
//
//=====================================================================================================================================
//
void* PROGRAMMER_EraseMemory ( void* currentItem )
{
  if ( PROGRAMMER_FullEraseMemory() )
    {
      g_infoMessage = "ERROR: Erase";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_infoMessage = "OK: Erase";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_GetAddressesOfSectorsToCleanUp ( void )
{
  typedef union
  {
    uint8_t ForWrite[4];
    uint32_t ForRead;

  } Adress;
  Adress static startAdress;
 
  _Bool status = 0;
  _Bool running = 1;
  uint8_t numberOfBytes = 0;
  //static uint8_t startAdressForWrite[4];
  if ( SD_OpenFileOnCard() )
    {
      status = 1;
      g_infoMessage = "Get String error";
    }
  char processingTheString[MAX_NUMBER_OF_CHARACTERS_IN_HEX_FILE_LINE];
  while ( running && status == 0 )
    {
      if ( SD_GetStringFromFileOnCard ( processingTheString, MAX_NUMBER_OF_CHARACTERS_IN_HEX_FILE_LINE ) )
        {
          status = 1; //если пустая строка, то это ошибка, потому что до нее не доджно дойти,\
        выход из цикла в последней  сроке 	switch case 0x31:
        g_infoMessage = "Get String error";
        break;
      }
    else
      {


          switch ( processingTheString[8] )
            {
            case 0x31: //  запись обозначает конец файла, данных не содержит.
              running = 0; //выход из цикла
              break;
            case 0x34: // запись расширенного адреса
              startAdress.ForWrite[3] = ( PROGRAMMER_SumToByte ( processingTheString[9] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[10] );
              startAdress.ForWrite[2] = ( PROGRAMMER_SumToByte ( processingTheString[11] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[12] );
              break;
            case 0x30:   //запись содержит данные двоичного файла
            {
              startAdress.ForWrite[1] = ( PROGRAMMER_SumToByte ( processingTheString[3] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[4] );
              startAdress.ForWrite[0] = ( PROGRAMMER_SumToByte ( processingTheString[5] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[6] );
              numberOfBytes = ( PROGRAMMER_SumToByte ( processingTheString[1] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[2] );
              
							sprintf (g_infoMessageArr, "%s%08x", "Check memory     0x", startAdress.ForRead );
							g_infoMessage = g_infoMessageArr;
							
							//Сравнение адресов в прошивке с адресами секторов полученных от программируемого контроллера
							
							_Bool adressIsCorrect =0;
              for ( uint16_t numRec =0; numRec<g_deviceData.currentDevice->numRecords; numRec++ )
                {
                  if ( startAdress.ForRead >=g_deviceData.currentDevice->memoryAllocated[numRec].address )
                    {
                      for ( uint16_t i =0; i<g_deviceData.currentDevice->memoryAllocated[numRec].occurence; i++ )
                        {
                          if ( startAdress.ForRead<	g_deviceData.currentDevice->memoryAllocated[numRec].address+ ( ( i+1 ) *g_deviceData.currentDevice->memoryAllocated[numRec].size ) )
                            {
															adressIsCorrect =1;                                                  
                              sectorNumbersForErase[i]=1;
                              break;
                            }
                        }                  
                    }              
                }
								
								
								
								if (adressIsCorrect==0){
								status =1;
								g_infoMessage = "Incorrect adress in HEX file for this microcontroller";	
									
								}
								
								
            }
            break;
            default:
              break;
            }
          //конец обработки строки
        }
    }//End while
  if ( SD_CloseFileOnCard() )
    {
      status = 1;
    }
  if ( status == 0 )
    {  
			return 0;
    }
		else{
		return 1;
		}
		
	
		
  
}
//
//
//=====================================================================================================================================
//
void* PROGRAMMER_UploadFirmwareAfterFullErase ( void* currentItem ){
	
	DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_UPLOADING );
	//Проверяем что выбран файл для загрузки	
	if (selectedFileToUpload.name[0]==0 || selectedFileToUpload.path[0]==0){
		g_infoMessage = "Error: file to upload is not selected.";
		DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
	}
	else{
		
		
	if (PROGRAMMER_FullEraseMemory()){
	g_infoMessage = "Full erase memory error!";
	DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
	
	}
	else{
	g_infoMessage = "Memory erased!";
		
		

	if (PROGRAMMER_UploadFirmware()==0)
g_infoMessage = "Upload successful";	
DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );	
		
	}
	
	}
	
	return currentItem;
}

//
//
//=====================================================================================================================================
//
void* PROGRAMMER_UploadFirmwareAfterEraseSectors ( void* currentItem ){
	
	DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_UPLOADING );
	_Bool status = 0;
	
	//Проверяем что выбран файл для загрузки	
	if (selectedFileToUpload.name[0]==0 || selectedFileToUpload.path[0]==0){
		g_infoMessage = "Error: file to upload is not selected.";
		DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
		status =1;
	}
	
	
	
	if (status ==0){
	if (PROGRAMMER_GetAddressesOfSectorsToCleanUp()){	
	status =1;
		//	g_infoMessage устанавливается внутри функции PROGRAMMER_GetAddressesOfSectorsToCleanUp
	DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );	
	}
	}
		
	if (status ==0){
	g_infoMessage = "Erasing process is in progress";
	if (PROGRAMMER_EraseMemorySectors ()){
	status =1;		
	}
	}
		
if (status ==0){
if (PROGRAMMER_UploadFirmware()==0)
g_infoMessage = "Upload successful";	
DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
}
else{
g_infoMessage = "Upload error";	
DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
}

	
	
	
	return currentItem;
}

//
//
//=====================================================================================================================================
//
static _Bool PROGRAMMER_UploadFirmware ( void )
{
  _Bool status = 0;
  uint8_t command[2] = { g_deviceData.writeMemoryCommand,
                         0xFF - g_deviceData.writeMemoryCommand
                       };
  uint8_t dataPackageToUpload[256];
  uint8_t returnedValue = 0x00;
  static uint8_t startAdressForWrite[4];
  uint8_t checksumStartAdress;
  uint8_t numberOfBytesMinusOne;
  uint8_t checksumOfString = 0;
  uint8_t numberOfBytes = 0;
  _Bool running = 1;
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_UPLOADING );
	
  if ( SD_OpenFileOnCard() )
    {
      status = 1;
      g_infoMessage = "Get String error";
			DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  char processingTheString[MAX_NUMBER_OF_CHARACTERS_IN_HEX_FILE_LINE];
  while ( running && status == 0 )
    {
      if ( SD_GetStringFromFileOnCard ( processingTheString, MAX_NUMBER_OF_CHARACTERS_IN_HEX_FILE_LINE ) )
        {
          status = 1; //если пустая строка, то это ошибка, потому что до нее не доджно дойти,\
        выход из цикла в последней  сроке 	switch case 0x31:
        g_infoMessage = "Get String error";
					DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        break;
      }
    else
      {
        //начало обработка строки

          switch ( processingTheString[8] )
            {
            case 0x31: //  запись обозначает конец файла, данных не содержит.
              running = 0; //выход из цикла
              break;
            case 0x34: // запись расширенного адреса
              startAdressForWrite[0] = ( PROGRAMMER_SumToByte ( processingTheString[9] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[10] );
              startAdressForWrite[1] = ( PROGRAMMER_SumToByte ( processingTheString[11] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[12] );
              break;
            case 0x30:   //запись содержит данные двоичного файла
            {
              startAdressForWrite[2] = ( PROGRAMMER_SumToByte ( processingTheString[3] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[4] );
              startAdressForWrite[3] = ( PROGRAMMER_SumToByte ( processingTheString[5] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[6] );
              checksumStartAdress = startAdressForWrite[0] ^ startAdressForWrite[1] ^ startAdressForWrite[2] ^ startAdressForWrite[3];
              numberOfBytes = ( PROGRAMMER_SumToByte ( processingTheString[1] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[2] );
              
							sprintf (g_infoMessageArr, "%s%02x%02x%02x%02x", "Upload 0x", startAdressForWrite[0],startAdressForWrite[1],startAdressForWrite[2],startAdressForWrite[3] );
							g_infoMessage = g_infoMessageArr;
							
							
							for ( uint8_t k = 0; k < numberOfBytes; k++ )
                {
                  dataPackageToUpload[k] = ( PROGRAMMER_SumToByte ( processingTheString[9 + ( k * 2 )] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[9 + ( k * 2 ) + 1] );
                }
              if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
                {
                  status = 1;
                  break;
                }
              if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
                {
                  status = 1;
                  break;
                }
              if ( returnedValue == 0x79 )
                {
                  if ( HAL_UART_Transmit ( &huart2, startAdressForWrite, 4, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  if ( HAL_UART_Transmit ( &huart2, &checksumStartAdress, 1, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  returnedValue = 0x00;
                  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  if ( returnedValue == 0x79 )
                    {
                      checksumOfString = 0;
                      for ( int j = 0; j < numberOfBytes; j++ )
                        {
                          checksumOfString ^= dataPackageToUpload[j];
                        }
                      numberOfBytesMinusOne = numberOfBytes - 1;
                      checksumOfString ^= numberOfBytesMinusOne;
                      HAL_UART_Transmit ( &huart2, &numberOfBytesMinusOne, 1, 100 );
                      HAL_UART_Transmit ( &huart2, dataPackageToUpload, numberOfBytes, 100 );
                      HAL_UART_Transmit ( &huart2, &checksumOfString, 1, 100 );
                      returnedValue = 0x00;
                      HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 );
                      if ( returnedValue != 0x79 )
                        {
                          status = 1;
                          g_infoMessage = "Data upload error";
													DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
                          break;
                        }
                    }
                  else
                    {
                      status = 1;
                      g_infoMessage = "Adress error";
											DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
                      break;
                    }
                }
              else
                {
                  status = 1;
                  g_infoMessage = "Command error";
									DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
                  break;
                }
            }
            break;
            default:
              break;
            }
          //конец обработки строки
        }
    }
  if ( SD_CloseFileOnCard() )
    {
      status = 1;
    }
  if ( status == 0 )
    {      
			return 0;
    }
		else{
			return 1;
		}
  
}
//
//
//=====================================================================================================================================
//
/*
void* PROGRAMMER_UploadFirmware ( void* currentItem )
{
  _Bool status = 0;
  uint8_t command[2] = { g_deviceData.writeMemoryCommand,
                         0xFF - g_deviceData.writeMemoryCommand
                       };
  uint8_t dataPackageToUpload[256];
  uint8_t returnedValue = 0x00;
  static uint8_t startAdressForWrite[4];
  uint8_t checksumStartAdress;
  uint8_t numberOfBytesMinusOne;
  uint8_t checksumOfString = 0;
  uint8_t numberOfBytes = 0;
  _Bool running = 1;
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
  if ( SD_OpenFileOnCard() )
    {
      status = 1;
      g_infoMessage = "Get String error";
    }
  char processingTheString[128];
  while ( running && status == 0 )
    {
      if ( SD_GetStringFromFileOnCard ( processingTheString, 128 ) )
        {
          status = 1; //если пустая строка, то это ошибка, потому что до нее не доджно дойти,\
        выход из цикла в последней  сроке 	switch case 0x31:
        g_infoMessage = "Get String error";
        break;
      }
    else
      {
        //начало обработка строки
        static char temp[5];
          for ( uint8_t i = 0; i < 4; i++ )
            {
              temp[i] = processingTheString[i + 3];
            }
          temp[4] = 0x00; //Нул терминатор
          g_infoMessage = temp;
          switch ( processingTheString[8] )
            {
            case 0x31: //  запись обозначает конец файла, данных не содержит.
              running = 0; //выход из цикла
              break;
            case 0x34: // запись расширенного адреса
              startAdressForWrite[0] = ( PROGRAMMER_SumToByte ( processingTheString[9] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[10] );
              startAdressForWrite[1] = ( PROGRAMMER_SumToByte ( processingTheString[11] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[12] );
              break;
            case 0x30:   //запись содержит данные двоичного файла
            {
              startAdressForWrite[2] = ( PROGRAMMER_SumToByte ( processingTheString[3] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[4] );
              startAdressForWrite[3] = ( PROGRAMMER_SumToByte ( processingTheString[5] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[6] );
              checksumStartAdress = startAdressForWrite[0] ^ startAdressForWrite[1] ^ startAdressForWrite[2] ^ startAdressForWrite[3];
              numberOfBytes = ( PROGRAMMER_SumToByte ( processingTheString[1] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[2] );
              for ( uint8_t k = 0; k < numberOfBytes; k++ )
                {
                  dataPackageToUpload[k] = ( PROGRAMMER_SumToByte ( processingTheString[9 + ( k * 2 )] ) << 4 ) | PROGRAMMER_SumToByte ( processingTheString[9 + ( k * 2 ) + 1] );
                }
              if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
                {
                  status = 1;
                  break;
                }
              if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
                {
                  status = 1;
                  break;
                }
              if ( returnedValue == 0x79 )
                {
                  if ( HAL_UART_Transmit ( &huart2, startAdressForWrite, 4, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  if ( HAL_UART_Transmit ( &huart2, &checksumStartAdress, 1, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  returnedValue = 0x00;
                  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
                    {
                      status = 1;
                      break;
                    }
                  if ( returnedValue == 0x79 )
                    {
                      checksumOfString = 0;
                      for ( int j = 0; j < numberOfBytes; j++ )
                        {
                          checksumOfString ^= dataPackageToUpload[j];
                        }
                      numberOfBytesMinusOne = numberOfBytes - 1;
                      checksumOfString ^= numberOfBytesMinusOne;
                      HAL_UART_Transmit ( &huart2, &numberOfBytesMinusOne, 1, 100 );
                      HAL_UART_Transmit ( &huart2, dataPackageToUpload, numberOfBytes, 100 );
                      HAL_UART_Transmit ( &huart2, &checksumOfString, 1, 100 );
                      returnedValue = 0x00;
                      HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 );
                      if ( returnedValue != 0x79 )
                        {
                          status = 1;
                          g_infoMessage = "Data upload error";
                          break;
                        }
                    }
                  else
                    {
                      status = 1;
                      g_infoMessage = "Adress error";
                      break;
                    }
                }
              else
                {
                  status = 1;
                  g_infoMessage = "Command error";
                  break;
                }
            }
            break;
            default:
              break;
            }
          //конец обработки строки
        }
    }
  if ( SD_CloseFileOnCard() )
    {
      status = 1;
    }
  if ( status == 0 )
    {
      g_infoMessage = "Upload succsessful";
    }
  return currentItem;
}
*/
//
//
//=====================================================================================================================================
//
void* PROGRAMMER_RunProgramm ( void* currentItem )
{
  _Bool status = 0;
  uint8_t command[2] = {g_deviceData.goCommand, 0xFF - g_deviceData.goCommand};
  if ( HAL_UART_Transmit ( &huart2, command, 2, 100 ) )
    {
      status = 1;
    }
  uint8_t returnedValue;
  if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
    {
      status = 1;
    }
 
  if ( returnedValue == 0x79 )
    {
      uint8_t startAdress[5] = {0x08, 0x00, 0x00, 0x00, 0x00};
      uint8_t	checksum = startAdress[0] ^ startAdress[1] ^ startAdress[2] ^ startAdress[3];
      startAdress[4] = checksum;
      if ( HAL_UART_Transmit ( &huart2, startAdress, 5, 100 ) )
        {
          status = 1;
        }
      if ( HAL_UART_Receive ( &huart2, &returnedValue, 1, 100 ) )
        {
          status = 1;
        }
      
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
void PROGRAMMER_ClearSectorNumbersForErase ( void)
{
for(int i=0;i<MAX_NUMBER_OF_SECTORS;i++)
{
	sectorNumbersForErase[i]=0;
}
}
