#ifndef RCL_UTILS_CJSON_H
#define RCL_UTILS_CJSON_H

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

#define CJSON_CDECL __cdecl
#define CJSON_STDCALL __stdcall

#if !defined(CJSON_HIDE_SYMBOLS) && !defined(CJSON_IMPORT_SYMBOLS) && !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type)   type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllimport) type CJSON_STDCALL
#endif
#else
#define CJSON_CDECL
#define CJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
#endif

#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 19

#include <stddef.h>

#define cJSON_Invalid (0)
#define cJSON_False  (1 << 0)
#define cJSON_True   (1 << 1)
#define cJSON_NULL   (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw    (1 << 7)

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512

typedef struct cJSON
{
    struct cJSON *next;
    struct cJSON *prev;
    struct cJSON *child;

    int type;
    char *string;

    char *valuestring;
    int valueint;
    double valuedouble;
} cJSON;

typedef struct cJSON_Hooks
{
      void *(CJSON_CDECL *malloc_fn)(size_t sz);
      void (CJSON_CDECL *free_fn)(void *ptr);
} cJSON_Hooks;

typedef int cJSON_bool;

#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000
#endif

#ifndef CJSON_CIRCULAR_LIMIT
#define CJSON_CIRCULAR_LIMIT 10000
#endif

CJSON_PUBLIC(const char*) cjson_version(void);
CJSON_PUBLIC(void) cjson_init_hooks(cJSON_Hooks* hooks);
CJSON_PUBLIC(cJSON *) cjson_parse(const char *value);
CJSON_PUBLIC(cJSON *) cjson_parse_with_length(const char *value, size_t buffer_length);
CJSON_PUBLIC(cJSON *) cjson_parse_with_opts(const char *value, const char **return_parse_end, cJSON_bool require_null_terminated);
CJSON_PUBLIC(cJSON *) cjson_parse_with_length_opts(const char *value, size_t buffer_length, const char **return_parse_end, cJSON_bool require_null_terminated);

CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt);
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length, const cJSON_bool format);

CJSON_PUBLIC(void) cjson_delete(cJSON *item);

CJSON_PUBLIC(int) cjson_get_array_size(const cJSON *array);
CJSON_PUBLIC(cJSON *) cjson_get_array_item(const cJSON *array, int index);

CJSON_PUBLIC(cJSON *) cjson_get_object_item(const cJSON * const object, const char * const string);
CJSON_PUBLIC(cJSON *) cjson_get_object_item_case_sensitive(const cJSON * const object, const char * const string);
CJSON_PUBLIC(cJSON_bool) cjson_has_object_item(const cJSON *object, const char *string);

CJSON_PUBLIC(const char *) cjson_get_error_ptr(void);

CJSON_PUBLIC(char *) cjson_get_string_value(const cJSON * const item);
CJSON_PUBLIC(double) cjson_get_number_value(const cJSON * const item);

CJSON_PUBLIC(cJSON_bool) cjson_is_invalid(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_false(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_true(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_bool(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_null(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_number(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_string(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_array(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_object(const cJSON * const item);
CJSON_PUBLIC(cJSON_bool) cjson_is_raw(const cJSON * const item);

CJSON_PUBLIC(cJSON *) cjson_create_null(void);
CJSON_PUBLIC(cJSON *) cjson_create_true(void);
CJSON_PUBLIC(cJSON *) cjson_create_false(void);
CJSON_PUBLIC(cJSON *) cjson_create_bool(cJSON_bool boolean);
CJSON_PUBLIC(cJSON *) cjson_create_number(double num);
CJSON_PUBLIC(cJSON *) cjson_create_string(const char *string);
CJSON_PUBLIC(cJSON *) cjson_create_raw(const char *raw);
CJSON_PUBLIC(cJSON *) cjson_create_array(void);
CJSON_PUBLIC(cJSON *) cjson_create_object(void);

CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string);
CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const cJSON *child);
CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child);

CJSON_PUBLIC(cJSON *) cjson_create_int_array(const int *numbers, int count);
CJSON_PUBLIC(cJSON *) cjson_create_float_array(const float *numbers, int count);
CJSON_PUBLIC(cJSON *) cjson_create_double_array(const double *numbers, int count);
CJSON_PUBLIC(cJSON *) cjson_create_string_array(const char *const *strings, int count);

CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item);

CJSON_PUBLIC(cJSON *) cjson_detach_item_via_pointer(cJSON *parent, cJSON * const item);
CJSON_PUBLIC(cJSON *) cjson_detach_item_from_array(cJSON *array, int which);
CJSON_PUBLIC(void) cjson_delete_item_from_array(cJSON *array, int which);
CJSON_PUBLIC(cJSON *) cjson_detach_item_from_object(cJSON *object, const char *string);
CJSON_PUBLIC(cJSON *) cjson_detach_item_from_object_case_sensitive(cJSON *object, const char *string);
CJSON_PUBLIC(void) cjson_delete_item_from_object(cJSON *object, const char *string);
CJSON_PUBLIC(void) cjson_delete_item_from_object_case_sensitive(cJSON *object, const char *string);

CJSON_PUBLIC(cJSON_bool) cjson_insert_item_in_array(cJSON *array, int which, cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cjson_replace_item_via_pointer(cJSON * const parent, cJSON * const item, cJSON * replacement);
CJSON_PUBLIC(cJSON_bool) cjson_replace_item_in_array(cJSON *array, int which, cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cjson_replace_item_in_object(cJSON *object,const char *string,cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cjson_replace_item_in_object_case_sensitive(cJSON *object,const char *string,cJSON *newitem);

CJSON_PUBLIC(cJSON *) cjson_duplicate(const cJSON *item, cJSON_bool recurse);
CJSON_PUBLIC(cJSON_bool) cjson_compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive);
CJSON_PUBLIC(void) cjson_minify(char *json);

CJSON_PUBLIC(cJSON*) cjson_add_null_to_object(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cjson_add_true_to_object(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cjson_add_false_to_object(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cjson_add_bool_to_object(cJSON * const object, const char * const name, const cJSON_bool boolean);
CJSON_PUBLIC(cJSON*) cjson_add_number_to_object(cJSON * const object, const char * const name, const double number);
CJSON_PUBLIC(cJSON*) cjson_add_string_to_object(cJSON * const object, const char * const name, const char * const string);
CJSON_PUBLIC(cJSON*) cjson_add_raw_to_object(cJSON * const object, const char * const name, const char * const raw);
CJSON_PUBLIC(cJSON*) cjson_add_object_to_object(cJSON * const object, const char * const name);
CJSON_PUBLIC(cJSON*) cjson_add_array_to_object(cJSON * const object, const char * const name);

#define cJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
CJSON_PUBLIC(double) cjson_set_number_helper(cJSON *object, double number);
#define cJSON_SetNumberValue(object, number) ((object != NULL) ? cjson_set_number_helper(object, (double)number) : (number))
CJSON_PUBLIC(char*) cjson_set_valuestring(cJSON *object, const char *valuestring);
#define cJSON_SetBoolValue(object, boolValue) ( \
    (object != NULL && ((object)->type & (cJSON_False|cJSON_True))) ? \
    (object)->type=((object)->type &(~(cJSON_False|cJSON_True)))|((boolValue)?cJSON_True:cJSON_False) : \
    cJSON_Invalid\
)
#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)
CJSON_PUBLIC(void *) cjson_malloc(size_t size);
CJSON_PUBLIC(void) cjson_free(void *object);

#ifdef __cplusplus
}
#endif

#endif
