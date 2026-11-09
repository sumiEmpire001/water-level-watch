#ifndef LORD_SUMI_WATER_LEVEL_WATCH_H
#define LORD_SUMI_WATER_LEVEL_WATCH_H
#include <math.h>
#include <stdint.h>
namespace waterlevel {
struct Settings {
 double tank_height_cm,low_percent,high_percent;
 uint32_t max_age_seconds;
 Settings(double height,double low=20,double high=80,uint32_t age=300)
  :tank_height_cm(height),low_percent(low),high_percent(high),max_age_seconds(age){}
};
struct Report {
 bool valid;double level_percent;bool pump_on;
 Report(bool v,double l,bool p):valid(v),level_percent(l),pump_on(p){}
};
inline bool valid_settings(const Settings& s) {
 return isfinite(s.tank_height_cm)&&isfinite(s.low_percent)&&isfinite(s.high_percent)&&
   s.tank_height_cm>0&&s.low_percent>=0&&s.low_percent<s.high_percent&&
   s.high_percent<=100&&s.max_age_seconds>0;
}
class Monitor {
 public:
 explicit Monitor(Settings settings):settings_(settings),pump_(false),has_last_(false),last_(0){}
 Report record(uint32_t timestamp,double distance_cm,uint32_t now) {
   if (!valid_settings(settings_) || timestamp>now || now-timestamp>settings_.max_age_seconds ||
       (has_last_&&timestamp<=last_) || !isfinite(distance_cm) ||
       distance_cm<0 || distance_cm>settings_.tank_height_cm) {
     pump_=false;
     return Report(false,0,false);
   }
   double percent=100.0*(settings_.tank_height_cm-distance_cm)/settings_.tank_height_cm;
   percent=floor(percent*100.0+0.5)/100.0;
   if (percent<=settings_.low_percent) pump_=true;
   else if (percent>=settings_.high_percent) pump_=false;
   has_last_=true; last_=timestamp;
   return Report(true,percent,pump_);
 }
 private:
 Settings settings_;bool pump_,has_last_;uint32_t last_;
};
}
#endif
