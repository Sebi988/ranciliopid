
#if (DISPLAY == 1 || DISPLAY == 2)  

    int displaystatus = 0 ; 
    /********************************************************
     initialize u8g2 display
    *****************************************************/
    void u8g2_prepare(void) 
    {
        //u8g2.setFont(u8g2_font_6x12_tf);
        u8g2.setFont(u8g2_font_profont11_tf);
        //u8g2.setFont(u8g2_font_IPAandRUSLCD_tf);
        u8g2.setFontRefHeightExtendedText();
        u8g2.setDrawColor(1);
        u8g2.setFontPosTop();
        u8g2.setFontDirection(0);
        u8g2.setDisplayRotation(DISPALYROTATE);
    }

    /********************************************************
     DISPLAY - print message
    *****************************************************/
    void displayMessage(String text1, String text2, String text3, String text4, String text5, String text6) 
    {
        u8g2.clearBuffer();
        u8g2.setCursor(0, 0);
        u8g2.print(text1);
        u8g2.setCursor(0, 10);
        u8g2.print(text2);
        u8g2.setCursor(0, 20);
        u8g2.print(text3);
        u8g2.setCursor(0, 30);
        u8g2.print(text4);
        u8g2.setCursor(0, 40);
        u8g2.print(text5);
        u8g2.setCursor(0, 50);
        u8g2.print(text6);
        u8g2.sendBuffer();
    }

    /********************************************************
     DISPLAY - print logo and message at boot
    *****************************************************/
    void displayLogo(String displaymessagetext, String displaymessagetext2) 
    {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 47, displaymessagetext.c_str());
        u8g2.drawStr(0, 55, displaymessagetext2.c_str());
        //Rancilio startup logo
        switch (machineLogo) {
          case 1:
          u8g2.drawXBMP(41, 2, startLogoRancilio_width, startLogoRancilio_height, startLogoRancilio_bits);
          break;

          case 2: 
          u8g2.drawXBMP(0, 2, startLogoGaggia_width, startLogoGaggia_height, startLogoGaggia_bits);
          break;

          case 3:
          u8g2.drawXBMP(22, 0, startLogoQuickMill_width, startLogoQuickMill_height, startLogoQuickMill_bits);
          break;         
        }
        u8g2.sendBuffer();
    }

    /********************************************************
     DISPLAY - EmergencyStop
    *****************************************************/
    void displayEmergencyStop(void) 
    {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_profont11_tf); // set font
        u8g2.drawXBMP(0, 0, logo_width, logo_height, logo_bits_u8g2);   //draw temp icon
        u8g2.setCursor(32, 24);
        u8g2.print("Ist :  ");
        u8g2.print(Input, 1);
        u8g2.print(" ");
        u8g2.print((char)176);
        u8g2.print("C");
        u8g2.setCursor(32, 34);
        u8g2.print("Soll:  ");
        u8g2.print(setPoint, 1);
        u8g2.print(" ");
        u8g2.print((char)176);
        u8g2.print("C");

    //draw current temp in icon
        if (isrCounter < 500)
        {
                u8g2.drawLine(9, 48, 9, 5);
                u8g2.drawLine(10, 48, 10, 4);
                u8g2.drawLine(11, 48, 11, 3);
                u8g2.drawLine(12, 48, 12, 4);
                u8g2.drawLine(13, 48, 13, 5);
                u8g2.setCursor(32, 4);
                u8g2.print("PID STOPPED");
        }
        u8g2.sendBuffer();
    }
    /********************************************************
     DISPLAY - Calibrationmode
    *****************************************************/   
    void displayDistance(int display_distance)
    {
        u8g2.clearBuffer();
        u8g2.setCursor(13, 12);
        u8g2.setFont(u8g2_font_fub20_tf);
        u8g2.printf("%d",display_distance);
        u8g2.print("mm");
      u8g2.sendBuffer();
    }
    /********************************************************
     DISPLAY - Shottimer
    *****************************************************/

       void displayShottimer(void) 
     {
        displaystatus = 0 ;// Indiktator für Reset Bezug im Display
        if ((machinestate == 30 )  && SHOTTIMER == 1)  // Shotimer muss 1 = True sein und Bezug vorliegen
        {
            // Dann Zeit anzeigen
            u8g2.clearBuffer();
            displaystatus = 1 ;// Indiktator für Bezug im Display
           // u8g2.drawXBMP(0, 0, logo_width, logo_height, logo_bits_u8g2);   //draw temp icon
            u8g2.drawXBMP(0, 0, brewlogo_width, brewlogo_height, brewlogo_bits_u8g2);
            u8g2.setFont(u8g2_font_profont22_tf);
            u8g2.setCursor(64, 25);
            u8g2.print(bezugsZeit / 1000, 1);
            u8g2.setFont(u8g2_font_profont11_tf);
            u8g2.sendBuffer();
            
        }
        if 
        (
        ((machinestate == 31)  &&  SHOTTIMER == 1) 
        ) // wenn die totalbrewtime automatisch erreicht wird, soll nichts gemacht werden, da sonst falsche Zeit angezeigt wird, da Schalter später betätigt wird als totalbrewtime
        {
           displaystatus = 1 ;// Indiktator für Bezug im Display
           u8g2.clearBuffer();
           u8g2.drawXBMP(0, 0, brewlogo_width, brewlogo_height, brewlogo_bits_u8g2);
           u8g2.setFont(u8g2_font_profont22_tf);
           u8g2.setCursor(64, 25);
           u8g2.print(lastbezugszeit/1000, 1);
           u8g2.setFont(u8g2_font_profont11_tf);
           u8g2.sendBuffer();
        }
    }
      
    /********************************************************
     DISPLAY - Heatinglogo
    *****************************************************/
     void heatinglogo(void) 
    {
         if (HEATINGLOGO  > 0 && machinestate == 10 ) 
        {
           // Für Statusinfos
           u8g2.clearBuffer();
           u8g2.drawFrame(8, 0, 110, 12);
            if (Offlinemodus == 0) 
            {
                  getSignalStrength();
                if (WiFi.status() == WL_CONNECTED) 
                {
                    u8g2.drawXBMP(40, 2, 8, 8, antenna_OK_u8g2);
                    for (int b = 0; b <= bars; b++) 
                    {
                    u8g2.drawVLine(45 + (b * 2), 10 - (b * 2), b * 2);
                    }
                } else 
                {
                    u8g2.drawXBMP(40, 2, 8, 8, antenna_NOK_u8g2);
                    u8g2.setCursor(88, 2);
                    u8g2.print("RC: ");
                    u8g2.print(wifiReconnects);
                }
                if (Blynk.connected()) 
                {
                    u8g2.drawXBMP(60, 2, 11, 8, blynk_OK_u8g2);
                } 
                else 
                {
                    u8g2.drawXBMP(60, 2, 8, 8, blynk_NOK_u8g2);
                }
                if (MQTT == 1) 
                {
                    if (mqtt.connect(hostname, mqtt_username, mqtt_password)) 
                     { 
                        u8g2.setCursor(77, 2);
                      u8g2.print("MQTT");
                    } else 
                    {
                        u8g2.setCursor(77, 2);
                        u8g2.print("");
                    }
                }
            } 
            else 
            {
                u8g2.setCursor(40, 2);
                u8g2.print("Offlinemodus");
            }
            if (HEATINGLOGO == 1) // rancilio logo
            {
    
                u8g2.drawXBMP(0, 14, Rancilio_Silvia_Logo_width, Rancilio_Silvia_Logo_height, Rancilio_Silvia_Logo);
                u8g2.drawXBMP(53,14, Heiz_Logo_width, Heiz_Logo_height, Heiz_Logo);
                u8g2.setFont(u8g2_font_profont22_tf);
              //  u8g2.setCursor(64, 25);
              //  u8g2.print((bezugszeit_last_Millis - startZeit) / 1000, 1);
              //  u8g2.setFont(u8g2_font_profont11_tf);
            }
            if (HEATINGLOGO == 2) // Gaggia Logo
            {
                u8g2.drawXBMP(0, 14, Gaggia_Classic_Logo_width, Gaggia_Classic_Logo_height, Gaggia_Classic_Logo);
                u8g2.drawXBMP(53,14, Heiz_Logo_width, Heiz_Logo_height, Heiz_Logo);
                u8g2.setFont(u8g2_font_profont22_tf);
              //  u8g2.setCursor(64, 25);
              //  u8g2.print((bezugszeit_last_Millis - startZeit) / 1000, 1);
              //  u8g2.setFont(u8g2_font_profont11_tf);
            }

               // Temperatur
          u8g2.setCursor(92, 30);
          u8g2.setFont(u8g2_font_profont17_tf);
          u8g2.print(Input,1);         
          u8g2.sendBuffer();
      }
    }
    
    /********************************************************
     DISPLAY - PID Off Logo
    *****************************************************/

     void OFFlogo(void) 
    {
     if (OFFLINEGLOGO == 1 && machinestate == 90)
     {
       u8g2.clearBuffer();
       u8g2.drawXBMP(38,0, OFFLogo_width, OFFLogo_height, OFFLogo); 
       u8g2.setCursor(0, 55);
       u8g2.setFont(u8g2_font_profont10_tf);
       u8g2.print("PID is disabled manually");   
       u8g2.sendBuffer();
     }
    }
    void steamLogo(void) 
    {
     if (machinestate == 40)
     {
       u8g2.clearBuffer();
       u8g2.drawXBMP(0,0, steamlogo_width, steamlogo_height, steamlogo); 
       u8g2.setCursor(64, 25);
       u8g2.setFont(u8g2_font_profont22_tf);
       u8g2.print(Input, 0);
       u8g2.setCursor(64, 25);
       u8g2.sendBuffer();
     }
    }
#endif
