# Binary tree

This project has for goal to transform a n generic tree into a binary tree. Then we transform again this binary tree to make it proper, perfect and complete.

The generic tree is the following :
![n generic tree](/assets/n_generic_tree.png)

## Rules to transform n tree into binary tree
To transform the n generic tree into a binary tree we use the following set of rules :
- the root of the Binary Tree is the Root of the Generic Tree.
- the left child of a node in the Generic Tree is the Left child of that node in the Binary Tree.
- the right sibling of any node in the Generic Tree is the Right child of that node in the Binary Tree.

## Definitions of binary tree
### Perfect 
A perfect binary tree is a binary tree in which all interior nodes have two children and all leaves have the same depth or same level.
### Complete
A complete binary tree is a binary tree in which every level, except possibly the last, is completely filled, and all nodes in the last level are as far left as possible.
### Full
A full binary tree (sometimes referred to as a proper or plane or strict binary tree) is a tree in which every node has either 0 or 2 children.

## Results
### Perfect
![result for the perfect binary tree](/assets/perfect_bt.png)
### Complete
![result for the perfect binary tree](/assets/complete_bt.png)
### Full
![result for the perfect binary tree](/assets/proper_bt.png)

## How to use the project
### Flags for CSFML
#### Libraries
```
-lcsfml-system -lcsfml-window -lcsfml-graphics
```