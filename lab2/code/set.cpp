#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

 /*
  *  Default constructor :create an empty Set
  */
int Set::get_count_nodes() {
	return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{ 0 } {
	head = new Node();
	tail = new Node();
	head->next = tail;
	tail->prev = head;
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
	insert_node(head, val);
}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
	Node* p = head;
	for (int a : list_of_values) {
		insert_node(p, a);
		p = p->next;
	}
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
	Node* p = S.head->next;
	Node* p1 = head;
	while (p != S.tail) {
		insert_node(p1, p->value);
		p = p->next;
		p1 = p1->next;
	}
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
	while (head->next != tail) {
		remove_node(head->next);
	}
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
	make_empty();
	delete(head);
	delete(tail);
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Call by valued is used
 */
Set& Set::operator=(Set S) {
	make_empty();

	Node* p = S.head->next;
	Node* p1 = head;
	while (p != S.tail) {
		insert_node(p1, p->value);
		p = p->next;
		p1 = p1->next;
	}

	return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
	Node* p = head->next;
	while (p != tail) {
		if (p->value == val) {
			return true;
		}
		p = p->next;
	}

	return false;
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
	return operator<=>(S) == std::partial_ordering::equivalent;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 */
std::partial_ordering Set::operator<=>(const Set& S) const {

	Node* p = S.head->next;
	Node* p1 = head->next;
	bool Sunique = false;
	bool thisunique = false;

	while (p != S.tail && p1 != tail) {
		if (p->value == p1->value) {
			p = p->next;
			p1 = p1->next;
		}

		else if (p->value < p1->value) {
			Sunique = true;
			p = p->next;
		}

		else if (p1->value < p->value) {
			thisunique = true;
			p1 = p1->next;
		}
	}

	if (p != S.tail) {
		Sunique = true;
	}
	if (p1 != tail) {
		thisunique = true;
	}

	if (thisunique && Sunique) return std::partial_ordering::unordered;
	else if (thisunique) return std::partial_ordering::greater;
	else if (Sunique) return std::partial_ordering::less;
	return std::partial_ordering::equivalent;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
	Node* p = S.head->next;
	Node* p1 = head->next;

	while(p != S.tail && p1 != tail) {

		if (p->value < p1->value) {
			insert_node(p1->prev, p->value);
			p = p->next;
		}

		else if(p->value > p1->value) {
			p1 = p1->next;
		}

		else { //equivalent
			p = p->next;
			p1 = p1->next;
		}
	}

	while (p != S.tail) {
		insert_node(p1->prev, p->value);
		p = p->next; 
	}
	
	return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
	//Node* p = S.head->next;
	//Node* p1 = head->next;

	//while (p != tail && p1 != tail) {

	//	if (p->value > p1->value) {

	//	}

	//	if (p->value == p1->value) {

	//	}

	//}

	return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
	



	return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

 /*
  * Insert a new Node storing val after the Node pointed by p
  * \param p pointer to a Node
  * \param val value to be inserted  after position p
  */
void Set::insert_node(Node* p, int val) {
	Node* newNode = new Node(val, p->next, p);
	p->next = p->next->prev = newNode;
	++counter;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete(p);
	--counter;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
	if (is_empty()) {
		os << "Set is empty!";
	}
	else {
		Set::Node* ptr{ head->next };

		os << "{ ";
		while (ptr != tail) {
			os << ptr->value << " ";
			ptr = ptr->next;
		}
		os << "}";
	}
}
