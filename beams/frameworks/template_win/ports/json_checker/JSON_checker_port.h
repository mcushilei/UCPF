
#ifndef __JSON_CHECKER_PORT_H__
#define __JSON_CHECKER_PORT_H__

#include "..\app_cfg.h"

#if defined(_MSC_VER)

#define json_checker_port_malloc    heap_malloc
#define json_checker_port_free      heap_free

#else

#define json_checker_port_malloc    json_checker_port_malloc
#define json_checker_port_free      json_checker_port_free

extern void *json_checker_port_malloc(size_t size);
extern void json_checker_port_free(void *ptr);

#endif



#endif
