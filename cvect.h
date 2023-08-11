#include "stddef.h"  // offsetof
#include "stdio.h"  // perror
#include "stdlib.h"  // EXIT_FAILURE free malloc realloc

struct vect_metadata {
    size_t capacity;
    size_t size;
    char data[];
};

static inline void *vect_init() {
    struct vect_metadata *metadata_ptr = malloc(sizeof(struct vect_metadata));
    if (metadata_ptr == NULL) {
        // errors should never pass silently
        perror("vect_init:malloc");
        exit(EXIT_FAILURE);
    }
    *metadata_ptr = (const struct vect_metadata){0};
    return metadata_ptr->data;
} 

#define vect_get_metadata_ptr(vect) ( \
    (struct vect_metadata *) ( \
        (char *) (vect) - offsetof(struct vect_metadata, data) \
    ) \
)

#define vect_free(vect) do { \
    free(vect_get_metadata_ptr(vect)); \
} while (0)

#define vect_push(vect_ptr, val) do { \
    struct vect_metadata *metadata_ptr = vect_get_metadata_ptr(*vect_ptr); \
    if (metadata_ptr->capacity == metadata_ptr->size) { \
        size_t capacity = metadata_ptr->capacity; \
        size_t new_capacity = capacity ? 2 * capacity : 1; \
        metadata_ptr = realloc( \
            metadata_ptr, \
            sizeof(struct vect_metadata) + sizeof((*vect_ptr)[0]) * new_capacity \
        ); \
        if (metadata_ptr == NULL) { \
            perror("vect_push:realloc"); \
            free(vect_get_metadata_ptr(*vect_ptr)); \
            exit(EXIT_FAILURE); \
        } \
        metadata_ptr->capacity = new_capacity; \
        *vect_ptr = (void *) metadata_ptr->data; \
    } \
    (*vect_ptr)[metadata_ptr->size++] = val; \
} while (0)

#define vect_pop(vect_ptr) do { \
    struct vect_metadata *metadata_ptr = vect_get_metadata_ptr(*vect_ptr); \
    if (metadata_ptr->size == 0) { \
        fprintf(stderr, "error: pop when vect size = 0\n"); \
        free(metadata_ptr); \
        exit(EXIT_FAILURE); \
    } \
    --metadata_ptr->size; \
} while (0)

#define vect_size(vect) (vect_get_metadata_ptr(vect)->size)
