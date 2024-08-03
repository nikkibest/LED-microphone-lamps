/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}

void setup_SPIFFS(){
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/batteryLevel.txt", "4.2"); // overwrites or makes a new file

    
    // appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    // readFile(SPIFFS, "/test.txt");
    // writeFile(SPIFFS, "/test.txt", "Hello ");
    // readFile(SPIFFS, "/test.txt");
    // renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    // readFile(SPIFFS, "/foo.txt");
    // deleteFile(SPIFFS, "/foo.txt");
    // testFileIO(SPIFFS, "/test.txt");
    // deleteFile(SPIFFS, "/test.txt");
    // Serial.println( "Test complete" );
}

void appendBatteryLevel(){
  char strBattery_Vsource[5];
  floatToString(battery_Vsource, strBattery_Vsource, sizeof(strBattery_Vsource), 2)
  appendFile(SPIFFS, "/batteryLevel.txt", strBattery_Vsource);
}



void serialInput_loop() {
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte == '\n') {
      inputBuffer[ii_buffer] = '\0';  // Null-terminate the string
      checkInput(inputBuffer);
      ii_buffer = 0;  // Reset ii_buffer for the next input
    } else {
      inputBuffer[ii_buffer++] = incomingByte;
      if (ii_buffer >= sizeof(inputBuffer) - 1) {
        ii_buffer = sizeof(inputBuffer) - 1;
      }
    }
  } 
}

void checkInput(char inputBuffer[])
{
  bool bInputCorrect = false;
  for (int ii=0; ii<strlen(inputBuffer); ii++) {
    if (inputBuffer[ii] == 'p') {
      bInputCorrect = changePrintBools(inputBuffer[ii+1]);
    } else if (inputBuffer[ii] == 's') {
      char * pch;
      pch = strtok (inputBuffer," ");

      bool bFirstPchSkip = false;
      bool bChangeSPIFFSrun = false;
      while (pch != NULL)
      {
        if (bFirstPchSkip) {
          bInputCorrect = changeSPIFFS(inputBuffer[ii+1], pch);
          bChangeSPIFFSrun = true;
        }
        pch = strtok (NULL, " ,.-");
        bFirstPchSkip = true;
      }
      if (!bChangeSPIFFSrun) {
        char* pch = "";
        bInputCorrect = changeSPIFFS(inputBuffer[ii+1], pch);
      }
    }
  }

  if (!bInputCorrect) {
    Serial.println("Wrong serial input!");
    Serial.println("Acceptable inputs are:");
    Serial.println(" 'p' for print boolean changes followed by:");
    Serial.println("   - 'm' for changing mic print boolean");
    Serial.println("   - 'r' for changing rotary print boolean");
    Serial.println("   - 'b' for changing battery print boolean \n");

    Serial.println(" 's' for SPIFFS changes followed by:");
    Serial.println("   - 'l' for listing SPIFFs file");
    Serial.println("   - 'r' for reading SPIFFs file");
    Serial.println("   - 'w' for writing SPIFFS file");
    Serial.println("   - 't' for testing SPIFFS file");
    Serial.println("   - 'd' for deleting SPIFFS file");
  }
}

bool changeSPIFFS(char inputChar, char* filename) {
  bool bInputCorrect = true;
  bool bCheck = filename!="";
  if (inputChar=='l') {
    Serial.println("Listing SPIFFS file: ");
  } else if (inputChar=='r' && bCheck) {
    Serial.print("Reading SPIFFS file: ");
    Serial.println(filename);
  } else if (inputChar=='w' && bCheck) {
    Serial.print("Writing SPIFFS file: ");
    Serial.println(filename);
  } else if (inputChar=='t' && bCheck) {
    Serial.print("Testing SPIFFS file: ");
    Serial.println(filename);
  } else if (inputChar=='d' && bCheck) {
    Serial.print("Deleting SPIFFS file: ");
    Serial.println(filename);
  } else {
    bInputCorrect = false;
  }
  return bInputCorrect;
}

bool changePrintBools(char inputChar) {
  bool bInputCorrect = true;
  if (inputChar=='m') {
    bPrint_mic = (!bPrint_mic);
    Serial.print("Mic print bool toggled: ");
    Serial.println(bPrint_mic);
  } else if (inputChar=='r') {
    bPrint_rot = (!bPrint_rot);
    Serial.print("Rotary print bool toggled: ");
    Serial.println(bPrint_rot);
  } else if (inputChar=='b') {
    bPrint_battery = (!bPrint_battery);
    Serial.print("Battery print bool toggled: ");
    Serial.println(bPrint_battery);
  } else {
    bInputCorrect = false;
  }
  return bInputCorrect;
}