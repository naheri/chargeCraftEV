#include "station_index.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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
    if (!node) return NULL; // Gestion erreur allocation
    node->station_id = id;
    node->info = info;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// --- Rotations AVL ---

static StationNode* right_rotate(StationNode *y) {
    StationNode *x = y->left;
    StationNode *T2 = x->right;

    // Rotation
    x->right = y;
    y->left = T2;

    // Mise à jour hauteurs
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static StationNode* left_rotate(StationNode *x) {
    StationNode *y = x->right;
    StationNode *T2 = y->left;

    // Rotation
    y->left = x;
    x->right = T2;

    // Mise à jour hauteurs
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// --- Logique d'Insertion Récursive ---

static StationNode* insert_rec(StationNode* node, int id, StationInfo info) {
    // 1. Insertion BST standard
    if (node == NULL) return new_node(id, info);

    if (id < node->station_id)
        node->left = insert_rec(node->left, id, info);
    else if (id > node->station_id)
        node->right = insert_rec(node->right, id, info);
    else {
        // Mise à jour (Update) des données si l'ID existe déjà
        node->info = info;
        return node;
    }

    // 2. Mise à jour hauteur
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Équilibrage
    int balance = get_balance(node);

    // Left Left
    if (balance > 1 && id < node->left->station_id)
        return right_rotate(node);

    // Right Right
    if (balance < -1 && id > node->right->station_id)
        return left_rotate(node);

    // Left Right
    if (balance > 1 && id > node->left->station_id) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left
    if (balance < -1 && id < node->right->station_id) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// Helper pour trouver le nœud minimum (successeur)
static StationNode* min_value_node(StationNode* node) {
    StationNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// --- Logique de Suppression Récursive (Bonus/Requis par .h) ---

static StationNode* delete_rec(StationNode* root, int id) {
    if (root == NULL) return root;

    // Recherche du nœud
    if (id < root->station_id)
        root->left = delete_rec(root->left, id);
    else if (id > root->station_id)
        root->right = delete_rec(root->right, id);
    else {
        // Nœud trouvé
        // Cas 1 ou 0 enfant
        if ((root->left == NULL) || (root->right == NULL)) {
            StationNode *temp = root->left ? root->left : root->right;
            if (temp == NULL) { // 0 enfant
                temp = root;
                root = NULL;
            } else { // 1 enfant
                *root = *temp; // Copie le contenu de l'enfant
                free(temp); // Libère l'enfant (dont on a copié le contenu)
                // Note: *root = *temp écrase les pointeurs left/right,
                // mais attention à ne pas faire de double free.
                // Une implémentation plus sûre pour les pointeurs :
                /*
                StationNode* child = root->left ? root->left : root->right;
                free(root);
                return child;
                */
               // Utilisons la version sûre (remplacement de pointeur) :
               StationNode* child = root->left ? root->left : root->right;
               free(root);
               return child;
            }
            if (root == NULL) { // Si c'était 0 enfant
                free(temp);
                return NULL;
            }
        } else {
            // Cas 2 enfants : Successeur in-order (plus petit du sous-arbre droit)
            StationNode* temp = min_value_node(root->right);
            root->station_id = temp->station_id;
            root->info = temp->info;
            root->right = delete_rec(root->right, temp->station_id);
        }
    }

    if (root == NULL) return root;

    // Mise à jour hauteur
    root->height = 1 + max(height(root->left), height(root->right));
    
    // Équilibrage
    int balance = get_balance(root);

    // LL
    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);
    // LR
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    // RR
    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    // RL
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

// --- Implémentation de l'API Publique ---

void si_init(StationIndex* idx) {
    if (idx) idx->root = NULL;
}

StationNode* si_find(StationNode* r, int id) {
    if (r == NULL || r->station_id == id) return r;
    if (id < r->station_id) return si_find(r->left, id);
    return si_find(r->right, id);
}

void si_add(StationIndex* idx, int id, StationInfo in) {
    if (idx) {
        idx->root = insert_rec(idx->root, id, in);
    }
}

int si_delete(StationIndex* idx, int id) {
    if (!idx || !idx->root) return 0;
    
    // Petite vérif d'existence avant suppression pour retourner le bon statut
    if (si_find(idx->root, id) == NULL) return 0;

    idx->root = delete_rec(idx->root, id);
    return 1;
}

// Parcours in-order pour remplir un tableau d'IDs
static void to_array_rec(StationNode* root, int* ids, int cap, int* count) {
    if (!root || *count >= cap) return;

    to_array_rec(root->left, ids, cap, count);
    
    if (*count < cap) {
        ids[(*count)] = root->station_id;
        (*count)++;
    }
    
    to_array_rec(root->right, ids, cap, count);
}

int si_to_array(StationNode* r, int* ids, int cap) {
    int count = 0;
    to_array_rec(r, ids, cap, &count);
    return count;
}

static void print_rec(StationNode* root, int level) {
    if (root == NULL) return;
    print_rec(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d (%dkW)\n", root->station_id, root->info.power_kW);
    print_rec(root->left, level + 1);
}

void si_print_sideways(StationNode* r) {
    print_rec(r, 0);
}

static void clear_rec(StationNode* node) {
    if (node == NULL) return;
    clear_rec(node->left);
    clear_rec(node->right);
    free(node);
}

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
    int len = strlen(buf);
    
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

void si_print_pretty(StationIndex* idx) {
    if (!idx || !idx->root) {
        printf("(Arbre vide)\n");
        return;
    }

    // 1. Calcul dynamique de la largeur
    // Au lieu de 40*h, on utilise une puissance de 2 : (2^h) * K
    // Cela garantit que les feuilles ne se chevauchent pas tout en restant serré en haut.
    int h = get_height_rec(idx->root);
    int rows = h * 2;
    
    // Facteur de densité : 4 caractères par feuille max (ex: "101 ")
    // Le décalage de bit (1 << (h-1)) calcule 2^(h-1) (nombre de feuilles max)
    int max_leaves = 1 << (h - 1);
    int cols = max_leaves * 4;
    
    // Sécurité min/max
    if (cols < 20) cols = 20;
    if (cols > 160) cols = 160; // Limite console

    // 2. Allocation
    char** canvas = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        canvas[i] = (char*)malloc(cols + 1);
        memset(canvas[i], ' ', cols);
        canvas[i][cols] = '\0';
    }

    // 3. Remplissage
    fill_buffer(canvas, idx->root, 0, 0, cols);

    // 4. Affichage
    printf("\n=== Visualisation AVL (Compact) ===\n");
    for (int i = 0; i < rows; i++) {
        int empty = 1;
        for(int j=0; j<cols; j++) if(canvas[i][j] != ' ') { empty=0; break; }
        
        if (!empty) {
            printf("%s\n", canvas[i]);
        }
    }
    printf("===================================\n");

    // Libération
    for (int i = 0; i < rows; i++) free(canvas[i]);
    free(canvas);
}
