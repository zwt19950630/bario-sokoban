#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */

#include "array.h"

void* createArray(size_t array_length, size_t elem_size, void* elem_zero)
{
	void* array;
	int i;

	array = malloc(array_length * elem_size);
	if (array == NULL)
		goto exception_array_bad_alloc;

	if (elem_zero != NULL)
	{
		for (i = 0; i < array_length; i++)
		{
			memcpy((char*)array + i * elem_size, elem_zero, elem_size);
		}
	}
	return array;

exception_array_bad_alloc:
	return NULL;
}

int resizeArray(void* array, size_t array_length, size_t new_array_length, size_t elem_size, void* elem_zero, void (*elem_free)(void* elem))
{
	void* new_array;
	int i;

	if (array == NULL || *((void**)array) == NULL)
		goto exception_array_null;

	new_array = malloc(new_array_length * elem_size);
	if (new_array == NULL)
		goto exception_new_array_bad_alloc;

	for (i = 0; i < ((new_array_length > array_length && elem_zero != NULL) ? new_array_length : array_length); i++)
	{
		if (i < new_array_length && i < array_length)
			memcpy((char*)new_array + i * elem_size, *((char**)array) + i * elem_size, elem_size);
		else if (i >= array_length && elem_zero != NULL)
			memcpy((char*)new_array + i * elem_size, elem_zero, elem_size);
		else if (i >= new_array_length)
			(*elem_free)(*((char**)array) + i * elem_size);
	}
	*((void**)array) = new_array;
	return 1;

exception_new_array_bad_alloc:
exception_array_null:
	return 0;
}

void destroyArray(void* array, size_t array_length, size_t elem_size, void (*elem_free)(void* elem))
{
	int i;
	if (array != NULL)
	{
		if (elem_free != NULL)
		{
			for (i = 0; i < array_length; i++)
			{
				(*elem_free)((char*)array + i * elem_size);
			}
		}
		free(array);
	}
}