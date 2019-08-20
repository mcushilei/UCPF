
#ifndef __CJSON_PLUG_H__
#define __CJSON_PLUG_H__

#include <stddef.h>
#include <stdbool.h>
#include "./cJSON_port.h"

/* some configrations */

/* Limits how deeply nested arrays/objects can be before cJSON rejects to parse them.
 * This is to prevent stack overflows. */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 8
#endif

#ifndef CJSON_PORT_MALLOC
#error "CJSON_PORT_MALLOC is not defined!"
#endif

#ifndef CJSON_PORT_FREE
#error "CJSON_PORT_FREE is not defined!"
#endif

#ifndef CJSON_PORT_REALLOC
#error "CJSON_PORT_REALLOC is not defined!"
#endif

#endif
