#include "sd_card.h"
#include "displayMenu.h"
#include "stdio.h"
#include "fatfs.h"
#include "string.h"
#include <stdlib.h>
#include "ringBuffer.h"
#include "ftp.h"
#include "ssd1306.h"
#include "bootloader.h"
//
//
//=====================================================================================================================================
//
static FATFS fileSystem;
static FRESULT res;
static DIR directory;
static FILINFO fileInfo;
static FIL fil;
static FIL fil2;

//static SD_SelectedFile_t selectedFileToUpload;
SD_SelectedFile_t selectedFileToUpload;
static char newFolderName[256] ="";


//
//
//=====================================================================================================================================
//
SD_CardStatus_t SD_InitCard ( void )
{
  if ( f_mount ( &fileSystem, "0", 1 ) == FR_OK )
    {
      return CARD_STATUS_OK;
    }
  else
    {
      return CARD_STATUS_ERROR;
    }
}
//
//
//=====================================================================================================================================
//
uint8_t SD_GetListFiles ( uint16_t* numberOfDirectories )
{
  g_numberOfListItemsForShow = 0;
  g_currentItemOfList = 0;
  uint16_t stringNumber = 0;
	
		for(uint16_t i =0;i<MAX_NUMBER_OF_CATALOG_ITEMS;i++)
  {
		g_fileInfo[i].file =0;
		g_fileInfo[i].size =0;
		g_fileInfo[i].modify[0] =0x00;
		g_fileInfo[i].modiflyCheck = 0;
		g_fileInfo[i].nameCheck = 0;
		g_fileInfo[i].sizeCheck = 0;
		g_fileInfo[i].typeCheck = 0;
  }
	
  if ( f_opendir ( &directory, "" ) == FR_OK )
    {
      while ( 1 )
        {
          if ( f_readdir ( &directory, &fileInfo ) == FR_OK )
            {
              if ( fileInfo.fname[0] == 0 ) //Когда будут прочитаны все элементы в директории, функция вернет пустую ASCIIZ-строку в поле fname[] (т. е. fname[0]==0) без какой-либо ошибки.
                {
                  break;
                }
              if ( stringNumber >= MAX_NUMBER_OF_CATALOG_ITEMS )
                {
                  return 1;
                }
//								g_rowOfListForScreen[stringNumber] = malloc(strlen(fileInfo.fname) + 1); //+1 нужно чтобы нуль терминатор скопировался
//
//
//
//								if (g_rowOfListForScreen[stringNumber] == 0) { //проверка что память выделена
//                    return 1;
//                }
              sprintf ( g_rowOfListForScreen[stringNumber],"%s", fileInfo.fname );
							if(fileInfo.fattrib & 0x10){
							g_fileInfo[stringNumber].file =0U;
							}
							else
							{
							g_fileInfo[stringNumber].file =1U;
							}	
							g_fileInfo[stringNumber].size = fileInfo.fsize;
							char temp[10];	

							
							sprintf(g_fileInfo[stringNumber].modify, "%d", (((fileInfo.fdate>>9) & (0x7F))+1980));
							
							sprintf(temp, "%02d", (((fileInfo.fdate>>5) & (0xF))));
							strcat(g_fileInfo[stringNumber].modify, temp);
							sprintf(temp, "%02d", ((fileInfo.fdate & (0x1F))));
							strcat(g_fileInfo[stringNumber].modify, temp);
							
							sprintf(temp, "%02d", (fileInfo.ftime>>11) & 0x1F		);
							strcat(g_fileInfo[stringNumber].modify, temp);
							sprintf(temp, "%02d", (((fileInfo.ftime>>5) & (0x3F))));
							strcat(g_fileInfo[stringNumber].modify, temp);
							sprintf(temp, "%02d", (((fileInfo.ftime) & (0x1F))*2));
							
              stringNumber++;
            } //END if
          else
            {
              return 1;
            }
        } //END while
    } //END f_opendir
  else
    {
			f_chdir ( "0:.." );
			f_closedir ( &directory );
      return 1;
    }
  f_closedir ( &directory );
  if ( stringNumber==0 )
    {
      f_chdir ( ".." );
      SD_GetListFiles ( &g_numberOfListItemsForShow );
      return 2;
    }
  *numberOfDirectories = stringNumber;
  return 0;
}
//
//
//=====================================================================================================================================
//
void* SD_OpenCard ( void* currentItem )
{
  if ( SD_GetListFiles ( &g_numberOfListItemsForShow ) )
    {
      g_infoMessage = "ERROR: Get list files error!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_typeList = TYPE_LIST_FILES_SD;
      g_currentItemOfList =0;
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
//Открываем папку или выбираем файл, для загрузчика
void SD_OpenFolderOrSelectFile ( uint16_t selectedItem )
{
  g_numberOfListItemsForShow = 0;
  g_currentItemOfList = 0;
  //Проверяем Файл или папка.

		if(g_fileInfo[selectedItem].file)//если файл
    {
      if ( g_typeList == TYPE_LIST_FILES_SD )
        {
					PROGRAMMER_ClearSectorNumbersForErase();
          f_getcwd ( selectedFileToUpload.path, MAX_LEN_PATH );
          strcpy ( selectedFileToUpload.name, g_rowOfListForScreen[selectedItem] );
					
					
					sprintf (g_infoMessageArr, "%s%s%s", "File \"", selectedFileToUpload.name,"\" is selected to upload." );
							
					g_infoMessage = g_infoMessageArr;
					
					     //g_infoMessage = "The file to upload is selected. ";
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );					
					g_previousWindow = TYPE_WINDOW_MENU;
					f_chdir ( "/" );	
        }
      else if ( g_typeList == TYPE_LIST_FIRMWARE )
        {
					g_infoMessage = "Don't reset device while blue led is on!";
					DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
          //копируем файл
          if ( SD_CopyFile ( g_rowOfListForScreen[selectedItem], PATH_TO_FIRMWARE ) )
            {
              g_infoMessage = "ERROR! Copying a file!";
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
            }
          else
            {
								NVIC_SystemReset();    //перезагружаем 
            }
          
        }

    }
  else   //если папка
    {
      f_chdir ( g_rowOfListForScreen[selectedItem] );
      uint8_t  resGetListFiles  = SD_GetListFiles ( &g_numberOfListItemsForShow );
      if ( resGetListFiles ==1 )
        {
          g_infoMessage = "ERROR: Get list files error!";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
      else if ( resGetListFiles ==2 )
        {
          g_infoMessage = "No files in folder!";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
      else
        {
          //g_typeList = TYPE_LIST_FILES_SD;
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
        }
    }
}
//
//
//=====================================================================================================================================
//
void SD_SelectFolderForFtpServer ( uint16_t selectedItem )
{
	if(g_fileInfo[selectedItem].file==0){
	f_chdir ( g_rowOfListForScreen[selectedItem] );
  f_getcwd ( unionFtp.settingsFTP[g_selectedFtpServer].path, MAX_LEN_PATH );
  f_chdir ( "/" );
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
	}

	
	

}

//
//
//=====================================================================================================================================
//
void SD_GoToParentDirectory ( void )
{
  uint8_t size = 255;
  char before [size];
  char after [size];
  f_getcwd ( before, size );
  f_chdir ( ".." );
  f_getcwd ( after, size );
  if ( strcmp ( before, after ) )
    {
      if ( SD_GetListFiles ( &g_numberOfListItemsForShow ) )
        {
          g_infoMessage = "ERROR: Get list files error!";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
      else
        {          
          g_currentItemOfList =0;
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
        }
    }
  else
    {
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
    }
}
//
//
//=====================================================================================================================================
//
_Bool SD_OpenFileOnCard ( void )
{
  char fileName[256] = "";
  strcat ( fileName, selectedFileToUpload.path );
  char slash[2] = "/";
  strcat ( fileName, slash );
  strcat ( fileName, selectedFileToUpload.name );
  if ( f_open ( &fil, fileName, FA_READ | FA_OPEN_EXISTING ) )
    {
      return 1;
    }
  return 0;
}
//
//
//=====================================================================================================================================
//
_Bool SD_GetStringFromFileOnCard ( char* stringFromFile, uint16_t leghtOfString )
{
  if ( f_gets ( stringFromFile, leghtOfString, &fil ) )
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
_Bool SD_CloseFileOnCard ( void )
{
  f_chdir ( ".." );
  if ( f_close ( &fil ) )
    {
      return 1;
    }
  return 0;
}
//
//
//=====================================================================================================================================
//
void SD_OpenFileToWrite ( char* fileName )
{
  //SD_CreateFolder();// не нужно тут
  //записываем в путь указанный в настойках ftp servera
  f_chdir ( unionFtp.settingsFTP[g_selectedFtpServer].path );
  f_open ( &fil, fileName, FA_WRITE | FA_CREATE_ALWAYS );
}
//
//
//=====================================================================================================================================
//
_Bool SD_WriteByteToFile ( void* buffer )
{
  UINT byteWritten;
  if ( f_write ( &fil, buffer, 1u, &byteWritten ) )
    {
      return 1;
    }
  if ( byteWritten!=1u )
    {
      return 1;
    }
  return 0;
}
//
//
//=====================================================================================================================================
//


void SD_EnterNameNewFolder ( void )
{
  ClearInputField();
  newFolderName[0] =0x00;
  g_inputDestination = newFolderName;
  g_pressEnt = SD_CreateNewFolder;
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_KEYBOARD );
}
//
//
//=====================================================================================================================================
//

void SD_CreateNewFolder ( void )
{
  if ( f_mkdir ( newFolderName ) == FR_OK )
    {
      SD_GetListFiles ( &g_numberOfListItemsForShow );
      g_pressEnt = 0x00;
    }
}
//
//
//=====================================================================================================================================
//
void SD_ConfirmationDeleting ( char* nameFolderOrFile )
{
	
	static char temp[108] = "";
	temp[0] = 0x00;
	
	strcat(temp, "Delete \"");
	
	if (strlen(nameFolderOrFile)> (SSD1306_WIDTH_7_FONT*2-3))
	{
	strncat(temp, nameFolderOrFile, (SSD1306_WIDTH_7_FONT*2-6));
	strcat(temp, "...");	
	}
	else{	
	strcat(temp, nameFolderOrFile);
	}
	
	
	strcat(temp, "\"?\r");
	strcat(temp, "R   - Yes\rEnt - No\rF   - Exit.");
	
	
      g_infoMessage = temp;
	
	g_pressR = SD_DeleteFolderOrFile;
	
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_CONFIRM );
	
	
	
}





//
//
//=====================================================================================================================================
//
_Bool SD_DeleteFolderOrFile ( char* nameFolderOrFile )
{	
  res = f_unlink ( nameFolderOrFile );
  if ( res == FR_OK )
    {
      SD_GetListFiles ( &g_numberOfListItemsForShow );
			DISPLAY_GoToSpecifiedWindow ( g_previousWindow );
      return 0;
    }
  return 1;
}

//
//
//=====================================================================================================================================
//

void* SD_SelectFileForUpdateFirmware ( void* currentItem )
{
	
	
	
	
	
  if ( SD_GetListFiles ( &g_numberOfListItemsForShow ) )
    {
      g_infoMessage = "ERROR: Get list files error!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_typeList = TYPE_LIST_FIRMWARE;
      g_currentItemOfList =0;
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
    }
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
_Bool SD_CopyFile ( char *srcFile, char *destFile )
{
  BYTE buffer[1024];
  UINT br = 0, bw = 0;
  if ( f_open ( &fil, ( const TCHAR* ) srcFile, FA_READ | FA_OPEN_EXISTING ) )
    {
      return 1;
    }
  if ( f_open ( &fil2, ( const TCHAR* ) destFile, FA_WRITE | FA_CREATE_ALWAYS ) )
    {
      f_close ( &fil );
      return 1;
    }
  do
    {
      if ( f_read ( &fil, buffer, sizeof ( buffer ), &br ) )
        {
          f_close ( &fil );
          f_close ( &fil2 );
          return 1;
        }
      if ( br )
        {
          if ( f_write ( &fil2, buffer, br, &bw ) )
            {
              f_close ( &fil );
              f_close ( &fil2 );
              return 1;
            }
          if ( bw!= br )
            {
              f_close ( &fil );
              f_close ( &fil2 );
              return 1;
            }
        }
      else
        {
          bw =0;
        }
    }
  while ( bw );
  f_close ( &fil );
  f_close ( &fil2 );
  return 0;
}
