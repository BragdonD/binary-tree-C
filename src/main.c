#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "SFML/Window.h"
#include "SFML/Graphics.h"

#define UNKNOWN_STRING "?"
#define COUNT 1
#define SCREEN_CENTER 600
#define HEIGHT_BETWEEN_NODE 100
#define MIN_HEIGHT 50

typedef struct Node_s {
    char* elem;
    struct Node_s *left;
    struct Node_s *right;
} Node;

void freeTree(Node* root);
Node* insertChild(Node* child, char* elem);
Node* insertRight(Node* root, char* elem);
Node* insertLeft(Node* root, char* elem);
Node* insertRoot(char* elem);
Node* createNode();
void drawTree(Node* root, Node* newRoot, int x, int last_x, int tab, sfRenderWindow *window, sfFont* font);
void drawNode(sfRenderWindow* window, sfFont* font, int x1, int x2, int depth, char* str);
Node* initTree();
void makeCompleteBT(Node* root);
void makeCompleteTree_Fill(Node* root, Node* newroot, const int maxdepth);
void makeCompleteBTRecursive(Node* root, Node* newroot,  bool* isCompleted);
void makeProperBT(Node* root);
void makePerfectBT(Node* root);
void makePerfect(Node* root, Node* newroot, const int maxdepth);
int getNodeDepth(Node* root, Node* tofind);
int getMaxDepth(Node* root);


int main(int argc, char const *argv[])
{
    if(argc < 2) {
        printf("usage: main <type>=[complete|perfect|proper]"); 
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "complete") != 0 && strcmp(argv[1], "proper") != 0 && strcmp(argv[1], "perfect") != 0) {
        printf("usage: main <type>=[complete|perfect|proper]"); 
        return EXIT_FAILURE;
    }
    /// Transforming the n generic tree into a binary tree
    /// We are applying the following rules :
    ///     root stay the same
    ///     right sibing of a node become right child of the same node
    ///     left child stay the left child of the node
    Node* root = initTree();
    if(root == NULL) return EXIT_FAILURE;
    if(strcmp(argv[1], "complete") == 0) {
        makeCompleteBT(root);
    }
    else if(strcmp(argv[1], "proper") == 0) {
        makeProperBT(root);
    }
    else {
        makePerfectBT(root);
    }

    ///Create the graphic windows
    sfVideoMode mode = {1200, 900, 32};
    sfRenderWindow* window;
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
    if(!window) {
        printf("Failed to open graphic window!");
        return EXIT_FAILURE;
    }
    ///Setup the font to draw some text
    sfFont *font = sfFont_createFromFile("arial.ttf");
    if(!font) {
        printf("Failed to load font!");
        return EXIT_FAILURE;
    }

    sfEvent event;
    drawTree(root, root, SCREEN_CENTER, 0, 2,window, font);
    sfRenderWindow_display(window);

    while (sfRenderWindow_isOpen(window)) {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event)) {
            /* Close window : exit */
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
                break;             
            }   
        }
    }
    sfRenderWindow_destroy(window);
    freeTree(root);
    return 0;
}

/// @brief Function to create a node
/// @return A pointer toward the node created
Node* createNode() {
    Node* temp = malloc(sizeof(Node));
    if(temp == NULL) return NULL;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

/// @brief Function to create a new root for a tree
/// @param elem the value to assign to the root node
/// @return a pointer toward the root of the tree
Node* insertRoot(char* elem) {
    Node* newRoot = NULL;
    newRoot = createNode();
    if(newRoot == NULL) return NULL; /// ERROR to be handle in the calling function
    newRoot->elem = elem;
    return newRoot;
}

/// @brief Insert a node as the left child of a node
/// @param root the root/node where we want to insert a node
/// @param elem the value to assign to the new node creted
/// @return a pointer toward the root/node where we just insert a child
Node* insertLeft(Node* root, char* elem) {
    Node* leftChild = NULL;
    if(root == NULL) return NULL;
    leftChild = insertRoot(elem);
    if(leftChild == NULL) return NULL; ///ERROR to be handle in the calling function
    root->left = leftChild;
    return root;
}

/// @brief Insert a node as the right child of a node
/// @param root the root/node where we want to insert a node
/// @param elem the value to assign to the new node creted
/// @return a pointer toward the root/node where we just insert a child
Node* insertRight(Node* root, char* elem) {
    Node* rightChild = NULL;
    if(root == NULL) return NULL;
    rightChild = insertRoot(elem);
    if(rightChild == NULL) return NULL; ///ERROR to be handle in the calling function
    root->right = rightChild;
    return root;

}

/// @brief Function to insert a value to a children node
/// @param child the children
/// @param elem the value to assign to the child
/// @return a pointer toward the child node
Node* insertChild(Node* child, char* elem) {
    if(child == NULL) return NULL;
    child->elem = elem;
    return child;
}

/// @brief Function to free the memory of a binary tree.
/// @param root A pointer toward the root of a tree
void freeTree(Node* root) {
    if(root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int getMaxDepth(Node* root) {
    if(root == NULL) return 0;
    int l = getMaxDepth(root->left);
    int r = getMaxDepth(root->right);
    if(l > r) 
        return l + 1;
    return r + 1;
}

int getNodeDepth(Node* root, Node* tofind) {
    if(root == NULL) return -1;
    int dist = -1;
    if((root == tofind) 
        || (dist = getNodeDepth(root->left, tofind)) >=0
        || (dist = getNodeDepth(root->right, tofind)) >=0)
        return dist + 1;
    return dist;
}

void makePerfect(Node* root, Node* newroot, const int maxdepth) {
    if(newroot == NULL) return; 
    if(newroot->left == NULL && getNodeDepth(root, newroot) + 1 != maxdepth) {
        insertLeft(newroot, UNKNOWN_STRING);
    }
    if(newroot->right == NULL && getNodeDepth(root, newroot) + 1 != maxdepth) {
        insertRight(newroot, UNKNOWN_STRING);
    }
    makePerfect(root, newroot->left, maxdepth);
    makePerfect(root, newroot->right, maxdepth);
}

void makePerfectBT(Node* root) {
    makePerfect(root, root, getMaxDepth(root));
}

void makeProperBT(Node* root) {
    if(root == NULL) return;
    if(root->left == NULL && root->right == NULL) return;
    if(root->left == NULL) ///Only left is NULL
        insertLeft(root, UNKNOWN_STRING);
    if(root->right == NULL)
        insertRight(root, UNKNOWN_STRING);
    makeProperBT(root->left);
    makeProperBT(root->right);
}

void makeCompleteBTRecursive(Node* root, Node* newroot,  bool* isCompleted) {
    if(newroot == NULL) return;
    if(newroot->right != NULL && getNodeDepth(root, newroot) == getMaxDepth(root) - 2)
        *isCompleted = true;
    if(*isCompleted && getNodeDepth(root, newroot) != getMaxDepth(root) - 1) {
        if(newroot->left == NULL)
            insertLeft(newroot, UNKNOWN_STRING);
        if(newroot->right == NULL)
            insertRight(newroot, UNKNOWN_STRING);
    }
    makeCompleteBTRecursive(root, newroot->right, isCompleted);
    makeCompleteBTRecursive(root, newroot->left, isCompleted);
}

void makeCompleteTree_Fill(Node* root, Node* newroot, const int maxdepth) {
    if(newroot == NULL) return; 
    if(getNodeDepth(root, newroot) + 1 == maxdepth) return;
    if(newroot->left == NULL) {
        insertLeft(newroot, UNKNOWN_STRING);
    }
    if(newroot->right == NULL) {
        insertRight(newroot, UNKNOWN_STRING);
    }
    makeCompleteTree_Fill(root, newroot->left, maxdepth);
    makeCompleteTree_Fill(root, newroot->right, maxdepth);
}

void makeCompleteBT(Node* root) {
    bool isCompleted = false;
    int maxdepth = getMaxDepth(root) -1;
    makeCompleteTree_Fill(root, root, maxdepth);
    makeCompleteBTRecursive(root, root, &isCompleted);
}

Node* initTree() {
    Node* root = insertRoot("A");
    insertLeft(root, "B");
    insertLeft(root->left, "E");
    insertRight(root->left->left, "F");
    insertRight(root->left, "B'");
    insertLeft(root->left->right, "F'");
    insertRight(root->left->right, "C");
    insertLeft(root->left->right->right, "G");
    insertRight(root->left->right->right, "D");
    insertRight(root->left->right->right->left, "H");
    insertRight(root->left->right->right->left->right, "I");
    return root;
}

void drawNode(sfRenderWindow* window, sfFont* font, int x1, int x2, int depth, char* str) {
    int y = depth * HEIGHT_BETWEEN_NODE + MIN_HEIGHT;

    sfCircleShape *circle = sfCircleShape_create();
    sfVector2f circle_position = {x1, y};
    sfVector2f text_position = {x1 + 3, y};
    sfCircleShape_setFillColor(circle, sfWhite);
    sfCircleShape_setRadius(circle, 8.0f);
    sfCircleShape_setPosition(circle, circle_position);

    sfText* text = sfText_create();
    sfText_setString(text, str);
    sfText_setColor(text, sfRed);
    sfText_setFont(text, font);
    sfText_setPosition(text, text_position);
    sfText_setCharacterSize(text, 15);

    sfRenderWindow_drawCircleShape(window,circle, NULL);
    
    if(x2 != 0) {
        sfVertexArray *line = sfVertexArray_create();
        sfVertex p1 = {{x1+4,y}, sfWhite}, p2 = {{x2+4,y-HEIGHT_BETWEEN_NODE}, sfWhite};
        sfVertexArray_append(line, p1);
        sfVertexArray_append(line, p2);
        sfVertexArray_setPrimitiveType(line, sfLines);
        sfRenderWindow_drawVertexArray(window, line, NULL);
        sfVertexArray_destroy(line);
    }

    sfRenderWindow_drawText(window, text, NULL);
    sfCircleShape_destroy(circle);
    sfText_destroy(text);
}

void drawTree(Node* root, Node* newRoot, int x, int last_x, int tab, sfRenderWindow *window, sfFont* font){
    if(newRoot == NULL) return;
    int depth = getNodeDepth(root, newRoot);
    int change = SCREEN_CENTER/tab;
    drawNode(window, font, x, last_x, depth, newRoot->elem);
    drawTree(root, newRoot->left, x - change, x, tab * 2, window, font);
    drawTree(root, newRoot->right, x + change, x, tab * 2, window, font);
}