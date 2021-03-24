void machinestatevoid() 
{
  /* 
  00 = init
  10 = kaltstart
  19 = Setpoint -1 Celsius
  20 = Setpoint überschritten, idel at setpoint
  30 = Bezug 
  35 = Nachlauf BD
  40 = Dampf
  50 = Blackflush
  80 = Emergency Stop
  90 = PID Offline
  100 = Sensorerror
  */
  //DEBUG_println(machinestate);
  switch (machinestate) 
  {
    // init
    case 0: 
      if (Input < (BrewSetPoint-1) )
      {
        machinestate = 10 ; // kaltstart
      }
      if (Input >= (BrewSetPoint-1) )
      {
        machinestate = 19 ; // machine is hot, jump to other state
      }
      
      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
      if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
      if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;

     // kaltstart
    case 10: 
      if (Input >= (BrewSetPoint-1) )
      {
        machinestate = 19 ;
      }
      if (SteamON == 1)
      {
        machinestate = 40 ; // Steam
      }

      if
      (
       (bezugsZeit > 0 && ONLYPID == 1) || // Bezugszeit bei Only PID  
       (ONLYPID == 0 && brewcounter > 10 && brewcounter <= 42) 
      )
      {
        machinestate = 30 ; // Brew
      }

      if (SteamON == 1)
      {
        machinestate = 40 ; // switch to  Steam
      }
      if ((backflushON))
      {
        machinestate = 50 ; // Backflush
      } 
      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
      // Setpoint -1 Celsius
      case 19: 
      if (Input >= (BrewSetPoint))
      {
        machinestate = 20 ;
      }
      if
      (
       (bezugsZeit > 0 && ONLYPID == 1) || // Bezugszeit bei Only PID  
       (ONLYPID == 0 && brewcounter > 10 && brewcounter <= 42) 
      )
      {
        machinestate = 30 ; // Brew
      }

      
      if (SteamON == 1)
      {
        machinestate = 40 ; // Steam
      }

      if (SteamON == 1)
      {
        machinestate = 40 ; // switch to  Steam
      }
      if ((backflushON))
      {
        machinestate = 50 ; // Backflush
      }
      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }  
    break;
    // normal PID
    case 20: 
      if
      (
       (bezugsZeit > 0 && ONLYPID == 1) || // Bezugszeit bei Only PID  
       (ONLYPID == 0 && brewcounter > 10 && brewcounter <= 42) 
      )
      {
        machinestate = 30 ; // Brew
      }
      if (SteamON == 1)
      {
        machinestate = 40 ; // Steam
      }
      if ((backflushON))
      {
        machinestate = 50 ; // Backflush
      }

      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;
     // Brew
    case 30:
      if
      (
       (bezugsZeit > 35*1000 && Brewdetection == 1 && ONLYPID == 1  ) ||  // 35 sec later and BD PID aktive SW Solution
       (bezugsZeit == 0      && Brewdetection == 3 && ONLYPID == 1  ) ||  // Voltagesensor reset bezugsZeit == 0
       ((brewcounter == 10 || brewcounter == 43)   && ONLYPID == 0  ) // switchoff BD PID aktive
      )
      {
       if ((ONLYPID == 1 && Brewdetection == 3) || ONLYPID == 0 ) // only delay of shotimer for voltagesensor or brewcounter
       {
         machinestate = 31 ;
         lastbezugszeitMillis = millis() ; // for delay
        
       }
       if (ONLYPID == 1 && Brewdetection == 1 && timerBrewdetection == 1) //direct to PID BD
       {
         machinestate = 35 ;
       }
      } 
      if (SteamON == 1)
      {
        machinestate = 40 ; // Steam
      }

      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;
    // Sec after shot finish
    case 31: //lastbezugszeitMillis
      if ( millis()-lastbezugszeitMillis > BREWSWITCHDELAY )
      {
       machinestate = 35 ;
       lastbezugszeit = 0 ;
      }
      if (SteamON == 1)
      {
        machinestate = 40 ; // Steam
      }

      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;
    // BD PID
    case 35:
      if (timerBrewdetection == 0)
      {
        machinestate = 20 ; // switch to normal PID
      }
      if
      (
       (bezugsZeit > 0 && ONLYPID == 1  && Brewdetection == 3) || // New Brew inner BD only by Only PID AND Voltage Sensor
       (ONLYPID == 0 && brewcounter > 10 && brewcounter <= 42) 
      )
      {
        machinestate = 30 ; // Brew
      }
      
      if (SteamON == 1)
      {
        machinestate = 40 ; // switch to  Steam
      }
      if ((backflushON))
      {
        machinestate = 50 ; // Backflush
      }
      if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
      if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;
    // Steam
    case 40:
      if (SteamON == 0)
      {
        machinestate = 20 ; //  switch to normal
      }

     if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }
    break;
    // Backflush
    case 50:
      if (backflushON == 0)
      {
        machinestate = 20 ; //  switch to normal
      }

     if (emergencyStop)
      {
        machinestate = 80 ; // Emergency Stop
      }
     if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
     if(sensorError)
      {
        machinestate = 100 ;// sensorerror
      }

    // emergencyStop 
    case 80: 
      if (!emergencyStop)
      {
        machinestate = 20 ; // normal PID
      }
      if (pidON == 0)
      {
        machinestate = 90 ; // offline
      }
      if(sensorError)
      {
        machinestate = 100 ;
      }
    break;
    // PID offline
    case 90: 
      if (pidON == 1)
      {
        if (Input >= (BrewSetPoint-1) )
        {
        machinestate = 19 ;
        }
        if (Input >= (BrewSetPoint))
        {
          machinestate = 20 ;
        }
      }
      if(sensorError)
      {
        machinestate = 100 ;
      }
    break;
    // sensor error
    case 100:
    // Nothing will happend :()
    break;
  } // switch case
} // end void