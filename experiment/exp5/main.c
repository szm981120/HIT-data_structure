#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxsize 1000
/*数据结构体*/
struct records{
    int key;//关键字
    char name[30];//人名
};
typedef struct records records;
/*模拟人名*/
char lastname[15][8] = {"Green","Blue","Jackson","Stark","Barkley","Jordan","Yang ","May","Chen","Ma","James","Bryant","Thomas","Robin","Parker"};
char firstname[15][8] = {"Jack ","Jerry ","Alice ","Alex ","Ben ","Bruce ","Julie ","Taylor ","Mary ","Nick ","Paul ","Kris ","Sara ","Emma ","Ming "};

void CreateList(int listSize, records **list);//构建一个容量为listSize的数据源，保存到结构体数组list中
void BubbleSort(int n, records **list);//对list中的数据冒泡排序
void QuickSort(records **list, int i, int j);//对list中的数据快速排序
/*在索引为i到j的数据中，以基准元素关键字为标准，把较小的元素放到左侧，较大的元素放到右侧，并返回那个基准元素的最后索引，作为分治索引*/
int Partition(records **list, int i, int j, int pivotIndex);
int FindPivot(records **list, int i, int j);//找到一个快速排序的基准数据
void Swap(records *a, records *b);//交换两个数据
void Menu();//用户菜单
void CreateList(int listSize, records **list){
    int s;//一个随机索引
    records *temp;//乱序交换时的临时变量
    int first,second,third;//生成人名时的随即索引
    int repeatChoice = 0, a, b, reverseChoice = 0, orderChoice = 0;
    srand(2018);
    /*如果生成的数据里需要有重复数据的，请输入一个正整数*/
    printf("Do you want some repeat data?(yes:positive integer)\n");
    scanf("%d",&repeatChoice);getchar();
    if(repeatChoice > 0){
        /*重复数据的值域为(a,b]*/
        printf("Repeat interval (a,b]:\n");
        scanf("%d,%d",&a,&b);getchar();
    }else{
        /*数据无重复，再次询问，是否要一系列反序数据*/
        printf("Do you want a series of data in reverse order?(yes:positive integer)\n");
        scanf("%d",&reverseChoice);getchar();
    }
    /*数据无重复，不反序，再次询问，是否要一系列正序数据*/
    if(reverseChoice <= 0){
        printf("Do you want a series of data in order?(yes:positive integer)\n");
        scanf("%d",&orderChoice);getchar();
    }
    for(int i=0;i<listSize;i++){
        if(repeatChoice <= 0)
            list[i]->key = i + 1;
        else
            list[i]->key = rand() % (b-a) + a + 1;//数据在(a,b]中重复
        first = rand() % 15;
        second = rand() % 15;
        third = rand() % 15;
        strcpy(list[i]->name,firstname[first]);
        strcat(list[i]->name,firstname[second]);
        strcat(list[i]->name,lastname[third]);
    }
    if(orderChoice <= 0){
        for(int i=0;i<listSize;i++){
            if(reverseChoice <= 0){
                /*乱序处理*/
                s = rand() % listSize;
                temp = list[s];
                list[s] = list[i];
                list[i] = temp;
            }else{
                /*反序处理*/
                list[i]->key = listSize - i;
            }
        }
    }

}
void BubbleSort(int n, records **list){
    int changeFlag = 0;
    /*冒泡排序执行n-1次*/
    for(int i=1;i<=n-1;i++){
        changeFlag - 0;
        /*每次从数据的底端向上遍历，排序*/
        for(int j=n-1;j>=i;j--){
            if(list[j]->key < list[j-1]->key){
                Swap(list[j],list[j-1]);
                changeFlag = 1;
            }
        }
        /*如果有一次遍历中，从未发生过交换，即已排好顺序，就没必要再继续遍历下去了*/
        if(changeFlag == 0)
            return;
    }
}
void QuickSort(records **list, int i, int j){
    int pivotIndex, partIndex;//基准数据索引，分治元素索引
    pivotIndex = FindPivot(list,i,j);
    if(pivotIndex!=-1){
        partIndex = Partition(list,i,j,pivotIndex);
        /*Partition执行后，基准数据位于分治索引处，该分治索引左侧数据关键字都较小，右侧都较大*/
        QuickSort(list,i,partIndex-1);//对左侧数据递归快排
        QuickSort(list,partIndex+1,j);//对右侧数据递归快排
    }
}
int Partition(records **list, int i, int j, int pivotIndex){
    int l = i, r = j;//左右游标
    int pivot = list[pivotIndex]->key;//基准关键字
    records pivotRecord = *list[pivotIndex];//基准数据
    /*把基准数据调换到数据段的最小端，表示把基准数据作为第一个坑*/
    if(pivotIndex!=i)
        Swap(list[pivotIndex],list[i]);
    do{
        /*从右游标向左找，找到第一个关键字小于基准关键字的，并将其放到左游标数据处*/
        /*如果是第一次，也就是把基准数据挖个坑*/
        while(l < r && list[r]->key >= pivot)
            r--;
        *list[l] = *list[r];//填坑
        /*从左游标向右找，找到第一个关键字大于基准关键字的，并将其放到右游标数据处*/
        while(l < r && list[l]->key <= pivot)
            l++;
        *list[r] = *list[l];//填坑
    }while(l < r);
    *list[l] = pivotRecord;//最后的坑，由最开始挖走的基准数据填上，这里的l就是分治索引了
    return l;
}
int FindPivot(records **list, int i, int j){
/* A possible bug */
/* 绝不能在初始化处对list寻址赋值firstkey，如果i>j，那么list[i]是越界的 */
/* 该基准元素的查找策略是，从小端开始遍历，找到第一不同的数据，选取较大者*/
//    int firstkey = list[i]->key;
    for(int k=i+1;k<=j;k++){
        if(list[k]->key > list[i]->key)
            return k;
        else if(list[k]->key < list[i]->key)
            return i;
    }
    return -1;
}
void Swap(records *a, records *b){
    records temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void Menu(){
    printf("\t1. Create a list and bubblesort.\n");
    printf("\t2. Create a list and quicksort.\n");
    printf("\t3. Test mode.\n");
}

int main()
{
    records **list;//数据源
    int listSize;//数据容量
    char choice;//用户选择
    clock_t start,stop;//计时装置
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1'://冒泡排序
            printf("How many records in the list?\n");
            scanf("%d",&listSize);getchar();
            /*为数据源申请空间*/
            list = (records**)malloc(sizeof(records*)*listSize);
            for(int i=0;i<listSize;i++)
                list[i] = (records*)malloc(sizeof(records));

            CreateList(listSize,list);

            start = clock();
            BubbleSort(listSize,list);
            stop = clock();
            /*排序结果输出*/
//            for(int i=0;i<listSize;i++){
//                printf("key: %d, name: %s\n",list[i]->key,list[i]->name);
//            }
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            /*释放list*/
            for(int i=0;i<listSize;i++)
                free(list[i]);
            free(list);
            break;
        case '2'://快速排序
            printf("How many records in the list?\n");
            scanf("%d",&listSize);getchar();
            /*为数据源申请空间*/
            list = (records**)malloc(sizeof(records*)*listSize);
            for(int i=0;i<listSize;i++)
                list[i] = (records*)malloc(sizeof(records));

            CreateList(listSize,list);

            start = clock();
            QuickSort(list,0,listSize-1);
            stop = clock();
            /*排序结果输出*/
//            for(int i=0;i<listSize;i++){
//                printf("key: %d, name: %s\n",list[i]->key,list[i]->name);
//            }
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            /*释放list*/
            for(int i=0;i<listSize;i++)
                free(list[i]);
            free(list);
            break;
        case '3'://测试用的，属于前两个的整合
            printf("How many records in the list?\n");
            scanf("%d",&listSize);getchar();
            list = (records**)malloc(sizeof(records*)*listSize);
            for(int i=0;i<listSize;i++)
                list[i] = (records*)malloc(sizeof(records));
            CreateList(listSize,list);
            start = clock();BubbleSort(listSize,list);stop = clock();
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            CreateList(listSize,list);
            start = clock();QuickSort(list,0,listSize-1);stop = clock();
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            for(int i=0;i<listSize;i++)
                free(list[i]);
            free(list);
            break;
        default:exit(0);
        }

    }
    return 0;
}
