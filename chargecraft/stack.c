#include "stack.h"
#include <stdlib.h>

void st_init(Stack* s){ s->top=0; }

/**
 * ajoute un élément en haut de la pile
 * alloue un nouveau noeud; retourne 0 si échec mémoire
 * lie le nouveau noeud à l'ancien sommet
 */
int  st_push(Stack* s, int v){
    StNode* n=(StNode*)malloc(sizeof*n); if(!n) return 0;
    n->v=v;
    // on insère le nouveau noeud en tête de la liste
    n->next=s->top;
    s->top=n;
    return 1;
}

/**
 * retire l'élément en haut de la pile et le stocke dans out si non NULL
 * libère la mémoire du noeud retiré
 * retourne 0 si la pile est vide
 */
int  st_pop(Stack* s, int* out){
    StNode* t=s->top;
    if(!t) return 0;
    if(out) *out=t->v;
    // on décale le sommet vers le noeud suivant
    s->top=t->next;
    // libération du noeud retiré
    free(t);
    return 1;
}

int  st_is_empty(Stack* s){ return s->top==0; }

/**
 * vide la pile en retirant tous les éléments
 * utilise st_pop pour gérer la libération mémoire
 */
void st_clear(Stack* s){ int tmp; while(st_pop(s,&tmp)){} }

