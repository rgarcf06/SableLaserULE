#include "ColorSable.h"
#include "SableLED.h"
#include "SonidoManager.h"
#include "MovimientoManager.h"
#include "WebManager.h"

#define PIN_KEY         7
#define PIN_BUSY        6
#define PIN_BTN_ON      3
#define PIN_BTN_COLOR   4
#define PIN_STRIP       10
#define PIN_DF_RX       20
#define PIN_DF_TX       21
#define PIN_SDA         8
#define PIN_SCL         9

bool reanudarReposo = false;

ColorSable        colorSable;
SableLED          sable(PIN_STRIP, 320, 10, 20);
SonidoManager     sonido(Serial1, PIN_BUSY);
MovimientoManager imu(PIN_SDA, PIN_SCL);
WebManager        web;

bool     btnOnAnterior    = HIGH;
bool     btnColorAnterior = HIGH;
uint32_t ultimoSwing      = 0;
#define  COOLDOWN_SWING  1100 

void leerBotones();
void gestionarIMU();

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BTN_ON,    INPUT_PULLUP);
  pinMode(PIN_BTN_COLOR, INPUT_PULLUP);

  sable.begin();
  delay(100);


  Serial1.begin(9600, SERIAL_8N1, PIN_DF_RX, PIN_DF_TX);
  delay(2000);
  sonido.begin(15);
  delay(1000);

  sable.setVelocidadApagado(2);

  sable.setColor(colorSable.getColor());

  if (!imu.begin()) {
    Serial.println("ERROR: IMU no detectada");
  }

  web.begin();
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

  web.loop();

  if (web.hayColorNuevo()) {
    uint8_t r, g, b;
    web.getRGB(r, g, b);
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
        delay(600);
        sable.toggleEncendido();
      } else {
        sable.toggleEncendido();
        sonido.reproducirForzado(SND_APAGADO);
      }
    }
  }
  btnOnAnterior = btnOn;

  if (btnColor == LOW && btnColorAnterior == HIGH) {
    delay(20);
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
  // 1. BLINDAJE: Si el sable se está encendiendo o apagando, ignoramos la IMU por completo.
  if (sable.estaAnimando() && !sable.estaEncendido()) { 
    return; 
  }

  EstadoIMU estado = imu.getEstado();

  switch (estado) {
    case IMU_GOLPE:
      // Volvemos al método nativo de anuncio compatible con tu sistema de archivos
      sonido.reproducirAdvert(SND_ADVERT_GOLPE);
      
      // Forzamos el destello por hardware en la hoja de LEDs inmediatamente
      sable.golpe(); 
      
      // TRUCO DE FIRMWARE: Forzamos la actualización del cooldown del swing 
      // para que el bucle de movimiento no sature el canal serie tras el impacto
      ultimoSwing = millis(); 
      break;

    case IMU_MOVIMIENTO:
      // Si el sable está resolviendo la animación física de un golpe anterior, 
      // bloqueamos el sonido del swing para que no se pisen en el búfer
      if ((millis() - ultimoSwing) > COOLDOWN_SWING) {
        sonido.reproducirAdvert(SND_ADVERT_MOVIMIENTO);
        ultimoSwing = millis();
      }
      break;

    case IMU_REPOSO:
      break;
  }
}
