#ifndef DS_SLIST_H
#define DS_SLIST_H

typedef struct SNode {
    int value;
    struct SNode* next;
} SNode;

typedef struct SList {
    SNode* head;
} SList;

void ds_slist_init(SList* l);                  /* O(1) */


int  ds_slist_insert_head(SList* l, int v);    /* O(1) */

/**
 * Supprime la première occurrence de la valeur `v` dans la liste.
 * 
 * @param l Pointeur vers la liste.
 * @param v Valeur à supprimer.
 * @return 1 si une valeur a été supprimée, 0 sinon.
 */
int  ds_slist_remove_value(SList* l, int v);   /* O(n) */

/**
 * Supprime le dernier élément de la liste et stocke sa valeur dans `out`.
 * 
 * @param l Pointeur vers la liste.
 * @param out Pointeur vers un entier où sera stockée la valeur supprimée.
 * @return 1 si un élément a été supprimé, 0 si la liste était vide.
 */
int  ds_slist_remove_tail(SList* l, int* out); /* O(n) */

/**
 * Calcule et retourne le nombre d'éléments dans la liste.
 * 
 * @param l Pointeur vers la liste.
 * @return Taille de la liste.
 */
int  ds_slist_size(SList* l);                  /* O(n) */

/**
 * Supprime tous les éléments de la liste, libérant la mémoire associée.
 * 
 * @param l Pointeur vers la liste.
 */
void ds_slist_clear(SList* l);                 /* O(n) */

/**
 * Met à jour la liste en déplaçant la valeur `v` en tête, ou l'ajoute si absente.
 * Si la capacité est dépassée, supprime le dernier élément.
 * 
 * @param l Pointeur vers la liste.
 * @param v Valeur à mettre à jour ou ajouter.
 * @param capacity Capacité maximale de la liste.
 */
void ds_slist_update_mru(SList* l, int v, int capacity); /* O(n) */

/**
 * Affiche la liste avec un titre, dans un format lisible et esthétique.
 * 
 * @param l Pointeur vers la liste.
 * @param title Titre à afficher avant la liste.
 */
void ds_slist_print_pretty(SList* l, const char* title); /* O(n) */
#endif
