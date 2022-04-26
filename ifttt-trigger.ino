/*
  IFTTT Trigger

  Provide notifications when sunlight exposure for a terrarium falls above/below threshold.
*/

#include <BH1750.h>

SYSTEM_THREAD(ENABLED);

BH1750 light_sensor(0x23, Wire);

const int LIGHT_THRESHOLD = 100;
const int SAMPLE_FREQ_MS = 1000;
String currentLightState = "low";

void setup()
{
  // Open serial communications
  Serial.begin();

  // -- Initialise light sensor
  light_sensor.begin();
  //  Set mode to forced high res
  light_sensor.set_sensor_mode(BH1750::forced_mode_high_res2);
}

void loop()
{
  // Take reading with light sensor
  light_sensor.make_forced_measurement();
  float lightLevel = light_sensor.get_light_level();
  Serial.println(String::format("%f", lightLevel));

  // Publish light state when it changes
  const String prevLightState = currentLightState;
  currentLightState = lightLevel > LIGHT_THRESHOLD ? "high" : "low";
  if (prevLightState != currentLightState) {
    Particle.publish("terrarium_light", currentLightState, PRIVATE);
  }

  delay(SAMPLE_FREQ_MS);
}
