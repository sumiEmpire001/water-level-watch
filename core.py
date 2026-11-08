from dataclasses import dataclass
from math import isfinite

@dataclass(frozen=True)
class Reading:
    timestamp: int
    distance_cm: float

class WaterLevelMonitor:
    def __init__(self, tank_height_cm, low_percent=20, high_percent=80, max_age_seconds=300):
        if not all(isfinite(v) for v in (tank_height_cm, low_percent, high_percent)) or not (tank_height_cm > 0 and 0 <= low_percent < high_percent <= 100):
            raise ValueError("invalid tank or thresholds")
        if type(max_age_seconds) is not int or max_age_seconds <= 0:
            raise ValueError("invalid max age")
        self.height, self.low, self.high, self.max_age = tank_height_cm, low_percent, high_percent, max_age_seconds
        self.last = None
        self.pump_on = False

    def record(self, reading, now):
        if type(reading.timestamp) is not int or type(now) is not int or reading.timestamp > now or now - reading.timestamp > self.max_age:
            raise ValueError("stale or future reading")
        if self.last is not None and reading.timestamp <= self.last:
            raise ValueError("out-of-order reading")
        if not isfinite(reading.distance_cm) or not 0 <= reading.distance_cm <= self.height:
            raise ValueError("distance outside tank")
        percent = round(100 * (self.height - reading.distance_cm) / self.height, 2)
        if percent <= self.low:
            self.pump_on = True
        elif percent >= self.high:
            self.pump_on = False
        self.last = reading.timestamp
        return {"level_percent": percent, "pump_on": self.pump_on, "timestamp": reading.timestamp}
