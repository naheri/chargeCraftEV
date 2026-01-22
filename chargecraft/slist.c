#include "slist.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Supprime le premier noeud contenant la valeur v dans la liste.
 * Retourne 1 si un noeud a été supprimé, 0 sinon.
 */
int ds_slist_remove_value(SList* l, int v) {
    SNode* p = NULL;
    SNode* c = l->head;
    while (c && c->value != v) {
        p = c;
        c = c->next;
    }
    
    if (!c) return 0;
    
    if (!p) l->head = c->next;
    else p->next = c->next;
    
    free(c);
    return 1;
}

/**
 * Supprime le dernier noeud de la liste.
 * Si out n'est pas NULL, stocke la valeur du noeud supprimé dans *out.
 * Retourne 1 si un noeud a été supprimé, 0 si la liste est vide.
 */
int ds_slist_remove_tail(SList* l, int* out) {
    if (!l->head) return 0;

    if (!l->head->next) {
        if (out) *out = l->head->value;
        free(l->head);
        l->head = NULL;
        return 1;
    }

    SNode* p = NULL;
    SNode* c = l->head;
    // on parcourt jusqu'à atteindre le dernier noeud
    while (c->next) {
        p = c;
        c = c->next;
    }

    if (out) *out = c->value;
    p->next = NULL;
    free(c);
    return 1;
}

/**
 * Calcule et retourne le nombre de noeuds dans la liste.
 */
int ds_slist_size(SList* l) {
    int count = 0;
    SNode* c = l->head;
    // on compte chaque noeud jusqu'à la fin
    while (c) {
        count++;
        c = c->next;
    }
    return count;
}

/**
 * Libère tous les noeuds de la liste et remet la tête à NULL.
 */
void ds_slist_clear(SList* l) {
    SNode* c = l->head;
    // on libère chaque noeud un par un
    while (c) {
        SNode* n = c->next;
        free(c);
        c = n;
    }
    l->head = NULL;
}

/**
 * Met à jour la liste en MRU (Most Recently Used) :
 * supprime la valeur v si elle existe, l'insère en tête,
 * et tronque la liste si sa taille dépasse la capacité.
 */
void ds_slist_update_mru(SList* l, int v, int capacity) {
    ds_slist_remove_value(l, v);

    ds_slist_insert_head(l, v);

    // tant que la liste dépasse la capacité, on supprime la queue
    while (ds_slist_size(l) > capacity) {
        ds_slist_remove_tail(l, NULL);
    }
}

/**
 * Affiche la liste avec un titre, en mettant en avant la dernière visite.
 */
void ds_slist_print_pretty(SList* l, const char* title) {
    printf("%s\n", title);
    
    if (!l || !l->head) {
        printf("  (Aucun historique disponible)\n");
        return;
    }

    SNode* curr = l->head;
    int index = 1;
    
    while (curr) {
        if (index == 1) {
            printf("  -> Station %d (Dernière visite)\n", curr->value);
        } else {
            printf("  -> Station %d\n", curr->value);
        }
        curr = curr->next;
        index++;
    }
    printf("\n");
}

void ds_slist_init(SList* l) {
    l->head = NULL;
}

int ds_slist_insert_head(SList* l, int v) {
    SNode* n = (SNode*)malloc(sizeof(SNode));
    if (!n) return 0;
    n->value = v;
    n->next = l->head;
    l->head = n;
    return 1;
}

void ds_slist_print(SList* l) {
    SNode* c = l->head;
    printf("[");
    while (c) {
        printf("%d", c->value);
        if (c->next) printf(" -> ");
        c = c->next;
    }
    printf("]\n");
}
