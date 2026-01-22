#include "queue.h"
#include <stdlib.h>

void q_init(Queue* q){ q->head=q->tail=0; }
int  q_is_empty(Queue* q){ return q->head==0; }

/**
 * Ajoute un événement à la fin de la file.
 * @param q Pointeur vers la file.
 * @param e Événement à ajouter.
 * @return 1 si l'ajout a réussi, 0 en cas d'échec d'allocation mémoire.
 */
int  q_enqueue(Queue* q, Event e){
    QNode* n=(QNode*)malloc(sizeof*n); if(!n) return 0;
    n->e=e; n->next=0;
    if(!q->tail) q->head=q->tail=n; else { q->tail->next=n; q->tail=n; } // ajout en fin de liste
    return 1;
}

/**
 * Retire l'événement en tête de la file.
 * @param q Pointeur vers la file.
 * @param out Pointeur pour stocker l'événement retiré (peut être NULL).
 * @return 1 si un élément a été retiré, 0 si la file est vide.
 */
int  q_dequeue(Queue* q, Event* out){
    QNode* h=q->head; if(!h) return 0; if(out) *out=h->e;
    q->head=h->next; // déplace la tête vers le suivant
    if(!q->head) q->tail=0; // file vide, on réinitialise la queue
    free(h); // libération mémoire du noeud retiré
    return 1;
}

/**
 * Vide complètement la file, libérant toute la mémoire associée.
 * @param q Pointeur vers la file.
 */
void q_clear(Queue* q){ Event dump; while(q_dequeue(q,&dump)){} }

