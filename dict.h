#ifndef DICT_H
#define DICT_H

#ifdef __cplusplus
#include <cstddef>
namespace jnp1 {
extern "C" {
#else
#include <stddef.h>
#endif

/* Creates new, empty dictionary and returns it's id. */
unsigned long dict_new();

/* If a dictionary with the argument id exists, deletes it;
 * otherwise does nothing. */
void dict_delete(unsigned long id);

/* If a dicitonary with the argument id exists, returns number of it's elements;
 * otherwise returns 0. */
size_t dict_size(unsigned long id);

/* If a dictionary with the argument id exists, inserts the argument value under
 * the argument key. If such a dictionary doesn't exist or the key or value are 
 * equal NULL - does nothing. */
void dict_insert(unsigned long id, const char* key, const char* value);

/* If a dictionary with the argument id exists and it contains a value under the
 * argument key it removes the key and the value; otherwise does nothing. */
void dict_remove(unsigned long id, const char* key);

/* If a dictionary with the argument id exists and it contains a value under the
 * argument key, it returns the pointer to the value; otherwise it returns the
 * value from the global dictionary. If the global dictionary doesn't contain
 * a value under the argument key, the function returns NULL. */
const char* dict_find(unsigned long id, const char* key);

/* If a dictionary with the argument id exists, deletes all of it's elements;
 * otherwise does nothing. */
void dict_clear(unsigned long id);

/* If both the dicitionaries with the argument ids exist, it copies as many
 * elements from the source dictionary to the destination dictionary as the 
 * destination dictionary can fit in; otherwise does nothing. */
void dict_copy(unsigned long src_id, unsigned long dst_id);

#ifdef __cplusplus
}
}
#endif

#endif // DICT_H

