#include <zephyr/init.h>
#include <zephyr/kernel.h>

/* Cirque Pinnacle needs ~500ms after power-on before its I2C interface is ready.
 * This hook runs at priority 85 (before the Cirque driver at priority 90) and
 * waits long enough that the hardware is guaranteed to be ready. */
static int cirque_startup_delay(void) {
    k_msleep(500);
    return 0;
}

SYS_INIT(cirque_startup_delay, POST_KERNEL, 85);
