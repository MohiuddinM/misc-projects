#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t **array;
    unsigned int num_objects;
    unsigned int max_num_objects;
} dyn_array_t;

dyn_array_t *dyn_array_init(unsigned int num_objects)
{
    dyn_array_t *array = malloc(sizeof(dyn_array_t));
    array->array = malloc(num_objects * sizeof(uint8_t *));
    array->max_num_objects = num_objects;
    array->num_objects = 0;
    return array;
}

void dyn_array_resize(dyn_array_t *array)
{
    array->max_num_objects = array->max_num_objects * 2;
    array->array = realloc(array->array, array->max_num_objects * sizeof(uint8_t *));
    if(array->array == NULL) exit(-1);
}

void dyn_array_add(dyn_array_t *array, void *object)
{
    if(array->num_objects == array->max_num_objects) dyn_array_resize(array);
    array->array[array->num_objects] = (uint8_t *) object;
    ++array->num_objects;
}

void *dyn_array_get_at(dyn_array_t *array, unsigned int index)
{
    return (void *) array->array[index];
}

void dyn_array_delete_at(dyn_array_t *array, unsigned int index)
{
    free(array->array[index]);
    array->array[index] = NULL;
}

void dyn_array_free(dyn_array_t *array)
{
    memset(array->array, 0, array->max_num_objects);
    for(unsigned int i = 0; i < array->num_objects; i++)
    {
        free(array->array[i]);
    }
    memset(array, 0, sizeof(dyn_array_t));
    free(array);
    array = NULL;
}

unsigned int dyn_array_get_max_size(dyn_array_t *array)
{
    return array->max_num_objects;
}

unsigned int dyn_array_get_size(dyn_array_t *array)
{
    return array->num_objects;
}
