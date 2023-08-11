#include "assert.h"  // assert
#include "stdlib.h"  // NULL

#include "cvect.h"

void test_vect_struct() {
    struct vect_metadata v = {0};
    assert(v.capacity == 0);
    assert(v.size == 0);
    assert(v.data != NULL);
}

void test_get_metadata_ptr() {
    struct vect_metadata v;
    assert(vect_get_metadata_ptr(v.data) == &v);
}

void assert_metadata(int *vect, size_t capacity, size_t size) {
    struct vect_metadata *metadata_ptr = vect_get_metadata_ptr(vect);
    assert(metadata_ptr->capacity == capacity);
    assert(metadata_ptr->size == size);
}

void test_vect_init() {
    int *vect = vect_init();

    assert(vect != NULL);
    assert_metadata(vect, 0, 0);

    vect_free(vect);
}

void push_range(int **vect_ptr, int num) {
    for (int i = 0; i < num; ++i) {
        vect_push(vect_ptr, i);
    }
}

void assert_is_range(int *vect, int num) {
    for (int i = 0; i < num; ++i) {
        assert(vect[i] == i);
    }
}

void test_vect_push(int num) {
    assert(num > 0);  // so capacity > 0 too

    size_t roundup_num_to_power_of_2 = 1;
    while (roundup_num_to_power_of_2 < (size_t) num) {
        roundup_num_to_power_of_2 *= 2;
    }

    int *vect = vect_init();

    push_range(&vect, num);

    assert_is_range(vect, num);
    assert_metadata(vect, roundup_num_to_power_of_2, num);

    vect_free(vect);
}

void test_vect_pop() {
    int *vect = vect_init();
    push_range(&vect, 2);

    vect_pop(&vect);

    assert_metadata(vect, 2, 1);
    assert(vect[0] == 0);

    vect_pop(&vect);

    assert_metadata(vect, 2, 0);

    vect_free(vect);
}

void test_vect_pop_push() {
    int *vect = vect_init();
    push_range(&vect, 2);
    
    vect_pop(&vect);
    vect_push(&vect, 1);

    assert_is_range(vect, 2);
    assert_metadata(vect, 2, 2);

    vect_free(vect);
}

void test_vect_size() {
    int n_push = 5;
    int n_pop = 4;
    int *vect = vect_init();

    push_range(&vect, n_push);
    assert(vect_size(vect) == (size_t) n_push);
    for (int i = 0; i < n_pop; ++i) {
        vect_pop(&vect);
    }
    assert(vect_size(vect) == (size_t) (n_push - n_pop));
    push_range(&vect, n_push);
    assert(vect_size(vect) == (size_t) (n_push - n_pop + n_push));

    vect_free(vect);
}


int main(void) {
    test_vect_struct();
    test_get_metadata_ptr();
    test_vect_init();
    for (int i = 1; i <= 9; ++i) {
        test_vect_push(i);
    }
    test_vect_pop();
    test_vect_pop_push();
    test_vect_size();
    return 0;
}
