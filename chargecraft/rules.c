#include "station_index.h"
#include "stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int eval_rule_postfix(char* toks[], int n, StationInfo* info){ /* O(n) */
    Stack st; st_init(&st);
    for(int i=0;i<n;i++){
        char* t=toks[i];
        if(strcmp(t,"power")==0)      st_push(&st, info->power_kW);
        else if(strcmp(t,"price")==0) st_push(&st, info->price_cents);
        else if(strcmp(t,"slots")==0) st_push(&st, info->slots_free);
        else if(strcmp(t,">=")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a>=b); }
        else if(strcmp(t,"<=")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a<=b); }
        else if(strcmp(t,">")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a>b); }
        else if(strcmp(t,"<")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a<b); }
        else if(strcmp(t,"==")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a==b); }
        else if(strcmp(t,"&&")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a&&b); }
        else if(strcmp(t,"||")==0){ int b,a; st_pop(&st,&b); st_pop(&st,&a); st_push(&st, a||b); }
        else { st_push(&st, atoi(t)); }
    }
    int ok=0; st_pop(&st,&ok); st_clear(&st); return ok!=0;
}
void rules_top_n_print(StationIndex* idx, char* tokens[], int token_count, int n) { /* O(n log n) */
    if (!idx || !idx->root) {
        printf("[Rules] Index vide.\n");
        return;
    }

    int cap = 1000;
    int* ids = (int*)malloc(cap * sizeof(int));
    if (!ids) return;

    int count = si_to_array(idx->root, ids, cap);

    printf("\n=== TOP-%d Stations (Filtre Postfix) ===\n", n);
    
    int matches = 0;
    for (int i = 0; i < count; i++) {
        StationNode* node = si_find(idx->root, ids[i]);
        
        if (node) {
            if (eval_rule_postfix(tokens, token_count, &node->info)) {
                printf("  %d. Station %d | Power: %d kW | Slots: %d | Prix: %d cts\n",
                       matches + 1,
                       node->station_id,
                       node->info.power_kW,
                       node->info.slots_free,
                       node->info.price_cents);
                
                matches++;
                if (matches >= n) break; 
            }
        }
    }

    if (matches == 0) {
        printf("  Aucune station ne correspond aux critères.\n");
    }
    printf("========================================\n");

    free(ids);
}

