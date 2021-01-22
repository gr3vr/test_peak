#include  <PeakDetection.h>

#define   pin     34

PeakDetection peakDetection;

long      time_past;
float     d;
int       peak, last_peak, cont;
float     filtered;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600);
  //peakDetection.begin(10, 15, 0.5);
  //peakDetection.begin(8, 3.50, 0.30); ->  funciona
  //peakDetection.begin(10, 3.50, 0.50);  ->  funcional mejor!
  peakDetection.begin(5, 6.50, 0.70);
}

void loop() {
  if (millis() - time_past >= 105) {
    d = (float)analogRead(pin);
    d = mapfloat(d, 0.0, 4095.0, 1.0, 3.0);
    d += random(-10, 10)/100.0;
    peakDetection.add(d);
    last_peak = peak;
    peak = peakDetection.getPeak();
    //filtered = peakDetection.getFilt();
    Serial.print(2);
    Serial.print(",");
    /*Serial.print(2.5);
      Serial.print(",");*/
    Serial.print(d);
    Serial.print(",");
    /*if ((d <= 1.5) || (d >= 2.5)) {
      // flag limites pasados ALARMA
      Serial.print(1);
      } else {
      Serial.print(0);
      }
      Serial.print(",");*/
    Serial.println(peak);

    //Serial.print(",");
    //Serial.println(filtered);

    time_past = millis();
  }
}
