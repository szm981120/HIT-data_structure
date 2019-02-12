#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXSIZE 30
/*图顶点最多30个，几乎所有的有关MAXSIZE的数组定义，大小均为MAXSIZE+1，也就是几乎用不到索引0，数组最小索引都是1*/

int matrix_n = 0, matrix_e = 0;//matrix_n顶点数，matrix_e,边数
int floydMatrix_n = 0, floydMatrix_e = 0;//floydMatrix_n顶点数，floydMatrix_e边数
/****************************************/
/**以下为功能函数**/
/****************************************/
void ReadFromFile(unsigned matrix[MAXSIZE+1][MAXSIZE+1]);//从文件中读取图，保存到unsigned matrix[MAXSIZE+1][MAXSIZE+1]中
void FloydReadFromFile(int floydMatrix[MAXSIZE+1][MAXSIZE+1]);//用Floyd算法时的从文件中读取图，保存到int floydMatrix[MAXSIZE+1][MAXSIZE+1]中
void Dijkstra(unsigned matrix[MAXSIZE+1][MAXSIZE+1] ,unsigned shortestDistance[MAXSIZE+1] ,int shortestPath[MAXSIZE+1], int sourceNode);
/*用dijkstra算法，根据图的加权邻接矩阵matrix，
构造最短路径长度数组unsigned shortestDistance[MAXSIZE+1]，
最短路径前驱索引数组int shortestPath[MAXSIZE+1]，
还要给定单源顶点序号
*/
int MinCost(unsigned n, unsigned shortestDistance[MAXSIZE+1], int comfirmedNodes[MAXSIZE+1]);
/*在已确定的顶点集confirmedNodes的补集中，是补集！！！
挑选出从原点在仅经过confirmedNodes集中的顶点的条件下，到其它顶点的最短路径，并返回那个，被认为是距离已确定顶点集最近的顶点序号*/
void BuildDshortestPathSet(int DshortestPathSet[MAXSIZE+1][MAXSIZE+1], int shortestPath[MAXSIZE+1], int sourceNode);
/*根据源点和最短路径前驱索引表，构造出最短路径列表int DshortestPathSet[MAXSIZE+1][MAXSIZE+1]*/
void PrintSingleSource(unsigned shortestDistance[MAXSIZE+1], int DshortestPathSet[MAXSIZE+1][MAXSIZE+1], int sourceNode);
/*输出源点到其他点的最短路径长度和最短路径*/
void Floyd(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int floydMatrix[MAXSIZE+1][MAXSIZE+1], int floydShortestPath[MAXSIZE+1][MAXSIZE+1]);
/*用Floyd算法，根据图的加权邻接矩阵floydMatrix，构造
最短路径长度二维数组int floydShortestDistance[MAXSIZE+1][MAXSIZE+1]，
最短路径前驱索引矩阵int floydShortestPath[MAXSIZE+1][MAXSIZE+1]*/
void BuildFshortestPathSet(int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1], int floydShortestPath[MAXSIZE+1][MAXSIZE+1]);
/*根据最短路径前驱索引矩阵，构造任意两点间的最短路径列表int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]*/
void PrintGlobal(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]);
/*输出任意两点间的最短路径长度和最短路径*/
void PrintSingleTargetPath(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]);
/*输出单目标的最短路径长度和最短路径*/
void PrintTwoNodesPath(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]);
/*输出单顶点对间的最短路径长度和最短路径*/
/****************************************/
/**以下为初始化函数**/
/****************************************/
void InitMatrix(unsigned matrix[MAXSIZE+1][MAXSIZE+1]);
void InitShortestPath(unsigned matrix[MAXSIZE+1][MAXSIZE+1], int shortestPath[MAXSIZE+1],int sourceNode);
void InitDshortestPathSet(int DshortestPathSet[MAXSIZE+1][MAXSIZE+1]);
void InitFloydMatrix(int floydMatrix[MAXSIZE+1][MAXSIZE+1]);
void InitFloydShortest(int floydMatrix[MAXSIZE+1][MAXSIZE+1], int floydShortestDistance[MAXSIZE+1][MAXSIZE+1],int floydShortestPath[MAXSIZE+1][MAXSIZE+1]);
void InitFshortestPathSet(int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]);
void menu();

void ReadFromFile(unsigned matrix[MAXSIZE+1][MAXSIZE+1]){
    FILE *fp;
    int i,j;
    unsigned k,max;
    if((fp=fopen("graph.txt","r"))==NULL) exit(1);
    max = 0;
    while((fscanf(fp,"%d,%d,%u ",&i,&j,&k))==3){
        matrix_e++;//边数
        matrix[i][j] = k;//边的权值
        //记录下最大的顶点序号
        if(i>max)max=i;
        if(j>max)max=j;
    }
    matrix_n = max;//最大的顶点序号，就是顶点个数了
    fclose(fp);
}
void FloydReadFromFile(int floydMatrix[MAXSIZE+1][MAXSIZE+1]){
    FILE *fp;
    int i,j,k,max;
    if((fp=fopen("graph.txt","r"))==NULL) exit(1);
    max = 0;
    while((fscanf(fp,"%d,%d,%d ",&i,&j,&k))==3){
        floydMatrix_e++;//边数
        floydMatrix[i][j] = k;//边的权值
        //记录下最大的顶点序号
        if(i>max)max=i;
        if(j>max)max=j;
    }
    floydMatrix_n = max;//最大的顶点序号，就是顶点个数了
    fclose(fp);
}
void Dijkstra(unsigned matrix[MAXSIZE+1][MAXSIZE+1] ,unsigned shortestDistance[MAXSIZE+1] ,int shortestPath[MAXSIZE+1], int sourceNode){
    int w;//在未确定的顶点中，距离已确定顶点集合最近的顶点序号
    unsigned sum;//源点经过顶点w，再到顶点j的距离
    int comfirmedNodes[MAXSIZE+1];//已确定最短路径的顶点序号集
    for(int i=1;i<=matrix_n;i++){//初始化
        shortestDistance[i] = matrix[sourceNode][i];//默认初始两点最短路径长是两点间的直接距离
        comfirmedNodes[i] = 0;
    }
    comfirmedNodes[sourceNode] = 1;//初始化
    for(int i=2;i<=matrix_n;i++){//遍历除源点以外的其它节点
        w = MinCost(matrix_n,shortestDistance,comfirmedNodes);
        comfirmedNodes[w] = 1;//顶点w已被确定
        for(int j=2;j<=matrix_n;j++){
            if(comfirmedNodes[j]==0){//以w作为踏板后，还要再遍历那些未确定的顶点
                if(matrix[w][j]==UINT_MAX || shortestDistance[w]==UINT_MAX){
                    sum = UINT_MAX;//无穷相加，还是无穷
                }else{
                    sum = shortestDistance[w] + matrix[w][j];
                }
                if(sum<shortestDistance[j]){//找到了一条最短路径，记录下来
                    shortestDistance[j] = sum;
                    shortestPath[j] = w;
                }
            }
        }
    }
}
int MinCost(unsigned n, unsigned shortestDistance[MAXSIZE+1], int comfirmedNodes[MAXSIZE+1]){
    unsigned temp = UINT_MAX;
    int w = 2;
    for(int i=2;i<=n;i++){
        if(comfirmedNodes[i]==0 && shortestDistance[i]<temp){//从未确定的顶点集中，选择距离最短的
            temp = shortestDistance[i];
            w = i;
        }
    }
    return w;
}
void BuildDshortestPathSet(int DshortestPathSet[MAXSIZE+1][MAXSIZE+1], int shortestPath[MAXSIZE+1], int sourceNode){
    int j = 1;
    int k;
    for(int i=1;i<=matrix_n;i++){
        j = 1;
        k = shortestPath[i];
        while(k!=0){
            DshortestPathSet[i][0]++;//这条路径上的边数
            /*回溯寻找路径上的顶点序号*/
            DshortestPathSet[i][j++] = k;
            k = shortestPath[k];
        }
        DshortestPathSet[i][j] = k;//把路径的起点，也就是回溯的终点记下
    }
}
void PrintSingleSource(unsigned shortestDistance[MAXSIZE+1], int DshortestPathSet[MAXSIZE+1][MAXSIZE+1], int sourceNode){
    for(int i=1;i<=matrix_n;i++){
        if(shortestDistance[i]==UINT_MAX){//无可达路
            printf(" No path from node %d to node %d",sourceNode,i);printf("\n");
        }else{//有可达路径
            printf("Distance from node %d to node %d: %u",sourceNode,i,shortestDistance[i]);
            printf("\tThe shortest path: ");
            for(int j=DshortestPathSet[i][0];j>0;j--)//在最短路径列表中，倒着输出，就是要找的路径了
                printf("%d ",DshortestPathSet[i][j]);
            printf("%d\n",i);
        }
    }
}
void Floyd(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int floydMatrix[MAXSIZE+1][MAXSIZE+1], int floydShortestPath[MAXSIZE+1][MAXSIZE+1]){
    int sum;
    for(int k=1;k<=floydMatrix_n;k++){//以k为踏板
        for(int i=1;i<=floydMatrix_n;i++){//从i
            for(int j=1;j<=floydMatrix_n;j++){//到j
                /*得到以k为踏板的从i到j的长度*/
                if(floydShortestDistance[i][k]==INT_MAX || floydShortestDistance[k][j]==INT_MAX){
                    sum = INT_MAX;
                }else{
                    sum = floydShortestDistance[i][k]+floydShortestDistance[k][j];
                }
                if(sum<floydShortestDistance[i][j]){//如果这个长度比已有的长度更短，那么就更新
                    floydShortestDistance[i][j] = sum;
                    floydShortestPath[i][j] = k;
                }
            }
        }
    }
}
void BuildFshortestPathSet(int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1], int floydShortestPath[MAXSIZE+1][MAXSIZE+1]){
    int k = 1;
    int m;
    for(int i=1;i<=floydMatrix_n;i++){
        for(int j=1;j<=floydMatrix_n;j++){
            k = 1;
            m = floydShortestPath[i][j];
            while(m!=0){
                FshortestPathSet[i][j][0]++;//从i到j最短路径上经过的边数
                /*回溯寻找路径上的顶点序号*/
                FshortestPathSet[i][j][k++] = m;
                m = floydShortestPath[i][m];
            }
            FshortestPathSet[i][j][k] = m;//把路径的起点，也就是回溯的终点记下
        }
    }
}
void PrintGlobal(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]){
    for(int i=1;i<=floydMatrix_n;i++){
        for(int j=1;j<=floydMatrix_n;j++){
            if(floydShortestDistance[i][j]==INT_MAX){//无可达路
                printf(" No path from node %d to node %d",i,j);printf("\n");
            }else{//有可达路径
                printf("Distance from node %d to node %d: %d",i,j,floydShortestDistance[i][j]);
                printf("\tThe shortest path: ");
                for(int k=FshortestPathSet[i][j][0];k>0;k--)//在最短路径列表中，倒着输出，就是要找的路径了
                    printf("%d ",FshortestPathSet[i][j][k]);
                printf("%d\n",j);
            }
        }
    }
}
void PrintSingleTargetPath(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]){
    int singleTarget;//单目标定点
    printf("Please input the single target.\n");
    scanf("%d",&singleTarget);getchar();
    for(int i=1;i<=floydMatrix_n;i++){
        if(floydShortestDistance[i][singleTarget]==INT_MAX){//无可达路
            printf("No path from node %d to node %d",i,singleTarget);printf("\n");
        }else{//有可达路径
            printf("The shortest path from node %d to node %d: ",i,singleTarget);
            for(int k=FshortestPathSet[i][singleTarget][0];k>0;k--)//在最短路径列表中，倒着输出，就是要找的路径了
                printf("%d ",FshortestPathSet[i][singleTarget][k]);
            printf("%d\n",singleTarget);
        }
    }
}
void PrintTwoNodesPath(int floydShortestDistance[MAXSIZE+1][MAXSIZE+1], int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]){
    int node1,node2;//指定顶点对
    printf("Please input the two nodes and separate them with a comma.\n");
    scanf("%d,%d",&node1,&node2);getchar();
    if(floydShortestDistance[node1][node2]==INT_MAX){//无可达路
        printf(" No path from node %d to node %d",node1,node2);printf("\n");
    }else{//有可达路径
        printf("Distance from node %d to node %d: %d",node1,node2,floydShortestDistance[node1][node2]);
        printf("\tThe shortest path: ");
        for(int k=FshortestPathSet[node1][node2][0];k>0;k--)//在最短路径列表中，倒着输出，就是要找的路径了
            printf("%d ",FshortestPathSet[node1][node2][k]);
        printf("%d\n",node2);
    }
    if(floydShortestDistance[node2][node1]==INT_MAX){//无可达路
        printf(" No path from node %d to node %d",node2,node1);printf("\n");
    }else{//有可达路径
        printf("Distance from node %d to node %d: %d",node2,node1,floydShortestDistance[node2][node1]);
        printf("\tThe shortest path: ");
        for(int k=FshortestPathSet[node2][node1][0];k>0;k--)//在最短路径列表中，倒着输出，就是要找的路径了
            printf("%d ",FshortestPathSet[node2][node1][k]);
        printf("%d\n",node1);
    }
}
void InitMatrix(unsigned matrix[MAXSIZE+1][MAXSIZE+1]){
    for(int i=1;i<MAXSIZE+1;i++){
        for(int j=1;j<MAXSIZE+1;j++){
            matrix[i][j] = UINT_MAX;//初始化边权值都为无穷大
        }
    }
    for(int i=0;i<MAXSIZE+1;i++)
        matrix[i][i] = 0;//规定自己到自己的距离为0
}
void InitShortestPath(unsigned matrix[MAXSIZE+1][MAXSIZE+1], int shortestPath[MAXSIZE+1],int sourceNode){
    for(int i=1;i<=matrix_n;i++){
        if(matrix[sourceNode][i]!=UINT_MAX){//邻接于源点的顶点
            shortestPath[i] = sourceNode;//源点就是该顶点的前驱节点
        }else{
            shortestPath[i] = 0;
        }
    }
    shortestPath[sourceNode] = 0;
}
void InitDshortestPathSet(int DshortestPathSet[MAXSIZE+1][MAXSIZE+1]){
    for(int i=0;i<MAXSIZE+1;i++){
        for(int j=0;j<MAXSIZE+1;j++){
            DshortestPathSet[i][j] = 0;
        }
    }
}
void InitFloydMatrix(int floydMatrix[MAXSIZE+1][MAXSIZE+1]){
    for(int i=1;i<MAXSIZE+1;i++){
        for(int j=1;j<MAXSIZE+1;j++){
            floydMatrix[i][j] = INT_MAX;//初始化边权值都为无穷大
        }
    }
    for(int i=0;i<MAXSIZE+1;i++)
        floydMatrix[i][i] = 0;//规定自己到自己的距离为0
}
void InitFloydShortest(int floydMatrix[MAXSIZE+1][MAXSIZE+1], int floydShortestDistance[MAXSIZE+1][MAXSIZE+1],int floydShortestPath[MAXSIZE+1][MAXSIZE+1]){
    for(int i=1;i<=floydMatrix_n;i++){
        for(int j=1;j<=floydMatrix_n;j++){
            floydShortestDistance[i][j] = floydMatrix[i][j];
            if(floydMatrix[i][j]!=INT_MAX && i!=j){
                floydShortestPath[i][j] = i;
            }else{
                floydShortestPath[i][j] = 0;
            }
        }
    }
}
void InitFshortestPathSet(int FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1]){
    for(int i=0;i<MAXSIZE+1;i++){
        for(int j=0;j<MAXSIZE+1;j++){
            for(int k=0;k<MAXSIZE+1;k++){
                FshortestPathSet[i][j][k] = 0;
            }
        }
    }
}
int main()
{
    unsigned matrix[MAXSIZE+1][MAXSIZE+1];//加权邻接矩阵
    unsigned shortestDistance[MAXSIZE+1];//最短路径长度
    int shortestPath[MAXSIZE+1], DshortestPathSet[MAXSIZE+1][MAXSIZE+1];//最短路径前驱索引表，最短路径列表
    int floydMatrix[MAXSIZE+1][MAXSIZE+1], floydShortestDistance[MAXSIZE+1][MAXSIZE+1];//加权邻接矩阵，最短路径长度二维数组
    int floydShortestPath[MAXSIZE+1][MAXSIZE+1], FshortestPathSet[MAXSIZE+1][MAXSIZE+1][MAXSIZE+1];//最短路径前驱索引矩阵，最短路径列表
    char choice;
    int sourceNode;//单源顶点
    while(1){
        menu();scanf("%c",&choice);getchar();

        switch(choice){
        case '1':
            printf("Please input the source node.\n");
            scanf("%d",&sourceNode);getchar();
            InitMatrix(matrix);
            ReadFromFile(matrix);
            InitShortestPath(matrix,shortestPath,sourceNode);
            Dijkstra(matrix,shortestDistance,shortestPath,sourceNode);
            InitDshortestPathSet(DshortestPathSet);
            BuildDshortestPathSet(DshortestPathSet,shortestPath,sourceNode);
            PrintSingleSource(shortestDistance,DshortestPathSet,sourceNode);
            break;
        case '2':
            InitFloydMatrix(floydMatrix);
            FloydReadFromFile(floydMatrix);
            InitFloydShortest(floydMatrix,floydShortestDistance,floydShortestPath);
            Floyd(floydShortestDistance,floydMatrix,floydShortestPath);
            InitFshortestPathSet(FshortestPathSet);
            BuildFshortestPathSet(FshortestPathSet,floydShortestPath);
            for(int i=1;i<=floydMatrix_n;i++){
                for(int j=1;j<=floydMatrix_n;j++){
                    if(floydShortestDistance[i][j]==INT_MAX){
                        printf("0 ");
                    }else{
                        printf("%d ",floydShortestDistance[i][j]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            for(int i=1;i<=floydMatrix_n;i++){
                for(int j=1;j<=floydMatrix_n;j++){
                    printf("%d ",floydShortestPath[i][j]);
                }
                printf("\n");
            }
            PrintGlobal(floydShortestDistance,FshortestPathSet);
            break;
        case '3':
            InitFloydMatrix(floydMatrix);
            FloydReadFromFile(floydMatrix);
            InitFloydShortest(floydMatrix,floydShortestDistance,floydShortestPath);
            Floyd(floydShortestDistance,floydMatrix,floydShortestPath);
            InitFshortestPathSet(FshortestPathSet);
            BuildFshortestPathSet(FshortestPathSet,floydShortestPath);
            PrintSingleTargetPath(floydShortestDistance,FshortestPathSet);
            break;
        case '4':
            InitFloydMatrix(floydMatrix);
            FloydReadFromFile(floydMatrix);
            InitFloydShortest(floydMatrix,floydShortestDistance,floydShortestPath);
            Floyd(floydShortestDistance,floydMatrix,floydShortestPath);
            InitFshortestPathSet(FshortestPathSet);
            BuildFshortestPathSet(FshortestPathSet,floydShortestPath);
            PrintTwoNodesPath(floydShortestDistance,FshortestPathSet);
            break;
        default:exit(0);
        }
    }
    return 0;
}
void menu(){
    printf("\t1. Single source shortest path.\n");
    printf("\t2. Global shortest path.\n");
    printf("\t3. Shortest path of single target.\n");
    printf("\t4. Shortest path between two nodes.\n");
}
