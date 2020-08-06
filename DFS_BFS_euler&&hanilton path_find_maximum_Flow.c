#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define SIZE 40

int directed=0;
struct node
{
    int label;
    int weight;
    int flow;
    int free;
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
    newNode->free = weight;
    newNode->flow=0;
    newNode->next = NULL;
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

//******************************//

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
    if(visited[vertex]==0)
    {
        visited[vertex]=1;
        enqueue(Q,vertex);
        while(!isEmpty(Q))
        {
            int currentNode=dequeue(Q);
             printf("%d -> ",currentNode);
            Node* temp=G->adj_list[currentNode];

            while(temp)
            {
                if(visited[temp->label]==0){
                  visited[temp->label]=1;
                  enqueue(Q,temp->label);
                }
                temp=temp->next;
            }
        }
    }
}
//***********End of BFS*******************//

//*******************************//

//***********Hamiltonian Path*******************//
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
int checkPath(int *path,int n,Graph* G)
{
    for (int i=0; i<n-1;i++)
    {
        if(!isNeighbor(G,path[i],path[ i+1]))
        {
            return 0;
        }
    }
    return 1;
}
void print(int *num, int n)
{
 int i;
 for ( i = 0 ; i < n ; i++)
    printf("%d ", num[i]);
   printf("\n");
}
void swap (int *x, int *y)
{   int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void permute (int *a, int i, int n,Graph* G)
{
    int j;
    if (i == n){
       if(checkPath(a,n,G) )
        {print (a, n);}
    }
    else
        for (j = i; j < n; j++) {
            swap ((a+i),(a+j));
            permute (a, i+1, n,G);
            swap ((a+i), (a+j));
        }
}
void hamiltonianPath(Graph* G)
{
    int path[G->num_vertices];
    int temp;
    for(int i=0;i<G->num_vertices;i++)
    {
        path[i]=i;
    }
   // printf("Hamiltonian Path : ");
    permute (path, 1, G->num_vertices,G);
}
//***********End of Hamiltonian Path*******************//

//******************************//

//***********Print the graph*******************//
void printGraph(Graph* G)
{
    int v;
    for (v = 0; v < G->num_vertices; v++)
    {
        Node* temp = G->adj_list[v];
        printf("%d = ", v);
        while (temp)
        {
            printf("%d weights %d flow %d -> ", temp->label,temp->weight,temp->flow);
            temp = temp->next;
        }
        printf("\n");
    }
}
//***********End of Printing the graph*******************//

//****************************************//

//***********Find Maximum Flow*******************//
int available(Graph* G, int src, int dest)
{
    Node* tempList= G->adj_list[src];
    while(tempList != NULL)
    {
        if(tempList->label == dest)
            return tempList->free;
        tempList=tempList->next;

    }
    return 0;
}

int min_free(int *weights , int  n)
{
    int min=INT_MAX;
   for(int i=0;i<n;i++)
   {
       //printf("%d  ",weights[i]);
       if(weights[i]<min){
        min=weights[i];
       }
   }

  // printf("Max weight: %d using : ",min);
    return min;
}
void permuteMaxFlow (int *a, int i, int n,Graph* G,int des)
{
    int j;
    if (i == n-1){
        int free[n-1];
        int max_flow=0;
        a[n-1]=des;
       if(checkPath(a,n,G))
        {
            for (int j=0; j<n-1; j++)
			{
			    free[j]=available(G,a[j],a[j+1]);
            }
            max_flow=min_free(free,i);
            if(max_flow >0)
            {
                printf("max flow: %d using the following path:  ",max_flow);

                for(int i=0;i<n-1;i++)
                {
                    changeFlow(G,a[i],a[i+1] ,max_flow);
                }
                print (a, n);
            }
        }
    }
    else
        for (j = i; j < n-1; j++) {
            swap ((a+i),(a+j));
            permuteMaxFlow (a, i+1, n,G,des);
            swap ((a+i), (a+j));
        }
}


void pathFinder(Graph* G, int src , int des , int nodes[] )
{
    int start=1;
    for(int n=2; n<G->num_vertices;n++){
        int data[n];
        data[0]=src;
        combinationUtil(nodes, data,start ,G->num_vertices-1, 1, n,des,G);
    }
    permuteMaxFlow (nodes, 1,G->num_vertices,G,des);

}
void changeFlow(Graph* G,int src, int dest,int max_flow)
{
    Node* tempList= G->adj_list[src];
    while(tempList!= NULL)
    {
        if(tempList->label == dest  &&  free>max_flow)
        {
            tempList->flow+=  max_flow;
            tempList->free-= max_flow;
        }
    tempList=tempList->next;
    }
}

void combinationUtil(int arr[], int data[], int start, int end, int index, int r ,int des,Graph *G)
{
	if (index == r-1)
	{
	    int free[r-1];
	    data[index]=des;
	    int max_flow=0;

        if(checkPath(data,r,G))
            {
                for (int j=0; j<r-1; j++)
                {
                    free[j]=available(G,data[j],data[j+1]);
                }
                max_flow=min_free(free,index);
                if(max_flow >0)
                {
                    printf("max flow: %d using the following path: ",max_flow);
                    for(int i=0;i<r-1;i++)
                    {
                        changeFlow(G,data[i],data[i+1] ,max_flow);
                    }
                    print (data, r);
                }
           }
        }
	for (int i=start; i<end && end-i+1 >= r-index; i++)
	{
		data[index] = arr[i];
		//permute the elements of new  matrix
		permuteMaxFlow (data, i, r,G, des);
       combinationUtil(arr, data, i+1, end, index+1, r,des,G);

	}
}

int maxFlow(Graph* G,int src,int des , int directed)
{
    int nodes[G->num_vertices];
    for(int i=0;i<G->num_vertices;i++)
    {
        nodes[i]=i;
    }
    int d[G->num_vertices];
    int n=1;
    if(!directed  && src> des)
    {
        int temp=src;
        src=des;
        des=temp;
    }
    d[0] = src;
    d[G->num_vertices-1] = des;
    for(int j=0;j<G->num_vertices;j++)
    {
        if(nodes[j]!=src && nodes[j]!=des)
        {
            d[n]=nodes[j];
            n++;
        }
    }
    pathFinder(G,src ,des, d);
    printGraph(G);
    Node* tmp= G->adj_list[src];
    int flow=0;
    while(tmp)
    {
        flow+=tmp->flow;
        tmp=tmp->next;
    }
return flow;

//***********End of Finding the Maximum Flow*******************//

//******************************//

//***********Store the adjacency Matrix*******************//
}
void AdjMatris(Graph* G)
{
    int i,j;
    FILE *fp = fopen("output.txt","w");
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

//***********Eulerian Path*******************//
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
int ConnectedComponent(Graph* G)
{
    int visited[G->num_vertices];
    int i;
    int noc = 1;
    int visitCounter=0;
    for(i=0;i<G->num_vertices;i++)
        visited[i] = 0;
    DFS(G,0,visited,0);
    for(i=0;i<G->num_vertices;i++)
        if(visited[i] == 1)
            visitCounter++;
    while(visitCounter != G->num_vertices)
    {
        for(i=0;i<G->num_vertices;i++)
            if(visited[i] == 0)
            {
                DFS(G,i,visited,0);
                break;
            }
        visitCounter=0;
        for(i=0;i<G->num_vertices;i++)
        if(visited[i] == 1)
            visitCounter++;
        noc++;
    }
    //printf("%d \n",noc);
    return noc;
}
int delete_edge(int first, int second, Graph *G)
{
    Node* temp=G->adj_list[first];
    int weight=0;
    if(temp!=NULL){
    if(temp->label==second){
        weight=temp->weight;
        G->adj_list[first]=temp->next;
    }
    else{
        Node* prev=temp;
        temp=temp->next;
        while(temp->next!= NULL)
        {
            if(temp->label==second)
            {
               weight=temp->weight;
                prev->next=temp->next;
                temp->next=NULL;
            }
            else
            {
               temp=temp->next;
                prev=prev->next;
            }
        }
        //temp is last node
        if(prev->next->label==second)
        {
            weight=prev->next->weight;
            prev->next=NULL;
        }
        free(temp);
    }
    }
   // printf("end deletion\n");
    return weight;
}
int isBridge(int src, int des,Graph* G)
{
    int weight=0;
    weight=delete_edge(src,des,G);
    if(directed==0){
        delete_edge(des,src,G);
    }
    //undirected graph
    if(ConnectedComponent(G)==1)
    {
        add_edge(G,src,des,directed,weight);
        return 0;
    }
    else{
        add_edge(G,src,des,directed,weight);
        return 1;
    }
}
int findStartVertex(Graph* G)
{
    for(int i=0;i<G->num_vertices;i++)
    {
        if(degree(i,G)%2!= 0)
        {
            return i;
        }
    }
    return 0;
}
int checkEulerianPath(Graph* G)
{
    int oddVertices =0;
    for(int i=0;i<G->num_vertices;i++)
    {
        if(degree(i,G)%2 != 0)
        {
            oddVertices++;
        }
    }
    if(oddVertices==2 || oddVertices==0 )
    {
        printf("odd Vertices number : %d\n",oddVertices);
        return 1;
    }
    return 0;
}
void PathFleuryAlgorithm(int start ,  Graph* G)
{

        Node* tmp= G->adj_list[start];
            if(! isBridge(start,tmp->label,G) && degree(start,G)>1)
            {
                printf("%d -> ",tmp->label);
                delete_edge(start, tmp->label, G);
                delete_edge(tmp->label, start, G);
                PathFleuryAlgorithm(tmp->label, G);
            }
            else if( degree(start,G)==1)
            {
                //neighbor :
                printf("%d ->",tmp->label);
                delete_edge(start, tmp->label, G);
                delete_edge(tmp->label, start, G);
                if( G->adj_list[tmp->label]  !=NULL){
                PathFleuryAlgorithm(tmp->label, G);
                }
            }
            else
            {
                int tmpLabel;
                //when the degree is more than 1 and the edges are bridge if all edges are bridges we will select last edge
                while( tmp != NULL)
                {

                if(tmp->next->next ==NULL )
                    {
                        tmpLabel=tmp->label;
                        break;
                    }
                    else if(! isBridge(start,tmp->label,G) && degree(start,G)>1)
                    {
                        tmpLabel=tmp->label;
                        break;
                    }
                    tmp=tmp->next;
                }
                printf("%d ->",tmpLabel);
                delete_edge(start, tmpLabel, G);
                delete_edge(tmpLabel, start, G);
               PathFleuryAlgorithm(tmpLabel , G);
            }
}
void FleuryAlgorithm(Graph* G)
{
    if(checkEulerianPath(G)){
        //printf("\nThe Eulerian Path is : \n");
        int start=findStartVertex(G);
        printf("%d -> ",start);
      PathFleuryAlgorithm(start , G);
    }
    else{
        printf("\nThe graph does not contain Eulerian Path\n");
    }
}
//***********End Of Eulerian Path*******************//


//****************************************//


//***************Number of nodes**************//
int numberOfNodesConnectedComponent(Graph* G)
{
    int visited[G->num_vertices];
    int i;
    int noc = 1;
    int visitCounter=0;
    int nodesCounter=0;
    int newVisited1=0;
    int newVisited2=0;
    for(i=0;i<G->num_vertices;i++)
        visited[i] = 0;
    DFS(G,0,visited,0);
    for(i=0;i<G->num_vertices;i++)
        {
            if(visited[i] == 1)
            {
                  nodesCounter+=1;
                  visitCounter++;
            }
        }
    printf("component %d     has : %d nodes \n",noc,nodesCounter);
    while(visitCounter != G->num_vertices)
    {
        newVisited2= visitCounter;
        newVisited1=0;
        for(i=0;i<G->num_vertices;i++){
            if(visited[i] == 0)
            {
                DFS(G,i,visited,0);
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
        printf("component %d     has : %d nodes \n",noc,nodesCounter);
    }
    return noc;
}
//***********************************//

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
                        add_edge(G,i,next,0,0);
                    }
                    else{
                        if( i>= next-r && i<=next+r) {
                        add_edge(G,i,next,0,0);
                       }
                    }
            }
        }

}
Graph* createHararyGraph(int k, int n)
{
    int r =  k / 2;
    Graph* G = CreateNullGraph(n);
    int evenK;
    if(k%2==0)
        evenK=k;
    else
        evenK=k-1;

    twoEvenDim(evenK,n, G);
    if(n%2==0 )
    {
         if(k%2 !=0)
        {
                for(int i=0;i<=(n/2);i++)
              {
                  add_edge(G,i,i+(n/2),0,0);
              }
        }
    }
    else if(n%2!= 0)
        {
            add_edge(G,0,n-1,0,0);
            add_edge(G,0,(n-1)/2,0,0);
            add_edge(G,0,(n+1)/2,0,0);
            for(int i=1;i<(n-1)/2;i++)
            {
                add_edge(G,i,i+((n+1)/2),0,0);
            }
        }
    return G;

}


//***********************************//
void main()
{
    int noV = 5;
    directed = 1;
    Graph* G=CreateNullGraph(noV);
    add_edge(G,0,1,directed,10);
    add_edge(G,0,2,directed,20);
    add_edge(G,0,3,directed,15);
    add_edge(G,1,2,directed,20);
    add_edge(G,2,4,directed,20);
    add_edge(G,3,4,directed,18);
    //createHararyGraph
    //Graph* G = createHarayGraph(6,8);
    //number of nodes in each component
//    printf("%d\n",numberOfNodesConnectedComponent(G));
     AdjMatris(G);
     int visited[noV];
    for(int i=0;i<noV;i++)
    {
        visited[i]=0;
    }
     printf("****** DFS ******\n");
     DFS(G, 0 ,visited,1);
     printf("\n\n**********************************************\n\n");
     printf("****** BFS ******\n");
     for(int i=0;i<noV;i++)
     {
        visited[i]=0;
      }
      Queue* Q= CreateNullQueue();
      BFS(G, 0 , visited,Q);
     printf("\n\n**********************************************\n\n");
     printf("****** Find the Maximum Flow********\n");
     int source, dest;
     printf("Please Enter the source & destination of the flow: ");
     scanf("%d %d",&source,&dest);
     printf("\nThe Maximum flow from %d to %d is :  %d\n",source,dest, maxFlow(G,source,dest,directed));
     printf("\n\n**********************************************\n\n");
     printf("****** Hamiltonian Paths ********\n");
     hamiltonianPath(G);
     printf("\n\n**********************************************\n\n");
     printf("****** Eulerian Path ********\n");
     FleuryAlgorithm(G);
      printf("\n\n**********************************************\n\n");

 }

