# Cubo compacto — CAD inicial

Archivo: [cube-prototype.scad](cube-prototype.scad)

Es un modelo paramétrico de volumen e interfaz para avanzar con el concepto cúbico: cuerpo redondeado, tapa desmontable, zona circular de apoyo NFC, tres huecos para pulsadores grandes, eje para encoder y rejilla de altavoz.

## Estado

- Modelo conceptual v0.1; no es todavía un ajuste final para impresión.
- No contiene cunas de componentes, cierre final de batería ni figura NFC.
- Las dimensiones exteriores y la ubicación de controles son parámetros editables al inicio del SCAD.
- Al escoger placa, botones, altavoz, antena NFC y circuito de batería habrá que ajustar cotas, agujeros y soportes.

## Piezas

En OpenSCAD, cambia `part` a `lower` para exportar el cuerpo, `lid` para la tapa, o déjalo en `assembly` para ver el conjunto.

Antes de usarlo con una niña, la tapa de servicio y la batería deben quedar inaccesibles sin herramienta, y los controles/figuras deben quedar firmemente sujetos. Es un prototipo doméstico, no un juguete certificado.
