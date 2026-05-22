/* Redirect to the platform threading_alt implementation in port/inc/.
 * The stub definition previously here conflicted with port/inc/threading_alt.h
 * which contains the full struct (mutex, count, is_valid) required by threading_alt.c.
 */
#include "../../port/inc/threading_alt.h"
