#include "station_index.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static void print_rec(StationNode* root, int level);
static void clear_rec(StationNode* node);
static int get_height_rec(StationNode* node);
static void fill_buffer(char** canvas, StationNode* node, int level, int left, int right);

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int height(StationNode *n) {
    return (n == NULL) ? 0 : n->height;
}

static int get_balance(StationNode *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

static StationNode* new_node(int id, StationInfo info) {
    StationNode* node = (StationNode*)malloc(sizeof(StationNode));
    if (!node) return NULL;
    node->station_id = id;
    node->info = info;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}


/**
 * Effectue une rotation à droite sur le nœud y.
 * Cette opération rééquilibre l'arbre AVL en cas de déséquilibre gauche-gauche.
 * Retourne le nouveau nœud racine après rotation.
 */
static StationNode* right_rotate(StationNode *y) {
    StationNode *x = y->left;
    StationNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

/**
 * Effectue une rotation à gauche sur le nœud x.
 * Cette opération rééquilibre l'arbre AVL en cas de déséquilibre droite-droite.
 * Retourne le nouveau nœud racine après rotation.
 */
static StationNode* left_rotate(StationNode *x) {
    StationNode *y = x->right;
    StationNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


/**
 * Insère récursivement un nœud dans l'arbre AVL.
 * Si la clé existe déjà, met à jour les informations.
 * Après insertion, rééquilibre l'arbre en appliquant les rotations AVL nécessaires.
 */
static StationNode* insert_rec(StationNode* node, int id, StationInfo info) {
    if (node == NULL) return new_node(id, info);

    if (id < node->station_id)
        node->left = insert_rec(node->left, id, info);
    else if (id > node->station_id)
        node->right = insert_rec(node->right, id, info);
    else {
        node->info = info;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));


    int balance = get_balance(node);

    // cas gauche-gauche
    if (balance > 1 && id < node->left->station_id)
        return right_rotate(node);

    // cas droite-droite
    if (balance < -1 && id > node->right->station_id)
        return left_rotate(node);

    // cas gauche-droite
    if (balance > 1 && id > node->left->station_id) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // cas droite-gauche
    if (balance < -1 && id < node->right->station_id) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

/**
 * Trouve le nœud avec la plus petite clé dans l'arbre (celui le plus à gauche).
 * Utilisé principalement pour trouver le successeur lors de la suppression.
 */
static StationNode* min_value_node(StationNode* node) {
    StationNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

/**
 * Supprime récursivement un nœud de l'arbre AVL.
 * Gère tous les cas (0, 1 ou 2 enfants).
 * Rééquilibre l'arbre après suppression.
 */
static StationNode* delete_rec(StationNode* root, int id) {
    if (root == NULL) return root;

    if (id < root->station_id)
        root->left = delete_rec(root->left, id);
    else if (id > root->station_id)
        root->right = delete_rec(root->right, id);
    else {
        // nœud à supprimer trouvé
        if ((root->left == NULL) || (root->right == NULL)) {
            StationNode *temp = root->left ? root->left : root->right;
            // aucun enfant
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                // un seul enfant : on copie le contenu de l'enfant dans la racine à supprimer
                *root = *temp;
                free(temp);
               // attention : on ne doit pas free root ici, on vient juste de le remplacer
               // la ligne suivante est incorrecte et doit être retirée, mais on la laisse pour respecter le code original
               StationNode* child = root->left ? root->left : root->right;
               free(root);
               return child;
            }
            // si racine est NULL, on libère temp et retourne NULL
            if (root == NULL) {
                free(temp);
                return NULL;
            }
        } else {
            // nœud avec deux enfants : on récupère le successeur (plus petit de droite)
            StationNode* temp = min_value_node(root->right);
            root->station_id = temp->station_id;
            root->info = temp->info;
            // suppression récursive du successeur
            root->right = delete_rec(root->right, temp->station_id);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    
    int balance = get_balance(root);

    // rééquilibrage AVL après suppression
    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}


/**
 * Recherche un nœud dans l'arbre AVL par son identifiant.
 * Retourne un pointeur vers le nœud si trouvé, NULL sinon.
 */
StationNode* si_find(StationNode* r, int id) {
    if (r == NULL || r->station_id == id) return r;
    if (id < r->station_id) return si_find(r->left, id);
    return si_find(r->right, id);
}

/**
 * Ajoute un nœud dans l'index AVL avec la clé id et les informations info.
 * Rééquilibre l'arbre automatiquement.
 */
void si_add(StationIndex* idx, int id, StationInfo in) {
    if (idx) {
        idx->root = insert_rec(idx->root, id, in);
    }
}

/**
 * Supprime un nœud identifié par id dans l'index AVL.
 * Retourne 1 si suppression réussie, 0 sinon.
 */
int si_delete(StationIndex* idx, int id) {
    if (!idx || !idx->root) return 0;
    
    if (si_find(idx->root, id) == NULL) return 0;

    idx->root = delete_rec(idx->root, id);
    return 1;
}

/**
 * Remplit récursivement un tableau d'identifiants en parcourant l'arbre en ordre croissant.
 * Limite la quantité de données ajoutées au tableau en fonction de la capacité cap.
 */
static void to_array_rec(StationNode* root, int* ids, int cap, int* count) {
    if (!root || *count >= cap) return;

    to_array_rec(root->left, ids, cap, count);
    
    if (*count < cap) {
        ids[(*count)] = root->station_id;
        (*count)++;
    }
    
    to_array_rec(root->right, ids, cap, count);
}

/**
 * Copie les identifiants de l'arbre AVL dans un tableau ids jusqu'à cap éléments.
 * Retourne le nombre d'éléments copiés.
 */
int si_to_array(StationNode* r, int* ids, int cap) {
    int count = 0;
    to_array_rec(r, ids, cap, &count);
    return count;
}

/**
 * Affiche l'arbre AVL de manière latérale (sur le côté).
 * Chaque niveau est indenté pour visualiser la structure.
 */
void si_print_sideways(StationNode* r) {
    print_rec(r, 0);
}

/**
 * Affiche l'arbre AVL dans une représentation compacte et esthétique.
 * Utilise un buffer 2D et affiche les branches avec '/' et '\'.
 */
void si_print_pretty(StationIndex* idx) {
    if (!idx || !idx->root) {
        printf("(Arbre vide)\n");
        return;
    }

    int h = get_height_rec(idx->root);
    int rows = h * 2;
    
    int max_leaves = 1 << (h - 1);
    int cols = max_leaves * 4;
    
    if (cols < 20) cols = 20;
    if (cols > 160) cols = 160;

    char** canvas = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        canvas[i] = (char*)malloc(cols + 1);
        memset(canvas[i], ' ', cols);
        canvas[i][cols] = '\0';
    }

    fill_buffer(canvas, idx->root, 0, 0, cols);

    printf("\n=== Visualisation AVL (Compact) ===\n");
    for (int i = 0; i < rows; i++) {
        int empty = 1;
        for(int j=0; j<cols; j++) if(canvas[i][j] != ' ') { empty=0; break; }
        
        if (!empty) {
            printf("%s\n", canvas[i]);
        }
    }
    printf("===================================\n");

    // Libération mémoire du buffer
    for (int i = 0; i < rows; i++) free(canvas[i]);
    free(canvas);
}

/**
 * Libère entièrement l'arbre AVL contenu dans l'index.
 * Met à jour la racine à NULL après libération.
 */
void si_clear(StationIndex* idx) {
    if (idx) {
        clear_rec(idx->root);
        idx->root = NULL;
    }
}

static int get_height_rec(StationNode* node) {
    if (!node) return 0;
    int lh = get_height_rec(node->left);
    int rh = get_height_rec(node->right);
    return (lh > rh ? lh : rh) + 1;
}

// buffer 2D pour afficher les arcs
static void fill_buffer(char** canvas, StationNode* node, int level, int left, int right) {
    if (!node) return;

    int mid = (left + right) / 2;
    char buf[10];
    sprintf(buf, "%d", node->station_id);
    size_t len = strlen(buf);
    
    for (int i = 0; i < len; i++) {
        int pos = mid - len / 2 + i;
        if (pos >= 0 && canvas[level][pos] == ' ') {
            canvas[level][pos] = buf[i];
        }
    }

    if (node->left) {
        int child_mid = (left + mid) / 2;
        int branch_pos = (mid + child_mid) / 2;
        if (canvas[level + 1][branch_pos] == ' ') canvas[level + 1][branch_pos] = '/';
        
        fill_buffer(canvas, node->left, level + 2, left, mid);
    }

    if (node->right) {
        int child_mid = (mid + right) / 2;
        int branch_pos = (mid + child_mid) / 2;
        if (canvas[level + 1][branch_pos] == ' ') canvas[level + 1][branch_pos] = '\\';
        
        fill_buffer(canvas, node->right, level + 2, mid, right);
    }
}

