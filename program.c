/*
* Author: Jani Heinikoski
* Created: 26.11.2020
* Last updated: 07.12.2020
* Sources:
Goel, A. 2020, Print Binary Tree in 2-Dimensions, GeeksforGeeks, [Website]. [Updated 11.02.2020]. [Referred 26.11.2020]. Available: https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/.
*/

#include "program.h"

unsigned short SHOW_ROTATIONS = true;
unsigned short SHOW_INSERTIONS = true;

/*---------------------------------------TREE OPERATIONS------------------------------------------*/

void key_in_tree(Node* root, int key, unsigned short* found) {
    /* Changes found to true if arg key is in the bst defined by arg root */
    if (root == NULL || *found == true) {
        return;
    }

    if (key == root->key) {
        *found = true;
        return;
    } else if (key < root->key) {
        key_in_tree(root->left, key, found);
    } else {
        key_in_tree(root->right, key, found);
    }
}

void ll_rotation(Node** pivot) {
    Node* child = (*pivot)->left;

    if (SHOW_ROTATIONS == true) {
        printf("\n----------------------------LL rotation------------------------------\n");
        printf("\nSubtree before rotation:\n");
        (*pivot)->balance_factor = 2; /* This is just for printing purposes ("temp bf") */
        print_tree_wrapper(*pivot);
    }
    /* Update balance factors to match state after rotation */
    (*pivot)->balance_factor = 0;
    (*pivot)->left->balance_factor = 0;
    /* Do the actual ll -rotation */
    (*pivot)->left = child->right;
    child->right = (*pivot);
    *pivot = child;

    if (SHOW_ROTATIONS == true) {
        /* Print the result */
        printf("\nSubtree after rotation:\n");
        print_tree_wrapper(*pivot);
        printf("\n---------------------------------------------------------------------\n\n");
    }
}

void lr_rotation(Node** pivot) {
    Node* child = (*pivot)->left;
    Node* gchild = child->right;

    if (SHOW_ROTATIONS == true) {
        printf("\n----------------------------LR rotation------------------------------\n");
        printf("\nSubtree before rotation:\n");
        (*pivot)->balance_factor = 2; /* This is just for printing purposes ("temp bf") */
        print_tree_wrapper(*pivot);
    }
    
    /* Update balance factors to match state after rotation */
    switch (gchild->balance_factor) {
        case -1:
            child->balance_factor = 1;
            (*pivot)->balance_factor = 0;
            break;
        case 0:
            child->balance_factor = (*pivot)->balance_factor = 0;
            break;
        case 1:
            (*pivot)->balance_factor = -1;
            child->balance_factor = 0;
            break;
    }
    gchild->balance_factor = 0;
    /* Do the actual lr -rotation */
    (*pivot)->left = gchild->right;
    gchild->right = *pivot;
    child->right = gchild->left;
    gchild->left = child;
    *pivot = gchild;

    if (SHOW_ROTATIONS == true) {
        /* Print the result */
        printf("\nSubtree after rotation:\n");
        print_tree_wrapper(*pivot);
        printf("\n---------------------------------------------------------------------\n\n");
    }
}

void rr_rotation(Node** pivot) {
    Node* child = (*pivot)->right;

    if (SHOW_ROTATIONS == true) {
        printf("\n----------------------------RR rotation------------------------------\n");
        printf("\nSubtree before rotation:\n");
        (*pivot)->balance_factor = -2; /* This is just for printing purposes ("temp bf") */
        print_tree_wrapper(*pivot);
    }
    /* Update balance factors to match state after rotation */
    (*pivot)->balance_factor = 0;
    (*pivot)->right->balance_factor = 0;
    /* Do the actual rr -rotation */
    (*pivot)->right = child->left;
    child->left = *pivot;
    *pivot = child;

    if (SHOW_ROTATIONS == true) {
        /* Print the result */
        printf("\nSubtree after rotation:\n");
        print_tree_wrapper(*pivot);
        printf("\n---------------------------------------------------------------------\n\n");
    }
}

void rl_rotation(Node** pivot) {
    Node* child = (*pivot)->right;
    Node* gchild = child->left;

    if (SHOW_ROTATIONS == true) {
        printf("\n----------------------------RL rotation------------------------------\n");
        printf("\nSubtree before rotation:\n");
        (*pivot)->balance_factor = -2; /* This is just for printing purposes ("temp bf") */
        print_tree_wrapper(*pivot);
    }
    /* Update balance factors to match state after rotation */
    switch (gchild->balance_factor) {
        case -1:
            child->balance_factor = 0;
            (*pivot)->balance_factor = 1;
            break;
        case 0:
            child->balance_factor = (*pivot)->balance_factor = 0;
            break;
        case 1:
            (*pivot)->balance_factor = 0;
            child->balance_factor = -1;
            break;
    }
    gchild->balance_factor = 0;
    /* Do the actual lr -rotation */
    (*pivot)->right = gchild->left;
    gchild->left = *pivot;
    child->left = gchild->right;
    gchild->right = child;
    *pivot = gchild;
    if (SHOW_ROTATIONS == true) {
        /* Print the result */
        printf("\nSubtree after rotation:\n");
        print_tree_wrapper(*pivot);
        printf("\n---------------------------------------------------------------------\n\n");
    }
}

void insert_node(Node** root, Node* to_add) {
    static int update_bal_factors = 0;
    /* Add node to its place (always an 'empty' leaf) */
    if (*root == NULL) {
        *root = to_add;
        update_bal_factors = 1;
        if (SHOW_INSERTIONS == true) {
            printf("Inserted node (key=%d) to the tree.\n", to_add->key);
        }
        return;
    }
    /* The BST ordering invariant: 
    if key to add is l.t. root --> add to its left subtree
    else if key to add is g.t. root --> add to its right subtree
    else key already exists --> error */
    if (to_add->key < (*root)->key) {
        insert_node(&(*root)->left, to_add);
        if (update_bal_factors == true) { /* Update balance factors of added node's ancestors (if no rotations made) */
            switch ((*root)->balance_factor) {
                case -1:
                    /* Root was right-heavy before insertion */
                    (*root)->balance_factor = 0;
                    update_bal_factors = false;
                    break;
                case 0:
                    /* Root was balanced before insertion */
                    (*root)->balance_factor = 1;
                    break;
                case 1:
                    /* Root was left-heavy before insertion */
                    if ((*root)->left->balance_factor == 1) {
                        /* Imbalance due to root's left child's left subtree */
                        ll_rotation(root);
                    } else {
                        /* Imbalance due to root's left child's right subtree */
                        lr_rotation(root);
                    }
                    /* Tree is perfectly balanced after a rotation (rest of the bal. factors remain unchanged) */
                    update_bal_factors = false;
                    break;
            }
        }
    } else if (to_add->key > (*root)->key) {
        insert_node(&(*root)->right, to_add);
        if (update_bal_factors == true) {
            switch ((*root)->balance_factor) {
                case -1:
                    if ((*root)->right->balance_factor == -1) {
                        rr_rotation(root);
                    } else {
                        rl_rotation(root);
                    }
                    update_bal_factors = false;
                    break;
                case 0:
                    (*root)->balance_factor = -1;
                    break;
                case 1:
                    (*root)->balance_factor = 0;
                    update_bal_factors = false;
                    break;
            }
        }
    } else {
        printf("No duplicate keys allowed.\n");
        update_bal_factors = false;
        return;
    }
}

/*---------------------------------------FILE READING------------------------------------------*/

FILE* open_file(char* filename) {
    if (filename == NULL) {
        perror("Invalid filename");
        exit(-2);
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(-3);
    }
    return file;
}

iArray* get_keys(char* filename) {
    /* Parses keys from a file with given (arg) filename, puts them into a new array and returns it. */
    FILE* file = open_file(filename);
    char buffer[MAX_KEY_LEN] = "";
    int key_count = 0, key;
    iArray* keys = new_array();
    
    while (fgets(buffer, MAX_KEY_LEN, file) != NULL) {
        key = parse_int_from_string(buffer);
        if (key != -1) {
            key_count++;
            add_key(keys, key);
        }
    }
    
    fclose(file);
    return keys;
}

/*---------------------------------------DYNAMIC MEMORY------------------------------------------*/

Node* new_node(int key) {
    Node* newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        perror("Malloc failed.");
        exit(-1);
    }

    newNode->balance_factor = 0;
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

iArray* new_array() {
    iArray* ret = malloc(sizeof(iArray));
    
    if (ret == NULL) {
        perror("Malloc failed.");
        exit(-1);
    }

    ret->values = malloc(sizeof(int));
    ret->size = sizeof(int);
    ret->count = 0;

    return ret;
}

void free_tree(Node** root) {
    if (*root == NULL) {
        return;
    }
    free_tree(&(*root)->left);
    free_tree(&(*root)->right);
    free(*root);
    *root = NULL;
}

void free_array(iArray* arr) {
    free(arr->values);
    free(arr);
}

/*---------------------------------------UTILITY FUNCTIONS------------------------------------------*/

void print_tree(Node* root, int space) {
    /*(Goel 2020)*/
    /* Prints the tree in a 2d fashion to stdout */
    if (root == NULL) {
        return;
    }

    space += 10;
    print_tree(root->right, space);
    
    int i;
    printf("\n");
    for (i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%d,%d", root->key, root->balance_factor);

    print_tree(root->left, space);
}

void print_inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    print_inorder(root->left);
    printf("[key: %d, bf: %d], ", root->key, root->balance_factor);
    print_inorder(root->right);
}

void print_tree_wrapper(Node* root) {
    print_tree(root, 0);
    printf("\n");
}

int parse_int_from_string(char* str) {
    /* Parses an integer from the argument */
    if (str == NULL || str[0] == '\0') {
        return -1;
    }

    long ret;
    int i = 0;

    while (str[i] >= '0' && str[i] <= '9') {
        i++;
    }

    str[i] = '\0';
    ret = (int)strtol(str, NULL, 10);

    if (ret == 0 && str[0] != '0') {
        return -1;
    } else {
        return (int) ret;
    }
}

void add_key(iArray* arr, int key) {
    /* Adds a new key into the dynamic iArray arr */
    if ((arr->count * sizeof(int)) >= arr->size) {
        arr->size += sizeof(int);
        arr->values = realloc(arr->values, arr->size);
    }
    arr->values[arr->count++] = key;
}

int get_int_from_stdin() {
    /* Parses an integer value from the standard input stream */
    char buff[(MAX_KEY_LEN + 1)];
    fgets(buff, (MAX_KEY_LEN + 1), stdin);
    return parse_int_from_string(buff);
}

int cli_get_option() {
    /* Can be replaced with any UI.
    Modify main function accordingly */
    printf(MENU);
    int choice = get_int_from_stdin();
    if (choice >= 0 && choice <= 11) {
        return choice;
    } else {
        return -1;
    }
}

void add_n_keys(Node** root, int count) {
    int max_key = 0, i;

    if (*root != NULL) {
        max_key = (*root)->key;
        Node* n = *root;
        while (n->right != NULL) {
            max_key = n->right->key;
            n = n->right;
        }
    }

    for (i = (max_key + 1); i < (max_key + count + 1); i++) {
        insert_node(root, new_node(i));
    }
}

/*---------------------------------------MAIN FUNCTION------------------------------------------*/

int main(void) {
    Node* root = NULL;
    int opt, key, i;
    unsigned short key_found = false;
    char buff[256];

    iArray* keys_from_file;
    
    while (1) {
        opt = cli_get_option();
        switch (opt) {
            case 0:
                printf("Exiting... \n");
                if (root != NULL) {
                    free_tree(&root);
                }
                return 0;
            case 1:
                printf("Enter a key (must be non-negative and <= 9 digits long): ");
                key = get_int_from_stdin();
            
                if (key >= 0 && key < __INT_MAX__) {
                    if (SHOW_INSERTIONS == true) {
                        printf("\nInserting node %d\n", key);
                    }
                    insert_node(&root, new_node(key));
                } else {
                    printf("\nInvalid key\n");
                }
                break;
            case 2:
                if (root == NULL) {
                    printf("\nThe tree is empty.\n");
                } else {
                    print_tree_wrapper(root);
                }
                break;
            case 3:
                if (root == NULL) {
                    printf("\nThe tree is empty.\n");
                } else {
                    printf("\n");
                    print_inorder(root);
                    printf("\n");
                }
                break;
            case 4:
                if (root == NULL) {
                    printf("\nThe tree is already empty.\n");
                } else {
                    free_tree(&root);
                }
                break;
            case 5:
                key_found = false;
                printf("Enter a key (must be non-negative and <= 9 digits long): ");
                key = get_int_from_stdin();
                key_in_tree(root, key, &key_found);
                if (key_found == true) {
                    printf("\nKey was found in the tree.\n");
                } else {
                    printf("\nKey isn't in the tree.\n");
                }
                break;
            case 6:
                printf("Enter the filename (max. 255 chars): ");
                if (fgets(buff, 256, stdin) != NULL) { /* Read filename from user */
                    printf("\n");
                    /* Replace fgets' newline with null terminator (if exists) */
                    for (i = 0; i < 256; i++) {
                        if (buff[i] == '\n') {
                            buff[i] = '\0';
                            break;
                        }
                    }
                    /* Try to read the keys to iArray from the file */
                    keys_from_file = get_keys(buff); 
                    /* Inform user if keys were found */
                    if (keys_from_file->count > 0) {
                        printf("Keys found from file %s: %d\n", buff, keys_from_file->count);
                    } else {
                        printf("No keys found from file %s\n", buff);
                        free_array(keys_from_file);
                        keys_from_file = NULL;
                    }
                    /* If keys were found, ask if they should be added to the tree */
                    if (keys_from_file != NULL) {
                        printf("Insert keys from %s to the tree? (1=yes, else=no): ", buff);
                        int i = get_int_from_stdin();
                        if (i == true) {
                            printf("\n---------------------------------------------------------------------\n\n");
                            for (i = 0; i < keys_from_file->count; i++) {
                                insert_node(&root, new_node(keys_from_file->values[i]));
                            }
                            if (SHOW_INSERTIONS == true) {
                                printf("Keys inserted to the tree, current tree:\n");
                                print_tree_wrapper(root);
                            } else {
                                printf("Keys inserted to the tree.\n");
                            }
                        }
                        free_array(keys_from_file);
                    }
                } else {
                    perror("Fgets failed\n");
                    exit(-5);
                }
                break;
            case 7:
                printf("How many keys to add: ");
                key = get_int_from_stdin();
                add_n_keys(&root, key);
                break;
            case 8:
                SHOW_ROTATIONS = false;
                break;
            case 9:
                SHOW_ROTATIONS = true;
                break;
            case 10:
                SHOW_INSERTIONS = false;
                break;
            case 11:
                SHOW_INSERTIONS = true;
                break;
            default:
                printf("\nInvalid input. Type 0 and hit enter to exit.\n");
                break;
        }
    }

    return 0;
}