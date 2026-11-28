# Alimentación de 20 Ah y protección térmica

Revisión de diseño: 28 de noviembre de 2026. Sustituye el objetivo anterior de 5 Ah.
Es una especificación de hardware para desarrollar y verificar, no un montaje certificado
ni una declaración de que el firmware existente mida temperaturas.

## Arquitectura elegida

Pack Li-ion **1S, 3,6–3,7 V nominales, 4,2 V al finalizar la carga, unos 20 Ah**,
ensamblado por un fabricante con protección eléctrica y térmica documentadas. No usar un
pack de 7,4/11,1 V ni trasladar los mAh de un powerbank a la salida de 5 V.

La batería debe incorporar protección contra sobretensión, subtensión, sobrecorriente y
cortocircuito, así como protección térmica durante carga y descarga. El proveedor debe
especificar qué protecciones son autónomas y qué salida de sonda queda para el cargador.
Una sonda NTC accesible no demuestra por sí sola que el pack corte por temperatura.
No montar celdas sueltas ni poner baterías independientes en paralelo dentro de la caja.

Ruta de energía:

- Fuente USB-C de 5 V con corriente y cable documentados, protección y limitación de entrada.
- Cargador conmutado 1S CC/CV de 4,2 V, power-path, entrada NTC y permiso de carga externo.
- Pack protegido con fusible DC junto a su salida; dimensionar según corriente de carga,
  descarga, curva del fusible y capacidad del cableado. La carga también atraviesa esa ruta.
- Salida SYS del cargador hacia regulación de 3,3 V y alimentación del audio dentro de
  especificaciones. SYS puede superar 3,3 V: nunca conectarla directamente al ESP32 o RC522.
- Corte/enclavamiento del sistema por hardware. El apagado del reproductor no debe anular
  la protección del pack ni la supervisión autónoma de la carga.

## Componentes y referencias

| Elemento | Elección de diseño | Situación |
|---|---|---|
| Pack | Fabricante español BPI Batteries; solicitar 1S de 20 Ah con NTC y protección térmica de carga y descarga | Su catálogo contiene packs de 20 Ah y fabricación a medida; no confirma que esa variante incluya la sonda o el corte térmico requeridos |
| Cargador | Familia TI BQ25606, conmutada, hasta 3 A y power-path | Candidato técnico; falta cerrar una placa con esquema, límites y montaje verificables |
| Referencia de banco | TI BQ25606EVM-772 | Placa de evaluación; referencia para ensayos, no pieza aprobada automáticamente para la caja infantil |
| Sonda primaria de batería | NTC integrada por el fabricante, con curva y tolerancia conocidas y compatibles con la entrada TS | Obligatoria; no sustituirla por una resistencia fija |
| Supervisión adicional | Dos DS18B20 originales, alimentados con tres hilos: uno sobre la batería y otro en la zona del cargador | Su lectura y actuación por firmware están pendientes de implementar |
| Barrera térmica independiente | Protección térmica del pack y bloqueo de carga por NTC; añadir comparador/interruptor térmico si el conjunto no asegura la ventana de temperatura especificada | Debe actuar sin ESP32 y con el reproductor apagado |
| Fusible y conectores | Fusible DC no rearmable, conectores polarizados con retención y cableado dimensionado | Referencias y calibres pendientes del pack y de la corriente final |

No comprar el antiguo MCP73871 genérico como cargador cerrado para esta revisión.
No se da por validada una placa comercial solo porque anuncie el nombre de un chip TI.
Conservar ESP32-WROOM-32, RC522, NTAG213, DFPlayer y los controles de padres.
El microrruptor de ranura sigue siendo una mejora prevista y no está implementado.

## Límites y comportamiento requeridos

Estos son objetivos provisionales del proyecto, no límites universales de las baterías.
Los límites finales deben ser iguales o más restrictivos que los del pack concreto, con
margen por tolerancia de sensores, montaje y retardo térmico.

| Condición | Acción requerida |
|---|---|
| Batería fuera de 0–45 °C | Bloqueo de carga independiente del ESP32, incluso apagado |
| Batería a partir de 40 °C durante carga o por debajo de 5 °C | Supervisión adicional solicita suspensión de carga |
| Batería a partir de 45 °C durante uso | Detener audio, inhibir carga y solicitar corte seguro |
| Zona del cargador a partir de 60 °C | Inhibir carga y detener uso; no confundir esta medida con la temperatura interna del chip |
| Sonda ausente, cortocircuitada, CRC inválido o lectura caducada | Estado de fallo; no interpretar la ausencia como temperatura normal |
| Arranque/reinicio | Carga bajo control térmico autónomo; permiso suplementario cerrado hasta validar las lecturas |
| Fallo detectado | Registrar causa y señal roja prioritaria, incluso con modo silencioso o brillo de usuario a cero; sin sonido de alarma fuerte |
| Enfriamiento posterior a un fallo grave | No reanudar audio automáticamente; exigir intervención de un adulto y temperaturas normales |

La sonda NTC del cargador no debe conectarse también al ADC del ESP32 sin una interfaz
que garantice que la medida no altera los umbrales. Usar las sondas digitales separadas.
No activar cargas hasta disponer de lecturas posteriores a conversiones completas.
Identificar las sondas digitales por sus ROM y función física; no confiar en el orden de
enumeración. No exponer umbrales de protección como ajustes editables del panel web.

Una entrada NTC del BQ25606 no garantiza la ventana 0–45 °C: su perfil JEITA nominal
admite un intervalo más amplio y reduce tensión en caliente. Revisar y verificar la red
TS o añadir una barrera externa para cumplir el límite del pack; no usar el valor nominal
de JEITA como prueba de compatibilidad térmica.

## Corriente, temporizador y carga

El BQ25606 admite hasta 3 A y tiene temporizador nominal de seguridad de 10 horas para
carga normal. Una batería de 20 Ah a 2 A necesita ya 10 horas ideales antes de la fase CV:
no es una combinación validada para una carga completa. A 3 A, la fase CC ideal ronda
6,7 horas, pero todavía hay que comprobar fase CV, reducciones térmicas, consumo de la
caja y corriente que acepta el pack. El límite de entrada y la fuente también cuentan.

No desactivar ni reiniciar periódicamente el temporizador para aparentar una carga completa.
Si el ensayo no termina con margen, seleccionar otra placa con temporizador configurable
y protecciones documentadas. No prometer corriente ni tiempo de carga por el conector USB-C.
En la primera validación no permitir reproducción durante la carga; habilitarla solo tras
verificar power-path, márgenes de entrada y comportamiento térmico del conjunto.

## Integración física

- NTC acoplada a las celdas por el fabricante; sondas adicionales retenidas y aisladas
  eléctricamente. Un sensor que mide el aire de la caja no sustituye al de la batería.
- Compartimento de batería separado de la placa de carga, sin compresión ni tornillos
  apuntando a las celdas; respetar las holguras que especifique el fabricante.
- Batería y electrónica retenidas mecánicamente; no depender solo de adhesivo.
- Conectores y cables inaccesibles sin herramienta, con alivio de tracción.
- Verificar temperatura de superficies accesibles en la carcasa terminada. Los umbrales
  internos de la batería o del chip no son límites de temperatura de contacto infantil.
- Mantener batería, cobre y metal alejados de la antena RC522 y probar las tarjetas reales.

## Ensayos pendientes para cerrar la selección

Verificar los umbrales con termistor simulado y carga electrónica o batería simulada;
no provocar un cortocircuito ni sobrecalentar una batería real para probar protecciones.

1. Documentar referencia y revisión de pack, cargador, sondas, fuente, fusible y conectores.
2. Comprobar que NTC abierta/cortocircuitada bloquea la carga sin intervención del ESP32.
3. Simular frío y calor, ESP32 apagado/bloqueado y desconexión de sensores.
4. Verificar cierre seguro del permiso de carga al arrancar, reiniciar y perder alimentación.
5. Medir temperaturas y corriente en una carga completa, incluida la fase CV y el temporizador.
6. Medir uso, espera y apagado real, y caída de tensión con audio y administración wifi.
7. Validar retención de las sondas, fusible, polaridad y temperaturas superficiales en la caja.

## Fuentes primarias

- [BPI Batteries: packs y fabricación a medida](https://bpibatteries.com/productos/litio-recargable/).
- [TI BQ25606: características y hoja de datos](https://www.ti.com/product/BQ25606).
- [Hoja de datos BQ25606: TS, JEITA y temporizador](https://www.ti.com/lit/ds/symlink/bq25606.pdf).
- [TI BQ25606EVM-772: placa de evaluación](https://www.ti.com/tool/BQ25606EVM-772).
- [Analog Devices DS18B20: alimentación, ROM, CRC y conversión](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf).

No hay todavía presupuesto cerrado, confirmación de stock del pack a medida, esquema
final, firmware térmico ni pruebas físicas del conjunto. Las estimaciones anteriores de
autonomía son hipótesis y deberán recalcularse con las pérdidas del circuito elegido.
