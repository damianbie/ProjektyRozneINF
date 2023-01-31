#include "esp_system.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <FreeRTOS/FreeRTOS.h>


//singleton
class System
{
public:
    static System& getInstance();

    void initFileSystem();
private:
    System();
};