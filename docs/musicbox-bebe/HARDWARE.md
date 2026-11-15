# Diseño de hardware — primera iteración

## Prioridades

- Manejo comprensible sin leer: una zona NFC grande y botones con iconos en relieve.
- Offline: pistas en microSD; sin depender de Wi-Fi ni de un móvil para reproducir.
- Reparabilidad: componentes atornillados y cableados con conectores; no pegar la batería ni la placa.
- Carcasa compacta, redondeada, sin bordes afilados ni tornillos accesibles desde fuera salvo el cierre de servicio.

## Distribución de controles

- Zona NFC amplia en la cara superior, marcada con relieve.
- Cuatro botones momentáneos de gran diámetro, contrastados y con símbolos distintos: pausa, anterior, siguiente y volumen.
- Interruptor principal recessed/protegido en un lateral; evitar un mando que pueda desprenderse.
- Rejilla de altavoz integral con huecos que no permitan introducir piezas; altavoz atornillado por dentro.
- USB de carga en un rebaje. Tapa o acceso solo para adultos.

Los botones comerciales de 12 mm de algunos diseños no son suficientemente grandes para el objetivo. La carcasa debe diseñarse para pulsadores momentáneos robustos y fijados desde el interior; el firmware puede mapearlos a las funciones soportadas.

## Alimentación

Usar una batería 1S Li-ion protegida de fabricante conocido, con cargador y circuito de power-path/load-sharing adecuados para poder cargar mientras el sistema está apagado. La electrónica debe tener protección contra sobrecarga, sobredescarga, sobrecorriente y cortocircuito; añadir fusible si la placa elegida no lo integra. Dimensionar el regulador a partir de la corriente real del amplificador y la placa.

No conectar una celda Li-ion directamente a una placa cargadora genérica sin verificar protección, salida y uso simultáneo. No cargar con la caja abierta ni dejarla al alcance durante la carga. En la primera versión, tratar la batería como módulo sellado y reemplazable solo por un adulto.

## NFC y figuras

Usar etiquetas compatibles con el lector elegido. Integrar cada etiqueta dentro de una figura grande, de una sola pieza, sin imanes ni partes pequeñas; no usar llaveros, fichas pequeñas ni cordones. Verificar lectura a través del espesor final del plástico y mantener cobre, batería y altavoz alejados de la antena.

## Impresión y ensamblaje

- PETG como primera opción para una carcasa más tenaz que PLA; evitar piezas finas o decoraciones desprendibles.
- Paredes gruesas, radios amplios en esquinas, refuerzos alrededor de pulsadores y altavoz.
- Tornillos de cierre ocultos y insertos metálicos; el compartimento de batería solo se abre con herramienta.
- Evitar adhesivos como único medio de retención de batería, altavoz, botones o antena.
- Probar ajuste y lectura NFC con piezas de prueba antes de imprimir la carcasa completa.

## Verificaciones antes de uso

1. Comprobar polaridad, corriente de carga, temperatura y tensión de salida sin la niña presente.
2. Probar que ningún botón, tapa, rejilla o inserto se desprende al tirar o golpear de forma razonable.
3. Comprobar que la batería no puede extraerse sin herramienta y que no hay cables accesibles.
4. Probar la caja a volumen limitado y revisar que no se calienta de forma anormal.
5. Inspeccionar las figuras NFC por grietas o delaminación; retirarlas si se dañan.
