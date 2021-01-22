#include  <PeakDetection.h>
#include  <SimpleKalmanFilter.h>

#define   pin         34
#define   limit_max   2.5
#define   limit_min   1.5

PeakDetection peakDetection;
SimpleKalmanFilter kf = SimpleKalmanFilter(0.05, 0.05, 0.07);

/*
  peakdetection.begin(lag, threshold, influence);
  
  Esta librería puede conseguirse en: https://github.com/leandcesar/PeakDetection
  
  Lag:  El parámetro de retardo determina cuánto se suavizarán 
        sus datos y qué tan adaptable es el algoritmo a los 
        cambios en el promedio a largo plazo de los datos.
          
          Ej: Si se pone 10, se necesitaran 10 muestras antes
              de que el umbral del algoritmo se ajuste.

  Threshold:  El parámetro de umbral es el número de desviaciones estándar 
              de la media móvil por encima del cual el algoritmo clasificará 
              un nuevo punto de datos como una señal. 
                
                Ej: Si una nueva muestra de datos tiene 4.0 desviaciones estándar 
                    por encima de la media móvil y el parámetro de umbral se 
                    establece en 3.5, el algoritmo identificará dicha muestra 
                    como un pico.
  
  Influence:  Este parámetro determina la influencia de las señales en el umbral 
              de detección del algoritmo. Su rango va de 0.00 a 1.00

                Ej: Si ocurre una ruptura estructural, un cambio inesperado en el tiempo, 
                    este valor parámetro deberá de acercarse más al 1.00 para que el umbral
                    sea capaz de reaccionar rápidamente.
*/

float     d;                        // Variable para guardar la data entrante
long      time_past;                // Variable para guardar el tiempo (en ms) transcurrido
int       limit_alert = 0;          // Alarma de límite superado
int       peak;                     // Variable para guardar el valor de la función getPeak()

/* Función para realizar un mapeo para que nos devuelva un float */
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(9600);
  peakDetection.begin(5, 6.50, 0.75);         // Inicialización de la librería de peakDetection
}

void loop() {
  if (millis() - time_past >= 50) {           // Si el tiempo es >= a 50 ms
    //d vendría siendo nuestros valores Ax, Ay, Az y Gx, Gy, Gz
    d = (float)analogRead(pin);               // Solo para demostración, para variar la señal
    d = mapfloat(d, 0.0, 4095.0, 1.0, 3.0);   // Lo llevamos a una escala para mostar en el Serial Plotter
    d += random(-10, 10) / 100.0;             // Agregamos ruido

    Serial.print(d);                          // Muestreo de la señal
    Serial.print(",");

    d = kf.updateEstimate(d);                 // Filtrado por kalman de la señal
    peakDetection.add(d);                     // Agregamos el valor de la señal filtrada a la función de detección
    peak = peakDetection.getPeak();           // Y guardamos en la variable el valor de dicha función, su valor
    Serial.print(2);
    Serial.print(",");
    Serial.print(d);
    Serial.print(",");
    if ((d <= limit_min) || (d >= limit_max)) { // Si la señal ha pasado los límites establecidos establecemos una alarma
      limit_alert = 1;                        
    } else {
      limit_alert = 0;
    }
    Serial.print(limit_alert);
    Serial.print(",");
    Serial.println(peak);

    time_past = millis();
  }
}
