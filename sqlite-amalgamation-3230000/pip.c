#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node
{
    int data;
    struct node *next;
}node;
node* createlinkedlist(int n);
void display(node *head);
 int main()
 {
     int i,x,n,flag=0;
     node *HEAD=NULL;
     node *temp=NULL;
     printf("Enter The No. Customers : ");
     scanf("%d",&n);
     HEAD=createlinkedlist(n);
     while(1)
     {

     label : printf("Enter no of counters that are empty : ");
     scanf("%d",&x);
	 if(x>3 || x<0)
	 {
		printf("Invalid data entry!\nPlease enter again!\n");
		goto label;
	 }
     else if(x>=n)
     {
		 
         for(i=0;i<n;i++)
         {
             temp=HEAD->next;
             free(HEAD);
             temp=HEAD;
         }
         printf("queue is empty\n");
		 break;
     }

      else if(x<n)
      {
          for(i=0;i<x;i++)
          {
              temp=HEAD->next;
              free(HEAD);
              HEAD=temp;
          }
          printf("%d customers are yet to be served\n",(n-x));
		  n=n-x;
      }
	 


     }
 }

node* createlinkedlist(int n)
{


    int i;
    node *head=NULL;
    node *temp=NULL;
    node *p=NULL;

    for(i=0;i<n;i++)
    {
        temp=(node*)malloc(sizeof(node));
        printf("Enter token no of customer : ");
        scanf("%d",&(temp->data));
        temp->next=NULL;
        if(head==NULL)
        {
            head=temp;
        }
        else
        {
            p=head;
            while(p->next!=NULL)
            {
                p=p->next;
            }
            p->next=temp;
        }
    }
        return head;
    }

void display(node *head)
{
    node *p=head;
    while(p->next !=NULL)
    {
        printf("%d->",p->data);
        p=p->next;
    }
}
