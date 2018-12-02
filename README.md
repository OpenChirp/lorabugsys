# overrides
This directory holds upgraded/modified core libraries.

## ADCBuf and ADC libraries
These libraries were carefully upgraded/merged to use the new code from from `simplelink_cc2640r2_sdk_2_20_00_49` on Nov 29, 2018 by Craig.
To make use of them, you must add this overrides directory to the includes list with higher priority than the core libraries. Next, you must modify your equivalent `CC2650_LAUNCHXL.c` file to have the following two definitions (converted to use your own CC2650_BLAH_ADC...COUNT defs.):
```C
const uint_least8_t ADCBuf_count = CC2650_LAUNCHXL_ADCBufCOUNT;
const uint_least8_t ADC_count = CC2650_LAUNCHXL_ADCCOUNT;
```
They were merged from `tirtos_cc13xx_cc26xx_2_21_00_06`.

# General Developer Resources

## Drivers Documentation
* `#include <ti/drivers/>` [TI-RTOS Drivers Documentation][TIRTOSDriversDoc]
* `#include <ti/sysbios/>` [SYSBIOS Guide][SYSBIOSGuideDoc] | [SYSBIOS and XDC API Documentation][SYSBIOSandXDCDoc]
* `#include <xds/runtime/>` [SYSBIOS and XDC API Documentation][SYSBIOSandXDCDoc]
* `#include <driverlib/>` [Non-RTOS Driver Library][CC26XXWareDoc] This is the bare-metal interface used by TI-RTOS. You can find some interesting low-level functionality that is not exposed by TI-RTOS drivers, like `SysCtrlSystemReset()` found in `<driverlib/sys_ctrl.h>`.
  That being said, you should not be directly using peripherals that are already controlled by a TI-RTOS driver.

## Systems Documentation
* [The Idea Behind XDC](http://rtsc.eclipse.org/mediawiki-tip/images/1/12/XDCtools_Getting_Started_Guide.pdf)

## Debugging
* To do proper debug stepping, you need to load the debugging symbols for the
  functions that are found in the ROM. TI calls this the golden ROM image.

[TIRTOSDriversDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_21_01_08/exports/tirtos_full_2_21_01_08/products/tidrivers_full_2_21_01_01/docs/doxygen/html/index.html
[SYSBIOSGuideDoc]: http://www.ti.com/lit/pdf/spruex3
[SYSBIOSandXDCDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/sysbios/6_42_01_20/exports/bios_6_42_01_20/docs/cdoc/
[CC26XXWareDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_21_01_08/exports/tirtos_full_2_21_01_08/products/cc26xxware_2_24_03_17272/doc/driverlib/index.html