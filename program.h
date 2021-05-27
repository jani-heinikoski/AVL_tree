#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#ifndef PROGRAM_HEADER
#define PROGRAM_HEADER

    #define MAX_KEY_LEN 9
    #define KEY_FILE "keys.txt"
    #include <stdio.h>
    #include <stdlib.h>

    #define MENU "\nChoose an option:\n"\
                "1) Insert a new node to the tree\n"\
                "2) Print tree\n"\
                "3) Print tree in inorder\n"\
                "4) Free and create a new tree\n"\
                "5) Check if a key is in the tree\n"\
                "6) Read keys from file\n"\
                "7) Add n keys to tree\n"\
                "8) Hide rotations\n"\
                "9) Show rotations\n"\
                "10) Hide insertions\n"\
                "11) Show insertions\n"\
                "0) Exit the program\n"\
                "Input: "

    typedef struct node Node;

    struct node {
        Node* left;
        Node* right;
        int key;
        short balance_factor;
    };

    typedef struct int_array iArray;

    struct int_array {
        int* values;
        size_t size;
        int count;
    };
    /* Util */
    Node* new_node(int);
    void add_n_keys(Node**, int);
    /* Tree printing */
    void print_tree(Node*, int);
    void print_tree_wrapper(Node*);
    /* File reading and parsing */
    FILE* open_file(char*);
    iArray* get_keys(char*);
    int parse_int_from_string(char*);
    /* Dynamic array */
    iArray* new_array();
    void add_key(iArray*, int);
    void free_array(iArray*);
    /* Tree operations */
    void insert_node(Node**, Node*);
    void print_inorder(Node*);
    void free_tree(Node**);
    void key_in_tree(Node*, int, unsigned short*);
    /* Left rotations */
    void ll_rotation(Node**);
    void lr_rotation(Node**);
    /* Right rotations */
    void rr_rotation(Node**);
    void rl_rotation(Node**);
    /* UI */
    int get_int_from_stdin();
    int cli_get_option();

#endif