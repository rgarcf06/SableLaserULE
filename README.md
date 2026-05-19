# ⚔️ SableLaserULE

Proyecto de sable láser interactivo desarrollado con ESP32-C3 Super Mini. Reproduce sonidos y efectos de luz reactivos al movimiento, golpes y encendido/apagado, simulando el comportamiento de un sable láser real.

---

## 👥 Equipo

- Adrián
- Ana
- Roberto

---

## 🔧 Hardware

| Componente | Descripción |
|-----------|-------------|
| ESP32-C3 Super Mini | Microcontrolador principal |
| DFMiniMp3 | Módulo reproductor de audio con tarjeta SD |
| BMI160 | Sensor IMU (acelerómetro + giroscopio) vía I2C |
| Tira LED NeoPixel | 160 LEDs WS2812B |
| Sensor TCS34725 | Sensor de color |
| Botón derecho | Encender/apagar los LEDs del sable |
| Botón izquierdo | Cambiar color de los LEDs (solo con sable encendido) |
| Botón base | Encender/apagar la alimentación del sable |

---

## 📌 Pines

| Pin | Función |
|-----|---------|
| 3 | Botón encender/apagar sable |
| 4 | Botón cambiar color |
| 6 | PIN_BUSY (DFMiniMp3) |
| 7 | KEY (alimentación) |
| 8 | SDA (I2C) |
| 9 | SCL (I2C) |
| 10 | Tira LED |
| 20 | RX DFMiniMp3 |
| 21 | TX DFMiniMp3 |

---

## 🎵 Sonidos

Los archivos de audio deben estar en la tarjeta SD dentro de la carpeta `/mp3`:

| Archivo | Evento |
|---------|--------|
| `001.mp3` | Zumbido de reposo |
| `002.mp3` | Encendido |
| `003.mp3` | Movimiento / swing |
| `004.mp3` | Apagado |
| `005.mp3` | Golpe / clash |

---

## 🎨 Colores disponibles

El botón izquierdo cicla entre los colores más icónicos de los sables láser:

- Azul
- Rojo
- Verde
- Morado
- Blanco
- Amarillo

---

## ⚙️ Funcionamiento

- **Botón base**: controla la alimentación del sable entero.
- **Botón derecho**: enciende o apaga los LEDs con animación progresiva desde el centro hacia los extremos, sincronizada con el audio.
- **Botón izquierdo**: cambia el color de los LEDs (solo funciona con el sable encendido).
- **Movimiento**: el giroscopio detecta rotación y reproduce el sonido de swing.
- **Golpe**: el acelerómetro detecta impactos bruscos, reproduce el sonido de clash y hace un destello blanco en los LEDs.
- **Reposo**: cuando no hay movimiento ni golpes, reproduce el zumbido de fondo en bucle continuo.

---

## 📦 Dependencias

Las siguientes librerías deben estar instaladas:

| Librería | Versión |
|---------|---------|
| DFPlayer Mini Mp3 by Makuna | 1.2.3 |
| Adafruit NeoPixel | 1.15.4 |
| DFRobot_BMI160 | 1.0.0 |

Instalación con Arduino CLI:
```bash
arduino-cli lib install --git-url https://github.com/Makuna/DFMiniMp3.git
arduino-cli lib install "Adafruit NeoPixel"
arduino-cli lib install "DFRobot BMI160"
```

---

## 🚀 Compilar y subir

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c3
arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:esp32c3
```

---

## 📁 Estructura del proyecto

```
PracticaFinal/
├── Codigo/
│   ├── PracticaFinal.ino
│   ├── SableLED.h / .cpp
│   ├── SonidoManager.h / .cpp
│   ├── ColorSable.h / .cpp
│   └── MovimientoManager.h / .cpp
├── 3D/
│   └── (archivos 3D del sable)
├── wav modificado/
│   └── (archivos de audio)
└── README.md
```
