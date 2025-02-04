#include "main.h"
#include "storingSettings.h"
#include "Ftp.h"
#include "esp8266_lib.h"
//
//
//=====================================================================================================================================
//
static FLASH_EraseInitTypeDef eraseInitStruct
= {.TypeErase = FLASH_TYPEERASE_SECTORS, // постраничная очистка
   .Sector = FLASH_SECTOR_7, // адрес страницы
   .NbSectors = 1, // кол-во страниц для стирания
   .VoltageRange = FLASH_VOLTAGE_RANGE_3,
   .Banks = FLASH_BANK_1
  };
//
//
//=====================================================================================================================================
//
// Проверяет что это первый запуск
void STORING_CheckFirstConnection ( void )
{
  if ( * ( uint8_t* ) ( ADRESS_BEGINNING_SECTOR ) ==0xFF && * ( uint8_t* ) ( ADRESS_BEGINNING_SECTOR + SIZE_FTP_UNION ) ==0xFF )
    {
      STORING_SaveSettingsToFlash ();
    }
}
//
//
//=====================================================================================================================================
//
static _Bool STORING_WriteToFlash ( uint32_t adress, uint32_t size, uint32_t* data )
{
  for ( uint16_t i = 0; i < ( size / 4 ); i++ )
    {
      if ( HAL_FLASH_Program ( FLASH_TYPEPROGRAM_WORD, adress + ( i * 4 ), data[i] ) != HAL_OK )
        {
          return 1;
        }
    }
  return 0;
}
//
//
//=====================================================================================================================================
//Запись настроек во Flash
_Bool STORING_SaveSettingsToFlash ( void )
{
  HAL_StatusTypeDef stat;
  uint32_t page_error = 0; // переменная, в которую запишется адрес страницы при неудачном стирании
  if ( HAL_FLASH_Unlock() != HAL_OK )
    {
      return 1;
    }
  for ( uint8_t i = 0; i < 2; i++ )
    {
      stat = HAL_FLASHEx_Erase ( &eraseInitStruct, &page_error );
      if ( stat == HAL_OK )
        {
          break;
        }
    }
  if ( stat != HAL_OK )
    {
      HAL_FLASH_Lock();
      return 1;
    }
  if ( STORING_WriteToFlash ( ADRESS_BEGINNING_SECTOR, SIZE_FTP_UNION, unionFtp.data32 ) )
    {
      HAL_FLASH_Lock();
      return 1;
    }
  if ( STORING_WriteToFlash ( ADRESS_BEGINNING_SECTOR + SIZE_FTP_UNION, SIZE_UNION_WIFI, g_unionWifi.data32 ) )
    {
      HAL_FLASH_Lock();
      return 1;
    }
  HAL_FLASH_Lock();
  return 0;
}
//
//
//=====================================================================================================================================
//Чтение настроек из Flash
void STORING_ReadSettingsFromFlash ( void )
{
  for ( uint16_t i = 0; i < ( SIZE_FTP_UNION / 4 ); i++ )
    {
      unionFtp.data32[i] = * ( uint32_t* ) ( ADRESS_BEGINNING_SECTOR + ( i * 4 ) );
    }
  for ( uint16_t i = 0; i < ( SIZE_UNION_WIFI / 4 ); i++ )
    {
      g_unionWifi.data32[i] = * ( uint32_t* ) ( ADRESS_BEGINNING_SECTOR + SIZE_FTP_UNION + ( i * 4 ) );
    }
}
