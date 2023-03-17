void setup() {
  // put your setup code here, to run once:
#include <Arduino.h>

const int SENSOR_PIN[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const int NUM_SENSORS = sizeof(SENSOR_PIN) / sizeof(SENSOR_PIN[0]);

volatile bool player_turn = false; // false for player 1, true for player 2
volatile unsigned long player_start_time[2] = {0, 0}; // start time for each player
unsigned long player_time_left[2] = {10*60*1000, 10*60*1000}; // time left for each player
unsigned long last_time_update = 0; // last time the display was updated

void handle_interrupt() {
  unsigned long current_time = millis();
  if (player_turn) {
    player_time_left[1] -= (current_time - player_start_time[1]);
    player_start_time[0] = current_time;
  } else {
    player_time_left[0] -= (current_time - player_start_time[0]);
    player_start_time[1] = current_time;
  }
  player_turn = !player_turn;
}

void setup() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_PIN[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN[i]), handle_interrupt, FALLING);
  }
}

void loop() {
  unsigned long current_time = millis();
  if (current_time - last_time_update >= 1000) {
    last_time_update = current_time;
    int minutes[2] = {player_time_left[0] / (60 * 1000), player_time_left[1] / (60 * 1000)};
    int seconds[2] = {(player_time_left[0] / 1000) % 60, (player_time_left[1] / 1000) % 60};
    Serial.print("Player 1: ");
    Serial.print(minutes[0]);
    Serial.print(":");
    Serial.print(seconds[0]);
    Serial.print("   Player 2: ");
    Serial.print(minutes[1]);
    Serial.print(":");
    Serial.print(seconds[1]);
    Serial.println();
  }
}
}

void loop() {
  // put your main code here, to run repeatedly:

}
