/*
Proprietăți Red-Black Tree:
1. Culoarea nodurilor: fiecare nod este fie roșu, fie negru;
2. Proprietatea rădăcinii: rădăcina arborelui este întotdeauna neagră;
3. Regula nodurilor roșii: dacă un nod este roșu, atunci ambii copii ai săi sunt negri (nu pot exista două noduri roșii consecutive);
4. Regula nodurilor negre: pentru fiecare nod, toate drumurile de la acel nod la descendenții săi frunze conțin același număr de noduri negre (numit black-height);
5. Regula frunzelor: toate frunzele (NIL/NULL) sunt considerate negre.
*/

#include <string>
#include <iostream>

enum Color {
	Red,
	Black
};

class Node {
	int value;
	Color color;
	Node *left, *right, *parent;

public:
	// constructor
	Node(const int value_, const Color& color_) : value(value_), color(color_) {
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

	~Node() = default;

	// getters & setters
	[[nodiscard]] Node* getLeftNode() const;
	void setLeftNode(Node* left_);

	[[nodiscard]] Node* getRightNode() const;
	void setRightNode(Node* right_);

	[[nodiscard]] Node *getParent() const;
	void setParent(Node* parent_);

	[[nodiscard]] int getValue() const;
	void setValue(int value_);

	[[nodiscard]] Color getColor() const;
	void setColor(Color color_);
};

Node* Node::getLeftNode() const {
	return left;
}

void Node::setLeftNode(Node* left_) {
	left = left_;
}

Node* Node::getRightNode() const {
	return right;
}

void Node::setRightNode(Node* right_) {
	right = right_;
}

Node *Node::getParent() const {
	return parent;
}

void Node::setParent(Node* parent_) {
	parent = parent_;
}

int Node::getValue() const {
	return value;
}

void Node::setValue(int value_) {
	value = value_;
}

Color Node::getColor() const {
	return color;
}

void Node::setColor(Color color_) {
	color = color_;
}

class RedBlackTree {
	Node* root;
	Node* NIL;

	// methods
	void rotateLeft(Node* x);												// rotatiile mentin echilibrul arborelui
	void rotateRight(Node* x);
	void insertRebalance(Node* node);										// rebalansarea arborelui dupa insert / remove
	void removeRebalance(Node* node);										// ^ presupune recolorari si rotatii
	void deleteTree(Node* node);

	static void printTreeHelper(Node* node, Node* NIL);						// metoda folosita pentru afisare

public:
	RedBlackTree() {
		NIL = new Node(0, Black);
		NIL->setLeftNode(NIL);
		NIL->setRightNode(NIL);
		NIL->setParent(NIL);
		root = NIL;
	}

	~RedBlackTree() {													    // sterge recursiv toate nodurile din arbore
		deleteTree(root);
		delete NIL;
	}

	// methods
	void insert(int value);
	[[nodiscard]] Node* search(int value) const;

	void replaceSubtree(Node *x, Node *y);
	void remove(int value);

	void printTree() {
		printTreeHelper(root, NIL);
	}
};

void RedBlackTree::rotateLeft(Node* x) {
	Node* y = x->getRightNode();							// y este fiul drept al lui x
															// y urca pe pozitia lui x, iar x devine fiul stang al acestuia (x < y)
	auto leftNodeY = y->getLeftNode();
	x->setRightNode(leftNodeY);								// fiul stang al lui y devine fiul drept al lui x
															// intrucat leftNode(y) > x
	if (leftNodeY != NIL)
		leftNodeY->setParent(x);

	auto parentX = x->getParent();
	y->setParent(parentX);									// parintele lui x devine parintele lui y

	if (parentX == NIL)										// cazul 1: y devine radacina (x era radacina)
		root = y;
	else if (x == parentX->getLeftNode())					// cazul 2: x era fiu stang, deci y ii ia locul
		parentX->setLeftNode(y);
	else
		parentX->setRightNode(y);							// cazul 3: x era fiu drept

	y->setLeftNode(x);										// y > x => x devine fiu stang pentru y
	x->setParent(y);
}

void RedBlackTree::rotateRight(Node *x) {
	Node* y = x->getLeftNode();								// y este fiul stang al lui x
															// y urca pe pozitia lui x, iar x devine fiul drept al acestuia
	auto rightNodeY = y->getRightNode();					// toata logica e in paralel cu cea de la rotatia la stanga
	x->setLeftNode(rightNodeY);

	if (rightNodeY != NIL)
		rightNodeY->setParent(x);

	auto parentX = x->getParent();
	y->setParent(parentX);

	if (parentX == NIL)										// tratarea celor trei cazuri (x radacina, x fiu stang, x fiu drept)
		root = y;
	else if (x == parentX->getLeftNode())
		parentX->setLeftNode(y);
	else
		parentX->setRightNode(y);

	y->setRightNode(x);
	x->setParent(y);
}

void RedBlackTree::insertRebalance(Node *node) {					// nodul parametru are culoarea rosie (orice nod nou inserat este rosu)
	while (node->getParent()->getColor() == Red) {					// se incalca regula nodurilor rosii care nu pot fi consecutive
		Node* parent = node->getParent();
		Node* grandparent = parent->getParent();

		if (parent == grandparent->getLeftNode()) {					// cazul 1: parintele nodului este fiu stang
			Node* uncle = grandparent->getRightNode();

			if (uncle->getColor() == Red) {							// daca fratele nodului parinte e rosu -> triada de rosii
				parent->setColor(Black);							// solutie: parent si uncle devin noduri negre
				uncle->setColor(Black);								// grandparent devine rosu
				grandparent->setColor(Red);
				node = grandparent;									// nodul curent devine grandparent, pentru a continua bucla
			}														// colorarea lui G cu rosu ar putea propaga violatia regulii nr. 4 mai sus
			else {
				if (node == parent->getRightNode()) {
					node = parent;
					rotateLeft(node);								// daca nodul e fiu drept, facem rotatie la stanga a.i sa obtinem configuratia unei drepte
				}

				parent->setColor(Black);							// din configuratia unei linii drepte, facem rotatie la dreapta in grandparent
				grandparent->setColor(Red);							// astfel ca parent devine root in subarborele curent, deci e recolorat cu negru
				rotateRight(grandparent);							// grandparent devine rosu pentru a nu incalca regula nodurilor negre
			}
		}
		else {
			Node* uncle = grandparent->getLeftNode();				// simetric, pentru cazul in care parent este fiu drept

			if (uncle->getColor() == Red) {
				parent->setColor(Black);
				uncle->setColor(Black);
				grandparent->setColor(Red);
				node = grandparent;
			}
			else {
				if (node == parent->getLeftNode()) {
					node = parent;
					rotateRight(node);
				}

				parent->setColor(Black);
				grandparent->setColor(Red);
				rotateLeft(grandparent);
			}
		}
	}

	root->setColor(Black);											// asigura ca radacina ramane nod negru
}

void RedBlackTree::removeRebalance(Node* node) {							// rebalanseaza arborele dupa stergerea unui nod
	while (node != root && node->getColor() == Black && node != NIL) {		// intrucat dupa stergere se poate incalca principiul nodurilor negre
		auto nodeParent = node->getParent();

		if (node == nodeParent->getLeftNode()) {							// cazul 1: nodul este fiu stang
			Node* sibling = nodeParent->getRightNode();

			if (sibling->getColor() == Red) {								// daca fratele nodului este rosu, il recoloram in negru
				sibling->setColor(Black);									// intrucat e mai usor de tratat problema astfel
				nodeParent->setColor(Red);
				rotateLeft(nodeParent);										// rotatie pentru a transforma cazul intr-unul mai simplu
				sibling = nodeParent->getRightNode();
			}

			if ((sibling->getLeftNode() == NIL || sibling->getLeftNode()->getColor() == Black) &&
				(sibling->getRightNode() == NIL || sibling->getRightNode()->getColor() == Black)) {
				sibling->setColor(Red);
				node = nodeParent;												// daca ambii frati sunt noduri negre sau NIL, nu pot imprumuta negru
				}																// corectia continua mai sus
			else {
				if (sibling->getRightNode() == NIL || sibling->getRightNode()->getColor() == Black) {			// daca fratele nodului este negru si are cel putin un fiu rosu
					if (sibling->getLeftNode() != NIL)
						sibling->getLeftNode()->setColor(Black);				// fiul rosu devine nod negru, iar fratele devine rosu

					sibling->setColor(Red);										// aplicam rotatie pentru a obtine configuratia unei linii drepte
					rotateRight(sibling);

					sibling = sibling->getParent()->getRightNode();				// fiul stang al fratelui devine acum fratele nodului (il inlocuieste)
				}

				auto nodeParentColor = nodeParent->getColor();
				sibling->setColor(nodeParentColor);								// noul frate preia culoarea parintelui

				nodeParent->setColor(Black);

				if (sibling->getRightNode() != NIL)
					sibling->getRightNode()->setColor(Black);

				rotateLeft(nodeParent);										// rotatie la stanga
				node = root;												// iesim din bucla
			}
		}
		else {																// cazul 2 este simetric
			Node* sibling = nodeParent->getLeftNode();

			if (sibling->getColor() == Red) {
				sibling->setColor(Black);
				nodeParent->setColor(Red);
				rotateRight(nodeParent);
				sibling = nodeParent->getLeftNode();
			}

			if ((sibling->getRightNode() == NIL || sibling->getRightNode()->getColor() == Black) &&
				(sibling->getLeftNode() == NIL || sibling->getLeftNode()->getColor() == Black)) {
				sibling->setColor(Red);
				node = nodeParent;
				}
			else {
				if (sibling->getLeftNode() == NIL || sibling->getLeftNode()->getColor() == Black) {
					if (sibling->getRightNode() != NIL)
						sibling->getRightNode()->setColor(Black);

					sibling->setColor(Red);
					rotateLeft(sibling);
					sibling = nodeParent->getLeftNode();
				}

				sibling->setColor(nodeParent->getColor());
				nodeParent->setColor(Black);

				if (sibling->getLeftNode() != NIL)
					sibling->getLeftNode()->setColor(Black);

				rotateRight(nodeParent);
				node = root;
			}
		}
	}

	if (node != NIL)
		node->setColor(Black);
}

void RedBlackTree::deleteTree(Node *node) {								// elibereaza memoria ocupata de arbore
	if (node == NIL)
		return;

	deleteTree(node->getLeftNode());
	deleteTree(node->getRightNode());

	delete node;
}

void RedBlackTree::printTreeHelper(Node *node, Node *NIL) {				// functie responsabila de afisare
	if (node == NIL)
		return;

	std::string color;
	if (node->getColor() == Red)
		color = "Red";
	else
		color = "Black";

	std::cout << "Node: " << node->getValue() << " (" << color << "):\n";

	if (node->getLeftNode() == NIL)
		std::cout << "	left node: NIL\n";
	else {
		std::string leftColor;
		if (node->getLeftNode()->getColor() == Red)
			leftColor = "Red";
		else
			leftColor = "Black";

		std::cout << "	left node: " << node->getLeftNode()->getValue() << " (" << leftColor << ")\n";
	}

	if (node->getRightNode() == NIL)
		std::cout << "	right node: NIL\n";
	else {
		std::string rightColor;
		if (node->getRightNode()->getColor() == Red)
			rightColor = "Red";
		else
			rightColor = "Black";

		std::cout << "	right node: " << node->getRightNode()->getValue() << " (" << rightColor << ")\n";
	}

	std::cout << '\n';

	printTreeHelper(node->getLeftNode(), NIL);
	printTreeHelper(node->getRightNode(), NIL);
}


void RedBlackTree::insert(int value) {
	auto newNode = new Node(value, Red);					// orice nod nou inserat este rosu
	newNode->setLeftNode(NIL);									// ^ altfel incalca regula numarului egal de noduri negre pe toate caile
	newNode->setRightNode(NIL);									// fiii noului nod sunt NIL, adica noduri negre

	Node* y = NIL;												// y va deveni parintele nodului nou
	Node* x = root;

	while (x != NIL) {
		y = x;													// pornind de la radacina, coboram in arbore, cautand pozitia corecta
		auto valueX = x->getValue();
		if (newNode->getValue() < valueX)
			x = x->getLeftNode();
		else
			x = x->getRightNode();
	}

	newNode->setParent(y);
	auto valueY = y->getValue();

	if (y == NIL)												// y e NIL => arborele era gol, deci noul nod devine radacina
		root = newNode;
	else if (newNode->getValue() < valueY)						// altfel, conectam noul nod ca fiu stang sau fiu drept al lui y
		y->setLeftNode(newNode);								// ^ in functie de valoarea lui
	else
		y->setRightNode(newNode);

	insertRebalance(newNode);									// se pot obtine doua noduri rosii consecutive -> rebalansare dupa insert
}

Node* RedBlackTree::search(int value) const {
	Node* currentNode = root;												// incepand de la radacina, cauta nodul dat ca parametru

	while (currentNode != NIL && currentNode->getValue() != value) {
		if (value < currentNode->getValue())
			currentNode = currentNode->getLeftNode();
		else
			currentNode = currentNode->getRightNode();
	}

	if (currentNode != NIL)
		return currentNode;

	return nullptr;
}

void RedBlackTree::replaceSubtree(Node* x, Node* y) {				// metoda auxiliara folosita pentru a inlocui doua noduri
	if (x->getParent() == NIL)
		root = y;
	else if (x == x->getParent()->getLeftNode())
		x->getParent()->setLeftNode(y);
	else
		x->getParent()->setRightNode(y);

	if (y != NIL)
		y->setParent(x->getParent());
}

void RedBlackTree::remove(int value) {
	Node* z = search(value);									// cautam nodul care contine valoarea pe care vrem sa o stergem
	if (z == nullptr)
		return;

	Node* y = z;												// nodul care va fi eliminat
	Color colorY = y->getColor();								// retinem culoarea ca sa facem eventual rebalansare
	Node* x;

	if (z->getLeftNode() == NIL) {
		x = z->getRightNode();									// x va fi nodul care il inlocuieste; inlocuim cu copilul drept
		replaceSubtree(z, z->getRightNode());				// inlocuim nodurile (trebuie sa scapam de gap)
	}
	else if (z->getRightNode() == NIL) {						// simetric, inlocuim cu copilul stang
		x = z->getLeftNode();
		replaceSubtree(z, z->getLeftNode());
	}
	else {														// cazul in care z are doi copii
		y = z->getRightNode();

		while (y->getLeftNode() != NIL)							// gasim succesorul lui z
			y = y->getLeftNode();

		colorY = y->getColor();
		x = y->getRightNode();

		if (y->getParent() == z) {								// daca y este fiu al lui z, nu trebuie sa il mutam mai mult (inlocuim direct)
			if (x != NIL)
				x->setParent(y);
		}
		else {													// altfel trebuie sa reconfiguram (y este mai jos in arbore)
			replaceSubtree(y, y->getRightNode());
			y->setRightNode(z->getRightNode());
			if (y->getRightNode() != NIL)
				y->getRightNode()->setParent(y);
		}

		replaceSubtree(z, y);									// inlocuim complet z cu y si legam subarborele stang al lui z la noul y

		y->setLeftNode(z->getLeftNode());
		if (y->getLeftNode() != NIL)
			y->getLeftNode()->setParent(y);
		y->setColor(z->getColor());								// y ia culoarea lui z, ca sa pastram echilibtrul
	}

	delete z;													// stergem nodul original

	if (colorY == Black)										// daca nodul eliminat este negru, este necesara rebalansare
		removeRebalance(x);										// ^ pentru a nu incalca principiul nodurilor negre
}

int main() {
	RedBlackTree RBT;

	// test cu valori ordonate crescator (worst-case pentru BST)
	std::cout << "Arborele initial: \n";
	RBT.insert(1);
	RBT.insert(2);
	RBT.insert(3);
	RBT.insert(4);
	RBT.insert(5);
	RBT.insert(6);
	RBT.insert(7);
	RBT.insert(8);
	RBT.insert(9);
	RBT.insert(10);

	RBT.printTree();

	RBT.remove(6);
	std::cout << "Arborele dupa remove: \n";
	RBT.printTree();

	// RBT.insert(12);
	// std::cout << "Dupa insert: \n";
	// RBT.printTree();
	//
	// RBT.insert(14);
	// std::cout << "Dupa al doilea insert: \n";
	// RBT.printTree();


	// test cu valori random
	// RBT.insert(41);
	// RBT.insert(38);
	// RBT.insert(31);
	// RBT.insert(12);
	// RBT.insert(19);
	// RBT.insert(8);
}


/*
exemplu rotatie:
-- arborele initial:

  10n  x
    \
	15r  y
   /  \
 12n   20n   a b


-- dupa rotatie la stanga:

     15n   y
    /   \
  10r    20n   x b
    \
    12n  a

---------------------------------

exemplu pentru incalcarea regulii 4: noduri rosii consecutive
-- presupunem ca avem urmatoarea structura:

	G
  /   \
 P     U
 |
 N

unde g = grandparent, p = parent, u = uncle si n = node

nodul n este nou introdus -> are culoarea rosie
daca parintele nodului este rosu, incalca regula nr. 4

presupunem ca G este negru, P, U sunt rosii
-> triada rosie
recolorarea presupune urmatoarele modificari: P si U devin noduri negre, G devine rosu
obtinem urmatoarea structura:

	G(R)
  /    \
 P(B)   U(B)
 |
 N(R)

de ce devine rosu G? pentru a nu incalca regula nodurilor negre, care spune ca toate drumurile simple de la un nod la frunze
contin acelasi numar de noduri negre;
(daca G nu ar fi radacina si totusi il pastram negru, s ar adauga un nod negru in plus la drumurile care trec prin G)

---------------------------------

nota: scopul functiei removeRebalance
- corecteaza arborele dupa stergerea unui nod, in special cand se introduce un black height deficit;
^ la stergerea unui nod negru, echilibrul de noduri negre de pe diferite drumuri se poate rupe

algoritmul incearca sa rezolve deficitul prin:
- imprumutarea unui nod negru de la fratele nodului problematic (redistrubuie culoarea fara a adauga efectiv alt nod negru)
- propagarea problemei in sus catre parintele nodului
- modificarea configuratiei
*/
