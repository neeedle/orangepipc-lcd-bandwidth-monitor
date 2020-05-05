/*
 * Copyright (c) 2020 Agostino Saviano <agostinosaviano01@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "PCD8544.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>

// pin definitions
#define BACKLIGHT  7
#define CLOCK     21
#define DIN       22
#define DC        23
#define RESET     24
#define CE        25

int main(int argc, char** argv)
  {
    
    int opt;
    unsigned char i;
    
    bool backlight_on = false; // backlight off by default
    int updateInterval = 1000; // in milliseconds
    char *interface = NULL;
    
    const char command[] = "ifstat -q -i %s 0.1 1 | tail -n 1 | column -t";
    char command_buffer[59]; // length of command (43) + max length of linux network interface (15) + NULL terminator
    char line_buffer[15];
    float downspeed, upspeed; // in KB/s
    
    // argument handling
    while ((opt = getopt(argc, argv, "i:b::d:h::")) != -1)
    {
      switch(opt)
        {
          case 'b': backlight_on = true; break;
          case 'd': updateInterval = atoi(optarg); break;
          case 'h': fprintf(stderr, "Usage: %s -i interface [-d interval] [-b] \n", argv[0]); exit(EXIT_SUCCESS); break;
          case 'i': interface = optarg; break;
          default:
            fprintf(stderr, "Usage: %s -i interface [-d interval] [-b] \n", argv[0]); 
            exit(EXIT_FAILURE);
        }
    }
      
    // check mandatory parameter
    if (!interface) 
      {
        fprintf(stderr, "Required option: -i\n"); 
        exit(EXIT_FAILURE);
      }
    
    sprintf(command_buffer, command, interface); // construct command
    
    wiringPiSetup();
    
    // lazily initialize all pins as outputs
    // (change the loop accordingly if you need other pins too)
    for (int i = 0; i < 28; i++)
      pinMode (i, OUTPUT);
    
    (backlight_on) ? digitalWrite(BACKLIGHT, LOW) : digitalWrite(BACKLIGHT, HIGH); // turn backlight on or off
    
    // initialize lcd object
    PCD8544 pcd8544(CLOCK, DIN, DC, RESET, CE);
    pcd8544.begin(84, 48, CHIP_PCD8544);
    
    while(1)
      {
        // execute command
        FILE* pipe = popen(command_buffer, "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        
        try 
          {
            while (fgets(line_buffer, sizeof(line_buffer), pipe) != NULL)
              {
                // retrieve two floats from command output; we can reuse the buffer while printing
                sscanf(line_buffer, "%f %f", &downspeed, &upspeed); 
                
                // first line, print download speed
                pcd8544.setCursor(0, 0);
                sprintf(line_buffer, "DOWN %9.2f", downspeed);
                for(i = 0; i < strlen(line_buffer); i++)
                  pcd8544.write(line_buffer[i]);
                  
                
                // second line, print upload speed
                pcd8544.setCursor(0, 1);
                sprintf(line_buffer, "UP   %9.2f", upspeed);
                for(i = 0; i < strlen(line_buffer); i++)
                  pcd8544.write(line_buffer[i]);
              }
          }
        catch (...)
          {
              pclose(pipe);
              throw;
          }
        
        pclose(pipe);
        delay(updateInterval);  // can be written in a non-blocking way, but it serves no purpose
        
      }
    
    return 0;
  }
