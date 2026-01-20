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
    while (c->next) {
        p = c;
        c = c->next;
    }

    if (out) *out = c->value;
    p->next = NULL;
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

void ds_slist_update_mru(SList* l, int v, int capacity) {
    ds_slist_remove_value(l, v);

    ds_slist_insert_head(l, v);

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
