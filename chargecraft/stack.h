#ifndef DS_STACK_H
#define DS_STACK_H
typedef struct StNode{ int v; struct StNode* next; } StNode;
typedef struct Stack{ StNode* top; } Stack;

void st_init(Stack* s);            /* O(1) */

/**
 * Ajoute un élément au sommet de la pile.
 * @param s Pointeur vers la pile.
 * @param v Valeur entière à empiler.
 * @return 0 si l'opération réussit, une valeur non nulle sinon (échec d'allocation).
 */
int  st_push(Stack* s, int v);     /* O(1) */

/**
 * Retire l'élément au sommet de la pile et le retourne.
 * @param s Pointeur vers la pile.
 * @param out Pointeur vers un entier où la valeur dépilée sera stockée.
 * @return 0 si la pile n'était pas vide et l'opération réussit, une valeur non nulle sinon (pile vide).
 */
int  st_pop(Stack* s, int* out);   /* O(1) */

int  st_is_empty(Stack* s);        /* O(1) */

/**
 * Vide la pile en libérant tous les noeuds.
 * @param s Pointeur vers la pile à vider.
 */
void st_clear(Stack* s);           /* O(n) */

#endif

