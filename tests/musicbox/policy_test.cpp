#include "musicbox_policy.hpp"
#include <assert.h>
#include <iostream>
using namespace musicbox;

int main() {
  ParentGesture wake;
  assert(wake.update(0, true, false) == ParentAction::none);
  assert(wake.update(8000, true, false) == ParentAction::none);
  wake.update(8100, false, false);
  wake.update(8200, true, false);
  assert(wake.update(11199, true, false) == ParentAction::none);
  assert(wake.update(11200, true, false) == ParentAction::shutdown);
  assert(wake.update(20000, true, false) == ParentAction::none);

  ParentGesture chord;
  chord.update(0, false, false);
  chord.update(100, true, false);
  chord.update(200, true, true);
  assert(chord.update(3200, true, true) == ParentAction::none);
  assert(chord.update(5199, true, true) == ParentAction::none);
  assert(chord.update(5200, true, true) == ParentAction::toggleWifi);
  assert(chord.update(10000, true, true) == ParentAction::none);
  chord.update(10100, false, true);
  assert(chord.update(14000, false, true) == ParentAction::none);
  chord.update(14100, false, false);
  chord.update(14200, false, true);
  assert(chord.update(17200, false, true) == ParentAction::toggleLock);

  ParentGesture aborted;
  aborted.update(0, false, false);
  aborted.update(100, true, true);
  aborted.update(1000, true, false);
  assert(aborted.update(8000, true, false) == ParentAction::none);
  aborted.update(8100, false, false);
  aborted.update(8200, true, true);
  assert(aborted.update(13200, true, true) == ParentAction::toggleWifi);

  ParentGesture rollover;
  rollover.update(0xfffff000U, false, false);
  rollover.update(0xffffff00U, false, true);
  assert(rollover.update(uint32_t(0xffffff00U + 3000U), false, true) == ParentAction::toggleLock);
  assert(!wifiExpired(599999, 0, false));
  assert(wifiExpired(600000, 0, false));
  assert(!wifiExpired(1000000, 0, true));
  assert(!wifiExpired(650000, 100000, false));
  assert(wifiExpired(uint32_t(0xffffff00U + 600000U), 0xffffff00U, false));

  StatusPulse led;
  assert(led.output(0) == Color::off);
  led.flash(100, Color::red, 2);
  assert(led.output(100) == Color::red);
  assert(led.output(220) == Color::off);
  assert(led.output(400) == Color::red);
  assert(led.output(401, true) == Color::off);
  assert(led.output(700) == Color::off);
  assert(!led.active(700));
  led.flash(0xfffffff0U, Color::blue);
  assert(led.output(30) == Color::blue);
  assert(led.output(300) == Color::off);
  std::cout << "MusicBox gesture, timeout, LED and rollover checks passed\n";
}
