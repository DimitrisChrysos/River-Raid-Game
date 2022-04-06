
#include <stdlib.h>

#include "ADTList.h"
#include "state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	List objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι το αριστερό μέρος της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά. Η πίστα έχει σταθερό πλάτος, οπότε όλες οι
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_WIDTH.
//
// - Στον άξονα y το 0 είναι η αρχή της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα αντιθέτως επεκτείνεται προς τα πάνω, οπότε όλες οι
//   συντεταγμένες των αντικειμένων είναι _αρνητικές_.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_y) {
	// Προσθέτουμε BRIDGE_NUM γέφυρες.
	// Στο διάστημα ανάμεσα σε δύο διαδοχικές γέφυρες προσθέτουμε:
	// - Εδαφος, αριστερά και δεξιά της οθόνης (με μεταβαλλόμενο πλάτος).
	// - 3 εχθρούς (ελικόπτερα και πλοία)
	// Τα αντικείμενα έχουν SPACING pixels απόσταση μεταξύ τους.

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x στο αριστερό άκρο της οθόνης
			start_y - 4 * (i+1) * SPACING,	// Η γέφυρα i έχει y = 4 * (i+1) * SPACING
			SCREEN_WIDTH,					// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terain_left = create_object(
			TERAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_WIDTH/3),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_WIDTH/2);
		Object terain_right = create_object(
			TERAIN,
			SCREEN_WIDTH - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		list_insert_next(state->objects, list_last(state->objects), terain_left);
		list_insert_next(state->objects, list_last(state->objects), terain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			list_insert_next(state->objects, list_last(state->objects), enemy);
		}
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	// Δημιουργούμε τη λίστα των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = list_create(NULL);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to) {
	List list = list_create(NULL);
	for(ListNode node = list_first(state->objects);
		node != LIST_EOF;
		node = list_next(state->objects, node)) {

		Object temp_object = list_node_value(state->objects, node);
		if (temp_object->rect.y >= y_from && temp_object->rect.y <= y_to)  {
			list_insert_next(list, node, temp_object);
		}
	}
	return list;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	
	// jet movement
	
	if (keys->up == true)  {
		state->info.jet->rect.y -= 6*state->speed_factor;
	}
	else if (keys->down == true)  {
		state->info.jet->rect.y -= 2*state->speed_factor;
	}
	else if (keys->right == true)  {
		state->info.jet->rect.x += 3*state->speed_factor;
		state->info.jet->rect.y -= 3*state->speed_factor;
	}
	else if (keys->left == true)  {
		state->info.jet->rect.x -= 3*state->speed_factor;
		state->info.jet->rect.y -= 3*state->speed_factor;
	}
	else  {
		state->info.jet->rect.y -= 3*state->speed_factor;
	}
	

	// enemy movement

	List list = state_objects(state, 0, -800);
	for(ListNode node = list_first(list);
		node != LIST_EOF;
		node = list_next(list, node)) {

		Object temp_object = list_node_value(state->objects, node);
		if (temp_object->type == HELICOPTER)  {
			if (temp_object->forward == true)  {
				temp_object->rect.x += 4*state->speed_factor;
			}
			else  {
				temp_object->rect.x -= 4*state->speed_factor;
			}
		}
		else if (temp_object->type == WARSHIP)  {
			if (temp_object->forward == true)  {
				temp_object->rect.x += 3*state->speed_factor;
			}
			else  {
				temp_object->rect.x -= 3*state->speed_factor;
			}
		}
	}


	// collisions

	for(ListNode node = list_first(list);
		node != LIST_EOF;
		node = list_next(list, node)) {

		Object temp_object = list_node_value(state->objects, node);
		if (temp_object->type == HELICOPTER ||
			temp_object->type == WARSHIP ||
			temp_object->type == TERAIN ||
			temp_object->type == BRIDGE)  {

			if (CheckCollisionRecs(state->info.jet->rect, temp_object->rect) == true)  {
				state->info.playing = false;
			}
		}
		Object temp_terain;
		if (temp_object->type == HELICOPTER ||
			temp_object->type == WARSHIP)  {

			for(ListNode node = list_first(list);
				node != LIST_EOF;
				node = list_next(list, node)) {

				Object temp_object2 = list_node_value(state->objects, node);
				if (temp_object2->type == TERAIN)  {
					temp_terain = temp_object2;
				}
			}
			if (CheckCollisionRecs(temp_terain->rect, temp_object->rect) == true)  {
				state->info.playing = false;
			}
		}
	}


	// start and stop

	if (state->info.playing == false && keys->enter == true)  {
		state->info.playing = true;
	}
	if (state->info.paused == true && keys->n == true)  {
		state_update(state, keys);
	}


	// ~~ missile mechanics ~~

	// creating the missile
	if (keys->space == true && state->info.missile == NULL)  {
		state->info.missile = create_object(MISSLE, (SCREEN_WIDTH - 35)/2,  40, 35, 40);
	}

	// if there is a missile
	if (state->info.missile != NULL)  {

		// misile movement
		state->info.missile->rect.y -= 10;

		// misile collisions
		for(ListNode node = list_first(list);
			node != LIST_EOF;
			node = list_next(list, node)) {

			Object temp_object = list_node_value(state->objects, node);

			// misile collision with the terain
			Object temp_terain;
			if (temp_object->type == TERAIN)  {
				temp_terain = temp_object;
			}
			if (CheckCollisionRecs(temp_terain->rect, state->info.missile->rect) == true)  {
				state->info.missile = NULL;
			}

			// misile collision with a helicopter, warship or a bridge
			Object temp_object2;
			if (temp_object->type == HELICOPTER ||
				temp_object->type == WARSHIP ||
				temp_object->type == BRIDGE)  {

				temp_object2 = temp_object;
			}
			ListNode previous_node;
			if (CheckCollisionRecs(temp_object2->rect, state->info.missile->rect) == true)  {
				state->info.missile = NULL;
				list_remove_next(list, previous_node);
				state->info.score += 10;
			}
			previous_node = node;
		}
	}
	

	//making the track "infinite"
	int count_bridges = 0;
	for(ListNode node = list_first(list);
		node != LIST_EOF;
		node = list_next(list, node)) {

		Object temp_object = list_node_value(state->objects, node);
		if (temp_object->type == BRIDGE)  {
			count_bridges++;
		}
		temp_object = list_node_value(state->objects, list_next(list, node));
		if (count_bridges == BRIDGE_NUM - 1)  {
			if (state->info.jet->rect.y == temp_object->rect.y + 800)  {
				add_objects(state, temp_object->rect.y);
				state->speed_factor += state->speed_factor*0.3;
			}
		}
	}
}
//BRIDGE_NUM
//TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE


// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}