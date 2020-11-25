#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

/*
* Nodo bat bilatzeko funtzioa
*/
struct node* search(struct node *root, struct process_control_block x)
{
if(root==NULL || root->data.vruntime==x.vruntime) //if root->data is x then the element is found
        return root;
    else if(x.vruntime>root->data.vruntime) // x is greater, so we will search the right subtree
        return search(root->right_child, x);
    else //x is smaller than the data, so we will search the left subtree
        return search(root->left_child,x);
}

/*
* Zuhatzean ezkerraldean dagoen nodoa bueltatzen du (txikiena).
*/
struct node* find_minimum(struct node *root)
{
    if(root == NULL)
        return NULL;
    else if(root->left_child != NULL) // node with minimum value will have no left child
        return find_minimum(root->left_child); // left most element will be minimum
    return root;
}

/*
* Minimoa lortzeko funtzioa
*/
struct process_control_block get_min(struct node *root){
    struct node *min;
    min = find_minimum(root);
    return min->data;
}

/*
* Nodo bat sortzeko funtzioa
*/
struct node* new_node(struct process_control_block x)
{
    struct node *p;
    p = malloc(sizeof(struct node));
    p->data = x;
    p->left_child = NULL;
    p->right_child = NULL;

    return p;
}

/*
* Nodo bat zuhaitzean sartzeko funtzioa
*/
struct node* insert(struct node *root, struct process_control_block x)
{
    //searching for the place to insert
    if(root==NULL)
        return new_node(x);
    else if(x.vruntime>root->data.vruntime) // x is greater. Should be inserted to right
        root->right_child = insert(root->right_child, x);
    else // x is smaller should be inserted to left
        root->left_child = insert(root->left_child,x);
    return root;
}

/*
* Nodo bat zuhaitzetik ezabatzeko
*/
struct node* delete(struct node *root, struct process_control_block x)
{
    //searching for the item to be deleted
    if(root==NULL)
        return root;
    if (x.vruntime>root->data.vruntime)
        root->right_child = delete(root->right_child, x);
    else if(x.vruntime<root->data.vruntime)
        root->left_child = delete(root->left_child, x);
    else
    {
        //No Children
        if(root->left_child==NULL && root->right_child==NULL)
        {
            free(root);
            return NULL;
        }

        //One Child
        else if(root->left_child==NULL || root->right_child==NULL)
        {
            struct node *temp;
            if(root->left_child==NULL)
                temp = root->right_child;
            else
                temp = root->left_child;
            free(root);
            return temp;
        }

        //Two Children
        else
        {
            struct node *temp = find_minimum(root->right_child);
            root->data = temp->data;
            root->right_child = delete(root->right_child, temp->data);
        }
    }
    return root;
}

/*
* Nodoak inordenean pantailaratzeko
*/
void inorder(struct node *root)
{
    if(root!=NULL) // checking if the root is not null
    {
        inorder(root->left_child); // visiting left child
        printf(" (%d, %d) ", root->data.pid, root->data.vruntime); // printing data at root
        inorder(root->right_child);// visiting right child
    }else{
        printf("");
    }
}
