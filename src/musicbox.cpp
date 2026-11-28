#include "musicbox.hpp"
#ifdef MUSICBOX_BEBE

void MusicBox::begin() {
  power.begin();
  lock.begin();
  prefs.begin("musicbox", false);
  locked = prefs.getBool("locked", false);
  requestLedOptions(prefs.getUChar("brightness", 12), prefs.getBool("quiet", false));
  savedLedOptions = ledOptions.load();
  const uint8_t pins[] = {redPin, greenPin, bluePin};
  for (uint8_t channel = 0; channel < 3; ++channel) {
    pinMode(pins[channel], OUTPUT);
    digitalWrite(pins[channel], LOW);
    ledcSetup(channel, 1000, 8);
    ledcAttachPin(pins[channel], channel);
    ledcWrite(channel, 0);
  }
  flash(musicbox::Color::green);
}

musicbox::ParentAction MusicBox::readParents() {
  power.read();
  lock.read();
  return gestures.update(millis(), power.isPressed(), lock.isPressed());
}

void MusicBox::toggleLock() {
  locked = !locked;
  waitFrontRelease = true; // Never replay a child's held button on unlocking.
  prefs.putBool("locked", locked);
  flash(locked ? musicbox::Color::orange : musicbox::Color::green);
}

void MusicBox::off() {
  for (uint8_t channel = 0; channel < 3; ++channel) ledcWrite(channel, 0);
}

void MusicBox::loop(bool playing, bool wifi, bool batteryLow) {
  const uint16_t options = ledOptions.load();
  if (options != savedLedOptions) {
    savedLedOptions = options;
    prefs.putUChar("brightness", options & 0xff);
    prefs.putBool("quiet", (options & 256) != 0);
  }
  const uint32_t now = millis();
  if (pendingFlashes) {
    pulse.flash(now, pendingColor, pendingFlashes);
    pendingFlashes = 0;
  }
  if (!pulse.active(now)) {
    if (batteryLow && uint32_t(now - lastBatteryBlink) >= 30000) {
      lastBatteryBlink = now;
      pulse.flash(now, musicbox::Color::red);
    } else if (wifi && uint32_t(now - lastWifiBlink) >= 5000) {
      lastWifiBlink = now;
      pulse.flash(now, musicbox::Color::blue);
    }
  }
  const auto color = pulse.output(now, playing && (options & 256));
  const uint8_t level = options & 0xff;
  ledcWrite(0, color == musicbox::Color::red || color == musicbox::Color::orange ? level : 0);
  ledcWrite(1, color == musicbox::Color::green ? level : color == musicbox::Color::orange ? level / 3 : 0);
  ledcWrite(2, color == musicbox::Color::blue ? level : 0);
}
#endif
