# Red-BlackTree 

## [RO] 🟥
## Descriere
Proiectul constă în implementarea unui arbore *Red-Black* (Red-Black Tree, RBT) în C++, utilizând principiile programării orientate pe obiecte pentru a organiza nodurile și relațiile dintre acestea într-o manieră clară și modulară. Arborele Red-Black este un arbore binar de căutare autobalansat, conceput pentru a permite efectuarea operațiilor de *inserare*, *căutare* și *ștergere* a valorilor cu o complexitate logaritmică O(log n). 

Implementarea respectă *proprietățile fundamentale* ale unui RBT:
- Fiecare nod este colorat roșu sau negru;
- Rădăcina arborelui este întotdeauna neagră;
- Nu pot exista două noduri roșii consecutive pe niciun drum de la rădăcină la frunze;
- Toate drumurile de la un nod la frunzele sale conțin același număr de noduri negre (black-height);
- Frunzele NIL sunt considerate negre pentru a simplifica logica arborelui și gestionarea pointerilor.

Pentru a menține echilibrul arborelui, proiectul include:
- *Rotații la stânga și la dreapta*, pentru a corecta dezechilibrele apărute în urma efectuării operațiilor de inserare, respectiv ștergere;
- *Recolorări*, pentru a rezolva conflictele dintre nodurile roșii consecutive și pentru a păstra proprietățile black-height;
- *Reechilibrare automată* după fiecare operație care poate afecta structura arborelui.


## [EN] ⬛️
## Description
This project focuses on the design and implementation of a *Red-Black Tree* (RBT) in C++, built with object-oriented programming principles to achieve a clean, modular and maintainable structure. A Red-Black Tree is a self-balancing binary search tree that ensures efficient performance, allowing insertion, search, and deletion operations to be performed in O(log n) time.

The implementation highlights the core properties that define Red-Black Trees:
- Every node is assigned a color, red or black, and the root node is always black;
- Red nodes cannot have red children (no two consecutive red nodes on any path);
- Every path from a node to its descendant null nodes (leaves / NIL) has the same number of black nodes (black-height);
- All leaves are black;

  To maintain balance and preserve these properties, the tree supports:
  - *Left and right rotations* to correct imbalances caused by insertions or deletions;
  - *Recoloring* to resolve color conflicts and maintain the black-height property;
  - *Automatic rebalancing*, ensuring that the tree remains balanced after any structural changes.
