#ifndef DS_EVENTS_H
#define DS_EVENTS_H

/**
 * @brief Définit le type Event et déclare les événements utilisés dans le système.
 */

typedef struct Event {
    int ts;
    int vehicle_id;
    int station_id;
    int action;
} Event;

extern Event DS_EVENTS[];
extern int   DS_EVENTS_COUNT;

#endif
