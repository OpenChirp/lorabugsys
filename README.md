# General Developer Resources

## Library Documentation
* `#include <ti/drivers/>` [TI-RTOS Drivers Documentation][TIRTOSDriversDoc]
  This is where you will find peripheral control interfaces.
  These interfaces properly(ish) interact with the RTOS(SYS/BIOS).
* `#include <ti/sysbios/>` [SYSBIOS Guide][SYSBIOSGuideDoc] | [SYSBIOS and XDC API Documentation][SYSBIOSandXDCDoc]
  This is where you will find synchronization primitives and task control functions.
* `#include <xdc/>` [SYSBIOS and XDC API Documentation][SYSBIOSandXDCDoc]
  This is where you will find useful standard functionality, like timestamping,
  text formatting, and logging.
* `#include <driverlib/>` [Non-RTOS Driver Library][CC26XXWareDoc]
  This is the bare-metal interface used by TI-RTOS. You can find some
  interesting low-level functionality that is not exposed by TI-RTOS drivers,
  like `SysCtrlSystemReset()` found in `<driverlib/sys_ctrl.h>`.
  That being said, you should not be directly using peripherals that are
  already controlled by the TI-RTOS drivers mentioned in `ti/drivers`.

## Debugging
* To do proper debug stepping, you need to load the debugging symbols for the
  functions that are found in the ROM. TI calls this the golden ROM image.
  This symbols file can be found at either of these locations
  (should be identical files):
  - ti/tirtos_cc13xx_cc26xx_2_21_01_08/products/bios_6_46_01_38/packages/ti/sysbios/rom/cortexm/cc26xx/r2/golden/CC26xx/rtos_rom.xem3
  - ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages/ti/sysbios/rom/cortexm/cc26xx/r2/golden/CC26xx/rtos_rom.xem3

  Note, the source that created these ROM images is indeed in the source tree.
  It just takes a bit of searching.
* [Video about TI-RTOS Debugging](https://training.ti.com/debugging-common-application-issues-ti-rtos?cu=1127843)
* GNU GDB Server and Flashing http://www.ti.com/lit/an/swra446/swra446.pdf
* Checkout the [debugging quick start][DebuggingQuickStart].
* [SYS/BIOS FAQ](http://processors.wiki.ti.com/index.php/SYS/BIOS_FAQs#4_Exception_Dump_Decoding_Using_the_CCS_Register_View)

## Explanation of Components and Dictionary
* [`EMUPack`][EMUPackDownload] is the software that controls the debugger
  hardware. It controls things like flashing, providing a gdb server,
  upgrading debugger FW, and more.
  Annoyingly, it installs to the `ti/ccs_base` directory.
* `XDC` is a standard for providing reusable software components,
  called "packages", that are optimized for realtime embedded systems.
  In other words, it is this annoying (but somewhat cool) Javascript system
  for generating C/C++ code/headers, ROM symbol linking info, and build config.
  The `.cfg` in the project directory is XDC's Javascript style configuration
  that describes what components to include in the compilation and their
  settings.
  XDC also brings lots of target standardized libraries for necessary things
  like snprintf (System_snprintf), logging, and error handling.
  The command responsible for processing all the .cfgs is the `xs` command
  found in `ti/xdctools_VERSION_core`.
* `SYS/BIOS` is the underlying RTOS. It has been ported to many architectures
  and platforms, including x86. `TI-RTOS` seems to be more of the summation of SYS/BIOS and
  all of the hand written drivers and control logic for a given MCU.
  The subsystem outside of SYS/BIOS that control power and peripherals are
  somewhat significant, thus the combination takes on the TI-RTOS name.
* `XDS` typically refers to the series of TI debugging hardware like the [XDS110][XDS110DebugProbeManual], which is embedded in the TI Launchpads.
  Another note worthy XDS debugger is the XDS200, which is said to be faster
  (and more expensive) than the XDS110.
* `CCS` is the TI curated IDE with awesome debugging support.

## Systems Documentation
* [The Idea Behind XDC](http://rtsc.eclipse.org/mediawiki-tip/images/1/12/XDCtools_Getting_Started_Guide.pdf)

## Examples
* Projects for use with the GNU C Compiler, GNUMake, XDC, and TI-RTOS:
  `ti/tirtos_cc13xx_cc26xx_2_21_01_08/examples/GNU/CC2650_LAUNCHXL`.

# The `overrides` directory
This directory holds upgraded/fixed core libraries and drivers for the CC2650.

## ADCBuf and ADC libraries
These libraries were carefully upgraded/merged to use the new code from from `simplelink_cc2640r2_sdk_2_20_00_49` on Nov 29, 2018 by Craig.
To make use of them, you must add this overrides directory to the includes list with higher priority than the core libraries. Next, you must modify your equivalent `CC2650_LAUNCHXL.c` file to have the following two definitions (converted to use your own CC2650_BLAH_ADC...COUNT defs.):
```C
const uint_least8_t ADCBuf_count = CC2650_LAUNCHXL_ADCBufCOUNT;
const uint_least8_t ADC_count = CC2650_LAUNCHXL_ADCCOUNT;
```
They were merged from `tirtos_cc13xx_cc26xx_2_21_00_06`.

## The uDMA Driver
This fixes a critical issue that was solved in the C2640R2 2.30.00.28 SDK.
The issue deals with a misuse of `uDMAChannelDisable` function
from within `UDMACC26XX_channelDisable`. The `uDMAChannelDisable` function
expects a channel number, but was given a channel mask. This results in
DMA channels never being disabled, both during the startup of the SPI driver
and the closing of the ADCBuf driver (in the ADCBuf cleanup routine).
This issue was resoled in the cc2640r2 SDK by simply making the direct register
write from within the `UDMACC26XX_channelDisable` function, since this register
expects a channel mask.
This upgrade also brings the `UDMACC26XX_disableAttribute` function, that isn't used.
See commit 784e0035696feb367439ae55194fdafc63fefc59.

There is one other uDMA problem that needs to be solved.
The internal `isOpen` variable is never initialized to `FALSE`.
This means that the driver may not follow the proper initialization
routine on system reset.
The following function should be added to the LORABUG.c file, right after
the uDMA setup:
```C
void LORABUG_initUDMA() {
    UDMACC26XX_init((UDMACC26XX_Handle)&(UDMACC26XX_config[0]));
}
```
The function should then be called before SPI or ADCBuf init functions in main.

## The SPI Driver

Major changes like
```C
Power_setConstraint(PowerCC26XX_NEED_FLASH_IN_IDLE);
Power_setConstraint(PowerCC26XX_DISALLOW_XOSC_HF_SWITCHING);
```
```C
Power_releaseConstraint(PowerCC26XX_DISALLOW_XOSC_HF_SWITCHING);
```

```C
const uint_least8_t SPI_count = CC2650_LAUNCHXL_SPICOUNT;
```

These changes were merged from `simplelink_cc2640r2_sdk_2_30_00_28`.

## The Power Driver
The most important
The new SPI driver depends on the power constraint
`PowerCC26XX_DISALLOW_XOSC_HF_SWITCHING`, which was only implemented
in the new Power driver. For this reason, I had to upgrade the Power driver
also.

From the updated [PowerCC26XX.h](overrides/ti/drivers/power/PowerCC26XX.h):
```C
+#define PowerCC26XX_DISALLOW_XOSC_HF_SWITCHING 6
+/*!< Constraint: Prevent power driver from switching to XOSC_HF when the crystal is
+ *   ready. The RTOS clock will be rescheduled to try again in the future.
+ *   This is a workaround to prevent the flash from being accessed by a bus master
+ *   other than the CPU while switching to XOSC_HF. This would cause a bus stall.
+ *   This functionality is only implemented on CC26X0, CC26X0R2, and CC13X0 as the
+ *   bug was fixed in hardware on later devices.
  */
```
The silicon bug being reference here is in the CC2650 Errata.

These changes were merged from `simplelink_cc2640r2_sdk_2_30_00_28`.

## sys_ctl.[ch]
This had to updated because the new Power driver required a different
function interface to `SysCtrlAdjustRechargeAfterPowerDown`.
Another interesting change to this library is the correction to the
`SysCtrlResetSourceGet` function, which returns the last reset source.
I believe this is to compensate for a silicon bug which causes a issue
for a particular type of reset and further results in misreported reset source.

These changes were merged from `simplelink_cc2640r2_sdk_2_30_00_28`.

[TIRTOSDriversDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_21_01_08/exports/tirtos_full_2_21_01_08/products/tidrivers_full_2_21_01_01/docs/doxygen/html/index.html
[SYSBIOSGuideDoc]: http://www.ti.com/lit/pdf/spruex3
[SYSBIOSandXDCDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/sysbios/6_42_01_20/exports/bios_6_42_01_20/docs/cdoc/
[CC26XXWareDoc]: http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_21_01_08/exports/tirtos_full_2_21_01_08/products/cc26xxware_2_24_03_17272/doc/driverlib/index.html

[EMUPackDownload]: http://processors.wiki.ti.com/index.php/XDS_Emulation_Software_Package
[DebuggingQuickStart]: http://dev.ti.com/tirex/content/simplelink_cc13x0_sdk_1_30_00_06/docs/ti154stack/ti154stack-sdg/ti154stack-sdg/debugging/general-debugging.html
[XDS110DebugProbeManual]: http://www.ti.com/lit/ug/sprui94/sprui94.pdf


All the best and happy debugging.
Craig Hesling 2018