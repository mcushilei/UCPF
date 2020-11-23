
#include "../app_cfg.h"
#include <stdlib.h>

void *json_checker_port_malloc( size_t size )
{
    return malloc( size );
}

void json_checker_port_free( void *ptr )
{
    free( ptr );
}

