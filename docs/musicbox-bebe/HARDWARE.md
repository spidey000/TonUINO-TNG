# Hardware del perfil MusicBox_Bebe

## Montaje admitido por el firmware

ESP32-WROOM-32 con DFPlayer Mini (audio y microSD propios) y RC522 SPI. No conectar un
MAX98357A o PN5180 esperando compatibilidad con este perfil. Los GPIO son números del chip.

| Conexión | GPIO ESP32 | Nota |
|---|---:|---|
| Frontal reproducir/pausar | 33 | Pulsador a GND, pull-up interno |
| Frontal siguiente/volumen + | 25 | Pulsador a GND, pull-up interno |
| Frontal anterior/volumen − | 26 | Pulsador a GND, pull-up interno |
| Padres: encendido | 32 | Pulsador a GND, pull-up; RTC/EXT0 para despertar |
| Padres: bloqueo | 34 | Pulsador a GND; **resistencia externa 10 kΩ a 3,3 V** |
| LED rojo | 14 | Resistencia individual en serie |
| LED verde | 21 | Resistencia individual en serie |
| LED azul | 27 | Resistencia individual en serie |
| RC522 SDA/SS | 5 | SPI, no I²C |
| RC522 RST | 22 | Alimentar RC522 a 3,3 V |
| RC522 SCK/MISO/MOSI | 18/19/23 | SPI estándar WROOM |
| DFPlayer TX → ESP32 RX | 16 | UART2 |
| ESP32 TX → DFPlayer RX | 17 | Seguir requisitos de entrada del módulo; resistencia serie si procede |
| DFPlayer BUSY | 4 | Activo bajo |
| Medida batería | 35 | Divisor resistivo, nunca celda directa |
| Salida de apagado | 13 | Activa baja por defecto; opcional circuito de corte compatible |

GPIO34 es solo entrada y **no tiene pull-up interno**. El GPIO32 permite despertar con el botón
de padres. Evitar alimentar indicadores o componentes desde GPIO: usar sus fuentes y drivers.
Las funciones incompatibles con este mapa producen error de compilación al activarlas.

## LED

RGB discreto de cátodo común a GND, un resistor por ánodo. Empezar con 1 kΩ por canal a 3,3 V;
validar corriente, color y brillo con la pieza elegida. PWM a 1 kHz, tres canales independientes.
Usar un difusor pequeño retenido desde el interior. No es un WS2812/NeoPixel ni RGB de ánodo común.

## Alimentación portátil

El objetivo actual es un **pack 1S protegido de unos 20.000 mAh**, con NTC y protección
térmica autónoma. Sustituye el objetivo inicial de 5.000 mAh. La selección y los requisitos
están en [Alimentación de 20 Ah y protección térmica](SAFETY_20AH.md).
El cargador MCP73871 genérico de la lista inicial deja de ser la selección para esta revisión.
La familia BQ25606 es candidata; aún hay que cerrar pack, placa, red térmica, temporizador,
fuente y fusible. No deducir seguridad de la capacidad ni de un nombre de chip.

**El firmware actual no mide temperatura ni controla el permiso de carga.** La supervisión
adicional con dos sensores y sus acciones está especificada y pendiente de implementación.
Los cortes primarios deben funcionar por hardware con ESP32 apagado o bloqueado.
No habilitar reproducción durante carga hasta verificar el circuito final.

Regulación eficiente para el ESP32/RC522 (3,3 V) y alimentación del DFPlayer dentro de las
especificaciones del módulo elegido. Dimensionar los picos de audio y evitar conversiones
innecesarias. La placa de desarrollo puede tener regulador/LED/puente USB que consumen aun dormida;
medir el conjunto antes de tratarla como diseño final de bajo consumo.

GPIO35 mide la batería mediante divisor: como punto de partida, 100 kΩ/100 kΩ y condensador
100 nF del nodo ADC a GND. Calibrar `voltMeasCorrection` desde el panel comparando con multímetro.
El firmware usa el perfil LiPo y no sustituye la protección de la batería.
GPIO13 puede controlar un circuito externo de corte/enclavamiento compatible. El perfil
actual permite omitirlo y entrar en reposo profundo con despertar por GPIO32; el diseño
objetivo de 20 Ah requiere añadir y verificar ese corte, manteniendo la ruta de carga protegida.
Si se corta la alimentación, el botón debe restaurarla por hardware; no basta el GPIO de
un ESP32 sin energía.

## Caja y tarjetas

- Diseño sencillo de imprimir, esquinas redondeadas y montaje atornillado.
- Ranura para tarjetas NFC encapsuladas en carcasas impresas finas, de tamaño manejable.
- Lector paralelo a la tarjeta insertada; probar con el plástico y distancia definitivos.
- Separar antena de batería, altavoz y metal; añadir guías para una inserción repetible.
- Tres controles infantiles frontales y dos pequeños controles traseros hundidos para padres.
- El control de volumen queda en pulsación larga de anterior/siguiente; este perfil no usa knob.
- Altavoz mono de 8 Ω, unos 40–50 mm, elegido por sensibilidad; limitar volumen y medirlo.
- Batería, conectores, difusor y pulsadores retenidos desde dentro; batería inaccesible sin herramienta.

Antes de usar: verificar polaridad, tensiones, corriente de carga, temperatura, retención mecánica,
lectura de tarjetas, volumen y consumo tanto reproduciendo como apagado. El CAD de la rama es
un prototipo anterior: adaptar antes de imprimir la caja definitiva.
