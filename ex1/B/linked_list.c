#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
	struct list* newlist;
	newlist=(struct list*)malloc(sizeof(struct list));
	/*node firstnode;
	firstnode.next=NULL;
	firstnode.prev=NULL;
	firstnode.data=NULL;
	newlist.head=firstnode;
	newlist.tail=firstnode;
	newlist.length=1;*/	
	newlist->length=0;
	newlist->head=NULL;
	newlist->tail=NULL;
	return newlist;
}

void list_delete(list_t list)
{
	for(int i=list->length;i<=0;i--){
		list_extract(list,i);
	}
	free(list);
}

void list_insert(list_t list, int index, int data)
{
	struct node* newnode = (struct node*)malloc(sizeof(struct node));
	newnode->data=data;
	struct node* current_node=list->head;
	if((list->length>0)&(list->length>index-1)){
		int nodes_to_traverse=index;
		while(nodes_to_traverse>0){
			current_node=current_node->next;
			nodes_to_traverse--;
		}
		current_node->prev->next=newnode;
		newnode->next=current_node;
		newnode->prev=current_node->prev;
		current_node->prev=newnode;
		list->length=list->length+1;
	}
	else{
		list_append(list, data);
	}
		
}

void list_append(list_t list, int data)
{
	struct node* newnode=(struct node*)malloc(sizeof(struct node));
	newnode->data=data;
	
	if(list->length>0){
		newnode->prev=list->tail;
		newnode->next=NULL;
		newnode->prev->next=newnode;
		list->tail=newnode;
	}
	else{
		list->head=newnode;
		list->tail=newnode;
		newnode->next=NULL;
		newnode->prev=NULL;
	}
	list->length=list->length+1;
	
}
/*Not the best solution in list_print regarding runtime*/
void list_print(list_t list)
{
	printf("Printing list:\n");
	for(int i=0; i<list->length;i++){
		printf("%i ", list_get(list,i));
	}
}

long list_sum(list_t list)
{
	long sum=0;
	for(int i=0; i<list->length;i++){
		sum+=list_get(list,i);
	}
	return sum;
}

int list_get(list_t list, int index)
{
	if((index<0)|(index>list->length)) exit(1);
	
	struct node* current_node=list->head;
	
	int nodes_to_traverse=index;
	
	while(nodes_to_traverse>0){
		current_node=current_node->next;
		nodes_to_traverse--;
	}	
	return current_node->data;	
}

int list_extract(list_t list, int index)
{
	struct node* current_node=list->head;
	int nodes_to_traverse=index;
	int data_to_return=0;
	while(nodes_to_traverse>0){
		current_node=current_node->next;
		nodes_to_traverse--;
	}	
	data_to_return=current_node->data;
	if(current_node->prev){
		current_node->prev->next=current_node->next;
	}	
	else if(current_node->next){ //This is the first element of the list
		current_node->next->prev=NULL;
		list->head=current_node->next;
	}
	if(current_node->next){
		current_node->next->prev=current_node->prev;
	}
	else if(current_node->prev){ //this is the last element of the list
		current_node->prev->next=NULL;
		list->tail=current_node->prev;
	}
	free(current_node);
	list->length=list->length-1;
	return data_to_return;
}






















