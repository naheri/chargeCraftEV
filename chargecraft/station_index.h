#ifndef DS_STATION_INDEX_H
#define DS_STATION_INDEX_H

typedef struct StationInfo {
    int power_kW;
    int price_cents;
    int slots_free;
    int last_ts; /* dernière maj */
} StationInfo;

typedef struct StationNode {
    int station_id;
    StationInfo info;
    struct StationNode* left;
    struct StationNode* right;
    int height;
} StationNode;

typedef struct StationIndex {
    StationNode* root;
} StationIndex;

void si_init(StationIndex* idx);                         /* O(1) */
StationNode* si_find(StationNode* r, int id);           /* O(log n) */
void si_add(StationIndex* idx, int id, StationInfo in); /* O(log n) */
int  si_delete(StationIndex* idx, int id);              /* O(log n) */
int  si_to_array(StationNode* r, int* ids, int cap);    /* O(n) */
void si_print_sideways(StationNode* r);                  /* O(n) */
void si_print_pretty(StationIndex* idx);                 /* O(n) */
void si_clear(StationIndex* idx);                         /* O(n) */

#endif
