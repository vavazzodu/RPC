#ifndef __SENTINEL__
#define __SENTINEL__

#include "Serialize.h"

#define SENTINEL_INSERTION_CODE(obj, b)     \
{                                           \
    if (obj == NULL){                       \
        unsigned int sentinel = 0xFFFFFFFF; \
        Serialize_data (b, (char *)&sentinel, sizeof(unsigned int)); \
        return;                                                      \
    }                                                                \
}

#define SENTINEL_DETECTION_CODE(b)                  \
{                                                   \
    unsigned int sentinel = 0;                      \
    De_serialize_data((char *)&sentinel, b, sizeof(unsigned int)); \
    if(sentinel == 0xFFFFFFFF)                                     \
        return NULL;                                               \
    else                                                           \
        skip_serialize_data(b, -1*sizeof(unsigned int));            \
}
#endif
