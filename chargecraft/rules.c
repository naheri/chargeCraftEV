#include "station_index.h"
#include "stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Évalue une expression logique donnée sous forme postfixée sur les attributs d'une station.
 * 
 * L'expression peut contenir des opérateurs logiques et des références aux attributs "power", "price", "slots".
 * Les opérandes sont soit des entiers directs, soit des valeurs extraites de la station.
 * 
 * @param toks tableau de chaînes représentant les tokens de l'expression postfixée
 * @param n nombre de tokens dans l'expression
 * @param info pointeur vers la structure StationInfo contenant les données de la station à tester
 * @return int 1 si la règle est satisfaite, 0 sinon
 */
int eval_rule_postfix(char* toks[], int n, StationInfo* info){ /* O(n) */
    Stack st; st_init(&st);
    for(int i=0;i<n;i++){
        char* t=toks[i];
        if(strcmp(t,"power")==0)      st_push(&st, info->power_kW);
        else if(strcmp(t,"price")==0) st_push(&st, info->price_cents);
        else if(strcmp(t,"slots")==0) st_push(&st, info->slots_free);
        else if(strcmp(t,">=")==0){ 
            // dépile deux valeurs et empile le résultat de la comparaison a >= b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a>=b); 
        }
        else if(strcmp(t,"<=")==0){ 
            // dépile deux valeurs et empile le résultat de la comparaison a <= b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a<=b); 
        }
        else if(strcmp(t,">")==0){ 
            // dépile deux valeurs et empile le résultat de la comparaison a > b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a>b); 
        }
        else if(strcmp(t,"<")==0){ 
            // dépile deux valeurs et empile le résultat de la comparaison a < b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a<b); 
        }
        else if(strcmp(t,"==")==0){ 
            // dépile deux valeurs et empile le résultat de la comparaison a == b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a==b); 
        }
        else if(strcmp(t,"&&")==0){ 
            // dépile deux valeurs et empile le résultat logique a && b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a&&b); 
        }
        else if(strcmp(t,"||")==0){ 
            // dépile deux valeurs et empile le résultat logique a || b
            int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a||b); 
        }
        else {
            // token numérique : convertit en entier puis empile
            st_push(&st, atoi(t));
        }
    }
    int ok=0; st_pop(&st,&ok); st_clear(&st); 
    return ok!=0;
}

/**
 * Affiche les informations des n premières stations qui satisfont une règle définie en postfix.
 * 
 * Parcourt l'index des stations et filtre celles correspondant à la règle donnée,
 * puis affiche leurs informations jusqu'à atteindre n correspondances.
 * 
 * @param idx pointeur vers l'index des stations
 * @param tokens tableau de chaînes correspondant à la règle postfixée à appliquer
 * @param token_count nombre de tokens dans la règle
 * @param n nombre maximal de stations à afficher
 */
void rules_top_n_print(StationIndex* idx, char* tokens[], int token_count, int n) { /* O(n log n) */
    if (!idx || !idx->root) {
        printf("[Rules] Index vide.\n");
        return;
    }

    int cap = 1000;
    int* ids = (int*)malloc(cap * sizeof(int));
    if (!ids) return;

    // récupère toutes les station_id dans un tableau
    int count = si_to_array(idx->root, ids, cap);

    printf("\n=== TOP-%d Stations (Filtre Postfix) ===\n", n);
    
    int matches = 0;
    for (int i = 0; i < count; i++) {
        StationNode* node = si_find(idx->root, ids[i]);
        
        if (node) {
            // teste la station avec la règle postfixée
            if (eval_rule_postfix(tokens, token_count, &node->info)) {
                printf("  %d. Station %d | Power: %d kW | Slots: %d | Prix: %d cts\n",
                       matches + 1,
                       node->station_id,
                       node->info.power_kW,
                       node->info.slots_free,
                       node->info.price_cents);
                
                matches++;
                if (matches >= n) break; // stoppe après n correspondances
            }
        }
    }

    if (matches == 0) {
        printf("  Aucune station ne correspond aux critères.\n");
    }
    printf("========================================\n");

    free(ids);
}

