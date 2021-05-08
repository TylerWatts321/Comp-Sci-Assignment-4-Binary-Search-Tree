        /* COP 3502C Assignment 4
This program is written by: Gordon Tyler Watts */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define MAXLEN 30
FILE* infile;
FILE* outfile;

typedef struct itemNode{
    char name[MAXLEN];
    int count;   
    struct itemNode *left, *right; 
}itemNode;

typedef struct treeNameNode
{
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
}treeNameNode;

// Called by main();
void scan_info(); // Called by main. Scans the first three numbers, calls bst_creation. 
void begin_bst_creation(int, int, int); // Function responsible for creating BST.

// Functions called by begin_bst_creation();
//
    treeNameNode* createTreeNameNode();                         // Creates the individual name node.
    treeNameNode* buildNameTree();                              //  These two functions builds the name node tree by connecting
    treeNameNode* insertNameNode(treeNameNode*, treeNameNode*); //  the name nodes together and adding items to each name node. 
                                                                //  insertNameNode(); puts the node in the correct position.
    // Functions called by buildNameTree();
    //
        treeNameNode* searchNameNode(treeNameNode*, char[]);
        itemNode* insertItemNode(itemNode*, itemNode*);
    //

    void traverse_in_traverse(treeNameNode*);                   // Displays all the entire tree info.
    // Called by traverse_in_traverse();
    // 
        void display_NameNodes(treeNameNode*);
        void display_NameNodeChildren(treeNameNode*);
        void print_ChildrenInfo(itemNode*);
    //
    
    void handle_queries(treeNameNode*);                         // Reads-in the queries/requests of the input file.
    // List of query functions called by handle_queries();
    //
        void query_search(treeNameNode*);                       // When called, we search for a specific item in the tree.
                                                                // Display if the item can be found or not.
        // Called by query_search();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            itemNode* searchItemNode(itemNode*, char[]);        
        //
        void query_item_before(treeNameNode*);                  // When called, we determine how many nodes come before a given item node.
        // Called by query_item_before();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            int count_item_before(itemNode*, char[]);
        //
        void query_height_balance(treeNameNode*);               // When called, we get the height of the left and right subtree of the name node,
                                                                // and whether or not it is balanced. 
        // Called by query_height_balance();
        //
            int get_height(itemNode*);
            int get_difference(int, int);
            void determine_balance(int);
        //
        void query_count(treeNameNode*);                        // When called, we retrieve the total number of items in that name node,
                                                                // and display it.
        // Called by query_count();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            int get_tree_sum(itemNode*);
        //
        void query_reduce(treeNameNode*);                       // When called, we modify the number of items in that item node by a specific number
                                                                // read-in by out.txt.
        // Called by query_reduce();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            itemNode* searchItemNode(itemNode*, char[]);
            void reduceItemNode(itemNode*, int);
        //
        void query_delete(treeNameNode*);                       // When called, we delete an item node from the BST.
        // Called by query_delete();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            itemNode* searchItemNode(itemNode*, char[]);
            itemNode* delete(itemNode*, itemNode*);
        //
        void query_delete_name(treeNameNode*);                  // When called, we delete a name node, and its children from the BST.
        // Called by query_delete_name();
        //
            treeNameNode* searchNameNode(treeNameNode*, char[]);
            treeNameNode* delete_name_node(treeNameNode*, treeNameNode*);
            void deleteTree(itemNode*);
        //
    //

    void delete_BST(treeNameNode*);                             // Responsible for deleting all the nodes, thereby freeing up our memory.
    // Functions called by delete_BST();\
    //
        treeNameNode* findLeaf(treeNameNode*);
        treeNameNode* delete_name_node(treeNameNode*, treeNameNode*);
        itemNode* finditemLeaf(itemNode*);
        itemNode* delete(itemNode*, itemNode*); 
    //
//

// Extraneous functions that are generally used in BST's.
itemNode* parent(itemNode*, itemNode*);
itemNode* minVal(itemNode*);
int isLeaf(itemNode*);
int hasOnlyRightChild(itemNode*);
int hasOnlyLeftChild(itemNode*);

treeNameNode* name_parent(treeNameNode*, treeNameNode*);
treeNameNode* name_minVal(treeNameNode*);
int name_isLeaf(treeNameNode*);
int name_hasOnlyRightChild(treeNameNode*);
int name_hasOnlyLeftChild(treeNameNode*);

int main()
{
    atexit(report_mem_leak);
    infile = fopen("in.txt", "r");
    outfile = fopen("out.txt", "w");
    scan_info(); // Scans info and starts program.
    fclose(outfile);
    fclose(infile);
}

void scan_info()
{
    int treeCount, numItems, numQueries;
    fscanf(infile, "%d", &treeCount); // "N" value
    fscanf(infile, "%d", &numItems); // "I" value
    fscanf(infile, "%d", &numQueries); // "Q" value
    begin_bst_creation(treeCount, numItems, numQueries); // Starts main program.
}

void begin_bst_creation(int N, int I, int Q)
{
    treeNameNode* NameNode;
    treeNameNode* root = NULL;


    for (unsigned int i = 0; i < N; i++) // For "N" name nodes...
    {
        NameNode = createTreeNameNode(); //Generate a name node.
        root = insertNameNode(root, NameNode); //Insert it to the BST.
    }
    
    for (unsigned int i = 0; i < I; i++) // For "I" item nodes...
    {
        buildNameTree(root); // Generate an item node, insert it to its respective name node.
    }

    traverse_in_traverse(root); // Print the info of the whole BST.

    for (unsigned int i = 0; i < Q; i++) // For "Q" queries.
    {
        handle_queries(root); // Pick a query task to do, and execute it.
    }
    
    fprintf(outfile, "\n");
    delete_BST(root); // Free the memory allocated for the binary search tree.
}

treeNameNode* createTreeNameNode()
{
    treeNameNode* newNode = (treeNameNode*)malloc(sizeof(treeNameNode));
    fscanf(infile, "%s", newNode->treeName);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->theTree = NULL;

    return newNode;
}

treeNameNode* insertNameNode(treeNameNode* curNode, treeNameNode* tempNode)
{
    // If there is not a curent node.
    if(curNode == NULL)
    {
        // Set it as the root.
        return tempNode;
    }
    else
    {
        // If our inserted node is larger than our root...
        if(strcmp(curNode->treeName, tempNode->treeName) < 0)
        {
            if(curNode->right != NULL) // If there is a node already to the right.
                curNode->right = insertNameNode(curNode->right, tempNode); // Insert the node somewhere to the right.
            
            else
                curNode->right = tempNode; // Otherwise, make the immediate right the inserted node.
        }
        // If our inserted node is NOT larger than our root...
        else
        {
            if(curNode->left != NULL) // If there is a node already to the left.
                curNode->left = insertNameNode(curNode->left, tempNode); // Insert the node somewhere to the left.
            
            else
                curNode->left = tempNode; // Otherwise, make the immediate left the inserted node.
        }
    }

    return curNode; // In other extraneous cases, return our inserted node.
}

treeNameNode* searchNameNode(treeNameNode *root, char treeName[50])
{
    if (root != NULL) // If our current root exists.
    {
        // If our current root is not the node we are searching for...
        if(strcmp(root->treeName, treeName) != 0)
        {
            // Move to the right if it is larger than our root.
            if(strcmp(root->treeName, treeName) < 0)
                return searchNameNode(root->right, treeName);
            // Move to the left if it is smaller than our root.
            if(strcmp(root->treeName, treeName) > 0)
                return searchNameNode(root->left, treeName);
        }
        
        // If it is the node we want, return it.
        else
            return root;   
    }
    // Extraneous cases.
    else
        return NULL;
}

treeNameNode* buildNameTree(treeNameNode* realRoot)
{
    char treeName[50];
    treeNameNode* root;

    fscanf(infile, "%s", treeName);
    root = searchNameNode(realRoot, treeName); // Search for the node we want, set it to the root.
    
    // Malloc our node.
    itemNode* curItemNode = (itemNode*)malloc(sizeof(itemNode));
    curItemNode->left = NULL;
    curItemNode->right = NULL;

    fscanf(infile, "%s", curItemNode->name);
    fscanf(infile, "%d", &curItemNode->count);

    // Set the root of the item tree equal to the return of insertItemNode.
    root->theTree = insertItemNode(curItemNode, root->theTree);

    return realRoot;
}

itemNode* insertItemNode(itemNode* item, itemNode* root)
{
    // If our current root pointer DNE, return our item.
    if (root == NULL)
        return(item);

    else
    {
        // If our item is greater than the root, insert somewhere to the right.
        if(strcmp(root->name, item->name) < 0)
        {
            // Insert somewhere to the right.
            if(root->right != NULL)
                root->right = insertItemNode(item, root->right);
            // Insert immediately to the right if there is nothing already there.
            else
                root->right = item;
        }
        // If our item is less than the root, insert somewhere to the left.
        else
        {
            // Insert somewhere to the left.
            if(root->left != NULL)
                root->left = insertItemNode(item, root->left);
            // Insert immediately to the left if there is nothing already there.
            else
                root->left = item;
        }
        return root;
    }
}

void traverse_in_traverse(treeNameNode* root)
{
    display_NameNodes(root); // Displays each name node.
    display_NameNodeChildren(root); // Displays children.
}

void display_NameNodes(treeNameNode* root)
{
    if(root != NULL)
    {
        display_NameNodes(root->left);
        fprintf(outfile, "%s ", root->treeName);
        display_NameNodes(root->right);
    }
}

void display_NameNodeChildren(treeNameNode* root)
{
    
    if(root != NULL)
    {
        display_NameNodeChildren(root->left);
        fprintf(outfile, "\n===%s===\n", root->treeName);
        print_ChildrenInfo(root->theTree);
        display_NameNodeChildren(root->right);
    }
}

void print_ChildrenInfo(itemNode* root)
{
    if(root != NULL)
    {
        print_ChildrenInfo(root->left);
        fprintf(outfile, "%s ", root->name);
        print_ChildrenInfo(root->right);
    }
}

void handle_queries(treeNameNode* root)
{
    char request[50];
    fscanf(infile, "%s ", request);

    if (strcmp(request, "search") == 0)
        query_search(root);//search function call
    else if (strcmp(request, "item_before") == 0)
        query_item_before(root);//item_before function call
    else if (strcmp(request, "height_balance") == 0)
        query_height_balance(root);//height_balance function call
    else if (strcmp(request, "count") == 0)
        query_count(root);//count function call
    else if (strcmp(request, "reduce") == 0)
        query_reduce(root);//reduce function call
    else if (strcmp(request, "delete") == 0)
        query_delete(root);//delete function call
    else if (strcmp(request, "delete_name") == 0)
        query_delete_name(root);//delete name tree
    
}

void query_search(treeNameNode* root)
{
    char treeName[50];
    char itemName[50];

    fscanf(infile, "%s", treeName);
    fscanf(infile, "%s", itemName);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Retrieve a temp name node root.
    if (tempRoot == NULL)
    {
        fprintf(outfile, "\n%s does not exist", treeName);
        return;
    }

    itemNode* itemNode = searchItemNode(tempRoot->theTree, itemName); // Retrieve the item we are searching for.

    if (itemNode != NULL)
        fprintf(outfile, "\n%d %s found in %s", itemNode->count, itemNode->name, tempRoot->treeName);
    else
        fprintf(outfile, "\n%s not found in %s", itemName, tempRoot->treeName);
    
}

// Same format as searchItemNode();, just uses itemNode pointer.
itemNode* searchItemNode(itemNode *root, char itemName[50])
{
    if (root != NULL)
    {
        if(strcmp(root->name, itemName) != 0)
        {
            if(strcmp(root->name, itemName) < 0)
                return searchItemNode(root->right, itemName);

            if(strcmp(root->name, itemName) > 0)
                return searchItemNode(root->left, itemName);
        }
        else
            return root;
    }
    else
    {
        return NULL;
    }
}

void query_item_before(treeNameNode* root)
{
    char treeName[50];
    char itemName[50];

    fscanf(infile, "%s", treeName);
    fscanf(infile, "%s", itemName);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Search for the node we want to count for.

    int counter = count_item_before(tempRoot->theTree, itemName); // We add one here to count the root node of the tree.
    fprintf(outfile, "\nitem before %s: %d", itemName, counter);
}

int count_item_before(itemNode *root, char itemName[50])
{
    // If the root exists.
    if(root != NULL)
    {
        // For all the nodes less than our selected node, count each node.
        if(strcmp(root->name, itemName) < 0)
        {
            
            return count_item_before(root->right, itemName) + count_item_before(root->left, itemName) + 1;
        }
            
        else
            return count_item_before(root->right, itemName) + count_item_before(root->left, itemName);
    }
    else
        return 0;
}

void query_height_balance(treeNameNode* root)
{
    char treeName[50];
    int left_height, right_height, difference = 0;

    fscanf(infile, "%s", treeName);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Search for the name node we want the height / balance of.
    left_height = get_height(tempRoot->theTree->left); // Gets the height of the left subtree.
    right_height = get_height(tempRoot->theTree->right); // Gets the height of the right subtree.
    difference = get_difference(left_height, right_height); // Get the difference.

    fprintf(outfile, "\n%s: left height %d, right height %d, difference %d,", tempRoot->treeName, left_height, right_height, difference);
    determine_balance(difference); // Determine the balance of the tree.
}

int get_height(itemNode* root)
{
    if(root == NULL)
    {
        return -1;
    }
    else
    {
        int left_height = get_height(root->left) + 1; // Move to the left; count the node we are currently on.
        int right_height = get_height(root->right) + 1; // Move to the right; count the ndoe we are currently on.
        
        if(right_height > left_height)  // Return whatever subtree has the larger value.
            return right_height;
        else
            return left_height;
    }   
}

int get_difference(int left_height, int right_height)
{
    // Return the absolute value of the difference.
    return abs(left_height - right_height);
}

void determine_balance(int difference)
{
    // Determine balance, print corresponding statement.
    if (difference > 1)
        fprintf(outfile, " balanced");
    else
        fprintf(outfile, " not balanced");
}

void query_count(treeNameNode* root)
{
    char treeName[50];
    int sumTotal;

    fscanf(infile, "%s", treeName);

    treeNameNode* tempRoot = searchNameNode(root, treeName);  // Get the name node we want to get the count of.
    sumTotal = get_tree_sum(tempRoot->theTree); // Retrieve the sum total of the name node.

    fprintf(outfile, "\n%s count %d", tempRoot->treeName, sumTotal);
}

int get_tree_sum(itemNode* root)
{
    if (root != NULL)
    {
        // Accumulate sum of the right subtree, the left, and the current node's count.
        return get_tree_sum(root->right) + get_tree_sum(root->left) + root->count;
    }  
    else
        return 0;
}

void query_reduce(treeNameNode* root)
{
    char treeName[50], itemName[50];
    int reduce_num;

    fscanf(infile, "%s", treeName);
    fscanf(infile, "%s", itemName);
    fscanf(infile, "%d", &reduce_num);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Find the name node root that houses the item we want.
    itemNode* tempItemNode = searchItemNode(tempRoot->theTree, itemName); // Find the item.

    reduceItemNode(tempItemNode, reduce_num); // Subtract the count of the node by the number we have read in.

    
    fprintf(outfile, "\n%s reduced", tempItemNode->name);
}

void reduceItemNode(itemNode* itemNode, int reduce_num)
{
    itemNode->count = itemNode->count - reduce_num;
}

void query_delete(treeNameNode* root)
{
    char treeName[50], itemName[50];

    fscanf(infile, "%s", treeName);
    fscanf(infile, "%s", itemName);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Find the name node root that houses the item we want.
    itemNode* tempItemNode = searchItemNode(tempRoot->theTree, itemName); // Find the item.

    tempRoot->theTree = delete(tempRoot->theTree, tempItemNode); // Delete that item.
    fprintf(outfile, "\n%s deleted from %s", itemName, tempRoot->treeName);
}

void query_delete_name(treeNameNode* root)
{
    char treeName[50];
    fscanf(infile, "%s", treeName);

    treeNameNode* tempRoot = searchNameNode(root, treeName); // Retrieve the name node we want to eliminate.

    deleteTree(tempRoot->theTree); // Delete the name subtree.
    tempRoot->theTree = NULL;
    root = delete_name_node(root, tempRoot); // Delete the name node.
    fprintf(outfile, "\n%s deleted", treeName);
}

itemNode* delete(itemNode* root, itemNode* deleteNode)
{
    itemNode* par = parent(root, deleteNode); // retrieve the parent of our node.
    itemNode* new_del_node;
    itemNode* save_node;

    if (isLeaf(deleteNode)) // If our node we want to delete is a leaf,
    {
        if(par == NULL) // If it has no parent. It is a lone node.
        {
            free(root); // Simply delete it.
            return NULL;
        }
        // If the node we want to delete is to the left of its parent..
        if(strcmp(par->name, deleteNode->name) > 0)
        {
            free(par->left); // Delete it.
            par->left = NULL; // Set the parents left to NULL.
        }
        else
        {
            free(par->right);// Delete it.
            par->right = NULL; // Set the parents right to NULL
        }

        return root;
    }

    if(hasOnlyLeftChild(deleteNode)) // If our node has a left child.
    {
        if(par == NULL) // If it has no parent.
        {
            save_node = deleteNode->left; // Save the child.
            free(deleteNode); // Delete our current node.
            return save_node; // Make the child our new root.
        }
        if(strcmp(par->name, deleteNode->name) > 0)
        {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }
        else
        {
            save_node = par->right; // Save the node to delete.
            par->right = par->right->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }
        
        return root;
    }

    if(hasOnlyRightChild(deleteNode))
    {
        if(par == NULL) // If it has no parent...
        {
            save_node = deleteNode->right;  // Save the child.
            free(deleteNode); // Delete our current node.
            return save_node; // Make the child our new root.
        }
        if(strcmp(par->name, deleteNode->name) > 0)
        {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->right; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }
        else
        {
            save_node = par->right; // Save the node to delete.
            par->right = par->right->right; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }

        return root;
    }

    //if your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = minVal(deleteNode->right);
    char new_name[MAXLEN];
    strcpy(new_name, new_del_node->name);
    int new_count = new_del_node->count;
    delete(parent(root, new_del_node), new_del_node);  // Now, delete the proper value.

    // Restore the data to the original node to be deleted.
    strcpy(deleteNode->name, new_name);
    deleteNode->count = new_count;

    return root;
}

void deleteTree(itemNode* root)
{
    while (root != NULL)
    {
        itemNode* delNode = finditemLeaf(root);
        root = delete(root, delNode);
    }
}

 // Same as delete, however now it applies to the name BST.
treeNameNode* delete_name_node(treeNameNode* root, treeNameNode* deleteNode)
{
    treeNameNode* par = name_parent(root, deleteNode);
    treeNameNode* new_del_node;
    treeNameNode* save_node;

    if (name_isLeaf(deleteNode))
    {
        if(par == NULL)
        {
            free(root);
            return NULL;
        }
        if(strcmp(par->treeName, deleteNode->treeName) > 0)
        {
            free(root->left);
            par->left = NULL;
        }
        else
        {
            free(par->right);
            par->right = NULL;
        }

        return root;
    }

    if(name_hasOnlyLeftChild(deleteNode))
    {
        if(par == NULL)
        {
            save_node = deleteNode->left;
            free(deleteNode);
            return save_node;
        }
        if(strcmp(par->treeName, deleteNode->treeName) > 0)
        {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }
        else
        {
            save_node = par->right; // Save the node to delete.
            par->right = par->right->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }
        
        return root;
    }

    if(name_hasOnlyRightChild(deleteNode))
    {
        if(par == NULL)
        {
            save_node = deleteNode->right;
            free(deleteNode);
            return save_node;
        }
        if(strcmp(par->treeName, deleteNode->treeName) > 0)
        {
            save_node = par->left;
            par->left = par->left->right;
            free(save_node);
        }
        else
        {
            save_node = par->right;
            par->right = par->right->right;
            free(save_node);
        }

        return root;
    }

    // If your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = name_minVal(deleteNode->right);
    char new_name[MAXLEN];
    strcpy(new_name, new_del_node->treeName);
    delete_name_node(name_parent(root, new_del_node), new_del_node);  // Now, delete the proper value.

    // Restore the data to the original node to be deleted.
    strcpy(deleteNode->treeName, new_name);

    return root;
}

void delete_BST(treeNameNode* root)
{
    while(root != NULL)
    {
        treeNameNode* delNode = findLeaf(root); // Retrieve a leaf node of the name tree.
        deleteTree(delNode->theTree); // Delete all the items in the name node.
        root = delete_name_node(root, delNode); // Delete the name node from the name tree.
    }
}

treeNameNode* findLeaf(treeNameNode* root) // Finds a leaf node in the name tree.
{
    if (root != NULL)
    {
        if (name_isLeaf(root))
        {
            return root;
        }
        else
        {
            if(root->left != NULL)
            {
                findLeaf(root->left);
            }
            else
            {
                findLeaf(root->right);
            }
        }
    }
    else
    {
        return NULL;
    }
    
}

itemNode* finditemLeaf(itemNode* root) // Same as the function above, just used for item nodes.
{
    if (root != NULL)
    {
        if (isLeaf(root))
        {
            return root;
        }
        else
        {
            if(root->left != NULL)
            {
                finditemLeaf(root->left);
            }
            else
            {
                finditemLeaf(root->right);
            }
        }
    }
    else
    {
        return NULL;
    }
}

itemNode* parent(itemNode* root, itemNode* node) 
{
  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (strcmp(root->name, node->name) > 0)
    return parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (strcmp(root->name, node->name) < 0);
    return parent(root->right, node);

  return NULL; // Catch any other extraneous cases.
}

treeNameNode* name_parent(treeNameNode* root, treeNameNode* node) 
{
  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (strcmp(root->treeName, node->treeName) > 0)
    return name_parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (strcmp(root->treeName, node->treeName) < 0);
    return name_parent(root->right, node);

  return NULL; // Catch any other extraneous cases.
}

int name_isLeaf(treeNameNode* node)
{
    return (node->right == NULL && node->left == NULL);
}
int isLeaf(itemNode* node)
{
    return (node->right == NULL && node->left == NULL);
}

int name_hasOnlyLeftChild(treeNameNode* node)
{
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyLeftChild(itemNode* node)
{
    return (node->left != NULL && node->right == NULL);
}

int name_hasOnlyRightChild(treeNameNode* node)
{
    return (node->right != NULL && node->left == NULL);
}

int hasOnlyRightChild(itemNode* node)
{
    return (node->right != NULL && node->left == NULL);
}

itemNode* minVal(itemNode *root) {

  // Root stores the minimal value.
  if (root->left == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else
    return minVal(root->left);
}

treeNameNode* name_minVal(treeNameNode *root) 
{
  // Root stores the minimal value.
  if (root->left == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else
    return name_minVal(root->left);
}