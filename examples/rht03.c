/*
 * rht03.c:
 *	Driver for the MaxDetect series sensors
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>

#include <wiringPi.h>
#include <maxdetect.h>

#define	RHT03_PIN	7
#define TEMP_URL_SIZE	200
#define HUM_URL_SIZE	200
#define BUFFER_SIZE	10

#define TEMP_BASE_URL	"www.ouroboros.com/temp/"	
#define HUM_BASE_URL	"www.ouroboros.com/hum/"	

/*
 ***********************************************************************
 * The main program
 ***********************************************************************
 */

int main (void)
{
  int temp, rh ;
  int newTemp, newRh ;

  temp = rh = newTemp = newRh = 0 ;

  wiringPiSetup () ;
  piHiPri       (55) ;

  char tempURL[TEMP_URL_SIZE] = TEMP_BASE_URL;
  char humURL[HUM_URL_SIZE] = HUM_BASE_URL;
  char buffer[BUFFER_SIZE];


  for (;;)
  {

    delay (1000) ;
    
    if (!readRHT03 (RHT03_PIN, &newTemp, &newRh))
      continue ;

    if ((temp != newTemp) || (rh != newRh))
    {
      temp = newTemp ;
      rh   = newRh ;
      if ((temp & 0x8000) != 0)	// Negative
      {
	temp &= 0x7FFF ;
	temp = -temp ;
      }
      //printf ("Temp: %5.1f, RH: %5.1f%%\n", temp / 10.0, rh / 10.0) ;
    }

    printf ("Temp: %5.1f, RH: %5.1f%%\n", temp / 10.0, rh / 10.0) ;
    
    // clear buffer, add temp value to buffer and concatonate to url
    memset(&buffer[0], 0, sizeof(buffer));
    memset(&tempURL[0], 0, sizeof(tempURL));
    strcpy(tempURL, TEMP_BASE_URL);
    sprintf(buffer, "%d", temp / 10 );
    strcat(tempURL, buffer );

    // repeat for humidity readings
    memset(&buffer[0], 0, sizeof(buffer) );
    memset(&humURL[0], 0, sizeof(humURL) );
    strcpy(humURL, HUM_BASE_URL);
    sprintf(buffer, "%d", rh / 10 );
    strcat(humURL, buffer );

    //send measurements to server
    printf("%s\n", tempURL );
    printf("%s\n", humURL );

  }

  return 0 ;
}
