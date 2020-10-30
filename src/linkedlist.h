
struct node {
   int data;
   int key;
   struct node *next;
};


void printList();
void insertFirst(int key, int data); 
struct node* deleteFirst();
bool isEmpty();
int length();
struct node* find(int key);
struct node* deletenode(int key);
void sort();
void reverse(struct node** head_ref); 
