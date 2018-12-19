/**@file error.c
 *
 * @date Nov 7, 2018
 * @author Craig Hesling
 */

#ifndef ERROR_H_
#define ERROR_H_

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>

#include "io.h"

#include <stdbool.h>

/*--------------------------------------------------------*
 *                    Configuration                       *
 *--------------------------------------------------------*/

/**@def DISABLE_DEBUG_PRINT
 * When defined, all debugprintf and debughexdump functions
 * are disabled and the constants associated with them
 * disappear.
 */
//#define DISABLE_DEBUG_PRINT


/**@def RESET_ON_ERROR
 * When defined, the exception and error handlers will issue
 * a system reset if JTAG is not connected.
 * When not defined, exception and error handlers will enter
 * an infinite loop to hang the system.
 */
#define RESET_ON_ERROR

/*--------------------------------------------------------*
 *                  Interface Functions                   *
 *--------------------------------------------------------*/

void TimestampNow(Types_Timestamp64 *time);
bool TimestampDiffNs(uint64_t *nanoseconds, Types_Timestamp64 *before, Types_Timestamp64 *after);

#ifndef DISABLE_DEBUG_PRINT

#define debugprintf(fmt, args...) allprintf(fmt, ## args)
#define debughexdump(data, size)  allhexdump(data, size)

#else

#define debugprintf(fmt, ...)    do { } while(0)
#define debughexdump(data, size) do { } while(0)

#endif


#endif /* ERROR_H_ */
