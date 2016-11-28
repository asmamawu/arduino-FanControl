/*
 * FanControl - arduino code for controlling my fan board
 * Copyright (C) 2016 Benjamin Abendroth
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <LowPower.h>
#include <stdint.h>

#define GROUND_MASTER_PIN     A0
#define GROUND_SWITCH_PIN     A1 
#define VOLTAGE_SWITCH_PIN    A2

#define RELAY_ON              LOW
#define RELAY_OFF             HIGH

void setup() {
   pinMode(GROUND_MASTER_PIN, OUTPUT);
   pinMode(GROUND_SWITCH_PIN, OUTPUT);
   pinMode(VOLTAGE_SWITCH_PIN, OUTPUT);
   digitalWrite(GROUND_MASTER_PIN, RELAY_OFF);
}

static inline
void delayLowPowerSeconds(unsigned int secs) {
   while (secs > 8) {
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
      secs -= 8;
   }
   while (secs--) {
      LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);  
   }
}

static inline
void activateFan(uint8_t fan, unsigned int secs) {
   digitalWrite(GROUND_SWITCH_PIN, fan);
   digitalWrite(VOLTAGE_SWITCH_PIN, fan);
   digitalWrite(GROUND_MASTER_PIN, RELAY_ON);

   delayLowPowerSeconds(secs);

   digitalWrite(GROUND_MASTER_PIN, RELAY_OFF);
   digitalWrite(GROUND_SWITCH_PIN, RELAY_OFF);
   digitalWrite(VOLTAGE_SWITCH_PIN, RELAY_OFF);
}

void loop() {
   int r, f, i;

   for (i = 1; i <= 5; ++i)
      for (r = 0; r < 10; ++r)
         for (f = 0; f <= 1; ++f)
            activateFan(f, i);
}
