#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<limits.h>
#define m 5                 //no of children
#define n 256               //word size
#define limit ceil((m+1)/2) //min children of a node

typedef struct bt_node_tag
{
    struct bt_node_tag* parent;
    int count;
    char data[m-1][n];
    int wcount[m-1];
    struct bt_node_tag* down[m];
}bt_node;

bool IsAllNULL(bt_node* root);
void MakeAllNULL(bt_node** root);
void ShiftRight(bt_node** root, int pos);                                           //just one pos right
void SortBTree(bt_node** root);                                                     //considers only last val
int FindPos(bt_node* root, char d[]);
void ShiftLeft(bt_node** root, int pos);                                            //from pos to 0
bt_node* SplitBTreeNode(bt_node** root, char dptr[],int* valcount, bt_node* ddown); //return less node and modify given into max node
void PrintBTree(bt_node* root);
void SetParent(bt_node**root);                                                      //used while splitting the node
void PushVal(bt_node** root, char val[], int valcount, bt_node* valdown);           //except for leaf insertion
bt_node* InsertBTree(bt_node* root, char d[]);
void RemoveNode(bt_node** root,int pos);
void StepLeft(bt_node** root, int pos);                                             //just one pos left
void MergeBTreeNodes(bt_node** ptr, bt_node* ptr2);
void ParentHelp(bt_node** root);
void SetMaxLess(bt_node** root, int delpos);                                        //in deletion of non leaf node, replace the given node with maximum val less than given node
bool SiblingHelp(bt_node** root);
void AdjustBTreeNode(bt_node** root, int delpos);                                   //removing and setting-after-deleting a node
void FreqPrint(bt_node* root, int k);                                               //words occuring more than k times
bool FindNode(bt_node**root, char d[]);                                             //if its there
bt_node* FindRoot(bt_node* root);
bt_node* DeleteBTreeNode(bt_node* root, char d[]);                                  //search for the node
void PrintBTreeNode(bt_node* root);
bt_node* InitBTree(bt_node* root);
void RainPrint(bt_node* root, char l[], char u[]);                                  //search for both
void Rain(bt_node* root, char l[], char u[]);                                       //print the range
void MinPrint(bt_node* root);                                                       //driver func
void FindMin(bt_node* root, int* minptr);                                           //find min occ of word in dic
void PrintMin(bt_node* root, int min);                                              //print all words with min occ
void PrintBTreeStepWise(bt_node*root);                                              //just for mansi maam

bool IsAllNULL(bt_node* root)
{
    bool ret_val=1;
    bt_node* ptr=root;
    int i;
    for(i=0;i<ptr->count&&ret_val==1;i++)
    {
        if(ptr->down[i]!=NULL)
        {
            ret_val=0;
        }
    }
    return ret_val;
}

void MakeAllNULL(bt_node** root)
{
    int i;
    bt_node* ptr=*root;
    ptr->parent=NULL;
    ptr->count=0;
    for(i=0;i<m;i++)
    {
        ptr->down[i]=NULL;
    }
}

void ShiftRight(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i;
    for(i=ptr->count-3;i>=pos;i--)
    {
        strcpy(ptr->data[i+1],ptr->data[i]);
        ptr->wcount[i+1]=ptr->wcount[i];
    }
    for(i=ptr->count-2;i>=pos;i--)
    {
        ptr->down[i+1]=ptr->down[i];
    }
}

void SortBTree(bt_node** root)
{
    bt_node* ptr=*root;
    if(ptr->count>2)
    {
        int i;
        char val[n];
        strcpy(val,ptr->data[ptr->count-2]);
        int valcount=ptr->wcount[ptr->count-2];
        bt_node* valdown= ptr->down[ptr->count-1];
        for(i=0;i<ptr->count-1&&strcmp(val,ptr->data[i])==1;i++)
        {
        }
        ShiftRight(&ptr,i);
        strcpy(ptr->data[i],val);
        ptr->wcount[i]=valcount;
        ptr->down[i]=valdown;
    }
}

int FindPos(bt_node* root, char d[])
{
    int i;
    bt_node* ptr=root;
    for(i=0;i<m-1&&strcmp(ptr->data[i],d)==(-1);i++)
    {
    }
    return i;
}

void ShiftLeft(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i,j=0;
    for(i=pos;j<ptr->count-1;i++&&j++)
    {
        strcpy(ptr->data[j],ptr->data[i]);
        ptr->wcount[j]=ptr->wcount[i];
    }
    j=0;
    for(i=pos;j<ptr->count;i++&&j++)
    {
        ptr->down[j]=ptr->down[i];
    }
}

bt_node* SplitBTreeNode(bt_node** root, char dptr[],int* valcount, bt_node* ddown)
{
    bt_node* ptr=*root;
    int splitpos= ceil((m-1)/2);
    int valpos= FindPos(ptr,dptr);
    bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
    nptr->parent=ptr->parent;
    nptr->count=1;
    int i;
    for(i=0;i<splitpos;i++)
    {
        strcpy(nptr->data[i],ptr->data[i]);
        nptr->wcount[i]=ptr->wcount[i];
        nptr->down[i]=ptr->down[i];
        nptr->count=nptr->count+1;
    }
    nptr->down[i]=ptr->down[i];
    ptr->count=ptr->count-splitpos;
    ShiftLeft(&ptr,splitpos);
    if(valpos==splitpos)
    {
        ptr->down[0]=ddown;
    }
    else
    {
        if(valpos<splitpos)
        {
            strcpy(nptr->data[nptr->count-1], dptr);
            nptr->wcount[nptr->count-1]=*valcount;
            nptr->down[nptr->count]= ddown;
            nptr->count=nptr->count+1;
            SortBTree(&nptr);
        }
        else
        {
            strcpy(ptr->data[ptr->count-1], dptr);
            ptr->wcount[ptr->count-1]=*valcount;
            ptr->down[ptr->count]= ddown;
            ptr->count=ptr->count+1;
            SortBTree(&ptr);
        }
        strcpy(dptr,ptr->data[0]);
        *valcount=ptr->wcount[0];
        ptr->count=ptr->count-1;
        ShiftLeft(&ptr,1);
    }
    return nptr;
}

void PrintBTree(bt_node* root)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        if(ptr->count==0)
        {
            printf("\nEmpty BTree\n");
        }
        else
        {
            int i;
            for(i=0;i<(ptr->count);i++)
            {
                PrintBTree(ptr->down[i]);
                if(i<ptr->count-1)
                {
                    printf("%s : ",ptr->data[i]);
                    printf("%d\n",ptr->wcount[i]);
                }
            }
        }
    }
}

void SetParent(bt_node**root)
{
    bt_node* ptr=*root;
    int i;
    for(i=0;i<ptr->count;i++)
    {
        (ptr->down[i])->parent=ptr;
    }
}

void PushVal(bt_node** root, char val[], int valcount, bt_node* valdown)
{
    bt_node* ptr=*root;
    int s;
    for(s=0;s<ptr->count-1&&strcmp(ptr->data[s],val)!=0;s++)                    //
    {
    }
    if(strcmp(ptr->data[s],val)==0)                                             //checking if the word exist
    {
        ptr->wcount[s]=ptr->wcount[s]+1;
    }                                                                           //
    else
    {
        if(ptr->count<m)
        {
            strcpy(ptr->data[ptr->count-1],val);
            ptr->wcount[ptr->count-1]=valcount;
            ptr->down[ptr->count]=valdown;
            ptr->count=ptr->count+1;
            SortBTree(&ptr);
        }
        else
        {
            bt_node* lptr=SplitBTreeNode(&ptr,val,&valcount,valdown);           //val too is passed by ref
            SetParent(&lptr);
            if(ptr->parent==NULL)
            {
                bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
                nptr->parent=NULL;
                MakeAllNULL(&nptr);
                nptr->count=2;
                strcpy(nptr->data[0],val);
                nptr->wcount[0]=valcount;
                nptr->down[0]=lptr;
                nptr->down[1]=ptr;
                ptr->parent=nptr;
                lptr->parent=nptr;
            }
            else
            {
                PushVal(&(ptr->parent),val,valcount,lptr);
            }
        }
    }
}

bt_node* InsertBTree(bt_node* root, char d[])
{
    bool done=0;
    bt_node* ptr=root;
    if(ptr==NULL)                                                           //no node
    {
        bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
        nptr->parent=NULL;
        MakeAllNULL(&nptr);
        nptr->count=2;
        strcpy(nptr->data[0],d);
        nptr->wcount[0]=1;
        root=nptr;
    }
    else
    {
        while(ptr!=NULL&&done==0)
        {
            if(IsAllNULL(ptr))                                              //only root is present
            {
                if(ptr->count<m)
                {
                    int s;                                                  //if word is present
                    for(s=0;s<ptr->count-1&&strcmp(ptr->data[s],d)!=0;s++)
                    {
                    }
                    if(strcmp(ptr->data[s],d)==0)
                    {
                        ptr->wcount[s]=ptr->wcount[s]+1;
                    }
                    else
                    {
                        strcpy(ptr->data[ptr->count-1],d);
                        ptr->wcount[ptr->count-1]=1;
                        ptr->down[ptr->count]=NULL;
                        ptr->count=ptr->count+1;
                        SortBTree(&ptr);
                    }
                }
                else                                                        // if count = m
                {
                    int s;                                                  // if word present
                    for(s=0;s<ptr->count-1&&strcmp(ptr->data[s],d)!=0;s++)
                    {
                    }
                    if(strcmp(ptr->data[s],d)==0)
                    {
                        ptr->wcount[s]=ptr->wcount[s]+1;
                    }
                    else
                    {
                        int valcount=1;
                        bt_node* lptr=SplitBTreeNode(&ptr,d,&valcount,NULL);
                        if(ptr->parent==NULL)                               //is the root
                        {
                            bt_node* nptr=(bt_node*)malloc(sizeof(bt_node));
                            nptr->parent=NULL;
                            MakeAllNULL(&nptr);
                            nptr->count=2;
                            strcpy(nptr->data[0],d);
                            nptr->wcount[0]=valcount;
                            nptr->down[0]=lptr;
                            nptr->down[1]=ptr;
                            ptr->parent=nptr;
                            lptr->parent=nptr;
                        }
                        else
                        {
                            PushVal(&(ptr->parent),d,valcount,lptr);

                        }
                    }
                }
                done=1;
            }
            else                                                            //not a leaf
            {
                int i;
                for(i=0;i<ptr->count-1&&strcmp(ptr->data[i],d)==(-1);i++)
                {
                }
                ptr=ptr->down[i];
            }
        }
    }
    root=FindRoot(root);
    return root;
}

void RemoveNode(bt_node** root,int pos)
{
    bt_node*ptr=*root;
    int i;
    for(i=pos;i<ptr->count-1;i++)
    {
        strcpy(ptr->data[i],ptr->data[i+1]);
        ptr->wcount[i]=ptr->wcount[i+1];
        ptr->down[i]=ptr->down[i+1];
    }
    ptr->down[i]=ptr->down[i+1];
    ptr->count=ptr->count-1;
}

void StepLeft(bt_node** root, int pos)
{
    bt_node* ptr=*root;
    int i;
    if(ptr->count>2)
    {
        for(i=pos;i<ptr->count-1;i++)
        {
            strcpy(ptr->data[i],ptr->data[i+1]);
            ptr->wcount[i]=ptr->wcount[i+1];
        }
        for(i=pos+1;i<ptr->count-1;i++)
        {
            ptr->down[i]=ptr->down[i+1];
        }
        ptr->down[i]=ptr->down[i+1];
    }
    ptr->count=ptr->count-1;
}

void MergeBTreeNodes(bt_node** ptr, bt_node* ptr2)
{
    bt_node* ptr1= *ptr;
    int i, j=0;
    for(i=ptr1->count-1;j<ptr2->count-1;i++&&j++)
    {
        strcpy(ptr1->data[i],ptr2->data[j]);
        ptr1->wcount[i]=ptr2->wcount[j];
        ptr1->down[i]=ptr2->down[j];
        ptr1->count=ptr1->count+1;
    }
    ptr1->down[i]=ptr2->down[j];
}

void ParentHelp(bt_node** root)
{
    bt_node* ptr=*root;
    char val[n];
    strcpy(val,ptr->data[0]);
    ptr=ptr->parent;
    int i;
    for(i=0;i<ptr->count-1&&strcmp(val,ptr->data[i])==1;i++)
    {
    }
    if(ptr->parent!=NULL)
    {
        if(i==ptr->count-1)
        {
            strcpy((ptr->down[i-1])->data[(ptr->down[i-1])->count-1],ptr->data[i-1]);
            (ptr->down[i-1])->wcount[(ptr->down[i-1])->count-1]=ptr->wcount[i-1];
            (ptr->down[i-1])->count=(ptr->down[i-1])->count+1;
            MergeBTreeNodes(&(ptr->down[i-1]),ptr->down[i]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
            if(ptr->count<limit)
            {
                ParentHelp(&ptr);
            }
        }
        else
        {
            strcpy((ptr->down[i])->data[(ptr->down[i])->count-1],ptr->data[i]);
            (ptr->down[i])->wcount[(ptr->down[i])->count-1]=ptr->wcount[i];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            MergeBTreeNodes(&(ptr->down[i]),ptr->down[i+1]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
            if(ptr->count<limit)
            {
                ParentHelp(&ptr);
            }
        }
    }
    else
    {
        if(i==ptr->count-1)
        {
            strcpy((ptr->down[i-1])->data[(ptr->down[i-1])->count-1],ptr->data[i-1]);
            (ptr->down[i-1])->wcount[(ptr->down[i-1])->count-1]=ptr->wcount[i-1];
            (ptr->down[i-1])->count=(ptr->down[i-1])->count+1;
            MergeBTreeNodes(&(ptr->down[i-1]),ptr->down[i]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
        }
        else
        {
            strcpy((ptr->down[i])->data[(ptr->down[i])->count-1],ptr->data[i]);
            (ptr->down[i])->wcount[(ptr->down[i])->count-1]=ptr->wcount[i];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            MergeBTreeNodes(&(ptr->down[i]),ptr->down[i+1]);
            ptr->down[i+1]=NULL;
            StepLeft(&ptr,i);
        }
        if(ptr->count<2)
        {
            bt_node* temp=ptr;
            ptr=ptr->down[0];
            free(temp);
            ptr->parent=NULL;
        }
    }
}

void SetMaxLess(bt_node** root, int delpos)
{
    bt_node* ptr=*root;
    bt_node* lptr=ptr->down[delpos];
    int lpos;
    while(!IsAllNULL(lptr))
    {
        lptr=lptr->down[lptr->count-1];
    }
    lpos=lptr->count-2;
    strcpy(ptr->data[delpos],lptr->data[lpos]);
    AdjustBTreeNode(&lptr,lpos);
}

bool SiblingHelp(bt_node** root)
{
    bool can=0;
    bt_node* ptr=*root;
    char val[n];
    strcpy(val,ptr->data[0]);
    ptr=ptr->parent;
    int i;
    for(i=0;i<ptr->count-1&&strcmp(val,ptr->data[i])==1;i++)
    {
    }
    if(i==0)
    {
        if((ptr->down[i+1])->count>limit)
        {
            strcpy((ptr->down[i])->data[(ptr->down[i])->count-1],ptr->data[0]);
            (ptr->down[i])->wcount[(ptr->down[i])->count-1]=ptr->wcount[0];
            strcpy(ptr->data[0],(ptr->down[i+1])->data[0]);
            ptr->wcount[0]=(ptr->down[i+1])->wcount[0];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i+1])->down[0];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i+1])->count=(ptr->down[i+1])->count-1;
            ShiftLeft(&(ptr->down[i+1]),1);
            can=1;
        }
    }
    else
    {
        if(i!=ptr->count-1&&(ptr->down[i+1])->count>limit)
        {
            strcpy((ptr->down[i])->data[(ptr->down[i])->count-1],ptr->data[i]);
            (ptr->down[i])->wcount[(ptr->down[i])->count-1]=ptr->wcount[i];
            strcpy(ptr->data[i],(ptr->down[i+1])->data[0]);
            ptr->wcount[i]=(ptr->down[i+1])->wcount[0];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i+1])->down[0];
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i+1])->count=(ptr->down[i+1])->count-1;
            ShiftLeft(&(ptr->down[i+1]),1);
            can=1;
        }
        else if((ptr->down[i-1])->count>limit)
        {
            strcpy((ptr->down[i])->data[(ptr->down[i])->count-1],ptr->data[i-1]);
            (ptr->down[i])->wcount[(ptr->down[i])->count-1]=ptr->wcount[i-1];
            strcpy(ptr->data[i-1],(ptr->down[i-1])->data[(ptr->down[i-1])->count-2]);
            ptr->wcount[i-1]=(ptr->down[i-1])->wcount[(ptr->down[i-1])->count-2];
            (ptr->down[i])->down[(ptr->down[i])->count]=(ptr->down[i-1])->down[(ptr->down[i-1])->count-1];
            (ptr->down[i-1])->down[(ptr->down[i-1])->count-1]=NULL;
            (ptr->down[i])->count=(ptr->down[i])->count+1;
            (ptr->down[i-1])->count=(ptr->down[i-1])->count-1;
            can=1;
        }
    }
    return can;
}

void AdjustBTreeNode(bt_node** root, int delpos)
{
    bt_node* ptr=*root;
    if((ptr->count)-1<limit)
    {
        if(ptr->parent!=NULL)
        {
            if(IsAllNULL(ptr))
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    RemoveNode(&ptr,delpos);
                    bool done=0;
                    done=SiblingHelp(&ptr);
                    if(!done)
                    {
                        ParentHelp(&ptr);
                    }
                }
            }
            else
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    SetMaxLess(&ptr, delpos);
                }
            }
        }
        else            //root
        {
            if(IsAllNULL(ptr))
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    RemoveNode(&ptr,delpos);
                    if(ptr->count<2)
                    {
                        free(ptr);
                        ptr=NULL;
                    }
                }
            }
            else
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    SetMaxLess(&ptr, delpos);
                }

            }
        }
    }
    else
    {
        if(ptr->parent!=NULL)
        {
            if(IsAllNULL(ptr))
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    RemoveNode(&ptr,delpos);
                }

            }
            else
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    SetMaxLess(&ptr, delpos);
                }

            }
        }
        else            //root
        {
            if(IsAllNULL(ptr))
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    RemoveNode(&ptr,delpos);
                    if(ptr->count<2)
                    {
                        free(ptr);
                        ptr=NULL;
                    }
                }
            }
            else
            {
                if(ptr->wcount[delpos]>1)
                {
                    ptr->wcount[delpos]=ptr->wcount[delpos]-1;
                }
                else
                {
                    SetMaxLess(&ptr, delpos);
                }

            }
        }
    }
}

void FreqPrint(bt_node* root, int k)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        int i;
        for(i=0;i<(ptr->count);i++)
        {
            FreqPrint(ptr->down[i],k);
            if(i<ptr->count-1)
            {
                if(ptr->wcount[i]>k)
                {
                    printf("%s : ",ptr->data[i]);
                    printf("%d\n",ptr->wcount[i]);
                }

            }
        }
    }
}

void Rain(bt_node* root, char l[], char u[])
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        int i;
        for(i=0;i<(ptr->count);i++)
        {
            Rain(ptr->down[i],l,u);
            if(i<ptr->count-1)
            {
                if(strcmp(ptr->data[i],l)==1&&strcmp(ptr->data[i],u)==(-1))
                {
                    printf("%s : ",ptr->data[i]);
                    printf("%d\n",ptr->wcount[i]);
                }

            }
        }
    }
}

void RainPrint(bt_node* root, char l[], char u[])
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        bt_node* lptr=root;
        bt_node* uptr=root;
        if(FindNode(&lptr,l)&&FindNode(&uptr,u))
        {
            Rain(ptr,l,u);
            //printf("\nhi\n");

        }
        else
        {
            printf("\nWords Dont Exist\n");
        }
    }
}

bool FindNode(bt_node**root, char d[])
{
    bt_node* ptr=*root;
    bool found=0;
    int i;
    while(ptr!=NULL&&!found)
    {
        for(i=0;i<ptr->count-1&&strcmp(d,ptr->data[i])==1;i++)
        {
        }
        if(strcmp(d,ptr->data[i])==0)
        {
            found=1;
        }
        if(!found)
        {
            ptr=ptr->down[i];
        }
    }
    if(found)
    {
        *root=ptr;
    }
    return found;
}

bt_node* FindRoot(bt_node* root)
{
    bt_node*ptr=root;
    while(ptr->parent!=NULL)
    {
        ptr=ptr->parent;
    }
    return ptr;
}

bt_node* DeleteBTreeNode(bt_node* root, char d[])
{
    bt_node* ptr=root;
    bool found=0;
    found=FindNode(&ptr,d);
    if(!found)
    {
        printf("\nData Not found\n");
    }
    else
    {
        int i;
        for(i=0;strcmp(ptr->data[i],d)==(-1);i++)
        {
        }
        AdjustBTreeNode(&ptr,i);
    }
    ptr=FindRoot(ptr);
    return ptr;
}

void PrintBTreeNode(bt_node* root)
{
    bt_node* ptr=root;
    printf("\n");
    if(ptr==NULL)
    {
        printf("\nBTree is NULL\n");
    }
    else if(ptr->count!=0)
    {
        if(ptr->parent==NULL)
        {
            printf("\nPARENT: NULL\n");
        }
        else
        {
            printf("\nPARENT: xxxx\n");
        }
        printf("COUNT : %d\n",ptr->count);
        int i;
        for(i=0;i<ptr->count-1;i++)
        {
            printf("%s  : ",ptr->data[i]);
            printf("%d ,  ",ptr->wcount[i]);
        }
        printf("\n");
        for(i=0;i<ptr->count;i++)
        {
            if(ptr->down[i]==NULL)
            {
                printf("NULL, ");
            }
            else
            {
                printf("xxxx, ");
            }
        }
    }
}

bt_node* InitBTree(bt_node* root)
{
    char d[n];
    FILE* fptr=fopen("data.txt","r");
    fseek(fptr,0,SEEK_END);
    unsigned int len=ftell(fptr);
    if(len>0)
    {
        rewind(fptr);
        while(!feof(fptr))
        {
            fscanf(fptr,"%s",&d);
            root=InsertBTree(root,d);
        }
    }
    fclose(fptr);
    return root;
}

void FindMin(bt_node* root, int* minptr)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        int i;
        for(i=0;i<ptr->count;i++)
        {
            FindMin(ptr->down[i],minptr);
            if(i<ptr->count-1)
            {
                if(*minptr>ptr->wcount[i])
                {
                    *minptr=ptr->wcount[i];
                }
            }
        }
    }
}

void PrintMin(bt_node* root, int min)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        int i;
        for(i=0;i<ptr->count;i++)
        {
            PrintMin(ptr->down[i],min);
            if(i<ptr->count-1)
            {
                if(min==ptr->wcount[i])
                {
                    printf("%s: ",ptr->data[i]);
                    printf("%d\n",ptr->wcount[i]);
                }
            }
        }
    }
}

void MinPrint(bt_node* root)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        int min=INT_MAX;
        FindMin(ptr,&min);
        PrintMin(ptr,min);
    }
}
/*
void PrintBTreeStepWise(bt_node*root)
{
    bt_node*ptr=root;
    if(ptr!=NULL)
    {
        printf("\n.ROOT...........................................................................");
        PrintBTreeNode(ptr);
        printf("\n\n\n.LEVEL:1........................................................................");
        PrintBTreeNode(ptr->down[0]);
        printf("\n");
        PrintBTreeNode(ptr->down[1]);
        printf("\n\n\n.LEVEL:2........................................................................");
        PrintBTreeNode((ptr->down[0])->down[0]);
        printf("\n");
        PrintBTreeNode((ptr->down[0])->down[1]);
        printf("\n");
        PrintBTreeNode((ptr->down[0])->down[2]);
        printf("\n");
        PrintBTreeNode((ptr->down[1])->down[0]);
        printf("\n");
        PrintBTreeNode((ptr->down[1])->down[1]);
        printf("\n");
        PrintBTreeNode((ptr->down[1])->down[2]);
        printf("\n................................................................................\n");

    }
}
*/
void FindMaxMin(bt_node* root)
{
    bt_node* ptr=root;
    if(ptr!=NULL)
    {
        bt_node* lptr=ptr;
        bt_node* uptr=ptr;
        if(!IsAllNULL(ptr))
        {
            while(!IsAllNULL(lptr))
            {
                lptr=lptr->down[0];
                uptr=uptr->down[uptr->count-1];
            }
        }
        printf("\nMIN = %s : %d\n",lptr->data[0],lptr->wcount[0]);
        printf("\nMAX = %s : %d\n",uptr->data[uptr->count-2],uptr->wcount[uptr->count-2]);
    }
}

void main()
{
    bt_node* root=NULL;
    root=InitBTree(root);
    printf("\nTHE ORIINAL B-TREE\n");
    PrintBTree(root);
    printf("\nTHE ORIINAL B-TREE STEPWISE\n");
    //PrintBTreeStepWise(root);
    char d[n];
    strcpy(d,"f");
    root=DeleteBTreeNode(root,d);
    printf("\nAFTER DELETING %s\n",d);
    PrintBTree(root);
    char e[n];
    strcpy(d,"a");
    strcpy(e,"t");
    printf("\nWORDS BETWEEN %s AND %s\n",d,e);
    RainPrint(root,d,e);
    int k=1;
    printf("\nWORDS OCCUR MORE THAN %d TIMES\n",k);
    FreqPrint(root,k);
    printf("\nWORDS OCCUR THE LEAST TIMES\n");
    MinPrint(root);
    printf("\nMAX AND MIN OF THE DICTIONARY\n");
    FindMaxMin(root);
}
