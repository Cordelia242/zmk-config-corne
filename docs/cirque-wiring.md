# Cirque Pinnacle — Conexión al Typeractive Corne (lado derecho)

El Cirque se conecta al **lado derecho** (periférico) vía I2C.
El PCB del Typeractive Corne trae un header de 5 pines para el nice!view —
ese mismo header se usa para el Cirque, ya que comparten los pines D2/D3.

---

## Por qué usar el header del nice!view

El nice!view usa SPI con 5 pines: `VCC · GND · D1(CS) · D2(MOSI) · D3(SCK)`.
El Cirque en I2C necesita:         `VCC · GND · D1(DR)  · D2(SDA)  · D3(SCL)`.

Los pines físicos **D2 y D3 son idénticos** en ambos casos (mismos pads del PCB).
D1, que en el nice!view es CS, lo reutilizamos como la señal DR del Cirque.
**Las 5 conexiones salen del mismo header — sin soldar al socket del nano.**

---

## Header del nice!view en el PCB (lado derecho)

```
  ┌─────────────────────────────────────────────┐
  │          Typeractive Corne  — right half     │
  │                                              │
  │   Header nice!view / Cirque                  │
  │   ┌───┬───┬───┬───┬───┐                     │
  │   │VCC│GND│ D1│ D2│ D3│                     │
  │   └─┬─┴─┬─┴─┬─┴─┬─┴─┬─┘                    │
  │     │   │   │   │   │                        │
  │    3.3V GND DR SDA SCL                       │
  │                 (Cirque)                     │
  └─────────────────────────────────────────────┘
```

---

## Diagrama completo de conexión

```
  PCB header (right)              Cirque Pinnacle
  ──────────────────              (breakout / FPC adapter)
                                  ┌──────────────┐
  VCC (3.3V) ──────────────────► │ VCC          │
  GND        ──────────────────► │ GND          │
  D1         ◄────────────────── │ DR           │  ← interrupción activa HIGH
  D2         ◄───────────────────│ SDA          │  ← I2C datos
  D3         ◄───────────────────│ SCL          │  ← I2C reloj
                                  └──────────────┘
```

> DR es salida del Cirque y entrada del nice!nano.
> SDA/SCL son bidireccionales (protocolo I2C).

---

## Señales — tabla de referencia

| Pad del header | Función nice!view | Función con Cirque | Pin nice!nano |
|:--------------:|:-----------------:|:------------------:|:-------------:|
| VCC            | Alimentación      | Alimentación 3.3V  | VCC           |
| GND            | Tierra            | Tierra             | GND           |
| D1             | CS (chip select)  | DR (data ready)    | D1 / P0.06    |
| D2             | MOSI (SPI data)   | SDA (I2C data)     | D2 / P0.17    |
| D3             | SCK (SPI clock)   | SCL (I2C clock)    | D3 / P0.20    |

---

## nice!nano — pines usados en el lado derecho

```
              USB-C
         ┌─────────────┐
    D1   │ ●  ◄ DR     ○ │  RAW (VBAT)
    D0   │ ○           ○ │  GND
   GND   │ ○           ○ │  RST
   GND   │ ○           ○ │  VCC  ● ◄ 3.3V
    D2   │ ●  ◄ SDA    ○ │  A3
    D3   │ ●  ◄ SCL    ○ │  A2
    D4   │ ○           ○ │  A1
    D5   │ ○           ○ │  A0
    D6   │ ○           ○ │  D15
    D7   │ ○           ○ │  D14
    D8   │ ○           ○ │  D16
    D9   │ ○           ○ │  D10
         └─────────────┘

  ● = pin usado por el Cirque
```

---

## Configuración DTS correspondiente

```c
// corne_cirque_right.overlay
&pro_micro_i2c {
    status = "okay";

    glidepoint: glidepoint@2a {
        compatible = "cirque,pinnacle";
        reg = <0x2a>;
        status = "okay";
        data-ready-gpios = <&pro_micro 1 (GPIO_ACTIVE_HIGH)>;  // D1 = DR
        sensitivity = "4x";
        sleep-mode-enable;
        // primary-tap-enable;  /* descomentar para tap-to-click */
    };
};
```

`pro_micro 1` = D1, que en el header del PCB es el pad **D1 (CS)**.
`&pro_micro_i2c` usa D2 y D3 automáticamente según la definición del board.

---

## Resistencias pull-up

I2C requiere pull-ups en SDA y SCL (típicamente 4.7 kΩ a 3.3V).
Verifica si el breakout del Cirque las incluye.
Si no, añade resistencias entre SDA↔VCC y SCL↔VCC.
