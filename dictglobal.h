#ifndef DICTGLOBAL_H
#define DICTGLOBAL_H

#ifdef __cplusplus
namespace jnp1 {
extern "C" {
#endif

/* The maximum number of elements the global dictionary can contain. */
const int MAX_GLOBAL_DICT_SIZE = 42;

/* Returns the id of the global dictionary. */
unsigned long dict_global();

#ifdef __cplusplus
}
}
#endif

#endif // DICTGLOBAL_H
