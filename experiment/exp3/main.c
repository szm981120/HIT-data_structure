#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXSIZE 51//最多节点个数
//顶点
struct Node{
    unsigned order;//顶点编号
    int element;//顶点值（权）
};
//边
struct Edge{
    unsigned head;//始顶点
    unsigned tail;//终顶点
};
//以邻接矩阵表示的图
typedef struct{
    int vertex[MAXSIZE];//顶点元素
    int edge[MAXSIZE][MAXSIZE];//邻接矩阵
    int n,e;//顶点数，边数
}MatrixGraph;
//邻接表节点
typedef struct ListNode{
    unsigned adjvex;//该节点编号
    struct ListNode *next;//指向下一个，本首节点的邻接节点
}EdgeNode;
//邻接表首节点
typedef struct{
    int vertex;//元素
    EdgeNode *firstedge;//所邻接的第一个顶点
}VertexNode;
//用邻接表表示的图
typedef struct{
    VertexNode vexlist[MAXSIZE];//顶点集
    int n,e;//顶点数，边数
}ListGraph;
//队列
typedef struct{
    int front;
    int rear;
    int elements[MAXSIZE];
}Queue;
int visited[MAXSIZE];//用于记录遍历过的顶点编号
//函数声明
void menu();//菜单
void ReadFromFile(struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//从文件中读取图的顶点和边
void CreateMGraph(MatrixGraph *mtGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//创建邻接矩阵
void CreateGraph(ListGraph *lGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//创建邻接表
void MatrixToList(MatrixGraph *mtGraph, ListGraph *lGraph);//邻接矩阵转邻接表
void ListToMatrix(ListGraph *lGraph, MatrixGraph *mtGraph);//邻接表转邻接矩阵
void RecMatrixDFSTraverse(MatrixGraph *mtGraph);//递归先深遍历邻接矩阵图
void RecMatrixDFS(MatrixGraph *mtGraph, int i, FILE *fp);//邻接矩阵DFS递归函数
void RecListDFSTraverse(ListGraph *lGraph);//递归先深遍历邻接表图
void RecListDFS(ListGraph *lGraph, int i, FILE *fp);//邻接表DFS递归函数
void MatrixDFSTraverse(MatrixGraph *mtGraph);//非递归先深遍历邻接矩阵图
void ListDFSTraverse(ListGraph *lGraph);//非递归先深遍历邻接表图
int MatrixExistNext(MatrixGraph *mtGraph, int k);//邻接矩阵DFS，判断顶点k有无可遍历的下一个顶点
int ListExistNext(ListGraph *lGraph, int k);//邻接表DFS，判断顶点k有无可遍历的下一个顶点
void MatrixBFSTraverse(MatrixGraph *mtGraph);//非递归遍历先广遍历邻接矩阵图
void MatrixBFS(MatrixGraph *mtGraph, int k, FILE *fp);//非递归邻接矩阵BFS核心函数
void ListBFSTraverse(ListGraph *lGraph);//非递归遍历先广遍历邻接表图
void ListBFS(ListGraph *lGraph, int k, FILE *fp);//非递归邻接表BFS核心函数
void EnQueue(int k, Queue *nodeQueue);//BFS，顶点k入队
int DeQueue(Queue *nodeQueue);//BFS，返回出队顶点编号


void ReadFromFile(struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    FILE *fp;
    char ch;//读文件
    int i = 1;//计数器
    if((fp=fopen("graph.txt","r"))==NULL)exit(1);
    //每次读一个顶点到顶点集，直到第一行顶点读完
    while((fscanf(fp,"%d",&nodeSet[i++].element))==1){
        if((ch = fgetc(fp))=='\n')break;
    }
    nodeSet[0].order = i-1;//顶点总数
    nodeSet[0].element = INT_MAX;//无用
    i = 1;
    //每次读一条边，直到这一行边读完
    while((fscanf(fp,"%u,%u",&edgeSet[i].head,&edgeSet[i].tail))==2){
        i++;
        ch = fgetc(fp);
    }
    edgeSet[0].head = i-1;//边个数
    edgeSet[0].tail = INT_MAX;//无用
    fclose(fp);
}
void CreateMGraph(MatrixGraph *mtGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    mtGraph->n = nodeSet[0].order;//顶点数
    mtGraph->e = edgeSet[0].head;//边数
    //初始化顶点集
    for(int i=1;i<=mtGraph->n;i++)
        mtGraph->vertex[i-1] = nodeSet[i].element;
    //初始化邻接矩阵
    for(int i=0;i<mtGraph->n;i++){
        for(int j=0;j<mtGraph->n;j++)
            mtGraph->edge[i][j] = 0;
    }
    //根据边集更新邻接矩阵
    for(int i=0;i<mtGraph->e;i++)
        mtGraph->edge[edgeSet[i+1].head-1][edgeSet[i+1].tail-1] = 1;
}
void CreateGraph(ListGraph *lGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    EdgeNode *p = NULL;//邻接表新顶点节点
    lGraph->n = nodeSet[0].order;//顶点数
    lGraph->e = edgeSet[0].head;//边数
    //initial顶点集
    for(int i=0;i<lGraph->n;i++){
        lGraph->vexlist[i].vertex = nodeSet[i+1].element;
        lGraph->vexlist[i].firstedge = NULL;
    }
    //只要出现一个邻接于顶点k的顶点，就把这个顶点插入到顶点k的邻接表的首项
    //如果改成无向图，应该把每条边都设计成双向的，也就是注释掉的那四行
    for(int i=0;i<lGraph->e;i++){
//        p = (EdgeNode*)malloc(sizeof(EdgeNode));
//        p->adjvex = edgeSet[i+1].head;
//        p->next = lGraph->vexlist[edgeSet[i+1].tail].firstedge;
//        lGraph->vexlist[edgeSet[i+1].tail].firstedge = p;
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        p->adjvex = edgeSet[i+1].tail-1;
        p->next = lGraph->vexlist[edgeSet[i+1].head-1].firstedge;
        lGraph->vexlist[edgeSet[i+1].head-1].firstedge = p;
    }
}
void MatrixToList(MatrixGraph *mtGraph, ListGraph *lGraph){
    EdgeNode *p = NULL;//邻接表新顶点节点
    lGraph->n = mtGraph->n;
    lGraph->e = mtGraph->e;
    for(int i=0;i<lGraph->n;i++){
        lGraph->vexlist[i].vertex = mtGraph->vertex[i];
        lGraph->vexlist[i].firstedge = NULL;
    }
    for(int i=0;i<lGraph->n;i++){
        for(int j=0;j<lGraph->n;j++){
            if(mtGraph->edge[i][j]==1){//邻接矩阵中的邻接关系，转到邻接表上即可，其余按照邻接表的创建写代码
//                p = (EdgeNode*)malloc(sizeof(EdgeNode));
//                p->adjvex = i;
//                p->next = lGraph->vexlist[j].firstedge;
//                lGraph->vexlist[j].firstedge = p;
                p = (EdgeNode*)malloc(sizeof(EdgeNode));
                p->adjvex = j;
                p->next = lGraph->vexlist[i].firstedge;
                lGraph->vexlist[i].firstedge = p;
            }
        }
    }
}
void ListToMatrix(ListGraph *lGraph, MatrixGraph *mtGraph){
    EdgeNode *p = NULL;//邻接表遍历节点
    mtGraph->n = lGraph->n;
    mtGraph->e = lGraph->e;
    for(int i=0;i<mtGraph->n;i++){
        for(int j=0;j<mtGraph->n;j++){
            mtGraph->edge[i][j] = 0;
        }
    }
    for(int i=0;i<mtGraph->n;i++){
        mtGraph->vertex[i] = lGraph->vexlist[i].vertex;
    }
    for(int i=0;i<lGraph->n;i++){//邻接表中的邻接关系，转到邻接矩阵上即可，其余按照邻接矩阵的创建写代码
        p = lGraph->vexlist[i].firstedge;
        while(p!=NULL){
            mtGraph->edge[i][p->adjvex] = 1;
            p = p->next;
        }
    }
}
void RecMatrixDFSTraverse(MatrixGraph *mtGraph){
    FILE *fp;//遍历结果文件
    //初始化遍历标记
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("RecMatrixDFS.txt","w"))==NULL)exit(1);
    for(int i=0;i<mtGraph->n;i++){//遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
        if(!visited[i]){
            RecMatrixDFS(mtGraph,i,fp);//开始遍历
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
void RecMatrixDFS(MatrixGraph *mtGraph, int i, FILE *fp){
    //遍历当前节点，标记更新
    printf("%d(%d)\t",i+1,mtGraph->vertex[i]);
    fprintf(fp,"%d(%d)\t",i+1,mtGraph->vertex[i]);
    visited[i] = 1;
    //在邻接于该顶点的所有顶点中，如果有一个顶点仍未被遍历过，则遍历那个顶点
    //根据递归思想，可把一个强连通图中的所有顶点遍历完全
    for(int j=0;j<mtGraph->n;j++){
        if((mtGraph->edge[i][j]==1) && !visited[j]){
            fprintf(fp,"<%d,%d>\n",i+1,j+1);
            printf("<%d,%d>\n",i+1,j+1);
            RecMatrixDFS(mtGraph,j,fp);
        }
    }
}
void RecListDFSTraverse(ListGraph *lGraph){
    FILE *fp;
    //初始化遍历标记
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("RecListDFS.txt","w"))==NULL)exit(1);
    for(int i=0;i<lGraph->n;i++){//遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
        if(!visited[i]){
            RecListDFS(lGraph,i,fp);//开始遍历
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
void RecListDFS(ListGraph *lGraph, int i, FILE *fp){
    EdgeNode *p;
    //遍历当前节点，标记更新
    printf("%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
    fprintf(fp,"%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
    visited[i] = 1;
    p = lGraph->vexlist[i].firstedge;
    //搜索当前节点的邻接表，如果存在一个邻接于该顶点的未遍历过的顶点，那么去递归遍历那个顶点
    while(p){
        if(!visited[p->adjvex]){
            fprintf(fp,"<%d,%d>\n",i+1,p->adjvex+1);
            printf("<%d,%d>\n",i+1,p->adjvex+1);
            RecListDFS(lGraph,p->adjvex,fp);
        }
        p = p->next;
    }
}
void MatrixDFSTraverse(MatrixGraph *mtGraph){
    int stack[MAXSIZE];//用栈模拟递归遍历顺序
    int top = -1;//栈顶
    int k,j;//k为下一个节点，j为当前节点
    FILE *fp;//遍历结果文件
    //初始化遍历标记
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("MatrixDFS.txt","w"))==NULL)exit(1);
    //遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
    for(int i=0;i<mtGraph->n;i++){
        if(!visited[i]){
            //遍历当前节点，标记更新
            printf("%d(%d)\t",i+1,mtGraph->vertex[i]);
            fprintf(fp,"%d(%d)\t",i+1,mtGraph->vertex[i]);
            visited[i] = 1;
            //当前节点入栈
            stack[++top] = i;
            //如果当前节点有可以遍历的下一个节点，或者栈不为空，则进入循环，模拟递归
            k = i;
            while((j=MatrixExistNext(mtGraph,k))>=0 || top!=-1){
                if(j>=0){//当前节点有可以遍历的下一个节点
                    //遍历当前节点，标记更新
                    printf("<%d,%d>\n",k+1,j+1);
                    fprintf(fp,"<%d,%d>\n",k+1,j+1);
                    printf("%d(%d)\t",j+1,mtGraph->vertex[j]);
                    fprintf(fp,"%d(%d)\t",j+1,mtGraph->vertex[j]);
                    visited[j] = 1;
                    //该节点入栈
                    stack[++top] = j;
                    //判断新节点
                    k = j;
                }else{
                    //栈顶元素出栈，回溯遍历
                    k = stack[top--];
                }
            }
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
void ListDFSTraverse(ListGraph *lGraph){
    EdgeNode* stack[MAXSIZE];//用栈模拟递归遍历顺序
    int top = -1;//栈顶
    int k,j;//k为下一个节点，j为当前节点
    FILE *fp;//遍历结果文件
    //初始化遍历标记
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("ListDFS.txt","w"))==NULL)exit(1);
    //遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
    for(int i=0;i<lGraph->n;i++){
        if(!visited[i]){
            //遍历当前节点，标记更新
            printf("%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
            fprintf(fp,"%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
            visited[i] = 1;
            //当前节点入栈
            stack[++top] = i;
            //如果当前节点有可以遍历的下一个节点，或者栈不为空，则进入循环，模拟递归
            k = i;
            while((j=ListExistNext(lGraph,k))>=0 || top!=-1){
                if(j>=0){//当前节点有可以遍历的下一个节点
                    //遍历当前节点，标记更新
                    printf("<%d,%d>\n",k+1,j+1);
                    fprintf(fp,"<%d,%d>\n",k+1,j+1);
                    printf("%d(%d)\t",j+1,lGraph->vexlist[j].vertex);
                    fprintf(fp,"%d(%d)\t",j+1,lGraph->vexlist[j].vertex);
                    visited[j] = 1;
                    //该节点入栈
                    stack[++top] = j;
                    //判断新节点
                    k = j;
                }else{
                    //栈顶元素出栈，回溯遍历
                    k = stack[top--];
                }
            }
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
int MatrixExistNext(MatrixGraph *mtGraph, int k){
    for(int i=0;i<mtGraph->n;i++){
        if(mtGraph->edge[k][i]==1 && !visited[i])//如果有邻接于k的顶点，且未被访问过，那么返回这个符合条件的顶点
            return i;
    }
    return -1;//否则返回-1
}
int ListExistNext(ListGraph *lGraph, int k){
    EdgeNode *p = lGraph->vexlist[k].firstedge;
    //搜索顶点k的邻接表
    while(p!=NULL){
        if(!visited[p->adjvex]){//如果有邻接于k的顶点，且未被访问过，那么返回这个符合条件的顶点
            return (int)p->adjvex;
        }
        p = p->next;
    }
    return -1;//否则返回-1
}
void MatrixBFSTraverse(MatrixGraph *mtGraph){
    FILE *fp;//遍历结果文件
    //初始化遍历标记
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("MatrixBFS.txt","w"))==NULL)exit(1);
    //遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
    for(int i=0;i<mtGraph->n;i++){
        if(!visited[i]){
            MatrixBFS(mtGraph,i,fp);//开始遍历
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
void MatrixBFS(MatrixGraph *mtGraph, int k, FILE *fp){
    int i;//队首顶点编号
    EdgeNode *p = NULL;//邻接表遍历节点
    Queue nodeQueue;//顶点编号队列
    //队列初始化
    nodeQueue.front = nodeQueue.rear = 0;
    //遍历当前节点，标记更新
    printf("%d(%d)\t",k+1,mtGraph->vertex[k]);
    fprintf(fp,"%d(%d)\t",k+1,mtGraph->vertex[k]);
    visited[k] = 1;
    //当前节点入队
    EnQueue(k,&nodeQueue);
    //在队列不为空的情况下，也就是还有元素未被遍历的情况下，把队首顶点出队，并搜索邻接于这个顶点所有未被访问过的顶点，将他们依次入队
    while(nodeQueue.front!=nodeQueue.rear){
        i = DeQueue(&nodeQueue);
        for(int j=0;j<mtGraph->n;j++){
            if(mtGraph->edge[i][j]==1 && !visited[j]){
                fprintf(fp,"<%d,%d>\n",i+1,j+1);
                printf("<%d,%d>\n",i+1,j+1);
                fprintf(fp,"%d(%d)\t",j+1,mtGraph->vertex[j]);
                printf("%d(%d)\t",j+1,mtGraph->vertex[j]);
                visited[j] = 1;
                EnQueue(j,&nodeQueue);
            }
        }
    }
}
void ListBFSTraverse(ListGraph *lGraph){
    FILE *fp;//遍历结果文件
    //初始化遍历标记
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("ListBFS.txt","w"))==NULL)exit(1);
    //遍历所有节点，因为原图可能是一个非强连通图或着不是连通图
    for(int i=0;i<lGraph->n;i++){
        if(!visited[i]){
            ListBFS(lGraph,i,fp);//开始遍历
            printf("\n\n");fprintf(fp,"\n\n");//在非强连通图或非连通图的情况下，遍历会生成森林，为区分生成树，用换行隔开
        }
    }
    fclose(fp);
}
void ListBFS(ListGraph *lGraph, int k, FILE *fp){
    int i;
    EdgeNode *p = NULL;
    Queue nodeQueue;
    //队列初始化
    nodeQueue.front = nodeQueue.rear = 0;
    //遍历当前节点，标记更新
    printf("%d(%d)\t",k+1,lGraph->vexlist[k].vertex);
    fprintf(fp,"%d(%d)\t",k+1,lGraph->vexlist[k].vertex);
    visited[k] = 1;
    //当前节点入队
    EnQueue(k,&nodeQueue);
    //在队列不为空的情况下，也就是还有元素未被遍历的情况下，把队首顶点出队，并搜索邻接于这个顶点所有未被访问过的顶点，将他们依次入队
    while(nodeQueue.front!=nodeQueue.rear){
        i = DeQueue(&nodeQueue);
        p = lGraph->vexlist[i].firstedge;
        while(p){
            if(!visited[p->adjvex]){
                fprintf(fp,"<%d,%d>\n",i+1,p->adjvex+1);
                printf("<%d,%d>\n",i+1,p->adjvex+1);
                fprintf(fp,"%d(%d)\t",p->adjvex+1,lGraph->vexlist[p->adjvex].vertex);
                printf("%d(%d)\t",p->adjvex+1,lGraph->vexlist[p->adjvex].vertex);
                visited[p->adjvex] = 1;
                EnQueue(p->adjvex,&nodeQueue);
            }
            p = p->next;
        }
    }
}
void EnQueue(int k, Queue *nodeQueue){
    //在队尾入队
    nodeQueue->elements[nodeQueue->rear] = k;
    nodeQueue->rear++;
}
int DeQueue(Queue *nodeQueue){
   int ret;//队首顶点编号
   //在队首出队
   ret = nodeQueue->elements[nodeQueue->front];
   nodeQueue->front++;
   return ret;
}
int main()
{
    struct Node nodeSet[MAXSIZE];//图的顶点集
    struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2];//图的边集
    MatrixGraph *mtGraph = NULL;//邻接矩阵图
    ListGraph *lGraph = NULL;//邻接表图
    EdgeNode *p = NULL;//遍历邻接表临时节点
    char choice;//用户选项
    while(1){
        menu();//界面
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1':
            ReadFromFile(nodeSet,edgeSet);//读顶点集到nodeSet,读边集到edgeSet
            mtGraph = (MatrixGraph*)malloc(sizeof(MatrixGraph));
            CreateMGraph(mtGraph,nodeSet,edgeSet);//根据顶点集和边集创建邻接矩阵到mtGraph
            //显示创建结果
            printf("    ");
            for(int i=0;i<mtGraph->n;i++)
                printf("%d ",i+1);
            printf("\n");
            for(int i=0;i<mtGraph->n;i++){
                printf("%d  [",i+1);
                for(int j=0;j<mtGraph->n;j++)
                    printf("%d ",mtGraph->edge[i][j]);
                printf("]\n");
            }
            break;
        case '2':
            ReadFromFile(nodeSet,edgeSet);//读顶点集到nodeSet,读边集到edgeSet
            lGraph = (ListGraph*)malloc((sizeof(ListGraph)));
            CreateGraph(lGraph,nodeSet,edgeSet);//根据顶点集和边集创建邻接表到lGraph
            //显示创建结果
            for(int i=0;i<lGraph->n;i++){
                printf("%d: ",i+1);
                p = lGraph->vexlist[i].firstedge;
                while(p){
                    printf("%u->",p->adjvex+1);
                    p = p->next;
                }
                printf("\n");
            }
            break;
        case '3':
            //如果为创建，则输出提示信息并刷新界面，以下均如此
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            lGraph = (ListGraph*)malloc((sizeof(ListGraph)));
            MatrixToList(mtGraph,lGraph);//矩阵转表
            printf("Success! The existed matrix of graph has been transformed into a list.\n");
            break;
        case '4':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            mtGraph = (MatrixGraph*)malloc(sizeof(MatrixGraph));
            ListToMatrix(lGraph,mtGraph);//表转矩阵
            printf("Success! The existed list of graph has been transformed into a matrix.\n");
            break;
        case '5':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            RecMatrixDFSTraverse(mtGraph);//邻接矩阵递归DFS
            printf("\n");break;
        case '6':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            RecListDFSTraverse(lGraph);//邻接表递归DFS
            printf("\n");break;
        case '7':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            MatrixDFSTraverse(mtGraph);//邻接矩阵非递归DFS
            printf("\n");break;
        case '8':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            ListDFSTraverse(lGraph);//邻接表非递归DFS
            printf("\n");break;
        case '9':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            MatrixBFSTraverse(mtGraph);//邻接矩阵非递归BFS
            printf("\n");break;
        case '0':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            ListBFSTraverse(lGraph);//邻接表非递归BFS
            printf("\n");break;
        default:exit(0);
        }
    }
    return 0;
}
void menu(){
    printf("\n\t1. Build a graph from the file in the matrix way.\n");
    printf("\t2. Build a graph from the file in the list way.\n");
    printf("\t3. Turn the existed matrix of graph into a list.\n");
    printf("\t4. Turn the existed list of graph into a matrix.\n");
    printf("\t5. (Recursion) Traverse the matrix of graph with DFS.\n");
    printf("\t6. (Recursion) Traverse the list of graph with DFS.\n");
    printf("\t7. Traverse the matrix of graph with DFS\n");
    printf("\t8. Traverse the list of graph with DFS.\n");
    printf("\t9. (Recursion) Traverse the matrix of graph with BFS.\n");
    printf("\t0. (Recursion) Traverse the list of graph with BFS.\n");
    printf("\tExit: Please input any button except 0~9.\n");
}
