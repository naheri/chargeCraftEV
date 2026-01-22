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

/**
 * Recherche un noeud représentant une station par son identifiant dans l'arbre.
 * 
 * @param r Racine de l'arbre à parcourir.
 * @param id Identifiant de la station recherchée.
 * @return Pointeur vers le noeud correspondant ou NULL si non trouvé.
 */
StationNode* si_find(StationNode* r, int id);           /* O(log n) */

/**
 * Ajoute une nouvelle station dans l'index ou met à jour une station existante.
 * 
 * @param idx Index dans lequel insérer ou mettre à jour la station.
 * @param id Identifiant de la station.
 * @param in Informations associées à la station.
 */
void si_add(StationIndex* idx, int id, StationInfo in); /* O(log n) */

/**
 * Supprime une station de l'index en fonction de son identifiant.
 * 
 * @param idx Index dans lequel supprimer la station.
 * @param id Identifiant de la station à supprimer.
 * @return 1 si la suppression a réussi, 0 sinon.
 */
int  si_delete(StationIndex* idx, int id);              /* O(log n) */

/**
 * Copie les identifiants de toutes les stations dans un tableau.
 * 
 * @param r Racine de l'arbre à parcourir.
 * @param ids Tableau de destination pour les identifiants.
 * @param cap Capacité maximale du tableau.
 * @return Nombre d'identifiants copiés.
 */
int  si_to_array(StationNode* r, int* ids, int cap);    /* O(n) */

/**
 * Affiche l'arbre de stations de façon latérale pour en visualiser la structure.
 * 
 * @param r Racine de l'arbre à afficher.
 */
void si_print_sideways(StationNode* r);                  /* O(n) */

/**
 * Affiche l'index des stations de manière lisible et structurée.
 * 
 * @param idx Index à afficher.
 */
void si_print_pretty(StationIndex* idx);                 /* O(n) */

/**
 * Libère toutes les ressources associées à l'index et réinitialise l'index.
 * 
 * @param idx Index à nettoyer.
 */
void si_clear(StationIndex* idx);                         /* O(n) */

#endif

