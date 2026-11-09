# water-level-watch

Tank-level monitoring with distance-to-fill conversion, pump hysteresis, timestamp ordering and stale-reading rejection. This is a software decision engine, not hardware control; do not connect pumps without independent safety interlocks.

## Requirements and tests

Python 3.10+; standard library only.

```bash
python -m unittest discover -s tests -v
```

## Example

```python
from core import Reading, WaterLevelMonitor
monitor = WaterLevelMonitor(100)
print(monitor.record(Reading(1, 90), now=1))
```

GitHub Actions runs the unit tests across Python 3.10, 3.12 and 3.13. Check CI status before treating a push as verified.

## Arduino / ESP32 C++ firmware

The Python implementation is retained. The C++11 logic is at
`arduino/WaterLevelWatch/src/WaterLevelWatch.h` and is tested under CMake.
It validates distance, time ordering, tank height and stale readings, and
uses low/high water-level hysteresis. Invalid readings request pump OFF.

The `UltrasonicIndicator` Arduino sketch demonstrates an HC-SR04-style
ultrasonic distance sensor with LED-only pump recommendation. Example pins:
Uno trigger D2, echo D3, LED D8; classic ESP32 trigger GPIO26,
echo GPIO27, LED GPIO18. Measure `TANK_HEIGHT_CM` for your tank.
**Level-shift the HC-SR04's 5V ECHO to 3.3V before connecting to an ESP32.**

Build/test the reusable C++ code:
```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
GitHub Actions additionally attempts Arduino Uno and ESP32 firmware compilation.
Neither calibration nor hardware have been tested. The sketch controls an LED,
not a pump; never connect a physical pump without independent electrical,
dry-run, overflow, and fail-safe protections. The example's `millis()` clock
is not rollover-safe for unattended long-term deployment.
