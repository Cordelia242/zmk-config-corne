# Cirque Pinnacle — Conexión al nice!nano (Corne derecho)

El Cirque se conecta al **lado derecho** (periférico) vía I2C.
Usa el bus I2C que comparte con el OLED (si lo hay), más un pin libre para la señal DR.

---

## Señales necesarias

| Señal Cirque | Función | Pin nice!nano |
|---|---|---|
| VCC | Alimentación 3.3V | **VCC** |
| GND | Tierra | **GND** |
| SDA | I2C datos | **D2** |
| SCL | I2C reloj | **D3** |
| DR | Data Ready (interrupción) | **D0** |

---

## nice!nano — pinout lado derecho (half)

Los pines usados están marcados con `◄`.

```
              USB-C
         ┌─────────────┐
    D1   │ ○           ○ │  RAW (VBAT)
    D0   │ ●  ◄ DR     ○ │  GND
   GND   │ ○           ○ │  RST
   GND   │ ○           ○ │  VCC  ● ◄ VCC (3.3V)
    D2   │ ●  ◄ SDA    ○ │  A3
    D3   │ ●  ◄ SCL    ○ │  A2
    D4   │ ○           ○ │  A1
    D5   │ ○           ○ │  A0
    D6   │ ○           ○ │  D15
    D7   │ ○           ○ │  D14
    D8   │ ○           ○ │  D16
    D9   │ ○           ○ │  D10
         └─────────────┘
```

> **Nota:** VCC está en la columna derecha, 4.° pin desde arriba.
> D0, D2, D3 están en la columna izquierda.

---

## Diagrama de conexión

```
  nice!nano (right)          Cirque Pinnacle
  ─────────────────          (breakout / FPC adapter)
                             ┌──────────────┐
  VCC (3.3V) ───────────────►│ VCC          │
  GND        ───────────────►│ GND          │
  D2  (SDA)  ───────────────►│ SDA          │
  D3  (SCL)  ───────────────►│ SCL          │
  D0  (DR)  ◄────────────────│ DR           │
                             └──────────────┘
```

La flecha en DR es bidireccional en sentido lógico: el Cirque
**genera** la señal DR (activa HIGH cuando hay datos listos) y el
nice!nano la **lee** como interrupción GPIO.

---

## En el PCB del Typeractive Corne

```
  PCB right half
  ┌──────────────────────────────────────┐
  │                                      │
  │  ┌─────────┐   OLED header           │
  │  │nice!nano│   ┌───────────────┐     │
  │  │ socket  │   │ VCC GND SDA SCL│    │
  │  │         │   └──┬────┬───┬───┘     │
  │  │  D2 ────┼──────┼────┼───┘  SCL   │
  │  │  D3 ────┼──────┼────┘      SDA   │
  │  │  VCC────┼──────┘           VCC   │
  │  │  GND────┼─────────────     GND   │
  │  │         │                        │
  │  │  D0 ────┼── (sin pad propio,     │
  │  │         │    soldar directo al   │
  │  └─────────┘    socket del nano)    │
  └──────────────────────────────────────┘
```

- **VCC, GND, SDA, SCL** — ya tienen pads en el header del OLED del PCB.
  Puedes conectar el Cirque en paralelo a esos mismos pads.
- **DR (D0)** — no tiene pad propio en el PCB del Corne estándar.
  Hay que soldar un cable directamente al pin D0 del socket del nice!nano
  (columna izquierda, 2.° pin desde arriba con USB hacia arriba).

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
        data-ready-gpios = <&pro_micro 0 (GPIO_ACTIVE_HIGH)>;  // D0 = DR
        sensitivity = "4x";
        sleep-mode-enable;
    };
};
```

`&pro_micro_i2c` usa D2 (SDA) y D3 (SCL) automáticamente
según la definición del board `nice_nano//zmk`.

---

## Resistencias pull-up

El I2C requiere resistencias pull-up en SDA y SCL (típicamente 4.7kΩ a 3.3V).
El PCB del Corne ya las incluye para el OLED, así que **no necesitas añadir
pull-ups extra** si el Cirque comparte el mismo bus que el OLED.

Si no hay OLED en el lado derecho, verifica si el breakout del Cirque
trae pull-ups integrados. Muchos adaptadores ya las incluyen.
