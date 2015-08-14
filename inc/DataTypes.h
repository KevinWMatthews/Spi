#ifndef DataTypes_H
#define DataTypes_H

#include <stdint.h>

typedef enum
{
  PIN_LOW  = 0,
  PIN_HIGH = 1
} PinState;

typedef enum
{
  FALSE = 0,
  TRUE  = 1
} BOOL;

typedef volatile uint8_t * RegisterPointer;

#define RETURN_IF_NULL(pointer) if ((pointer) == NULL) return
#define RETURN_VALUE_IF_NULL(pointer, retVal) if ((pointer) == NULL) return (retVal)

#endif
