/**@file jtag.c
 *
 * @date Nov 8, 2018
 * @author Craig Hesling
 */

#include <stdbool.h>
#include "io.h"

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* Non-RTOS Drivers */
#include <driverlib/aon_wuc.h>  // AONWUCPowerStatusGet() & AONWUC_JTAG_POWER_ON

bool jtag_ispowered(void) {
    return (AONWUCPowerStatusGet()&AONWUC_JTAG_POWER_ON) != 0;
}
void jtag_write(const char *str, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++) {
        System_putch(str[i]);
    }
    System_flush();
}

void jtag_vprintf(const char *format, va_list args)
{
    // We don't care about putting this message in the buffer
    // if no one will see it
    if (!jtag_ispowered()) return;

    System_vprintf(format, args);
    System_flush();
}

void jtag_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    jtag_vprintf(format, args);
    va_end(args);
}

void jtag_hexdump(uint8_t *data, size_t size)
{
    phexdump(jtag_printf, data, size);
}
