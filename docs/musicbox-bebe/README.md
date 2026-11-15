# MusicBox NFC para bebé

Proyecto personal de caja musical offline, imprimible en 3D y modificable con vibe coding. La niña coloca una figura NFC grande sobre la zona marcada y empieza la música. Los controles principales son botones físicos grandes, con funciones simples.

## Base elegida

- Firmware: [tonuino/TonUINO-TNG](https://github.com/tonuino/TonUINO-TNG), Arduino, GPL-3.0. Incluye soporte de RFID/NFC, modos de reproducción y configuraciones de botones; es código fuente modificable.
- Carcasa: diseño propio derivado de las medidas de los componentes elegidos. No reutilizar la carcasa V4 de The Open Music Box como base de firmware: ese proyecto distribuye el firmware compilado, no el código fuente, por lo que no es una buena base para modificar lógica con vibe coding.
- Objetivo de primera versión: reproducción local desde microSD; sin pantalla, sin cuenta ni conexión necesaria para escuchar.

## Interacción prevista

1. Interruptor principal protegido para encender/apagar.
2. Poner una figura NFC grande en la zona superior: iniciar su álbum/lista.
3. Botones grandes: reproducir/pausar, anterior, siguiente y volumen.
4. Retirar la figura puede pausar la música, si la configuración elegida lo permite.

La configuración definitiva de botones y el comportamiento NFC se validarán con el firmware y la electrónica concretos antes de dibujar la carcasa final.

## Carpetas

- `HARDWARE.md`: criterios de seguridad, carcasa y montaje.
- `BOM.csv`: lista inicial de compra con decisiones pendientes señaladas.
- `cad/`: archivos CAD/STL se incorporarán cuando se defina el tamaño de botones y la placa.
- `firmware/`: configuración y cambios propios sobre TonUINO-TNG, después de crear el fork.

## Estado

- [x] Elegida una base de firmware open source para modificar.
- [x] Definidos los requisitos principales y BOM preliminar.
- [x] Fork creado: `spidey000/TonUINO-TNG`.
- [ ] Elegir y probar lector NFC, placa y circuito de batería.
- [ ] Prototipo de botones y carcasa.
- [ ] Validar audio, autonomía y cierre mecánico.

## Aviso de uso

Es un prototipo doméstico, no un juguete certificado. A esta edad debe usarse bajo supervisión adulta. No entregar tarjetas pequeñas, llaveros NFC ni piezas desprendibles; crear figuras grandes con la etiqueta NFC encapsulada. La batería debe quedar inaccesible dentro de un compartimento atornillado y el equipo no debe cargarse mientras lo usa la niña.
