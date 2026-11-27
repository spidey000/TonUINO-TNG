#ifndef MUSICBOX_POLICY_HPP
#define MUSICBOX_POLICY_HPP

#include <stdint.h>

// Hardware-independent rules: unsigned subtraction also handles millis() rollover.
namespace musicbox {
enum class ParentAction { none, shutdown, toggleLock, toggleWifi };

class ParentGesture {
public:
  ParentAction update(uint32_t now, bool power, bool lock) {
    // A button held to wake the box must be released before it can turn it off.
    if (!armed) {
      if (!power && !lock) armed = true;
      return ParentAction::none;
    }
    if (!power && !lock) {
      state = State::idle;
      return ParentAction::none;
    }
    if (state == State::consumed) return ParentAction::none;
    if (state == State::idle) {
      state = power && lock ? State::chord : power ? State::power : State::lock;
      since = now;
    } else if (state != State::chord && power && lock) {
      state = State::chord;
      since = now; // Both must actually be held for the full five seconds.
    }
    if (state == State::chord) {
      if (!power || !lock) state = State::consumed;
      else if (uint32_t(now - since) >= 5000) {
        state = State::consumed;
        return ParentAction::toggleWifi;
      }
    } else if ((state == State::power && !power) || (state == State::lock && !lock)) {
      state = State::consumed;
    } else if (uint32_t(now - since) >= 3000) {
      const auto result = state == State::power ? ParentAction::shutdown : ParentAction::toggleLock;
      state = State::consumed;
      return result;
    }
    return ParentAction::none;
  }
private:
  enum class State { idle, power, lock, chord, consumed };
  State state{State::idle};
  uint32_t since{0};
  bool armed{false};
};

inline bool wifiExpired(uint32_t now, uint32_t lastActivity, bool uploading) {
  return !uploading && uint32_t(now - lastActivity) >= 10UL * 60 * 1000;
}

enum class Color { off, green, red, orange, blue };
class StatusPulse {
public:
  void flash(uint32_t now, Color value, uint8_t count = 1) {
    since = now;
    color = value;
    flashes = count;
  }
  Color output(uint32_t now, bool quiet = false) const {
    const uint32_t elapsed = uint32_t(now - since);
    if (quiet || elapsed >= uint32_t(flashes) * 300 || elapsed % 300 >= 120)
      return Color::off;
    return color;
  }
  bool active(uint32_t now) const { return uint32_t(now - since) < uint32_t(flashes) * 300; }
private:
  uint32_t since{0};
  uint8_t flashes{0};
  Color color{Color::off};
};
} // namespace musicbox
#endif
