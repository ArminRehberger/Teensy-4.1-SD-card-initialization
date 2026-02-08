  // ########## SD card
  // https://github.com/PaulStoffregen/SD
  // https://forum.pjrc.com/index.php?threads/teensy-4-1-sd-card-writing.70642/
  // https://forum.pjrc.com/index.php?threads/teensy-4-1-sd-card-interface.75779/
  // https://forum.pjrc.com/index.php?threads/teensy-4-1-data-logging-to-sd-card-help.75401/
  // https://forum.pjrc.com/index.php?threads/cannot-initialize-sdcard-in-teensy-4-1-sdcard-slot.73171/
  // https://github.com/PaulStoffregen/SD/blob/Juse_Use_SdFat/examples/SdFat_Usage/SdFat_Usage.ino
  // https://docs.arduino.cc/libraries/sd/


// ###############################################################################################################
// ###############################################################################################################
// ##### Include
// Using library SD at version 2.0.0 in folder:     C:\Users\armin\AppData\Local\Arduino15\packages\teensy\hardware\avr\1.59.0\libraries\SD 
// Using library SdFat at version 2.1.2 in folder:  C:\Users\armin\AppData\Local\Arduino15\packages\teensy\hardware\avr\1.59.0\libraries\SdFat 
// Using library SPI at version 1.0 in folder:      C:\Users\armin\AppData\Local\Arduino15\packages\teensy\hardware\avr\1.59.0\libraries\SPI
#include <SD.h> // SD-card

// ###############################################################################################################
// ###############################################################################################################
// ##### SD card global variables
// Interface is SPI (MISO / MOSI)
#define SD_CONFIG SdioConfig(FIFO_SDIO) // Built-in SD-card (Slot) with max. speed (FIFO)
SdFs mysd;      // Access the built in SD card on Teensy 4.1 using DMA
FsFile myfile;  // FsFile offers more capability than regular SD "File"
FsFile myroot;

// ###############################################################################################################
// ###############################################################################################################
// ##### Setup
void setup()
{
  // ########## Serial Monitor
  Serial.begin(9600);
  while (!Serial && millis() < 1000) {} //wait for Serial Monitor
  Serial.println("Startup");

  // ##### Initialization SD card
  uint16_t InitSDOK=99;
  uint64_t clusterCount;
  uint64_t bytesPerCluster;
  uint64_t bytesTotal;
  uint8_t fatType;
  char file_name[256];

  if(!mysd.begin(SD_CONFIG))
  {
    Serial.println("Initialization SD card failed");
    InitSDOK = 0;
  }
  else
  {
    Serial.println("Initialization SD card succeeded");
  }
  
  if(InitSDOK !=0)
  {
    // ##### Volume size
    Serial.printf("ClusterCount %d\n", mysd.clusterCount());
    Serial.printf("BytesPerCluster %d\n", mysd.bytesPerCluster());
    clusterCount = mysd.clusterCount();
    bytesPerCluster = mysd.bytesPerCluster();
    bytesTotal = clusterCount * bytesPerCluster;

    Serial.print("Volume size (Bytes):  ");
    Serial.println(bytesTotal);

    bytesTotal /= 1024;
    Serial.print("Volume size (KByte):  ");
    Serial.println(bytesTotal);

    bytesTotal /= 1024;
    Serial.print("Volume size (MByte):  ");
    Serial.println(bytesTotal);

    Serial.print("Volume size (GByte):  ");
    Serial.println((float)bytesTotal / 1024.0);

    // ##### Card type
    Serial.print("Card Type: ");
    switch(mysd.card()->type())
    {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("Unknown");
    }

    // ##### FAT type
    fatType = mysd.fatType(); // or: uint8_t fatType = mysd.vol()->fatType();
    Serial.print("FAT Type: ");
    if(fatType == FAT_TYPE_EXFAT)
      Serial.println("exFAT");
    else if(fatType == FAT_TYPE_FAT32)
      Serial.println("FAT32");
    else if(fatType == FAT_TYPE_FAT16)
      Serial.println("FAT16");
    else
      Serial.println("Unknown/Invalid");

    // ##### List all files on SD card
    myroot = mysd.open("/");
    while(true)
    {
      myfile = myroot.openNextFile(); // Open next file
      if (!myfile)
        break; // No more files

      myfile.getName(file_name, sizeof(file_name)); // Get the name of the file
      Serial.print(file_name);
      Serial.print(" ");
      if(myfile.isDirectory())
      {
        Serial.println("/");
      }
      else
      {
         Serial.printf("%03.2f KByte\n", (float)myfile.size() / 1024.0);
      }
      myfile.close();
    }
    myroot.close();
  } // if(InitSDOK !=0)

} // setup

// ###############################################################################################################
// ###############################################################################################################
// ##### Loop
void loop()
{


}




/*


  char hexString[20];
  char CharReadFile;
  int i;
  int IndexHexCharacter;


#define SD_BUFFER_SIZE 32*512
#define SD_PICTURE_SIZE 128*128


  // ##### Open file for read
  myfile = mysd.open("BildNurZeichen.txt", FILE_READ);
  if(!myfile)
  {
    Serial.println("Error opening BildNurZeichen.txt");
    InitSDOK = 0;
  }
  else
  {
    Serial.println("Opening BildNurZeichen.txt succeeded");
  }

  // ##### Allocate memory for Picture 1
  // Malloc, Teensy 4.1 in RAM2
  uint16_t *Picture1 = (uint16_t *)malloc(sizeof(uint16_t) * SD_PICTURE_SIZE); // Bild 128x128
  if (Picture1 == NULL)
  {
    Serial.println("Picture1 Allocation Failed");
    InitSDOK = 0;
  }
  else
  {
    Serial.println("Picture1 Allocation succeeded");
  }

  // ##### Allocate memory for Picture 2
  // Malloc, Teensy 4.1 in RAM2
  uint16_t *Picture2 = (uint16_t *)malloc(sizeof(uint16_t) * SD_PICTURE_SIZE); // Bild 128x128
  if (Picture2 == NULL)
  {
    Serial.println("Picture2 Allocation Failed");
    InitSDOK = 0;
  }
  else
  {
    Serial.println("Picture2 Allocation succeeded");
  }

  // ##### Read all Pixels, for example Picture 128x128 = 16384 Pixel
  if(InitSDOK !=0)
  {
    for(i=0; i<SD_PICTURE_SIZE; i++)
    {
      // ##### Each Pixel has 2 Byte (16Bit) or 4 hex character
      // Pixel format: Red: 5 bit, Green: 6 bit, Blue: 5 bit
      // Find 4 hex character in the file
      IndexHexCharacter = 0;
      hexString[0] = '\0';
      do
      {
        if(myfile.available())
        {
          CharReadFile = myfile.read();
          if(isxdigit(CharReadFile)) // Ist Zeichen ein Hexadezimalzeichen
          {
            hexString[IndexHexCharacter] = CharReadFile;
            IndexHexCharacter++;
          }
        }
        else
        {
          break; // Kein Zeichen mehr im File, Abbruch
        }
      }while (IndexHexCharacter<4);

      // ##### Transyer Pixel data into uint16_t memory
      // hexString with 4 character = 16 bit
      Picture1[i] = strtol(hexString, NULL, 16);
      Picture2[i] = strtol(hexString, NULL, 16);

      //#####TEST#####
      //Serial.printf("##### Index: %i\n", i);
      //Serial.println(hexString);
      //Serial.println(Picture1[i], DEC);

    } // for(i=0; i<SD_PICTURE_SIZE; i++)

    // ##### Close file
    myfile.close();

    Serial.println("Pictures loaded to RAM of TFT succeeded");
  } // if(InitSDOK !=0)


*/




