//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

	// Προσθέστε επιπλέον ελέγχους
	List objects = state_objects(state, 0, -800);
	TEST_ASSERT(objects != NULL);
	

}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το αεροσκάφος μετακινείται 3 pixels πάνω
	Rectangle old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 3 );

	// Με πατημένο το δεξί βέλος, το αεροσκάφος μετακινείται 6 pixels μπροστά
	keys.up = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 6 );
	keys.up = false;


	// Προσθέστε επιπλέον ελέγχους
	

	// Με πατημένο το κάτω βέλος, το αεροσκάφος μετακινείται 2 pixels μπροστά
	keys.down = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 2 );
	keys.down = false;

	// Με πατημένο το δεξί βέλος, το αεροσκάφος μετακινείται 2 pixels δεξιά
	keys.right = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x + 3 && new_rect.y == old_rect.y - 3);
	keys.right = false;

	// Με πατημένο το αριστερό βέλος, το αεροσκάφος μετακινείται 2 pixels αριστερά
	keys.left = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x - 3 && new_rect.y == old_rect.y - 3);
	keys.left = false;
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};