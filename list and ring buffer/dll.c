#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node* prev;
    struct node* next;
};
typedef struct node node;
struct list
{
    node* start;
    node* current;
};
typedef struct list list;

void list_init(list* lst, int data)
{
    lst->start = (node*) malloc(sizeof(node));
    lst->start->data = data;
    lst->current = lst->start;
}
void list_add(list* lst, int data)
{
    node* newnode = (node*) malloc(sizeof(node));
    newnode->data = data;
    newnode->prev = lst->current;
    newnode->next = lst->start;

    lst->current->next = newnode;
    lst->start->prev = newnode;

    lst->current = newnode;
}
void list_del(node* delnode)
{
    node* prev = delnode->prev;
    node* next = delnode->next;
    prev->next = next;
    next->prev = prev;
    free(delnode);
}
void list_ins(node* afterthis)
{
}
int main()
{
    list* lst = (list*) malloc(sizeof(list));
    node* n;
    list_init(lst, 0);
    int i;
    for(i = 1; i < 100; i++) list_add(lst, i);

    printf("Reading start to end:\n");
    for(n = lst->start, i = 0; i < 100; n = n->next, i++)
    {
        printf("%d\t", n->data);
    }
    printf("\n\n");
    printf("Reading end to start:\n");
    for(n = lst->current, i = 0; i < 100; n = n->prev, i++) printf("%d\t", n->data);
    return 0;
}
