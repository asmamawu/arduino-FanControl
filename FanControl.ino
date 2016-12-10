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
#include <avr/power.h>
#include <stdint.h>

#define RANDOM_PIN            A6
#define GROUND_MASTER_PIN     10
#define GROUND_SWITCH_PIN     11
#define VOLTAGE_SWITCH_PIN    12

#define RELAY_ON              LOW
#define RELAY_OFF             HIGH

void setup() {
   power_all_disable();

   pinMode(GROUND_MASTER_PIN, OUTPUT);
   pinMode(GROUND_SWITCH_PIN, OUTPUT);
   pinMode(VOLTAGE_SWITCH_PIN, OUTPUT);
   digitalWrite(GROUND_MASTER_PIN, RELAY_OFF);

   randomSeed(analogRead(RANDOM_PIN));
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
   int r, f, d;

   for (d = 1; d <= 4; ++d)
      for (r = 0; r < 15; ++r)
         for (f = 0; f <= 1; ++f)
            activateFan(f, d);

   for (d = 5; d <= 10; ++d)
      for (r = 0; r < 5; ++r)
         for (f = 0; f <= 1; ++f)
            activateFan(f, d);

   for (r = 0; r < 5; ++r)
      for (f = 0; f <= 1; ++f)
         activateFan(f, 1 + random(10));
}
