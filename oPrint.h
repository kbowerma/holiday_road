//Globals


//decalre Prototypes
  void oPrintInfo();
  void oPrintInfo5();
  void oPrintNoDevices();
  void oPrintRelayMode();
  void oPrintTemp(int index, float mytemp);
  void oPrintTemp2(int index, float mytemp);
  void oPrintTemp3(int index, float mytemp);
  void oPrintMoisture();



// functions
  void oPrintInfo() {
      oled.clear(PAGE);
      oled.setCursor(0,0);
      //oled.print(MYVERSION);
      oled << "M1: " << M1PCT ;
      oled.setCursor(0,10);
      oled << freqChecker() << " Hz";
      oled.setCursor(0,20);
      oled.print("pfreq ");
      oled.print(PUSHFREQ);
      oled.setCursor(0,30);
      oled.print("devices ");
      oled.print(deviceCount);
      oled.setCursor(0,40);
      oled.print(convertMillisToHuman(millis()));
      oled.display();
  }

  void oPrintInfo5() {
    //uint32_t freemem = System.freeMemory();
    oled.clear(PAGE);
    oled.setCursor(0,0);
    oled.print(MYVERSION);
    oled.setCursor(0,10);
    //oled << "M: "  << freemem << endl;
    oled << "Fmem "  <<  System.freeMemory() / 1024 << "k" << endl;
    oled << "BT " << buttonvalue << " M1 " << M1PCT << endl;
    oled << "sVer:" << System.version().c_str() << endl;
    oled.display();
  }

  void oPrintNoDevices() {
    oled.clear(PAGE);
    oled.setCursor(0,0);
    oled.setFontType(1);
    oled << "NO" << endl << "DEVICES" << endl;
    oled.setFontType(0);
    oled.display();
  }

  void oPrintRelayMode() {
    if ( buttonvalue == 0 ) {
      // calling relayFunc("on"); crashes so replicating function
      digitalWrite(relay, HIGH);
      relayTimer.start();
    }
    oled.clear(PAGE);
    oled.setCursor(0,0);
    oled.setFontType(1);
    oled << "RELAY" << endl <<  "   " << digitalRead(relay) << endl;
    oled.setFontType(0);
    oled.display();

  }

  void oPrintTemp(int index, float mytemp){
      oled.setFontType(0);
      oled.setCursor(0,0);
      oled.print("devices ");
      oled.print(sensor.getDeviceCount());
      oled.setCursor(0,index*12+12);
      oled.print("T");
      oled.print(index);
      oled.print(" ");
      oled.print(mytemp);
      oled.display();

  }

  void oPrintTemp2(int index, float mytemp){
      oled.setFontType(0);
      oled.setCursor(0,0);
      oled.setCursor(0,index*12);
      oled.setColor(BLACK);
      oled.print("T");
      oled.print(index);
      oled.setColor(WHITE);
      oled.print(" ");
      oled.print(mytemp);
      oled.display();

  }

  void oPrintTemp3(int index, float mytemp){
      oled.setFontType(0);
      oled.setCursor(0,0);
      oled.setCursor(0,index*12);
      String name = deviceNames[index];
      String shortname = name.substring(0,4);
      oled << shortname;
      oled.print(" ");
      if (mytemp > 0 ) {
      oled.print(mytemp);
      }

      if (mytemp < 0 ) {
      oled.print("     ");

      Particle.publish("onewireloose", String(index) );
      Serial << "loose of onewire " << mytemp << "index " << index << endl;
      }
      oled.display();

  }

  void oPrintMoisture() {
    digitalWrite(M1POWER, HIGH);
     // loop to only check the moisture for 50 cycles
     for (int i=0; i <= 100; i++){
      M1PCT = map(analogRead(M1),1400,4020,100,0);
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled << "Moist" << endl <<  "   " << analogRead(M1) << endl;
      oled.setFontType(1);
      oled.setCursor(0,21);
      oled  << M1PCT << "%";
      oled.setFontType(0);
      oled.display();
      delay(10);
     }
     displayMode = 4;  // this works but I need to set the encoder postion too
     encoderPos = 4;

  }
