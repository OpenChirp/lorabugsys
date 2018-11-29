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
