#include <cassert>
#include <limits>
#include "WaterLevelWatch.h"
int main(){
 using namespace waterlevel;
 Monitor m(Settings(100));
 Report low=m.record(1,90,1);
 assert(low.valid && low.pump_on && low.level_percent==10);
 assert(m.record(2,50,2).pump_on);
 Report high=m.record(3,10,3);
 assert(high.valid && !high.pump_on && high.level_percent==90);
 assert(!m.record(4,-1,4).valid);
 assert(!m.record(5,std::numeric_limits<double>::quiet_NaN(),5).valid);
 assert(!m.record(6,30,400).valid);
 assert(!m.record(8,30,7).valid);
 assert(m.record(9,50,9).valid);
 assert(!m.record(9,50,9).valid);
 Monitor bad(Settings(0));assert(!bad.record(1,20,1).valid);
}
