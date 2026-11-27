#ifndef MUSICBOX_HPP
#define MUSICBOX_HPP

#include "constants.hpp"
#ifdef MUSICBOX_BEBE
#if !defined(TonUINO_Esp32) || TonUINO_Esp32 != 200
#error "MusicBox profile currently targets ESP32-WROOM-32 (TonUINO_Esp32=200)."
#endif
#if defined(FIVEBUTTONS) || defined(ROTARY_ENCODER) || defined(NEO_RING) || defined(BT_MODULE) || defined(USE_LED_BUTTONS) || defined(BUTTONS3X3) || defined(POTI)
#error "MusicBox uses three front buttons, two parent buttons and a discrete RGB LED; conflicting features must be disabled."
#endif

#include <atomic>
#include <JC_Button.h>
#include <Preferences.h>
#include "musicbox_policy.hpp"

class MusicBox {
public:
  // GPIO34 has no internal pull-up: fit an external 10k resistor to 3.3V.
  static constexpr uint8_t powerPin = 32, lockPin = 34;
  static constexpr uint8_t redPin = 14, greenPin = 21, bluePin = 27;
  void begin();
  void loop(bool playing, bool wifi, bool batteryLow);
  musicbox::ParentAction readParents();
  bool isLocked() const { return locked; }
  bool suppressFrontButtons() const { return locked || waitFrontRelease; }
  void releaseFrontButtons() { waitFrontRelease = false; }
  void toggleLock();
  void flash(musicbox::Color color, uint8_t count = 1) {
    pendingColor = color;
    pendingFlashes = count; // Start timing when the LED can actually be rendered.
  }
  void off();
  void requestLedOptions(uint8_t brightness, bool quiet) {
    ledOptions.store((quiet ? 256u : 0u) | (brightness > 32 ? 32 : brightness));
  }
  uint8_t brightness() const { return ledOptions.load() & 0xff; }
  bool quietDuringPlayback() const { return (ledOptions.load() & 256) != 0; }
private:
  Button power{powerPin, 25, true, true};
  Button lock{lockPin, 25, false, true};
  Preferences prefs;
  musicbox::ParentGesture gestures;
  musicbox::StatusPulse pulse;
  musicbox::Color pendingColor{musicbox::Color::off};
  uint8_t pendingFlashes{0};
  std::atomic<bool> locked{false};
  bool waitFrontRelease{false};
  std::atomic<uint16_t> ledOptions{12};
  uint16_t savedLedOptions{12};
  uint32_t lastWifiBlink{0}, lastBatteryBlink{0};
};
#endif
#endif
