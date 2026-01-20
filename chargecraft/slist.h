#ifndef DS_SLIST_H
#define DS_SLIST_H

/* Structure de nœud pour liste simplement chaînée */
typedef struct SNode {
    int value;
    struct SNode* next;
} SNode;

/* Wrapper pour la liste */
typedef struct SList {
    SNode* head;
} SList;

void ds_slist_init(SList* l);                  /* O(1) */

/* Insertions & Suppressions de base */
int  ds_slist_insert_head(SList* l, int v);    /* O(1) */
int  ds_slist_remove_value(SList* l, int v);   /* O(n) */
int  ds_slist_remove_tail(SList* l, int* out); /* O(n) */

/* Utilitaires */
int  ds_slist_size(SList* l);                  /* O(n) */
void ds_slist_print(SList* l);                 /* O(n) */
void ds_slist_clear(SList* l);                 /* O(n) */


void ds_slist_update_mru(SList* l, int v, int capacity); /* O(n) */
void ds_slist_print_pretty(SList* l, const char* title); /* O(n) */
#endif
