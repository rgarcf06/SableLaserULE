#include "ColorSable.h"
#include "SableLED.h"
#include "SonidoManager.h"
#include "MovimientoManager.h"
#include "BLEManager.h"

#define PIN_KEY         7
#define PIN_BUSY        6
#define PIN_BTN_ON      3
#define PIN_BTN_COLOR   4
#define PIN_TCS_LED     0
#define PIN_STRIP       10
#define PIN_DF_RX       20
#define PIN_DF_TX       21
#define PIN_SDA         8
#define PIN_SCL         9

bool reanudarReposo = false;

ColorSable        colorSable;
SableLED          sable(PIN_STRIP, 80, 28);
SonidoManager     sonido(Serial1, PIN_BUSY);
MovimientoManager imu(PIN_SDA, PIN_SCL);
BLEManager        ble;

bool btnOnAnterior    = HIGH;
bool btnColorAnterior = HIGH;

void leerBotones();
void gestionarIMU();

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN_ON,    INPUT_PULLUP);
  pinMode(PIN_BTN_COLOR, INPUT_PULLUP);
  pinMode(PIN_KEY,       OUTPUT);
  digitalWrite(PIN_KEY,  HIGH);


  Serial1.begin(9600, SERIAL_8N1, PIN_DF_RX, PIN_DF_TX);
  delay(2000);
  sonido.begin(25);
  delay(1000);


  sable.begin();
  sable.setColor(colorSable.getColor());

  if (!imu.begin()) {
    Serial.println("ERROR: IMU no detectada");
  }
  
  sonido.reproducirFondo(SND_REPOSO);
  delay(200);

  ble.begin("SableLaser");
}

void loop() {
  sonido.loop();
  imu.leer();
  sable.update();
  leerBotones();

  if (reanudarReposo) {
    reanudarReposo = false;
    sonido.reproducirFondo(SND_REPOSO);
  }

  if (sable.estaEncendido()) {
    gestionarIMU();
  }

  if (ble.hayColorNuevo()) {
    uint8_t r, g, b;
    ble.getRGB(r, g, b);
    sable.setColor(Adafruit_NeoPixel::Color(r, g, b));
  }

}

void leerBotones() {
  bool btnOn    = digitalRead(PIN_BTN_ON);
  bool btnColor = digitalRead(PIN_BTN_COLOR);

  if (btnOn == LOW && btnOnAnterior == HIGH) {
    delay(20);
    if (digitalRead(PIN_BTN_ON) == LOW) {
      if (!sable.estaEncendido()) {
        sonido.reproducirForzado(SND_ENCENDIDO);
        delay(150);
        sable.toggleEncendido();
      } else {
        sable.toggleEncendido();
        sonido.reproducirForzado(SND_APAGADO);
      }
    }
  }
  btnOnAnterior = btnOn;

  if (btnColor == LOW && btnColorAnterior == HIGH) {
    if (digitalRead(PIN_BTN_COLOR) == LOW) {
      if (sable.estaEncendido()) {
        colorSable.siguiente();
        sable.setColor(colorSable.getColor());
        Serial.println(colorSable.getNombre());
      }
    }
  }
  btnColorAnterior = btnColor;
}

void gestionarIMU() {
  EstadoIMU estado = imu.getEstado();

  switch (estado) {
    case IMU_GOLPE:
      if (!sable.estaAnimando()) {
        sonido.reproducirAdvert(SND_ADVERT_GOLPE);
        sable.golpe();
      }
      break;
    case IMU_MOVIMIENTO:
      sonido.reproducirAdvert(SND_ADVERT_MOVIMIENTO);
      break;
    case IMU_REPOSO:
      // no hace falta hacer nada, el /mp3 sigue en bucle
      break;
  }
}
