#include <iostream>
#include <cassert>
#include <unordered_map>
#include "dict.h"
#include "dictglobal.h"

#ifdef NDEBUG
const bool debug = false;
#else
const bool debug = true;
#endif

using std::string;
using std::cerr;
using std::endl;
using std::to_string;
using ids_t = unsigned long;
using dict_t = std::unordered_map<string, string>;
using dict_map_t = std::unordered_map<ids_t, dict_t>;

namespace {
    /* Prints diagnostic message, initializes the stream to prevent
     * initialization order fiasco. */
    void dbg_msg(const string& msg) {
        static std::ios_base::Init init;
        
        cerr << msg << endl;
    }

    ids_t id_generator() {
        static ids_t id_counter = 0;
        ids_t id = id_counter++;
        bool id_in_range = (id < id_counter);
        
        if (debug && !id_in_range) {
            dbg_msg("dict_new: couldn't create a new dictionary, id counter "
                    "overflowed");
        }
        
        assert(id_in_range);
        
        return id;
    }

    /* Returns a reference to the map contatining all dictionaries, prevents
     * initialization order fiasco. */
    dict_map_t& main_map() {
        static dict_map_t map_of_dicts;
        return map_of_dicts;
    }

    /* Checks if an element can be inserted into the global dictionary under the
     * argument key. */
    bool check_insert_to_global_dict(const char* key) {
        dict_t global_dict = main_map()[jnp1::dict_global()];
        size_t global_dict_size = main_map()[jnp1::dict_global()].size();

        return ((global_dict_size == jnp1::MAX_GLOBAL_DICT_SIZE &&
                 global_dict.find(key) != global_dict.end()) ||
                (global_dict_size < jnp1::MAX_GLOBAL_DICT_SIZE));
    }

    string replace_if_global_dict(ids_t id) {
        if (id == jnp1::dict_global()) {
            return "the global dictionary";
        }

        return "dict " + to_string(id);
    }

    string dict_doesnt_exist_msg(ids_t id) {
        return "dict " + to_string(id) + " does not exist";
    }

    string replace_if_null(const char* value) {
        if (value == NULL) {
            return "NULL";
        }

        return "'" + string(value) + "'";
    }

    
    /* Searches for a value under the argument key in a dictionary with the
     * argument id. Returns the  value if found. If the function can't find
     * a dictionary with the argument id or the argument key, returns NULL. */
    /* Searches for a value under the argument key in a dictionary with the
     * argument id. Returns the  value if found. If the function can't find
     * a dictionary with the argument id or the argument key, returns NULL. */
    const char* search_in_dict(ids_t id, const char* key) {
        dict_map_t::iterator main_it = main_map().find(id);
        if (main_it != main_map().end()) {
            dict_t::iterator dict_it = main_it->second.find(key);
            if (dict_it != main_it->second.end()) {
                const char* value = dict_it->second.c_str();
                
                if (debug) {
                    dbg_msg("dict_find: " + replace_if_global_dict(id) +
                            ", the key '" + key + "' has the value '" + value +
                            "'");
                }
                
                return value;
            }
        } else if (debug) {
            dbg_msg("dict_find: " + dict_doesnt_exist_msg(id));
        }

        if (debug) {
            string msg = "dict_find: the key '" + string(key) + "' not found";
            if (id != jnp1::dict_global()) {
                msg += ", looking up the global dictionary";
            }

            dbg_msg(msg);
        }
        
        return NULL;
    }
}

ids_t jnp1::dict_new() {
    if (debug)
        dbg_msg("dict_new()");
    
    ids_t id = id_generator();
    dict_t new_dict;
    main_map()[id] = new_dict;

    if (debug) {
        dbg_msg("dict_new()\ndict_new: dict " + to_string(id) +
                " has been created");
    }
    
    return id;
}

void jnp1::dict_delete(ids_t id) {
    dict_map_t::iterator it = main_map().find(id);

    if (debug) {
        dbg_msg("dict_delete(" + to_string(id) + ")");
    }

    if (it != main_map().end()) {
        if (id != jnp1::dict_global()) {
            main_map().erase(it);

            if (debug) {
                dbg_msg("dict_delete: dict " + to_string(id) +
                        " has been deleted");
            }
        } else if (debug) {
            dbg_msg("dict_delete: an attempt to remove the global dictionary");
        }
    } else if (debug) {
        dbg_msg("dict_delete: " + dict_doesnt_exist_msg(id));
    }
}

size_t jnp1::dict_size(ids_t id) {
    dict_map_t::iterator it = main_map().find(id);

    if (debug) {
        dbg_msg("dict_size(" + to_string(id) + ")");
    }

    if (it != main_map().end()) {
        size_t size = it->second.size();

        if (debug) {
            dbg_msg("dict_size: " + replace_if_global_dict(id) + " contains " +
                    to_string(size) + " element(s)");
        }
        
        return size;
    } else if (debug) {
        dbg_msg("dict_size: " + dict_doesnt_exist_msg(id));
    }
    
    return 0;
}

void jnp1::dict_insert(ids_t id, const char* key, const char* value) {
    dict_map_t::iterator it = main_map().find(id);

    if (debug) {
        dbg_msg("dict_insert(" + to_string(id) + ", " + replace_if_null(key) +
                ", " + replace_if_null(value) + ")");
    }

    if (it != main_map().end() && key != NULL && value != NULL) {
        if (id != jnp1::dict_global() || check_insert_to_global_dict(key)) {
            it->second[key] = value;

            if (debug) {
                dbg_msg("dict_insert: " + replace_if_global_dict(id) +
                       ", the pair ('" + key + "','" + value +
                       "') has been inserted");
            }
        } else if (debug) {
            dbg_msg("dict_insert: an attempt to insert into full global "
                    "dictionary");
        }
    } else if (debug) {
        if (it == main_map().end()) {
            dbg_msg("dict_insert: " + dict_doesnt_exist_msg(id));
        }

        if (key == NULL) {
            dbg_msg("dict_insert: " + replace_if_global_dict(id) +
                    ", an attempt to insert NULL key");
        }

        if (value == NULL) {
            dbg_msg("dict_insert: " + replace_if_global_dict(id) +
                    ", an attempt to insert NULL value");
        }
    }
}

void jnp1::dict_remove(ids_t id, const char* key) {
    dict_map_t::iterator main_it = main_map().find(id);

    if (debug) {
        dbg_msg("dict_remove(" + to_string(id) + ", " +
                replace_if_null(key) + ")");
    }

    if (key == NULL) {
        if (debug)
            dbg_msg("dict_remove: an attempt to remove NULL key");
        return;
    }

    if (main_it != main_map().end()) {
        dict_t::iterator dict_it = main_it->second.find(key);
        if (dict_it != main_it->second.end()) {
            main_it->second.erase(key);

            if (debug) {
                dbg_msg("dict_remove: "  + replace_if_global_dict(id) +
                        ", the key '" + key + "' has been removed");
            }
        } else if (debug) {
            dbg_msg("dict_remove: "  + replace_if_global_dict(id) +
                    " does not contain the key '" + key + "'");
        }
    } else if (debug) {
        dbg_msg("dict_remove: " + dict_doesnt_exist_msg(id));
    }
}

const char* jnp1::dict_find(ids_t id, const char* key) {
    if (debug) {
        dbg_msg("dict_find(" + to_string(id) + ", " +
                replace_if_null(key) + ")");
    }

    if (key == NULL) {
        if(debug) {
            dbg_msg("dict_find: " + replace_if_global_dict(id) +
                ", an attempt to find NULL key");
        }

        return NULL;
    }

    const char* value = search_in_dict(id, key);
    if (value == NULL) {
        return search_in_dict(jnp1::dict_global(), key);
    }

    return value;
}

void jnp1::dict_clear(ids_t id) {
    dict_map_t::iterator main_it = main_map().find(id);

    if (debug) {
        dbg_msg("dict_clear(" + to_string(id) + ")");
    }

    if (main_it != main_map().end()) {
        main_it->second.clear();

        if (debug) {
            dbg_msg("dict_clear: " + replace_if_global_dict(id) +
                    " has been cleared");
        }
    } else if (debug) {
        dbg_msg("dict_clear: " + dict_doesnt_exist_msg(id));
    }
}

void jnp1::dict_copy(ids_t src_id, ids_t dst_id) {
    dict_map_t::iterator src_it = main_map().find(src_id);
    dict_map_t::iterator dst_it = main_map().find(dst_id);
    if (debug) {
        dbg_msg("dict_copy(" + to_string(src_id) + ", " +
                to_string(dst_id) + ")");
    }

    if (src_it != main_map().end() && dst_it != main_map().end() &&
        src_id != dst_id) {
        bool full_global_dict = false;
        for (dict_t::iterator dict_it = src_it->second.begin();
             dict_it != src_it->second.end() && !full_global_dict; dict_it++) {
            if (dst_id == jnp1::dict_global()) {
                full_global_dict = (jnp1::dict_size(jnp1::dict_global()) ==
                             jnp1::MAX_GLOBAL_DICT_SIZE);
            }

            if (!full_global_dict) {
                dst_it->second[dict_it->first] = dict_it->second;
            }
        }

        if (debug) {
            if (!full_global_dict) {
                dbg_msg("dict_copy: " + replace_if_global_dict(src_id) +
                        " has been copied to " + replace_if_global_dict(dst_id));
            } else {
                dbg_msg("dict_copy: an attempt to copy into full global "
                        "dictionary");
            }
        }
    } else if (debug) {
        if (src_it == main_map().end()) {
            dbg_msg("dict_copy: " + dict_doesnt_exist_msg(src_id));
        }

        if (dst_it == main_map().end()) {
            dbg_msg("dict_copy: " + dict_doesnt_exist_msg(dst_id));
        }

        if (dst_id == src_id) {
            dbg_msg("dict_copy: an attempt to copy into the same dict");
        }
    }
}
