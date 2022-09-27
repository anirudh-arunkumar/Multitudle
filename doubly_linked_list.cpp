//=================================================================
// Implementation for DLL module.
//
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

//===========================
/* Creating nodes and lists */
//===========================

LLNode* create_llnode(void* data) {
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->data = data;
    return newNode;
}

DLinkedList* create_dlinkedlist(CompareFunction compare) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    newList->compare = compare;
    return newList;
}

//============================
/* Accessing nodes and lists */
//============================

int getSize(DLinkedList* dLinkedList){
    return dLinkedList->size;
}

LLNode* getHead(DLinkedList* dLinkedList){
    if (dLinkedList->head == NULL) {
        return NULL;
    } else {
        return dLinkedList->head;
    }
}

LLNode* getTail(DLinkedList* dLinkedList){
    if (dLinkedList->tail == NULL) {
        return NULL;
    } else {
        return dLinkedList->tail;
    }
}

LLNode* getNext(LLNode* node){
    if (node->next == NULL) {
        return NULL;
    } else {
        return node->next;
    }
}

LLNode* getPrev(LLNode* node){
    if (node->prev == NULL) {
        return NULL;
    } else {
        return node->prev;
    }
}

void* getData(LLNode* node){
    if (node->data == NULL) {
        return NULL;
    } else {
        return node->data;
    }
}

//============================
/* Inserting nodes into lists */
//============================

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
    if (prev_node == NULL){
        printf("PREVIOUS NODE NULL");
        return;
    }

    if (prev_node->next == NULL) {
        insertTail(dLinkedList, data);
    } else {
        LLNode* newNode = create_llnode(data);
        newNode->prev = prev_node;
        newNode->next = prev_node->next;
        (prev_node->next)->prev = newNode;
        prev_node->next = newNode;
        dLinkedList->size++;
        return;
    }
}


void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
    if (next_node == NULL){
        printf("NEXT NODE NULL");
        return;
    }
    if (next_node->prev == NULL) {
        insertHead(dLinkedList, data);
    } else {
        LLNode* newNode = create_llnode(data);
        newNode->prev = next_node->prev;
        newNode->next = next_node;
        (next_node->prev)->next = newNode;
        next_node->prev = newNode;
        dLinkedList->size++;
        return;
    }
}


void insertHead(DLinkedList* dLinkedList, void* data){
    LLNode* newNode = create_llnode(data);
    if (dLinkedList->head == NULL) { //if list is empty
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    } else {
        newNode->next = dLinkedList->head;
        (dLinkedList->head)->prev = newNode;
        dLinkedList->head = newNode;
        dLinkedList->size++;
  }
  return;
}

void insertTail(DLinkedList* dLinkedList, void* data){
    LLNode* newNode = create_llnode(data);
    if (dLinkedList->head == NULL) {
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    } else {
        newNode->prev = dLinkedList->tail;
        (dLinkedList->tail)->next = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    }
    return;
}

//============================
/* Looking up nodes in lists */
//============================

LLNode* findNode(DLinkedList* dLinkedList, void* data){
    if (dLinkedList->head == NULL){
        return NULL;
    }

    LLNode* currentNode = dLinkedList->head;
    while(currentNode) {
        if (dLinkedList->compare(currentNode->data, data)) {           
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

//===========================
/* Deleting nodes and lists */
//===========================

void deleteNode(DLinkedList* dLinkedList, LLNode* Node) {
    if (dLinkedList->head == NULL || Node == NULL){
        return;
    }

    //one element
    if (dLinkedList->size == 1) {
        dLinkedList->head = NULL;
        dLinkedList->tail = NULL;
        dLinkedList->size--;
        free(Node);
        return;
    }
    

    //delete tail
    if (Node->next == NULL) {
        dLinkedList->tail = Node->prev;
        dLinkedList->tail->next = NULL;
        dLinkedList->size--;
        free(Node);
        return;
    }

    //deleting head
    if (Node->prev == NULL) {
        dLinkedList->head = Node->next;
        dLinkedList->head->prev = NULL;
        dLinkedList->size--;
        free(Node);
        return;
    }

    

    //deleting in between
    Node->prev->next = Node->next;
    Node->next->prev = Node->prev;
    dLinkedList->size--;
    free(Node);
    return;
}

void destroyList(DLinkedList* dLinkedList){
    while (dLinkedList->head) {
        deleteNode(dLinkedList, getTail(dLinkedList));
    }
    free(dLinkedList);
    return;
}

//==================
/* Reversing lists */
//==================

void reverse(DLinkedList* dLinkedList){

    if(dLinkedList->head == NULL) {
        printf("EMPTY");
        return;
    }

    LLNode* temp = NULL;
    LLNode* currentNode = getHead(dLinkedList);
     
    while (currentNode != NULL)
    {
        temp = currentNode->prev;
        currentNode->prev = currentNode->next;
        currentNode->next = temp;            
        currentNode = currentNode->prev;
    }
    
    LLNode* tempHead = dLinkedList->head;
    LLNode* tempTail = dLinkedList->tail;

    dLinkedList->tail = tempHead;
    dLinkedList->head = tempTail;
    
    
    
    
    
    
    
    
    
    
    
    
    

    return;
    
}