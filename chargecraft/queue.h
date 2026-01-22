#ifndef DS_QUEUE_H
#define DS_QUEUE_H
#include "events.h"

typedef struct QNode { Event e; struct QNode* next; } QNode;
typedef struct Queue { QNode* head; QNode* tail; } Queue;

void q_init(Queue* q);                  /* O(1) */
int  q_is_empty(Queue* q);              /* O(1) */

/**
 * @brief Ajoute un événement à la fin de la file.
 * @param q Pointeur vers la file.
 * @param e Événement à ajouter.
 * @return 1 si l'ajout réussit, 0 sinon.
 */
int  q_enqueue(Queue* q, Event e);      /* O(1) */

/**
 * @brief Retire l'événement en tête de la file.
 * @param q Pointeur vers la file.
 * @param out Pointeur où stocker l'événement retiré.
 * @return 1 si la suppression réussit, 0 si la file est vide.
 */
int  q_dequeue(Queue* q, Event* out);   /* O(1) */

/**
 * @brief Vide la file en libérant tous les nœuds.
 * @param q Pointeur vers la file.
 */
void q_clear(Queue* q);                 /* O(n) */

#endif
