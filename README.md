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

