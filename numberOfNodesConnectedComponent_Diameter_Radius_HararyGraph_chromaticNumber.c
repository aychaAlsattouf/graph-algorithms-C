#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<math.h>
#define SIZE 100
struct node
{
    int label;
    int weight;
    int distance;
    int color;
    struct node* next;
};
typedef struct node Node;

struct graph
{
    int num_vertices;
    Node** adj_list;
};

typedef struct graph Graph;

struct queue {
    int items[SIZE];
    int front;
    int tail;
};
typedef struct queue Queue;
Node* createNode(int v,int weight)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->label = v;
    newNode->weight = weight;
    newNode->next = NULL;
    newNode->color=0;
    newNode->distance=0;
    return newNode;
}

Queue* CreateNullQueue()
{
    Queue* q= malloc(sizeof(Queue));
    q->front=-1;
    q->tail=-1;
    return q;
}
Graph* CreateNullGraph(int vertices)
{

    Graph* G = malloc(sizeof(Graph));
    G->num_vertices = vertices;

    G->adj_list = malloc(vertices * sizeof(Node*));

    int i;
    for (i = 0; i < vertices; i++) {
        G->adj_list[i] = NULL;
    }
    return G;
}

void add_edge(Graph* G, int src, int dest,int directed, int weight)
{
    Node* newNode = createNode(dest,weight);
    newNode->next = G->adj_list[src];
    G->adj_list[src] = newNode;

    if(!directed)
    {
        Node* srcNode = createNode(src,weight);
        srcNode->next = G->adj_list[dest];
        G->adj_list[dest] = srcNode;
    }
}



//***********DFS*******************//
void DFS(Graph* G, int vertex ,int visited[], int print)
{
    if(visited[vertex] == 0 )
    {
        if(print)
        {
            printf("%d -> ",vertex);
        }
        visited[vertex] = 1;
        Node* tmp = G->adj_list[vertex];
        while(tmp != NULL)
        {
            DFS(G,tmp->label,visited,print);
            tmp = tmp->next;
        }
    }

}
//***********End of DFS*******************//


//***************Number of nodes**************//
int numberOfNodesConnectedComponent(Graph* G)
{
    int visited[G->num_vertices];
    int visited2[G->num_vertices];
    int i;
    int noc = 1;
    int visitCounter=0;
    int nodesCounter=0;
    int newVisited1=0;
    int newVisited2=0;
    for(i=0;i<G->num_vertices;i++)
    {
          visited[i] = 0;
          visited2[i]=0;
    }
    DFS(G,0,visited,0);
    printf("component %d   :    ",noc);
    for(i=0;i<G->num_vertices;i++)
        {
            if(visited[i] == 1)
            {
                visited2[i] = 1;
                printf("%d    ",i);
                  nodesCounter+=1;
                  visitCounter++;
            }
        }
    printf("\nnumber of nodes in %d component are : %d\n",noc,nodesCounter);
    while(visitCounter != G->num_vertices)
    {
        newVisited2= visitCounter;
        newVisited1=0;
         printf("component %d   :    ",noc+1);
        for(i=0;i<G->num_vertices;i++){
            if(visited[i] == 0)
            {
                DFS(G,i,visited,0);
                for (int m=0;m<G->num_vertices;m++)
                {
                    if(visited[m]==1 && visited2[m]==0)
                    {
                          printf("%d  ",m);
                          visited2[m]=1;
                    }

                }
                for(i=0;i<G->num_vertices;i++)
                {
                    if(visited[i] == 1)
                    {
                          newVisited1++;
                    }
                }
                break;
                }
            }
        nodesCounter=newVisited1-newVisited2;
        visitCounter=0;
        for(i=0;i<G->num_vertices;i++)
        if(visited[i] == 1)
        {
            visitCounter++;
        }

        noc++;
        printf("\nnumber of nodes in %d component are : %d\n",noc,nodesCounter);

    }
    return noc;
}
//***********************************//

//***********Store the adjacency Matrix*******************//
int isNeighbor(Graph* G, int src, int dest)
{
    Node* tempList= G->adj_list[src];
    while(tempList != NULL)
    {
        if(tempList->label == dest)
            return tempList->weight;
        tempList=tempList->next;

    }
    return 0;
}
void AdjMatris(Graph* G)
{
    int i,j;
    FILE *fp = fopen("AdjMatris.txt","w");
    for(i=0;i<G->num_vertices;i++)
    {
        for(j=0;j<G->num_vertices;j++)
        {
            fprintf(fp,"%d ",isNeighbor(G,i,j));
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}
//***********End of Storing the adjacency Matrix *******************//

//******************************//

//********************* radius and diameter  ************//

//*************BFS***********************//
int isEmpty(struct queue* q) {
    if(q->tail == -1)
        return 1;
    else
        return 0;
}

void enqueue(struct queue* q, int value){
    if(q->tail == SIZE-1)
        printf("\nQueue is Full!!");
    else {
        if(q->front == -1)
            q->front = 0;
        q->tail++;
        q->items[q->tail] = value;
    }
}
int dequeue(struct queue* q){
    int item;
    if(isEmpty(q)){
        printf("Queue is empty");
        item = -1;
    }
    else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->tail){
           // printf("Resetting queue");
            q->front = q->tail = -1;
        }
    }
    return item;
}
void BFS(Graph* G, int vertex, int visited[], Queue* Q)
{
    //printf("vertex %d\n",vertex);
    if(visited[vertex]==0)
    {
        visited[vertex]=1;
        enqueue(Q,vertex);
        int node=1;
        while(!isEmpty(Q))
        {
            int currentNode=dequeue(Q);
             //printf("%d -> ",currentNode);
             Node* nodeCurrentNode=G->adj_list[currentNode];
            Node* temp=G->adj_list[currentNode];

            while(temp)
            {
                if(visited[temp->label]==0){
                  visited[temp->label]=1;
                  G->adj_list[temp->label]->distance=nodeCurrentNode->distance+1;
                //  node+=1;
                  enqueue(Q,temp->label);
                }
                temp=temp->next;
            }
        }
    }
   // return 0;
}
//***********End of BFS*******************//
int maximum_distance(Graph* G)
{
    int max=0;
    for(int i=0;i<G->num_vertices;i++)
    {
        if(max < G->adj_list[i]->distance)
        {
            max = G->adj_list[i]->distance;
        }
    }
    //printf("maximum %d\n",max);
    return max;
}

int max(int arr[] , int n)
{
    int max=0;
    for (int i =0;i<n;i++)
    {
        if(arr[i]>max)
        {
            max= arr[i];
        }
    }
    return max;
}

int min(int arr[] , int n)
{
    int min=INT_MAX;
    for (int i =0;i<n;i++)
    {
        if(arr[i]<min)
        {
            min= arr[i];
        }
    }
    return min;
}
void radius_diameter_finder(Graph* G){
    int visited[G->num_vertices];
    int distances[G->num_vertices];
     printf("*****Max distance for each node*******\n");
     for(int i=0;i<G->num_vertices;i++)
     {
        visited[i]=0;
        distances[i]=0;
      }
      Queue* Q= CreateNullQueue();
      for( int j =0;j<G->num_vertices;j++)
      {
        for(int i=0;i<G->num_vertices;i++)
         {
             G->adj_list[i]->distance=0;
            visited[i]=0;
          }
           BFS(G, j , visited,Q);
            distances[j]=maximum_distance(G);
            printf("for node %d    max_distance is %d\n",j,distances[j]);
      }
      int radius = min(distances,G->num_vertices);
      int diameter = max(distances,G->num_vertices);
      printf("Graph Diameter is :%d\nGraph Radius is : %d\n",diameter,radius);

}





//****************************************//
//***************create Harary Graph********************//

void twoEvenDim(int k,int n, Graph* G)
{
     int r =  k / 2;
        for(int i =n-1;i>=0;i--)
        {
                for(int j=i+1 ;j<=i+r ;j ++)
                {
                    int next= j;
                    if(j > n-1)
                  {
                        next=j-n;
                        add_edge(G,i,next,0,1);
                    }
                    else{
                        if( i>= next-r && i<=next+r) {
                        add_edge(G,i,next,0,1);
                       }
                    }
            }
        }

}
Graph* createHararyGraph(int k, int n)
{
    int r =  k / 2;
    Graph* G = CreateNullGraph(n);
    twoEvenDim(k,n, G);
    if(n%2==0 )
    {
         if(k%2 !=0)
        {
                for(int i=0;i<(n/2);i++)
              {
                  add_edge(G,i,i+(n/2),0,1);
              }
        }
    }
    else if(n%2!= 0)
        {
            add_edge(G,0,n-1,0,1);
            add_edge(G,0,(n-1)/2,0,1);
            add_edge(G,0,(n+1)/2,0,1);
            for(int i=1;i<(n-1)/2;i++)
            {
                add_edge(G,i,i+((n+1)/2),0,1);
            }
        }
    return G;

}

void print(int *num, int n)
{
 int i;
 for ( i = 0 ; i < n ; i++)
    printf("%d ", num[i]);
   printf("\n");
}

//***********************************//

int degree(int vertex, Graph* G)
{
    Node* temp=G->adj_list[vertex];
    int count=0;
    while(temp)
    {
        count++;
        temp=temp->next;
    }
    return count;
}

//***********Print the graph*******************//
void printGraph(Graph* G)
{
    int v;
    for (v = 0; v <G->num_vertices; v++)
    {
        Node* temp = G->adj_list[v];
        printf("%d = ", v);
        while (temp)
        {
            printf("%d color %d  ", temp->label,temp->color);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_node_color(Graph* G)
{
    for (int i=0;i<G->num_vertices;i++)
    {
        Node* temp=G->adj_list[i];
        Node* ne=G->adj_list[temp->label];
        while(ne)
        {
            if(ne->label==i)
            {
                printf("node: %d\t has color: %d\n",ne->label,ne->color);
            }
            ne=ne->next;
        }
    }
}
//***********End of Printing the graph*******************//

int calc_chromatic_number(int k, int n)
{
    Graph* G = createHararyGraph(k,n);
    int colors[G->num_vertices];
    int colored[G->num_vertices];
    int assigned[G->num_vertices];
    int color_counter=0;
    for(int i=0;i<G->num_vertices;i++)
    {
        colors[i]=i;
        colored[i]=0;
        assigned[i]=0;
    }
    for(int i=0;i<G->num_vertices;i++)
    {
        if(i==0)
        {
            assigned[i]=1;
            colored[i]=colors[i];
        }
        else{
            color_counter=0;
            for(int j=0;j<i;j++)
            {
                colored[i]=colors[color_counter];
                if(isNeighbor(G, i,j) && colored[i]==colored[j])
                {
                    color_counter+=1;
                }
                if(assigned[color_counter]==0)
                {
                    assigned[color_counter]=1;
                }
                 colored[i]=colors[color_counter];
            }
        }
    }
    int c=0;
    for(int i=0;i<G->num_vertices;i++)
    {
        if(assigned[i]==1)
        {
            c+=1;
        }
        printf("%d    colored with     %d\n",i,colored[i]);
    }
    return c;
}
//**************************************************8//
int max_index(int arr[],int n)
{
    int max=0;
    int max_index;
    for (int i=0;i<n;i++)
    {
        if(arr[i]>max)
        {
            max=arr[i];
            max_index=i;
        }
    }
    return max_index;
}

void repeated_color(int colors[],int n ,int repeated[])
{
    for(int i=0;i<n;i++)
    {
        int count=0;
        for(int j=0;j<n;j++)
        {
            if(colors[i]==colors[j] && colors[i]!=0)
            {
                count+=1;
            }
        }
        for(int m=0;m<n;m++)
        {
            if(colors[i]==colors[m])
            {
                repeated[m]=count;
            }
        }
    }
}
void vertex_color(Graph* G,int vertex,int n, int color)
{
    //printf("%d\t",temp->label);
    for(int k=0;k<G->num_vertices;k++){
    Node* temp= G->adj_list[k];
    while(temp)
    {
        if(temp->label == vertex)
        {
            temp->color = color;
        }
        temp=temp->next;
    }
    }

}

int exist(int arr[],int n,int integer)
{
    for (int m=0;m<n;m++)
    {
        if(arr[m]==integer)
        {
            return m;
        }
    }
    return -1;
}


int check_min_unsigned(int colors[],int assigned_color[], int currently_assigned[],int repeated[],int n,Graph* G)
{
    int min_repeated=INT_MAX;
    int index=-1;
    for(int m=0;m<n;m++)
    {
        if(assigned_color[m] != 1 && colors[m]!=0 && exist(currently_assigned,G->num_vertices,colors[m])==-1)
        {
            if(repeated[m]<min_repeated)
            {
                min_repeated=repeated[m];
                index=m;
            }
        }
    }
    return index;
}
void assign_color(Graph* G,int current_index,int colors[],int assigned_color[], int *colorCounter)
{
     int currently_assigned[G->num_vertices];
     int counter=0;
     int repeated[G->num_vertices];
     for(int j=0;j<G->num_vertices;j++)
     {
         currently_assigned[j]=0;
         repeated[j]=0;
     }
    Node* temp=G->adj_list[current_index];
    while(temp)
    {
        if(colors[temp->label] != 0)
        {
            assigned_color[temp->label] = 1;
            if(exist(currently_assigned,G->num_vertices,colors[temp->label]) == -1)
            {
            currently_assigned[counter]=colors[temp->label];
            counter+=1;
            }

        }
        temp=temp->next;
    }

    Node* t=G->adj_list[current_index];
    repeated_color(colors,G->num_vertices ,repeated);

    //printf("currently_assigned\n");
    //print(currently_assigned,G->num_vertices);
    //print(repeated,G->num_vertices);
    //print(colors,G->num_vertices);
    //printf("assigned_color\n");
    //print(assigned_color,G->num_vertices);

/*
    //without selecting the minimum color

    for(int m=0;m<G->num_vertices;m++)
    {
       // printf(" i    %d\n",m);

        if(assigned_color[m]!= 1 && colors[m]!=0 && exist(currently_assigned,G->num_vertices,colors[m])==-1)
        {
            printf("colors: %d\t***********current node: %d\n",colors[m],current_index);
            colors[current_index]=colors[m];
            vertex_color(G,current_index,t->label,colors[m]);
            printf("exist color %d\n",colors[m]);
            return;
        }
       }
       */
       //with selecting the minimum color
       int index=check_min_unsigned(colors,assigned_color,currently_assigned,repeated,G->num_vertices,G);
       if(index !=-1)
        {
            colors[current_index]=colors[index];
            vertex_color(G,current_index,t->label,colors[index]);
            return;
        }
        else{
        *colorCounter+=1;
        vertex_color(G,current_index,t->label,*colorCounter);
        colors[current_index] = *colorCounter;
        return;
        }

}

int calc_chromatic_number_2(int k,int n)
{
     Graph* G = createHararyGraph(k,n);
     int degrees[G->num_vertices];
     int current_index;
     int color_counter=0;
     int colors[G->num_vertices];
      int assigned_color[G->num_vertices];
     for (int i=0; i< G->num_vertices;i++)
     {
         degrees[i]=degree(i,G);
         colors[i]=0;
     }
     for(int i=0;i<G->num_vertices;i++)
     {
         current_index= max_index(degrees,G->num_vertices);
          for(int k=0;k<G->num_vertices;k++)
            {
                     assigned_color[k] = -1;
            }
             assign_color(G,current_index,colors,assigned_color , &color_counter);
        degrees[current_index]=0;
     }
     print_node_color( G);
     return color_counter;

}
//****************************************************//


int main()
{
      int k,n;
    printf("\n***********Please enter k and n of the Harary Graph:************\n");
    scanf("%d %d",&k,&n);
    Graph* G = createHararyGraph(k,n);
    printf("***************************************************\n");
   printf("\nchromatic number of(%d,%d) Harary graph is %d\n",k,n, calc_chromatic_number(k,n));
   printf("***************************************************\n");
   printf("\ncalculating the chromatic number using another algorithm\n\n");
   printf("\nchromatic number of(%d,%d) Harary graph  is %d\n",k,n, calc_chromatic_number_2(k,n));
    printf("***************************************************\n");
    AdjMatris(G);


   //radius and diameter of graph
    int noV=10;
    Graph* arbitrary_G=CreateNullGraph(noV);
   add_edge(arbitrary_G,0,1,0,1);
    add_edge(arbitrary_G,1,2,0,1);
    add_edge(arbitrary_G,1,7,0,1);
    add_edge(arbitrary_G,2,3,0,1);
    add_edge(arbitrary_G,2,4,0,1);
    add_edge(arbitrary_G,2,8,0,1);
    add_edge(arbitrary_G,5,6,0,1);
    add_edge(arbitrary_G,6,7,0,1);
    add_edge(arbitrary_G,7,8,0,1);
    add_edge(arbitrary_G,8,9,0,1);
    add_edge(arbitrary_G,7,8,0,1);
    add_edge(arbitrary_G,7,9,0,1);
    printf("\n***********Radius and Diameter Finder for the arbitrary Graph************\n");
    radius_diameter_finder(arbitrary_G);
    printf("\n");
     printf("***************************************************\n\n");

     //print the nodes inside each component
  Graph* separated_G=CreateNullGraph(noV);
   add_edge(separated_G,0,1,0,1);
    add_edge(separated_G,2,3,0,1);
    add_edge(separated_G,4,5,0,1);
    add_edge(separated_G,6,7,0,1);
    add_edge(separated_G,6,8,0,1);

    printf("\n***********Printing the nodes in each component in separated Graph************\n");
    numberOfNodesConnectedComponent(separated_G);
    printf("\n");
     printf("***************************************************\n\n");

}
