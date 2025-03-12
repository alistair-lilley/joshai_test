#include "light_monitor.hpp"

int main()
{
    LightMonitor monitor = LightMonitor("localhost", 8080);
    monitor.loopAndCheck();
}