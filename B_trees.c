#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ORDER 5
typedef struct BTreeNode {
int keys[ORDER - 1];
struct BTreeNode *children[ORDER];
int numKeys;
bool isLeaf;
} BTreeNode;
BTreeNode *createNode(bool isLeaf) {
BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
newNode->isLeaf = isLeaf;
newNode->numKeys = 0;
for (int i = 0; i < ORDER; i++)
newNode->children[i] = NULL;
return newNode;
}
void splitChild(BTreeNode *parent, int index, BTreeNode *child) {
BTreeNode *newChild = createNode(child->isLeaf);
newChild->numKeys = ORDER / 2 - 1;
for (int i = 0; i < ORDER / 2 - 1; i++)
newChild->keys[i] = child->keys[i + ORDER / 2];
if (!child->isLeaf) {
for (int i = 0; i < ORDER / 2; i++)
newChild->children[i] = child->children[i + ORDER / 2];
}
child->numKeys = ORDER / 2 - 1;
for (int i = parent->numKeys; i >= index + 1; i--)
parent->children[i + 1] = parent->children[i];
parent->children[index + 1] = newChild;
for (int i = parent->numKeys - 1; i >= index; i--)
parent->keys[i + 1] = parent->keys[i];
parent->keys[index] = child->keys[ORDER / 2 - 1];
parent->numKeys++;
}
void insertNonFull(BTreeNode *node, int key) {
int i = node->numKeys - 1;
if (node->isLeaf) {
while (i >= 0 && key < node->keys[i]) {
node->keys[i + 1] = node->keys[i];
i--;
}
node->keys[i + 1] = key;
node->numKeys++;
} else {
while (i >= 0 && key < node->keys[i])
i--;
i++;
if (node->children[i]->numKeys == ORDER - 1) {
splitChild(node, i, node->children[i]);
if (key > node->keys[i])
i++;
}
insertNonFull(node->children[i], key);
}
}
void insert(BTreeNode **root, int key) {
if ((*root)->numKeys == ORDER - 1) {
BTreeNode *newRoot = createNode(false);
newRoot->children[0] = *root;
splitChild(newRoot, 0, *root);
int i = 0;
if (newRoot->keys[0] < key)
i++;
insertNonFull(newRoot->children[i], key);
*root = newRoot;
} else {
insertNonFull(*root, key);
}
}
void traverse(BTreeNode *root) {
int i;
for (i = 0; i < root->numKeys; i++) {
if (!root->isLeaf)
traverse(root->children[i]);
printf("%d ", root->keys[i]);
}
if (!root->isLeaf)
traverse(root->children[i]);
}
BTreeNode *search(BTreeNode *root, int key) {
int i = 0;
while (i < root->numKeys && key > root->keys[i])
i++;
if (i < root->numKeys && key == root->keys[i])
return root;
if (root->isLeaf)
return NULL;
return search(root->children[i], key);
}
// Utility function to get the predecessor of the key
int getPredecessor(BTreeNode *node, int idx) {
BTreeNode *current = node->children[idx];
while (!current->isLeaf)
current = current->children[current->numKeys];
return current->keys[current->numKeys - 1];
}
// Utility function to get the successor of the key
int getSuccessor(BTreeNode *node, int idx) {
BTreeNode *current = node->children[idx + 1];
while (!current->isLeaf)
current = current->children[0];
return current->keys[0];
}
// Merge child[idx] with child[idx + 1]
void merge(BTreeNode *node, int idx) {
BTreeNode *child = node->children[idx];
BTreeNode *sibling = node->children[idx + 1];
child->keys[ORDER / 2 - 1] = node->keys[idx];
for (int i = 0; i < sibling->numKeys; i++)
child->keys[i + ORDER / 2] = sibling->keys[i];
if (!child->isLeaf) {
for (int i = 0; i <= sibling->numKeys; i++)
child->children[i + ORDER / 2] = sibling->children[i];
}
for (int i = idx + 1; i < node->numKeys; i++)
node->keys[i - 1] = node->keys[i];
for (int i = idx + 2; i <= node->numKeys; i++)
node->children[i - 1] = node->children[i];
child->numKeys += sibling->numKeys + 1;
node->numKeys--;
free(sibling);
}
// Remove the key from the node
void removeFromNode(BTreeNode *node, int key) {
int idx = 0;
while (idx < node->numKeys && node->keys[idx] < key)
idx++;
if (idx < node->numKeys && node->keys[idx] == key) {
if (node->isLeaf) {
for (int i = idx + 1; i < node->numKeys; i++)
node->keys[i - 1] = node->keys[i];
node->numKeys--;
} else {
if (node->children[idx]->numKeys >= ORDER / 2) {
int pred = getPredecessor(node, idx);
node->keys[idx] = pred;
removeFromNode(node->children[idx], pred);
} else if (node->children[idx + 1]->numKeys >= ORDER / 2) {
int succ = getSuccessor(node, idx);
node->keys[idx] = succ;
removeFromNode(node->children[idx + 1], succ);
} else {
merge(node, idx);
removeFromNode(node->children[idx], key);
}
}
} else {
if (node->isLeaf) {
printf("The key %d does not exist in the tree.\n", key);
return;
}
bool flag = (idx == node->numKeys);
if (node->children[idx]->numKeys < ORDER / 2) {
if (idx != 0 && node->children[idx - 1]->numKeys >= ORDER / 2)
merge(node, idx - 1);
else if (idx != node->numKeys && node->children[idx + 1]->numKeys >= ORDER / 2)
merge(node, idx);
else {
if (idx != node->numKeys)
merge(node, idx);
else
merge(node, idx - 1);
}
}
if (flag && idx > node->numKeys)
removeFromNode(node->children[idx - 1], key);
else
removeFromNode(node->children[idx], key);
}
}
void deleteKey(BTreeNode **root, int key) {
if (*root == NULL) {
printf("The tree is empty.\n");
return;
}
removeFromNode(*root, key);
if ((*root)->numKeys == 0) {
BTreeNode *tmp = *root;
if ((*root)->isLeaf)
*root = NULL;
else
*root = (*root)->children[0];
free(tmp);
}
}
int main() {
BTreeNode *root = createNode(true);
int choice, key;
printf("244g5a0511\n");
while (1) {
printf("\n1. Insert\n2. Search\n3. Delete\n4. Traverse\n5. Exit\nEnter your choice: ");
scanf("%d", &choice);
switch (choice) {
case 1:
printf("Enter key to insert: ");
scanf("%d", &key);
insert(&root, key);
break;
case 2:
printf("Enter key to search: ");
scanf("%d", &key);
if (search(root, key) != NULL)
printf("Element %d found in the B-Tree.\n", key);
else
printf("Element %d not found in the B-Tree.\n", key);
break;
case 3:
printf("Enter key to delete: ");
scanf("%d", &key);
deleteKey(&root, key);
break;
case 4:
printf("Traversal of the B-Tree:\n");
traverse(root);
printf("\n");
break;
case 5:
exit(0);
default:
printf("Invalid choice!\n");
}
}
return 0;
}
