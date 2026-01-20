#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "station_index.h"
#include "slist.h"
#include "queue.h"
#include "events.h"


#define NB_VEHICULES_SIMULES 8
#define MAX_VEH_ID 20
#define MRU_CAPACITY 5

// Prototype manuel
void rules_top_n_print(StationIndex* idx, char* tokens[], int token_count, int n);

int main(void) {
    printf("=== CHARGECRAFT: DEMO FLOTTE (%d VEHICULES) ===\n\n", NB_VEHICULES_SIMULES);
    srand(time(NULL));

    // --- 1. INITIALISATION DES STRUCTURES ---
    StationIndex idx;
    si_init(&idx);

    Queue q;
    q_init(&q);

    // TABLEAU DE LISTES MRU (Un historique par véhicule)
    // On alloue un tableau statique suffisant pour les IDs utilisés
    SList flotte_mru[MAX_VEH_ID];
    for (int i = 0; i < MAX_VEH_ID; i++) {
        ds_slist_init(&flotte_mru[i]);
    }

    // --- 2. PRÉ-CHARGEMENT DES STATIONS (AVL) ---
    printf("[INIT] Initialisation du réseau de bornes...\n");
    // On ajoute quelques stations supplémentaires pour que les 8 véhicules aient de la place
    si_add(&idx, 101, (StationInfo){22, 30, 5, 0});
    si_add(&idx, 102, (StationInfo){50, 45, 2, 0});
    si_add(&idx, 103, (StationInfo){100, 60, 1, 0});
    si_add(&idx, 104, (StationInfo){150, 70, 0, 0}); // Saturée
    si_add(&idx, 105, (StationInfo){22, 25, 10, 0}); // Vide
    
    // Affichage technique
    printf("Aperçu initial (Sideways) :\n");
    si_print_sideways(idx.root);
    printf("---------------------------------------------------\n");


    // --- 3. INGESTION (DS_EVENTS + Simulation Aléatoire) ---
    
    // A. Chargement des événements statiques (Fichier fourni)
    printf("[INGESTION] Chargement de %d événements historiques (DS_EVENTS)...\n", DS_EVENTS_COUNT);
    for (int i = 0; i < DS_EVENTS_COUNT; i++) {
        q_enqueue(&q, DS_EVENTS[i]);
    }

    // B. Génération de trafic pour atteindre 8 véhicules actifs
    printf("[INGESTION] Génération de trafic supplémentaire pour les véhicules 1 à %d...\n", NB_VEHICULES_SIMULES);
    int stations_dispo[] = {101, 102, 103, 104, 105};
    int ts_simu = 2000; // Timestamp fictif futur

    for (int v = 1; v <= NB_VEHICULES_SIMULES; v++) {
        // Chaque véhicule fait 2 actions aléatoires
        for (int k = 0; k < 2; k++) {
            int st_alea = stations_dispo[rand() % 5];
            
            // Événement 1: Se branche
            Event e_in = { .ts = ts_simu++, .vehicle_id = v, .station_id = st_alea, .action = 1 };
            q_enqueue(&q, e_in);

            // Événement 2: Se débranche (un peu plus tard)
            Event e_out = { .ts = ts_simu++, .vehicle_id = v, .station_id = st_alea, .action = 0 };
            q_enqueue(&q, e_out);
        }
    }


    // --- 4. TRAITEMENT DU FLUX ---
    printf("[PROCESS] Traitement de la file d'événements...\n");
    Event e;
    while(q_dequeue(&q, &e)) {
        StationNode* node = si_find(idx.root, e.station_id);
        if (node) {
            node->info.last_ts = e.ts;

            if (e.action == 1) { // PLUG IN
                if (node->info.slots_free > 0) node->info.slots_free--;
                
                // GESTION MRU
                // Vérification de sécurité sur l'ID
                if (e.vehicle_id >= 0 && e.vehicle_id < MAX_VEH_ID) {
                    ds_slist_update_mru(&flotte_mru[e.vehicle_id], e.station_id, MRU_CAPACITY);
                }

            } else if (e.action == 0) { // UNPLUG
                node->info.slots_free++;
            }
        }
    }
    printf("Traitement terminé.\n");
    printf("---------------------------------------------------\n");


    // --- 5. RÉSULTATS FINAUX ---

    // A. Affichage des historiques pour les 8 véhicules
    printf("[DEMO 1] Historiques de la flotte (MRU) :\n");
    for (int v = 1; v <= NB_VEHICULES_SIMULES; v++) {
        char buffer[64];
        sprintf(buffer, "--- Véhicule ID %d ---", v);
        ds_slist_print_pretty(&flotte_mru[v], buffer);
    }

    // B. Requête Top-N
    char* rules[] = { "power", "50", ">=", "slots", "1", ">=", "&&" };
    printf("\n[DEMO 2] Top-3 Stations (Power >= 50 && Slots >= 1) :\n");
    rules_top_n_print(&idx, rules, 7, 3);

    // C. Affichage visuel final
    printf("\n[DEMO 3] État final du réseau (Visualisation Top-Down) :\n");
    si_print_pretty(&idx);
    
    // Comparaison Sideways
    printf("\n(Debug: Structure interne Sideways)\n");
    si_print_sideways(idx.root);


    // --- 6. NETTOYAGE ---
    si_clear(&idx);
    q_clear(&q);
    // Nettoyage de chaque liste de la flotte
    for (int i = 0; i < MAX_VEH_ID; i++) {
        ds_slist_clear(&flotte_mru[i]);
    }
    printf("\nNettoyage terminé. Fin du programme.\n");
    
    return 0;
}
