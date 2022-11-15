/**
 * @file main.c
 * @author Thomas DUCLOS
 * @studentid 202232327
 * @brief This program transform a n binary tree into a binary tree and then transform this binary tree into either a complete, proper or perfect binary tree
 * @version 0.1
 * @date 2022-11-15
 * @copyright Copyright (c) 2022
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "SFML/Window.h"
#include "SFML/Graphics.h"

#define UNKNOWN_STRING "?" ///String to represent the random data inside an added node
#define SCREEN_CENTER 600  ///Center of the graphic window
#define HEIGHT_BETWEEN_NODE 100 ///Y space between each node
#define MIN_HEIGHT 50 ///Y offset

/// @brief This is a structure to represent the node of a binary tree
typedef struct Node_s {
    char* elem; ///Content of the node
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
    /// Check the command line parameters validity to generate the correct binary tree
    if(argc < 2) {
        printf("usage: main <type>=[complete|perfect|proper]"); 
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "complete") != 0 && strcmp(argv[1], "proper") != 0 && strcmp(argv[1], "perfect") != 0) {
        printf("usage: main <type>=[complete|perfect|proper]"); 
        return EXIT_FAILURE;
    }
    Node* root = initTree(); ///Initialisation of the binary tree
    if(root == NULL) return EXIT_FAILURE; ///Handle failure

    /// Create the correct binary tree asked on the command line
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
    if(!window) { ///Handle window creation failure
        printf("Failed to open graphic window!");
        return EXIT_FAILURE;
    }
    ///Setup the font to draw some text 
    sfFont *font = sfFont_createFromFile("arial.ttf"); ///Font to write the tree content
    if(!font) { ///Handle font loading failure
        printf("Failed to load font!");
        return EXIT_FAILURE;
    }

    sfEvent event; ///Will allow us to close the rendering window later
    drawTree(root, root, SCREEN_CENTER, 0, 2,window, font); ///Draw the screen in the rendering window
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
    ///Free all the allocated data
    sfRenderWindow_destroy(window);
    sfFont_destroy(font);
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

/// @brief Function to get the maximum deoth of a tree recursively
/// @param root A pointer toward the root of a tree
/// @return the maximum depth of the tree
int getMaxDepth(Node* root) {
    if(root == NULL) return 0;
    int l = getMaxDepth(root->left);
    int r = getMaxDepth(root->right);
    if(l > r) 
        return l + 1;
    return r + 1;
}

/// @brief Function to get the depth of a given node
/// @param root the root of the tree
/// @param tofind the node we need to find inside the tree
/// @return the depth of the node
int getNodeDepth(Node* root, Node* tofind) {
    if(root == NULL) return -1;
    int dist = -1;
    if((root == tofind) 
        || (dist = getNodeDepth(root->left, tofind)) >=0
        || (dist = getNodeDepth(root->right, tofind)) >=0)
        return dist + 1;
    return dist;
}

/// @brief Function to transform a random binary tree into a perfect one
/// @param root the root of the tree. It will never be modified
/// @param newroot the newroot of the tree which is modified at each recursive call
/// @param maxdepth the max depth of the tree.
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

/// @brief Helper function to make perfect binary tree with less parameters
/// @param root the root of the binary tree
void makePerfectBT(Node* root) {
    makePerfect(root, root, getMaxDepth(root));
}

/// @brief Function to make a proper binary tree
/// @param root the root of the binary tree
void makeProperBT(Node* root) {
    if(root == NULL) return;
    if(root->left == NULL && root->right == NULL) return;
    if(root->left == NULL)
        insertLeft(root, UNKNOWN_STRING);
    if(root->right == NULL)
        insertRight(root, UNKNOWN_STRING);
    makeProperBT(root->left);
    makeProperBT(root->right);
}

/// @brief Function to make a complete binary tree
/// @param root the root of the tree.
/// @param newroot the newroot of the tree which is modified at each recursive call
/// @param isCompleted boolean to test if we need to add children on the last line
void makeCompleteBTRecursive(Node* root, Node* newroot,  bool* isCompleted) {
    if(newroot == NULL) return;
    /// Check to see if the children are the most left possible
    if((newroot->right != NULL || newroot->left) && getNodeDepth(root, newroot) == getMaxDepth(root) - 2)
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

/// @brief Function to fill the complete tree to depth - 2
/// @param root the root of the tree
/// @param newroot the newroot of the tree which is modified at each recursive call
/// @param maxdepth the max depth of the tree
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

/// @brief Helper function to reduce the number of parameter to make a complete binary tree
/// @param root the root of the tree
void makeCompleteBT(Node* root) {
    bool isCompleted = false;
    int maxdepth = getMaxDepth(root) -1;
    makeCompleteTree_Fill(root, root, maxdepth);
    makeCompleteBTRecursive(root, root, &isCompleted);
}

/// @brief Function to initialise the binary tree with the n generic tree.
/// @brief We follow the following rules:
/// @brief - The root of the Binary Tree is the Root of the Generic Tree.
/// @brief - The left child of a node in the Generic Tree is the Left child of that node in the Binary Tree.
/// @brief - The right sibling of any node in the Generic Tree is the Right child of that node in the Binary Tree.
/// @return the root of the binary tree
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

/// @brief Function to draw a node of the tree to the rendering window
/// @param window the csfml rendering window
/// @param font the csfml font
/// @param x1 the position of the child circle
/// @param x2 the position of the parent circle
/// @param depth the depth of the node
/// @param str the content of the node
void drawNode(sfRenderWindow* window, sfFont* font, int x1, int x2, int depth, char* str) {
    int y = depth * HEIGHT_BETWEEN_NODE + MIN_HEIGHT;

    /// Init the Circle
    sfCircleShape *circle = sfCircleShape_create();
    sfVector2f circle_position = {x1, y};
    sfVector2f text_position = {x1 + 3, y};
    sfCircleShape_setFillColor(circle, sfWhite);
    sfCircleShape_setRadius(circle, 8.0f);
    sfCircleShape_setPosition(circle, circle_position);
    /// Init the text
    sfText* text = sfText_create();
    sfText_setString(text, str);
    sfText_setColor(text, sfRed);
    sfText_setFont(text, font);
    sfText_setPosition(text, text_position);
    sfText_setCharacterSize(text, 15);

    sfRenderWindow_drawCircleShape(window,circle, NULL); ///Draw the circle
    
    if(x2 != 0) { ///test if it is not the first node
        sfVertexArray *line = sfVertexArray_create(); ///Init the line
        sfVertex p1 = {{x1+4,y}, sfWhite}, p2 = {{x2+4,y-HEIGHT_BETWEEN_NODE}, sfWhite};
        sfVertexArray_append(line, p1);
        sfVertexArray_append(line, p2);
        sfVertexArray_setPrimitiveType(line, sfLines);
        sfRenderWindow_drawVertexArray(window, line, NULL); ///Draw the line
        sfVertexArray_destroy(line);
    }

    sfRenderWindow_drawText(window, text, NULL); /// Draw the text

    sfCircleShape_destroy(circle);
    sfText_destroy(text);
}

/// @brief Recursive functiion to draw a binary tree
/// @param root the root of the tree
/// @param newRoot the newroot of the tree which is modified at each recursive call 
/// @param x the x position of the node
/// @param last_x the x position of the parent node
/// @param tab the number of time we need to divise the distance
/// @param window the rendering window
/// @param font the font used to print the text
void drawTree(Node* root, Node* newRoot, int x, int last_x, int tab, sfRenderWindow *window, sfFont* font){
    if(newRoot == NULL) return;
    int depth = getNodeDepth(root, newRoot);
    int change = SCREEN_CENTER/tab;
    drawNode(window, font, x, last_x, depth, newRoot->elem);
    drawTree(root, newRoot->left, x - change, x, tab * 2, window, font);
    drawTree(root, newRoot->right, x + change, x, tab * 2, window, font);
}