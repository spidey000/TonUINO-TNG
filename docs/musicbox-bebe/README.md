# MusicBox NFC portátil

Firmware propio sobre [TonUINO-TNG](https://github.com/tonuino/TonUINO-TNG), GPL-3.0.
La niña introduce una tarjeta NFC dentro de una carcasa impresa fina en una ranura.
Reproduce audios locales, sin móvil ni conexión permanente.

## Perfil que incorpora los controles de padres

Compilar y cargar con PlatformIO:

```sh
pio run -e MusicBox_Bebe
pio run -e MusicBox_Bebe -t upload
```

Este perfil usa **ESP32-WROOM-32**, lector **RC522** y **DFPlayer Mini** con microSD.
No es ESPuino ni usa un MAX98357A/PN5180. Las otras variantes TonUINO mantienen su comportamiento.
Los GPIO del perfil están documentados en [HARDWARE.md](HARDWARE.md).
No activar FIVEBUTTONS, encoder, potenciómetro, NeoPixel, botones iluminados o módulo Bluetooth
junto a este perfil: comparten pines con los controles propios.

## Manejo

| Acción | Resultado |
|---|---|
| Encendido trasero, mantener 3 s | Apagar; desde reposo profundo, mantener 3 s para arrancar |
| Bloqueo trasero, mantener 3 s | Bloquear/desbloquear los controles infantiles; se conserva tras reiniciar |
| Ambos traseros durante 5 s completos | Activar/desactivar wifi, sin disparar las acciones individuales |
| Introducir tarjeta conocida | Reproducir su carpeta/selección, incluso con botones bloqueados |
| Pulsación frontal breve | Pausa/reproducción, pista anterior o siguiente |
| Mantener anterior/siguiente | Bajar/subir volumen |

Un gesto se ejecuta una vez hasta soltar ambos botones. El botón usado para despertar debe
soltarse antes de poder apagar otra vez. Un botón frontal sostenido al desbloquear no se ejecuta
hasta soltarlo. Las combinaciones frontales no abren administración ni borran ajustes.
El primer arranque tras conectar alimentación es automático; la pulsación de 3 s de encendido
se aplica al despertar del reposo. Un circuito externo que corte alimentación debe proporcionar
su propio encendido y enclavamiento: comprobarlo antes de elegir ese hardware.

## Wifi y panel web

Wifi apagada en cada arranque; Bluetooth interno sin activar. Al habilitar administración,
intenta la red guardada sin bloquear el bucle de reproducción y, si no conecta en 10 s,
crea la red **MusicBox**. Sin red guardada crea el punto de acceso directamente.

- Contraseña inicial del punto de acceso: `musicbox-parent`; puede cambiarse en `/wifi`.
- En el punto de acceso: abrir `http://192.168.4.1/musicbox`.
- En la wifi de casa: abrir `http://<IP-de-la-caja>/musicbox`.
- La wifi se apaga después de 10 minutos sin peticiones/acciones de administración.
  Las actualizaciones de estado enviadas por la caja no renuevan ese plazo.
- La misma combinación de botones permite apagarla antes. Si hay una actualización OTA,
  la desactivación se pospone hasta acabar. Si la transferencia se interrumpe 30 s,
  se aborta para que no bloquee indefinidamente el apagado de la radio.
- El apagado físico solicitado por padres o por batería agotada tiene prioridad sobre OTA.
- No entra en reposo por inactividad mientras está abierta la sesión temporal de administración.

El panel `/musicbox` permite guardar brillo (0–32, escala PWM de 255) y desactivar las
indicaciones durante reproducción. Los ajustes quedan en NVS, separados de la EEPROM original.
El panel original conserva los ajustes de reproducción y escritura/asignación de tarjetas.
Las tarjetas vacías o de administración solo abren programación mientras está habilitada la wifi.

**Limitación del DFPlayer:** su microSD no está conectada al ESP32. Los audios se copian con
un ordenador a esa microSD; este fork no puede subir canciones por wifi ni usar una mera tabla
de UID como ESPuino. TonUINO escribe la configuración de reproducción en cada tarjeta NFC.
Para subir audio por web habría que cambiar la arquitectura de audio y almacenamiento.

## RGB frontal y energía

Un único RGB convencional de cátodo común, con tres resistencias, sin animación permanente:

| Estado | Indicación |
|---|---|
| Arranque | Verde breve |
| Tarjeta conocida | Verde breve |
| Tarjeta vacía/no reconocida | Dos destellos rojos |
| Bloquear/desbloquear | Naranja/verde |
| Wifi activa | Azul breve cada 5 s |
| Batería baja | Rojo breve cada 30 s; prioridad sobre la señal de wifi |

Cada destello dura 120 ms y el LED permanece apagado entre avisos.
Brillo inicial 12/255, límite 32/255; 0 lo apaga. El modo silencioso suprime indicaciones
mientras suena audio, incluidas las de batería y wifi. Un nuevo evento reemplaza el anterior.

Volumen de arranque limitado a 5/30 y techo a 15/30 del DFPlayer, también con ajustes antiguos.
Son pasos del reproductor, **no decibelios**: medir el sonido del altavoz y carcasa reales.
Reposo tras 5 minutos de pausa/inactividad por defecto, configurable entre 1 y 15 minutos.
El reposo duerme lector y DFPlayer y apaga radios/RGB. La corriente real depende de reguladores,
LED de placa y módulos: no se promete una autonomía hasta medir el montaje.
El lector RC522 conserva su protocolo de lectura original; no se ha añadido un sensor de ranura
ni lectura intermitente del campo NFC sin validación en hardware.

## Validación

```sh
cmake -S . -B build
cmake --build build -j 2
ctest --test-dir build --output-on-failure
pio run -e MusicBox_Bebe -e TonUINO_Esp32_W_3
```

Las pruebas de reglas cubren gestos superpuestos/interrumpidos, una acción por pulsación,
liberación después de despertar, timeout con actualización y desbordamiento de `millis()`,
y pulsos RGB. GitHub Actions compila este perfil y el ESP32 original.
Queda validar con hardware lectura NFC a través de la carcasa, audio, carga, despertar y autonomía.
El CAD existente sigue siendo conceptual y necesita adaptar ranura, LED y dos controles traseros.
