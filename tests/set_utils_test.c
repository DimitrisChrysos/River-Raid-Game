//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "set_utils.h"

int* create_int(int value) {
    int* pointer = malloc(sizeof(int)); // δέσμευση μνήμης
    *pointer = value;                   // αντιγραφή του value στον νέο ακέραιο
    return pointer;
}

int compare_ints(Pointer a, Pointer b) {
    return *(int*)a - *(int*)b;
}

void test_set_find_eq_or_greater() {

    // test αν υπάρχει το value
    Set set = set_create(compare_ints, free);
    for (int i = 0 ; i < 20 ; i++)  {
        set_insert(set, create_int(i));
    }
    Pointer int_15 = create_int(15);
    Pointer value = set_find_eq_or_greater(set, int_15);
    TEST_ASSERT(value == int_15);

    // test αν δεν υπάρχει το value
    set = set_create(compare_ints, free);
    for (int i = 0 ; i < 20 ; i++)  {
        set_insert(set, create_int(i));
    }
    set_remove(set, int_15);
    value = set_find_eq_or_greater(set, int_15);
    Pointer int_16 = set_find(set, create_int(16));
    TEST_ASSERT(value == int_16);

    // test αν άδιο set
    set = set_create(compare_ints, free);
    value = set_find_eq_or_greater(set, int_15);
    TEST_ASSERT(value == NULL);
}

void test_set_find_eq_or_smaller() {

    // test αν υπάρχει το value
    Set set = set_create(compare_ints, free);
    for (int i = 0 ; i < 20 ; i++)  {
        set_insert(set, create_int(i));
    }
    Pointer int_15 = create_int(15);
    Pointer value = set_find_eq_or_greater(set, int_15);
    TEST_ASSERT(value == int_15);

    // test αν δεν υπάρχει το value
    set = set_create(compare_ints, free);
    for (int i = 0 ; i < 20 ; i++)  {
        set_insert(set, create_int(i));
    }
    set_remove(set, int_15);
    value = set_find_eq_or_smaller(set, int_15);
    Pointer int_14 = set_find(set, create_int(14));
    TEST_ASSERT(value == int_14);

    // test αν άδιο set
    set = set_create(compare_ints, free);
    value = set_find_eq_or_smaller(set, int_15);
    TEST_ASSERT(value == NULL);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_set_find_eq_or_greater", test_set_find_eq_or_greater },
	{ "test_set_find_eq_or_smaller", test_set_find_eq_or_smaller },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};