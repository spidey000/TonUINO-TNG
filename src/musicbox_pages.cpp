#include "constants.hpp"
#ifdef MUSICBOX_BEBE
#include "webservice_pages.hpp"

const char musicbox_html[] PROGMEM = R"html(
<!doctype html><html lang="es"><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MusicBox · Padres</title>
<style>
body{font:17px system-ui,sans-serif;margin:0;color:#24332b;background:#f5f4ef}
main{max-width:36rem;margin:auto;padding:24px}h1{font-size:28px}
label{display:block;margin:24px 0 8px}input[type=range]{width:100%}
button{background:#335943;color:white;border:0;padding:12px 20px;font:inherit;cursor:pointer}
a{color:#335943}small{display:block;line-height:1.5;color:#526256}output{font-weight:600}
</style>
<main><h1>MusicBox</h1>
<p>La wifi se apaga tras 10 minutos sin actividad. La música funciona sin conexión.</p>
<p id="lockState"></p>
<form id="options">
<label for="brightness">Brillo del LED: <output id="level">12</output></label>
<input id="brightness" name="brightness" type="range" min="0" max="32" value="12">
<small>0 apaga el LED. El brillo máximo está limitado para mantener indicaciones suaves.</small>
<label><input id="quiet" type="checkbox"> Apagar las indicaciones mientras suena audio</label>
<button>Guardar</button><p id="result" role="status"></p>
</form>
<p><a href="/settings">Volumen y reproducción</a> · <a href="/">Tarjetas NFC y controles</a> · <a href="/wifi">Red wifi</a></p>
<small>Encendido o bloqueo: mantener su botón trasero 3 segundos.
Ambos durante 5 segundos: activar o desactivar wifi. El bloqueo permite seguir usando tarjetas.
Los audios se copian a la microSD del DFPlayer desde un ordenador; este hardware no permite subirlos por wifi.</small>
</main>
<script>
const brightness=document.getElementById('brightness'),quiet=document.getElementById('quiet');
const result=document.getElementById('result'),level=document.getElementById('level');
brightness.oninput=()=>level.textContent=brightness.value;
async function load(){
 try{
  const response=await fetch('/musicbox_settings');if(!response.ok)throw Error();
  const data=await response.json();brightness.value=data.brightness;level.textContent=data.brightness;
  quiet.checked=data.quiet;document.getElementById('lockState').textContent='Botones infantiles: '+(data.locked?'bloqueados':'activos');
 }catch{result.textContent='No se pudo conectar. Activa la wifi con los botones traseros.';}
}
document.getElementById('options').onsubmit=async(event)=>{
 event.preventDefault();
 try{
  const response=await fetch('/musicbox_settings',{method:'POST',body:new URLSearchParams({brightness:brightness.value,quiet:quiet.checked?'1':'0'})});
  if(!response.ok)throw Error();result.textContent='Guardado';
 }catch{result.textContent='No se pudo guardar. Comprueba la conexión.';}
};
load();
</script></html>
)html";
#endif
