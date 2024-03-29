#include "StrList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _node {
    char* _data;
    struct _node* _next;
} Node;

struct _StrList {
    Node* _head;
    Node* _tail;
    size_t _size;
};

// Node

Node* Node_alloc(const char* string, Node* next) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (!p) {
        printf("Error! Out of memory...\n");
        exit(1);
    } 
    p->_data = (char*)malloc(sizeof(char)*strlen(string)+1);
    strcpy(p->_data, string);
    p->_next = next;
    return p;
}

void Node_free(Node* node) {
    if (node == NULL) return;
    if (node->_data != NULL) 
    {
        free(node->_data);
    }
    free(node);
}

// List ---------------------------------------------

StrList* StrList_alloc() {
    StrList* p = (StrList*)malloc(sizeof(StrList));
    if (!p) {
        printf("Error! Out of memory...\n");
        exit(1);
    } 
    p->_head = p->_tail = NULL;
    p->_size = 0;
    return p;
}

void StrList_free(StrList* StrList) {
    if (StrList == NULL) return;
    Node* p1 = StrList->_head;
    Node* p2;
    while (p1)
    {
        /* p2 is the prevous node, first free him than continue
           forward in the list */
        p2 = p1;
        p1 = p1->_next;
        Node_free(p2);
    }
    free(StrList);
}

// Methods -------------------------------------

size_t StrList_size(const StrList* StrList) {
    return StrList->_size;
    
}

void StrList_insertLast(StrList* StrList, const char* data) {
    Node* p = Node_alloc(data, NULL);
    Node* t = StrList->_tail;
    StrList->_tail = p;
    if (t == NULL)
    {
        StrList->_head = p;
    } else {
        t->_next = p;
    }
    StrList->_size = StrList->_size + 1;
}

void StrList_insertAt(StrList* StrList, const char* data,int index) {
    size_t lst_size = StrList->_size;
    if (index < 0 || index >= lst_size) return;
    if (index == lst_size)
    {
        StrList_insertLast(StrList, data);
        return;
    }
    Node* new_node = NULL;
    if (index == 0)
    {
        new_node = Node_alloc(data, StrList->_head);
        StrList->_head = new_node;
    } else {
    Node* nptr = StrList->_head;
    while (--index)
    {
        nptr = nptr->_next;
    }
    new_node = Node_alloc(data, nptr->_next);
    nptr->_next = new_node;
    }
    StrList->_size = StrList->_size + 1;
}


char* StrList_firstData(const StrList* StrList) {
    return StrList->_head->_data;
}

void StrList_print(const StrList* StrList) {
    if (StrList == NULL || !StrList->_head) return;
    Node* nptr = StrList->_head;
    while (nptr->_next) {
        printf("%s ", nptr->_data);
        nptr = nptr->_next;
    }
    printf("%s", nptr->_data);
}

void StrList_printAt(const StrList* Strlist,int index) {
    size_t size = StrList_size(Strlist);
    if (index < 0 || index >= size)
    {
        printf("Error! index is not valid\n");
        return;
    }
    Node* np = Strlist->_head;
    while (--index >= 0)
    {
        np = np->_next;
    }
    if (!np) return;
    char* ans = np->_data;
    printf("%s\n", ans);
}

/*
 * Return the amount of chars in the list.
*/
int StrList_printLen(const StrList* Strlist) {
    size_t ans = 0;
    Node* np = Strlist->_head;
    while (np)
    {
        ans += strlen(np->_data);
        np = np->_next;
    }
    return ans;
}

/*
Given a string, return the number of times it exists in the list.
*/
int StrList_count(StrList* StrList, const char* data) {
    size_t ans = 0;
    Node* np = StrList->_head;
    while (np)
    {
        char* current_string = np->_data;
        if (strcmp(current_string, data) == 0) ans++;
        np = np->_next;
    }
    return ans;
}

/*
	Given a string and a list, remove all the appearences of this string in the list.
*/
void StrList_remove(StrList* StrList, const char* data) {
    if (StrList == NULL || !StrList->_head) return;
    Node* prev = StrList->_head;
    Node* curr = prev->_next;
    while (curr) {
        if (strcmp(curr->_data, data) == 0) {
            Node* to_remove = curr;
            prev->_next = curr->_next;
            curr = curr->_next;
            if (StrList->_tail == to_remove) {
                StrList->_tail = prev;
            }
            Node_free(to_remove);
            StrList->_size = StrList->_size - 1;
        } else {
            prev = prev->_next;
            curr = curr->_next;
        }      
    }
    if (strcmp(StrList->_head->_data, data) == 0) {
        StrList_removeAt(StrList, 0);
    }
}

/*
	Given an index and a list, remove the string at that index.
*/
void StrList_removeAt(StrList* StrList, int index) {
    size_t size = StrList_size(StrList);
    if (index < 0 || index >= size)
    {
        printf("Error! index is not valid\n");
        return;
    }
    Node *to_remove = NULL, *prev = NULL;
    if (index == 0)
    {
        to_remove = StrList->_head;
        StrList->_head = StrList->_head->_next;
        if (StrList->_tail == to_remove) {
            StrList->_tail = StrList->_head;
        }
    } else {
        prev = StrList->_head;
        while (--index) {
            prev = prev->_next;
        }   
        to_remove = prev->_next;
        prev->_next = to_remove->_next;
        if (StrList->_tail == to_remove) {
            StrList->_tail = prev;
        }
    }
    Node_free(to_remove);
    StrList->_size = size - 1;
}

/*
 * Checks if two StrLists have the same elements
 * returns 0 if not and any other number if yes
 */
int StrList_isEqual(const StrList* StrList1, const StrList* StrList2) {
    const Node* p1 = StrList1->_head;
    const Node* p2 = StrList2->_head;
    while (p1) {
        if (p2 == NULL || strcmp(p1->_data, p2->_data)) return 0;
        p1 = p1->_next;
        p2 = p2->_next;
    }
    if (p2) return 0;
    return 1;
}

/*
 * Clones the given StrList. 
 * It's the user responsibility to free it with StrList_free.
 */
StrList* StrList_clone(const StrList* Strlist) {
    StrList* ans = StrList_alloc(); 
    const Node* old = Strlist->_head;
    Node** copy = &(ans->_head);
    ans->_size = Strlist->_size;
    while (old) {
        *copy = Node_alloc(old->_data, NULL);
        old = old->_next;
        copy = &((*copy)->_next);
    }
    return ans;
}

/*
 * Reverces the given StrList. 
 */
void StrList_reverse( StrList* StrList) {
    if (StrList->_size <= 1) return;
    Node *p1 = StrList->_head, *p2 = p1->_next;
    while (p2) {
        Node* p3 = p2->_next;
        p2->_next = p1;
        p1 = p2;
        p2 = p3;
    }
    Node* temp = StrList->_head;
    StrList->_head = StrList->_tail;
    StrList->_tail = temp;
    StrList->_tail->_next = NULL;
}

/*
 * Sort the given list in lexicographical order 
 */
void StrList_sort( StrList* StrList) {
    char** temp = (char**)malloc(sizeof(char*)*(StrList->_size));
    if (!temp) {
        printf("Error! Out of memory...\n");
        exit(1);
    } 
    size_t i = 0;
    Node* p = StrList->_head;
    while (p) {
        temp[i] = p->_data;
        i++;
        p = p->_next;
    }
    mergeSort(temp, 0, i-1);
    p = StrList->_head;
    i = 0;
    while (p) {
        p->_data = temp[i];
        p = p->_next;
        i++;
    }
    free(temp);
}

/*
 * Checks if the given list is sorted in lexicographical order
 * returns 1 for sorted,   0 otherwise
 */
int StrList_isSorted(StrList* StrList) {
    Node* np = StrList->_head;
    while (np->_next)
    {
        char* current_string = np->_data;
        char* next_string = np->_next->_data;
        if (strcmp(current_string, next_string) > 0)
        {
            return 0;
        }
        np = np->_next;
    }
    return 1;
}

// My help functions

void mergeSort(char** string_arr, size_t start, size_t end) {
    if (end <= start) return;
    size_t mid = (start + end)/2;
    mergeSort(string_arr, start, mid);
    mergeSort(string_arr, mid+1, end);
    merge(string_arr, start, mid, end);
}

void merge(char** string_arr, size_t l, size_t m, size_t r) {
    size_t i, j, k;
    size_t ls = m-l+1, rs = r-m;
    char** left = (char**)malloc(sizeof(char*)*(ls));
    char** right = (char**)malloc(sizeof(char*)*(rs));
    for (i  = 0; i < ls; i++) {
        left[i] = string_arr[l+i];
    }
    for (i = 0; i < rs; i++) {
        right[i] = string_arr[m+1+i];
    }
    i = j = 0;
    k = l;
    while (i < ls && j < rs) {
        if (strcmp(left[i], right[j]) <= 0) {
            string_arr[k] = left[i];
            i++;
        } else {
            string_arr[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < ls) {
        string_arr[k] = left[i];
        i++;
        k++;
    }
    while (j < rs) {
        string_arr[k] = right[j];
        j++;
        k++;
    }
    free(left);
    free(right);
}