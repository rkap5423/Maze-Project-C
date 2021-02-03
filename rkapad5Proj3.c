#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct mazeStruct
{
    int xsize, ysize;
    int xstart, ystart;
    int xend, yend;
} maze;

#define TRUE 1
#define FALSE 0
int debugMode=FALSE;
int size=0; //number of nodes in linked list


/* A linked list node */
typedef struct Node //Replace all comments in node functions------------------------------------
{
    // Any data type can be stored in this node
    int  xcoord;
    int ycoord;

    struct Node *next;
} Node;

void init(Node* head){ //function to initialize linked list to default node
    head->next = NULL;
    head->xcoord=0;
    head->ycoord=0;
}
/* Function to add a node at the beginning of linked list.*/
void push(struct Node** head, int newx,int newy)
{
    // Allocate memory for new node
    Node* new_node = (Node *) malloc(sizeof(Node));

    new_node->xcoord = newx;
    new_node->ycoord = newy;
    new_node->next = *head;
    *head = new_node;
    size++;
}

int isEmpty() //function to check if linked list is empty
{
    if ( size == 0)
        return TRUE;
    else
        return FALSE;
}


// Function to print an integer
void printCoords(Node* head)
{
    Node *temp=NULL; //start new list
    while (head!=NULL){ //copy old list into new list
        push(&temp,head->xcoord,head->ycoord);
        head=head->next;
    }

    while (temp!=NULL){ //print out new list
        if (temp->xcoord!=0 && temp->ycoord!=0)
            printf("(%d,%d)\n",temp->xcoord,temp->ycoord);
        temp=temp->next;
    }

}

int top(Node* head, int topx, int topy ){ //function to return by reference the coords at top of stack/list
    topx=head->xcoord;
    topy=head->ycoord;
}




void pop(Node** head) {//function to remove top element at stack/list
    Node* temp = NULL;

    if (*head == NULL) {
        return;
    }

    temp = (*head)->next;
    free(*head);
    *head = temp;
    size--;
}

void reset(Node** head){ //function to clear list/stack
    if (*head==NULL)
        return;
    while(isEmpty()==FALSE)
        pop(head);

}





int main (int argc, char **argv)
{
    maze m1;

    int xpos, ypos;
    int i,j;

    for (int x=0;x<argc;x++){
        if(strcmp(argv[x], "-d")==0) { //check for debug mode
            debugMode = TRUE;
            printf("Debug mode on\n");
            break;
        }

    }

    FILE *src;

    /* verify the proper number of command line arguments were given */
    if (debugMode==TRUE){
        if(argc != 3) {
            printf("Usage: %s <input file name>\n", argv[0]);
            exit(-1);
        }
    }
    else{
        if(argc != 2) {
            printf("Usage: %s <input file name>\n", argv[0]);
            exit(-1);
        }
    }


    /* Try to open the input file. */
    if ( ( src = fopen( argv[1], "r" )) == NULL && ( src = fopen( argv[2], "r" )) == NULL)
    {
        printf ( "Can't open input file: %s", argv[1] );
        exit(-1);
    }
    int check1=FALSE;
    /* read in the size, starting and ending positions in the maze */
    fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
    while(check1!=TRUE){
        if (m1.xsize>0 && m1.ysize>0) //check if size within bounds
            check1=TRUE;
        else{
            fprintf(stderr,"Invalid: Maze sizes must be greater than 0\n");
            fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
        }
    }
    check1=false;
    fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
    while(check1!=TRUE){
        if (m1.xstart<=m1.xsize && m1.ystart<=m1.ysize && m1.xstart>0 && m1.ystart>0) //check if starting position within bounds
            check1=TRUE;
        else{
            if(m1.xstart>m1.xsize||m1.xstart<=0){
                fprintf(stderr,"Invalid:Row %d is outside of range from 1 to %d\n",m1.xstart,m1.xsize);
            }
            if(m1.ystart>m1.ysize||m1.ystart<=0){
                fprintf(stderr,"Invalid:Column %d is outside of range from 1 to %d\n",m1.ystart,m1.ysize);
            }
            fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
        }
    }

    fscanf (src, "%d %d", &m1.xend, &m1.yend);
    check1=false;
    while(check1!=TRUE){
        if (m1.xend<=m1.xsize && m1.yend<=m1.ysize && m1.xend>0 && m1.yend>0) //check if end position within bounds
            check1=TRUE;
        else{
            if(m1.xend>m1.xsize||m1.xend<=0){
                fprintf(stderr,"Invalid:Row %d is outside of range from 1 to %d\n",m1.xend,m1.xsize);
            }
            if(m1.yend>m1.ysize||m1.yend<=0){
                fprintf(stderr,"Invalid:Column %d is outside of range from 1 to %d\n",m1.yend,m1.ysize);
            }
            fscanf (src, "%d %d", &m1.xend, &m1.yend);
        }
    }



    /* print them out to verify the input */
    printf ("size: %d, %d\n", m1.xsize, m1.ysize);
    printf ("start: %d, %d\n", m1.xstart, m1.ystart);
    printf ("end: %d, %d\n", m1.xend, m1.yend);


    char ** arr = (char **) malloc(sizeof(char *)*(m1.xsize+2)); //allocate new 2d array, maze
    for(int f = 0; f < m1.xsize+2; f++)
    {
        arr[f] = (char *) malloc(sizeof(char)*(m1.ysize+2));
    }


    /* initialize the maze to empty */
    for (i = 0; i < m1.xsize+2; i++)
        for (j = 0; j < m1.ysize+2; j++)
            arr[i][j] = '.';

    /* mark the borders of the maze with *'s */
    for (i=0; i < m1.xsize+2; i++)
    {
        arr[i][0] = '*';
        arr[i][m1.ysize+1] = '*';
    }
    for (i=0; i < m1.ysize+2; i++)
    {
        arr[0][i] = '*';
        arr[m1.xsize+1][i] = '*';
    }

    /* mark the starting and ending positions in the maze */
    arr[m1.xstart][m1.ystart] = 's';
    arr[m1.xend][m1.yend] = 'e';

    /* mark the blocked positions in the maze with *'s */
    while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
        if(xpos==m1.xstart && ypos==m1.ystart){
            fprintf(stderr,"Invalid: Attempting to block starting position\n");
        }
        else if (xpos==m1.xend && ypos==m1.yend){
            fprintf(stderr,"Invalid: Attempting to block ending position\n");
        }
        else{
            arr[xpos][ypos] = '*';
        }
    }

    /* print out the initial maze */
    for (i = 0; i < m1.xsize+2; i++)
    {
        for (j = 0; j < m1.ysize+2; j++)
            printf ("%c", arr[i][j]);
        printf("\n");
    }


    Node* head = (Node *) malloc(sizeof(Node)); //allocate linked list/stack
    init(head);//initialize it
    push(&head,m1.xstart,m1.ystart);
    arr[m1.xstart][m1.ystart]='V';

    while(isEmpty()==FALSE){ //loop runs until list/stack is empty
        if (head->xcoord==m1.xend && head->ycoord==m1.yend){ //we found the end
            break;
        }
        else if (arr[head->xcoord][head->ycoord+1]=='.' || arr[head->xcoord][head->ycoord+1]=='e'){ //go right
            if (debugMode==TRUE)
                printf("Pushing (%d,%d)\n",head->xcoord,head->ycoord+1);
            push(&head,head->xcoord,head->ycoord+1);
            arr[head->xcoord][head->ycoord]='V';
        }
        else if (arr[head->xcoord+1][head->ycoord]=='.' || arr[head->xcoord+1][head->ycoord]=='e'){ //go down
            if (debugMode==TRUE)
                printf("Pushing (%d,%d)\n",head->xcoord+1,head->ycoord);
            push(&head,head->xcoord+1,head->ycoord);
            arr[head->xcoord][head->ycoord]='V';
        }
        else if (arr[head->xcoord][head->ycoord-1]=='.' || arr[head->xcoord][head->ycoord-1]=='e'){ //go left
            if (debugMode==TRUE)
                printf("Pushing (%d,%d)\n",head->xcoord,head->ycoord-1);
            push(&head,head->xcoord,head->ycoord-1);
            arr[head->xcoord][head->ycoord]='V';
        }
        else if (arr[head->xcoord-1][head->ycoord]=='.' || arr[head->xcoord-1][head->ycoord]=='e'){ //go up
            if (debugMode==TRUE)
                printf("Pushing (%d,%d)\n",head->xcoord-1,head->ycoord);
            push(&head,head->xcoord-1,head->ycoord);
            arr[head->xcoord][head->ycoord]='V';
        }

        else{
            if (debugMode==TRUE)
                printf("Popping (%d,%d)\n",head->xcoord,head->ycoord);
            pop(&head);
        }
    }
    if (isEmpty()==TRUE){ //maze can't be solved
        printf("The maze has no solution");
    }
    else{ //maze can be solved
        for (i = 0; i < m1.xsize+2; i++) { //print out maze with path shown
            for (j = 0; j < m1.ysize + 2; j++)
                printf("%c", arr[i][j]);
            printf("\n");
        }
        printf("Path taken:\n");
        printCoords(head); //print out coords of path taken
    }

    for(int m = 0; m < m1.xsize+2; m++) //deallocate 2d array
    {
        free(arr[m]);
    }
    free(arr);

    fclose(src);



}