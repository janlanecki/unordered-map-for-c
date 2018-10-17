#include "dictglobal.h"
#include "dict.h"

unsigned long jnp1::dict_global() {
    static unsigned long global_id = jnp1::dict_new();

    return global_id;
}
