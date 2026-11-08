import unittest
from core import Reading, WaterLevelMonitor

class MonitorTests(unittest.TestCase):
    def test_hysteresis(self):
        m = WaterLevelMonitor(100)
        self.assertTrue(m.record(Reading(1, 90), 1)["pump_on"])
        self.assertTrue(m.record(Reading(2, 50), 2)["pump_on"])
        self.assertFalse(m.record(Reading(3, 10), 3)["pump_on"])
    def test_invalid(self):
        m = WaterLevelMonitor(100)
        with self.assertRaises(ValueError): m.record(Reading(1, 50), 400)
        with self.assertRaises(ValueError): m.record(Reading(5, -1), 5)
        with self.assertRaises(ValueError): WaterLevelMonitor(0)
    def test_order(self):
        m = WaterLevelMonitor(100)
        m.record(Reading(5, 40), 5)
        with self.assertRaises(ValueError): m.record(Reading(5, 30), 5)
