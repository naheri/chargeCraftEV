#include "slist.h"
#include <stdlib.h>
#include <stdio.h>

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

int ds_slist_remove_value(SList* l, int v) {
    SNode* p = NULL;
    SNode* c = l->head;
    
    // Recherche de la valeur
    while (c && c->value != v) {
        p = c;
        c = c->next;
    }
    
    // Non trouvé
    if (!c) return 0;
    
    // Suppression
    if (!p) l->head = c->next; // C'était la tête
    else p->next = c->next;    // C'était au milieu/fin
    
    free(c);
    return 1;
}

int ds_slist_remove_tail(SList* l, int* out) {
    if (!l->head) return 0; // Liste vide

    // Cas 1 élément
    if (!l->head->next) {
        if (out) *out = l->head->value;
        free(l->head);
        l->head = NULL;
        return 1;
    }

    // Cas N éléments : trouver l'avant-dernier
    SNode* p = NULL;
    SNode* c = l->head;
    while (c->next) {
        p = c;
        c = c->next;
    }

    if (out) *out = c->value;
    p->next = NULL; // L'avant-dernier devient le dernier
    free(c);
    return 1;
}

int ds_slist_size(SList* l) {
    int count = 0;
    SNode* c = l->head;
    while (c) {
        count++;
        c = c->next;
    }
    return count;
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

void ds_slist_clear(SList* l) {
    SNode* c = l->head;
    while (c) {
        SNode* n = c->next;
        free(c);
        c = n;
    }
    l->head = NULL;
}

// Implémentation de la logique MRU demandée par le PDF (Partie 1, point 3)
void ds_slist_update_mru(SList* l, int v, int capacity) {
    // 1. Éviter les doublons : on supprime l'existant pour le remettre en tête
    ds_slist_remove_value(l, v);

    // 2. Insertion en tête (Most Recently Used)
    ds_slist_insert_head(l, v);

    // 3. Gestion de la capacité (taille bornée)
    // Comme c'est une SList, le count est en O(n), mais sur un MRU court c'est OK.
    while (ds_slist_size(l) > capacity) {
        ds_slist_remove_tail(l, NULL);
    }
}
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
