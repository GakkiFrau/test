#include <stdio.h>
#include <stdlib.h>



struct LinkNode 
{
    int data;
    struct LinkNode * next;
};

#define LEN  (sizeof(struct LinkNode))

/* create list */
struct LinkNode * create_list(int val)
{
    
    struct LinkNode * pnode = (struct LinkNode *)malloc(LEN);

    if (pnode == NULL)
    {
        printf("err");
        exit(-1);
        //return -1;
    }

    pnode->data = val;
    pnode->next = NULL;   

    return pnode;
}

/* insert list */
void insert_list(struct LinkNode * header, int data)
{
    // create new list
    // 1.
    // struct LinkNode * node = (struct LinkNode *)malloc(LEN);
    // node->data = data;
    // node->next = NULL;
    // 2.
    struct LinkNode * node = create_list(data);

    while(header->next != NULL) {
        header = header->next;
    }

    header->next = node;

}

/* remove list */
void remove_list(struct LinkNode * header, int data)
{
    struct LinkNode * pCur = header;
    header = header->next;

    while(header != NULL) {

        if(header->data == data) {
            break;
        }
        pCur = header;
        header = header->next;
        
    }
    if (header != NULL ){
        pCur->next = header->next;

        free(header);
    }

}

/* find data */
struct LinkNode * find_list_data(struct LinkNode * node, int data)
{
    while (node != NULL) {
        if (node->data == data) {
            break;
        }
        node = node->next;
    }
    return node;
}



/* print list */
void print_list(struct LinkNode * phead)
{
    struct LinkNode * current = phead;

    while(current) {
        printf("%d-> ", current->data);
        current = current->next;
    }
    printf("NULL");
}


int main()
{
    struct LinkNode * header = create_list(5);
    // printf("header->data = %d\r\n", header->data);
    // printf("header->next = %p\r\n", header->next);

    insert_list(header, 3);
    insert_list(header, 4 );

    printf("%p\r\n",find_list_data(header, 3));


    printf("%p\r\n",header);
    printf("%d\r\n",header->data);
    printf("%p\r\n",header->next);
    printf("%p\r\n",header->next->next);
    printf("%p\r\n",header->next->next->next);

    print_list(header);

    return 0;
}