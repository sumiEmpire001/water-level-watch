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
