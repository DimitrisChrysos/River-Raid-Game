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
	TEST_ASSERT(info->missile == NULL);
	List objects = state_objects(state, 0, -800);
	TEST_ASSERT(objects != NULL);
	

	for(ListNode node = list_first(objects);
		node != LIST_EOF;
		node = list_next(objects, node)) {

		Object temp_object = list_node_value(objects, node);
		TEST_ASSERT(temp_object->rect.y <= 0 && temp_object->rect.y >= -800);
	}
	objects = state_objects(state, -50, -500);
	TEST_ASSERT(objects != NULL);
	objects = state_objects(state, -10, -1000);
	TEST_ASSERT(objects != NULL);
	objects = state_objects(state, -200, -700);
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

	// Έλεγχος κίνησης πυραύλων
	keys.space = true;
	state_update(state, &keys);
	old_rect = state_info(state)->missile->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->missile->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 10);
	keys.space = false;

	// Έλεγχος συγκρούσεων πυραύλων
	keys.space = true;
	state_update(state, &keys);
	old_rect = state_info(state)->missile->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->missile->rect;

	for(ListNode node = list_first(state_objects(state, 0, -800));
		node != LIST_EOF;
		node = list_next(state_objects(state, 0, -800), node)) {

		Object temp_object = list_node_value(state_objects(state, 0, -800), node);
		Object temp_terain;
		if (temp_object->type == TERAIN)  {
			temp_terain = temp_object;
		}
		TEST_CHECK( CheckCollisionRecs(temp_terain->rect, state_info(state)->missile->rect));
		Object temp_object2;
		if (temp_object->type == HELICOPTER ||
			temp_object->type == WARSHIP ||
			temp_object->type == BRIDGE)  {

			temp_object2 = temp_object;
		}
		TEST_CHECK( CheckCollisionRecs(temp_object2->rect, state_info(state)->missile->rect));
	}

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 10);
	keys.space = false;
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};