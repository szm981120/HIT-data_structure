#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXSIZE 51//���ڵ����
//����
struct Node{
    unsigned order;//������
    int element;//����ֵ��Ȩ��
};
//��
struct Edge{
    unsigned head;//ʼ����
    unsigned tail;//�ն���
};
//���ڽӾ����ʾ��ͼ
typedef struct{
    int vertex[MAXSIZE];//����Ԫ��
    int edge[MAXSIZE][MAXSIZE];//�ڽӾ���
    int n,e;//������������
}MatrixGraph;
//�ڽӱ�ڵ�
typedef struct ListNode{
    unsigned adjvex;//�ýڵ���
    struct ListNode *next;//ָ����һ�������׽ڵ���ڽӽڵ�
}EdgeNode;
//�ڽӱ��׽ڵ�
typedef struct{
    int vertex;//Ԫ��
    EdgeNode *firstedge;//���ڽӵĵ�һ������
}VertexNode;
//���ڽӱ��ʾ��ͼ
typedef struct{
    VertexNode vexlist[MAXSIZE];//���㼯
    int n,e;//������������
}ListGraph;
//����
typedef struct{
    int front;
    int rear;
    int elements[MAXSIZE];
}Queue;
int visited[MAXSIZE];//���ڼ�¼�������Ķ�����
//��������
void menu();//�˵�
void ReadFromFile(struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//���ļ��ж�ȡͼ�Ķ���ͱ�
void CreateMGraph(MatrixGraph *mtGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//�����ڽӾ���
void CreateGraph(ListGraph *lGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]);//�����ڽӱ�
void MatrixToList(MatrixGraph *mtGraph, ListGraph *lGraph);//�ڽӾ���ת�ڽӱ�
void ListToMatrix(ListGraph *lGraph, MatrixGraph *mtGraph);//�ڽӱ�ת�ڽӾ���
void RecMatrixDFSTraverse(MatrixGraph *mtGraph);//�ݹ���������ڽӾ���ͼ
void RecMatrixDFS(MatrixGraph *mtGraph, int i, FILE *fp);//�ڽӾ���DFS�ݹ麯��
void RecListDFSTraverse(ListGraph *lGraph);//�ݹ���������ڽӱ�ͼ
void RecListDFS(ListGraph *lGraph, int i, FILE *fp);//�ڽӱ�DFS�ݹ麯��
void MatrixDFSTraverse(MatrixGraph *mtGraph);//�ǵݹ���������ڽӾ���ͼ
void ListDFSTraverse(ListGraph *lGraph);//�ǵݹ���������ڽӱ�ͼ
int MatrixExistNext(MatrixGraph *mtGraph, int k);//�ڽӾ���DFS���ж϶���k���޿ɱ�������һ������
int ListExistNext(ListGraph *lGraph, int k);//�ڽӱ�DFS���ж϶���k���޿ɱ�������һ������
void MatrixBFSTraverse(MatrixGraph *mtGraph);//�ǵݹ�����ȹ�����ڽӾ���ͼ
void MatrixBFS(MatrixGraph *mtGraph, int k, FILE *fp);//�ǵݹ��ڽӾ���BFS���ĺ���
void ListBFSTraverse(ListGraph *lGraph);//�ǵݹ�����ȹ�����ڽӱ�ͼ
void ListBFS(ListGraph *lGraph, int k, FILE *fp);//�ǵݹ��ڽӱ�BFS���ĺ���
void EnQueue(int k, Queue *nodeQueue);//BFS������k���
int DeQueue(Queue *nodeQueue);//BFS�����س��Ӷ�����


void ReadFromFile(struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    FILE *fp;
    char ch;//���ļ�
    int i = 1;//������
    if((fp=fopen("graph.txt","r"))==NULL)exit(1);
    //ÿ�ζ�һ�����㵽���㼯��ֱ����һ�ж������
    while((fscanf(fp,"%d",&nodeSet[i++].element))==1){
        if((ch = fgetc(fp))=='\n')break;
    }
    nodeSet[0].order = i-1;//��������
    nodeSet[0].element = INT_MAX;//����
    i = 1;
    //ÿ�ζ�һ���ߣ�ֱ����һ�б߶���
    while((fscanf(fp,"%u,%u",&edgeSet[i].head,&edgeSet[i].tail))==2){
        i++;
        ch = fgetc(fp);
    }
    edgeSet[0].head = i-1;//�߸���
    edgeSet[0].tail = INT_MAX;//����
    fclose(fp);
}
void CreateMGraph(MatrixGraph *mtGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    mtGraph->n = nodeSet[0].order;//������
    mtGraph->e = edgeSet[0].head;//����
    //��ʼ�����㼯
    for(int i=1;i<=mtGraph->n;i++)
        mtGraph->vertex[i-1] = nodeSet[i].element;
    //��ʼ���ڽӾ���
    for(int i=0;i<mtGraph->n;i++){
        for(int j=0;j<mtGraph->n;j++)
            mtGraph->edge[i][j] = 0;
    }
    //���ݱ߼������ڽӾ���
    for(int i=0;i<mtGraph->e;i++)
        mtGraph->edge[edgeSet[i+1].head-1][edgeSet[i+1].tail-1] = 1;
}
void CreateGraph(ListGraph *lGraph, struct Node nodeSet[MAXSIZE], struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2]){
    EdgeNode *p = NULL;//�ڽӱ��¶���ڵ�
    lGraph->n = nodeSet[0].order;//������
    lGraph->e = edgeSet[0].head;//����
    //initial���㼯
    for(int i=0;i<lGraph->n;i++){
        lGraph->vexlist[i].vertex = nodeSet[i+1].element;
        lGraph->vexlist[i].firstedge = NULL;
    }
    //ֻҪ����һ���ڽ��ڶ���k�Ķ��㣬�Ͱ����������뵽����k���ڽӱ������
    //����ĳ�����ͼ��Ӧ�ð�ÿ���߶���Ƴ�˫��ģ�Ҳ����ע�͵���������
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
    EdgeNode *p = NULL;//�ڽӱ��¶���ڵ�
    lGraph->n = mtGraph->n;
    lGraph->e = mtGraph->e;
    for(int i=0;i<lGraph->n;i++){
        lGraph->vexlist[i].vertex = mtGraph->vertex[i];
        lGraph->vexlist[i].firstedge = NULL;
    }
    for(int i=0;i<lGraph->n;i++){
        for(int j=0;j<lGraph->n;j++){
            if(mtGraph->edge[i][j]==1){//�ڽӾ����е��ڽӹ�ϵ��ת���ڽӱ��ϼ��ɣ����ఴ���ڽӱ�Ĵ���д����
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
    EdgeNode *p = NULL;//�ڽӱ�����ڵ�
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
    for(int i=0;i<lGraph->n;i++){//�ڽӱ��е��ڽӹ�ϵ��ת���ڽӾ����ϼ��ɣ����ఴ���ڽӾ���Ĵ���д����
        p = lGraph->vexlist[i].firstedge;
        while(p!=NULL){
            mtGraph->edge[i][p->adjvex] = 1;
            p = p->next;
        }
    }
}
void RecMatrixDFSTraverse(MatrixGraph *mtGraph){
    FILE *fp;//��������ļ�
    //��ʼ���������
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("RecMatrixDFS.txt","w"))==NULL)exit(1);
    for(int i=0;i<mtGraph->n;i++){//�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
        if(!visited[i]){
            RecMatrixDFS(mtGraph,i,fp);//��ʼ����
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
void RecMatrixDFS(MatrixGraph *mtGraph, int i, FILE *fp){
    //������ǰ�ڵ㣬��Ǹ���
    printf("%d(%d)\t",i+1,mtGraph->vertex[i]);
    fprintf(fp,"%d(%d)\t",i+1,mtGraph->vertex[i]);
    visited[i] = 1;
    //���ڽ��ڸö�������ж����У������һ��������δ����������������Ǹ�����
    //���ݵݹ�˼�룬�ɰ�һ��ǿ��ͨͼ�е����ж��������ȫ
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
    //��ʼ���������
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("RecListDFS.txt","w"))==NULL)exit(1);
    for(int i=0;i<lGraph->n;i++){//�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
        if(!visited[i]){
            RecListDFS(lGraph,i,fp);//��ʼ����
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
void RecListDFS(ListGraph *lGraph, int i, FILE *fp){
    EdgeNode *p;
    //������ǰ�ڵ㣬��Ǹ���
    printf("%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
    fprintf(fp,"%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
    visited[i] = 1;
    p = lGraph->vexlist[i].firstedge;
    //������ǰ�ڵ���ڽӱ��������һ���ڽ��ڸö����δ�������Ķ��㣬��ôȥ�ݹ�����Ǹ�����
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
    int stack[MAXSIZE];//��ջģ��ݹ����˳��
    int top = -1;//ջ��
    int k,j;//kΪ��һ���ڵ㣬jΪ��ǰ�ڵ�
    FILE *fp;//��������ļ�
    //��ʼ���������
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("MatrixDFS.txt","w"))==NULL)exit(1);
    //�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
    for(int i=0;i<mtGraph->n;i++){
        if(!visited[i]){
            //������ǰ�ڵ㣬��Ǹ���
            printf("%d(%d)\t",i+1,mtGraph->vertex[i]);
            fprintf(fp,"%d(%d)\t",i+1,mtGraph->vertex[i]);
            visited[i] = 1;
            //��ǰ�ڵ���ջ
            stack[++top] = i;
            //�����ǰ�ڵ��п��Ա�������һ���ڵ㣬����ջ��Ϊ�գ������ѭ����ģ��ݹ�
            k = i;
            while((j=MatrixExistNext(mtGraph,k))>=0 || top!=-1){
                if(j>=0){//��ǰ�ڵ��п��Ա�������һ���ڵ�
                    //������ǰ�ڵ㣬��Ǹ���
                    printf("<%d,%d>\n",k+1,j+1);
                    fprintf(fp,"<%d,%d>\n",k+1,j+1);
                    printf("%d(%d)\t",j+1,mtGraph->vertex[j]);
                    fprintf(fp,"%d(%d)\t",j+1,mtGraph->vertex[j]);
                    visited[j] = 1;
                    //�ýڵ���ջ
                    stack[++top] = j;
                    //�ж��½ڵ�
                    k = j;
                }else{
                    //ջ��Ԫ�س�ջ�����ݱ���
                    k = stack[top--];
                }
            }
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
void ListDFSTraverse(ListGraph *lGraph){
    EdgeNode* stack[MAXSIZE];//��ջģ��ݹ����˳��
    int top = -1;//ջ��
    int k,j;//kΪ��һ���ڵ㣬jΪ��ǰ�ڵ�
    FILE *fp;//��������ļ�
    //��ʼ���������
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("ListDFS.txt","w"))==NULL)exit(1);
    //�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
    for(int i=0;i<lGraph->n;i++){
        if(!visited[i]){
            //������ǰ�ڵ㣬��Ǹ���
            printf("%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
            fprintf(fp,"%d(%d)\t",i+1,lGraph->vexlist[i].vertex);
            visited[i] = 1;
            //��ǰ�ڵ���ջ
            stack[++top] = i;
            //�����ǰ�ڵ��п��Ա�������һ���ڵ㣬����ջ��Ϊ�գ������ѭ����ģ��ݹ�
            k = i;
            while((j=ListExistNext(lGraph,k))>=0 || top!=-1){
                if(j>=0){//��ǰ�ڵ��п��Ա�������һ���ڵ�
                    //������ǰ�ڵ㣬��Ǹ���
                    printf("<%d,%d>\n",k+1,j+1);
                    fprintf(fp,"<%d,%d>\n",k+1,j+1);
                    printf("%d(%d)\t",j+1,lGraph->vexlist[j].vertex);
                    fprintf(fp,"%d(%d)\t",j+1,lGraph->vexlist[j].vertex);
                    visited[j] = 1;
                    //�ýڵ���ջ
                    stack[++top] = j;
                    //�ж��½ڵ�
                    k = j;
                }else{
                    //ջ��Ԫ�س�ջ�����ݱ���
                    k = stack[top--];
                }
            }
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
int MatrixExistNext(MatrixGraph *mtGraph, int k){
    for(int i=0;i<mtGraph->n;i++){
        if(mtGraph->edge[k][i]==1 && !visited[i])//������ڽ���k�Ķ��㣬��δ�����ʹ�����ô����������������Ķ���
            return i;
    }
    return -1;//���򷵻�-1
}
int ListExistNext(ListGraph *lGraph, int k){
    EdgeNode *p = lGraph->vexlist[k].firstedge;
    //��������k���ڽӱ�
    while(p!=NULL){
        if(!visited[p->adjvex]){//������ڽ���k�Ķ��㣬��δ�����ʹ�����ô����������������Ķ���
            return (int)p->adjvex;
        }
        p = p->next;
    }
    return -1;//���򷵻�-1
}
void MatrixBFSTraverse(MatrixGraph *mtGraph){
    FILE *fp;//��������ļ�
    //��ʼ���������
    for(int i=0;i<mtGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("MatrixBFS.txt","w"))==NULL)exit(1);
    //�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
    for(int i=0;i<mtGraph->n;i++){
        if(!visited[i]){
            MatrixBFS(mtGraph,i,fp);//��ʼ����
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
void MatrixBFS(MatrixGraph *mtGraph, int k, FILE *fp){
    int i;//���׶�����
    EdgeNode *p = NULL;//�ڽӱ�����ڵ�
    Queue nodeQueue;//�����Ŷ���
    //���г�ʼ��
    nodeQueue.front = nodeQueue.rear = 0;
    //������ǰ�ڵ㣬��Ǹ���
    printf("%d(%d)\t",k+1,mtGraph->vertex[k]);
    fprintf(fp,"%d(%d)\t",k+1,mtGraph->vertex[k]);
    visited[k] = 1;
    //��ǰ�ڵ����
    EnQueue(k,&nodeQueue);
    //�ڶ��в�Ϊ�յ�����£�Ҳ���ǻ���Ԫ��δ������������£��Ѷ��׶�����ӣ��������ڽ��������������δ�����ʹ��Ķ��㣬�������������
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
    FILE *fp;//��������ļ�
    //��ʼ���������
    for(int i=0;i<lGraph->n;i++)
        visited[i] = 0;
    if((fp=fopen("ListBFS.txt","w"))==NULL)exit(1);
    //�������нڵ㣬��Ϊԭͼ������һ����ǿ��ͨͼ���Ų�����ͨͼ
    for(int i=0;i<lGraph->n;i++){
        if(!visited[i]){
            ListBFS(lGraph,i,fp);//��ʼ����
            printf("\n\n");fprintf(fp,"\n\n");//�ڷ�ǿ��ͨͼ�����ͨͼ������£�����������ɭ�֣�Ϊ�������������û��и���
        }
    }
    fclose(fp);
}
void ListBFS(ListGraph *lGraph, int k, FILE *fp){
    int i;
    EdgeNode *p = NULL;
    Queue nodeQueue;
    //���г�ʼ��
    nodeQueue.front = nodeQueue.rear = 0;
    //������ǰ�ڵ㣬��Ǹ���
    printf("%d(%d)\t",k+1,lGraph->vexlist[k].vertex);
    fprintf(fp,"%d(%d)\t",k+1,lGraph->vexlist[k].vertex);
    visited[k] = 1;
    //��ǰ�ڵ����
    EnQueue(k,&nodeQueue);
    //�ڶ��в�Ϊ�յ�����£�Ҳ���ǻ���Ԫ��δ������������£��Ѷ��׶�����ӣ��������ڽ��������������δ�����ʹ��Ķ��㣬�������������
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
    //�ڶ�β���
    nodeQueue->elements[nodeQueue->rear] = k;
    nodeQueue->rear++;
}
int DeQueue(Queue *nodeQueue){
   int ret;//���׶�����
   //�ڶ��׳���
   ret = nodeQueue->elements[nodeQueue->front];
   nodeQueue->front++;
   return ret;
}
int main()
{
    struct Node nodeSet[MAXSIZE];//ͼ�Ķ��㼯
    struct Edge edgeSet[MAXSIZE*(MAXSIZE-1)/2];//ͼ�ı߼�
    MatrixGraph *mtGraph = NULL;//�ڽӾ���ͼ
    ListGraph *lGraph = NULL;//�ڽӱ�ͼ
    EdgeNode *p = NULL;//�����ڽӱ���ʱ�ڵ�
    char choice;//�û�ѡ��
    while(1){
        menu();//����
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1':
            ReadFromFile(nodeSet,edgeSet);//�����㼯��nodeSet,���߼���edgeSet
            mtGraph = (MatrixGraph*)malloc(sizeof(MatrixGraph));
            CreateMGraph(mtGraph,nodeSet,edgeSet);//���ݶ��㼯�ͱ߼������ڽӾ���mtGraph
            //��ʾ�������
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
            ReadFromFile(nodeSet,edgeSet);//�����㼯��nodeSet,���߼���edgeSet
            lGraph = (ListGraph*)malloc((sizeof(ListGraph)));
            CreateGraph(lGraph,nodeSet,edgeSet);//���ݶ��㼯�ͱ߼������ڽӱ�lGraph
            //��ʾ�������
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
            //���Ϊ�������������ʾ��Ϣ��ˢ�½��棬���¾����
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            lGraph = (ListGraph*)malloc((sizeof(ListGraph)));
            MatrixToList(mtGraph,lGraph);//����ת��
            printf("Success! The existed matrix of graph has been transformed into a list.\n");
            break;
        case '4':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            mtGraph = (MatrixGraph*)malloc(sizeof(MatrixGraph));
            ListToMatrix(lGraph,mtGraph);//��ת����
            printf("Success! The existed list of graph has been transformed into a matrix.\n");
            break;
        case '5':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            RecMatrixDFSTraverse(mtGraph);//�ڽӾ���ݹ�DFS
            printf("\n");break;
        case '6':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            RecListDFSTraverse(lGraph);//�ڽӱ�ݹ�DFS
            printf("\n");break;
        case '7':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            MatrixDFSTraverse(mtGraph);//�ڽӾ���ǵݹ�DFS
            printf("\n");break;
        case '8':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            ListDFSTraverse(lGraph);//�ڽӱ�ǵݹ�DFS
            printf("\n");break;
        case '9':
            if(mtGraph==NULL){
                printf("Fail! There is no matrix of graph.\n");break;
            }
            MatrixBFSTraverse(mtGraph);//�ڽӾ���ǵݹ�BFS
            printf("\n");break;
        case '0':
            if(lGraph==NULL){
                printf("Fail! There is no list of graph.\n");break;
            }
            ListBFSTraverse(lGraph);//�ڽӱ�ǵݹ�BFS
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
