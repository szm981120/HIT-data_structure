#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxsize 1000
/*���ݽṹ��*/
struct records{
    int key;//�ؼ���
    char name[30];//����
};
typedef struct records records;
/*ģ������*/
char lastname[15][8] = {"Green","Blue","Jackson","Stark","Barkley","Jordan","Yang ","May","Chen","Ma","James","Bryant","Thomas","Robin","Parker"};
char firstname[15][8] = {"Jack ","Jerry ","Alice ","Alex ","Ben ","Bruce ","Julie ","Taylor ","Mary ","Nick ","Paul ","Kris ","Sara ","Emma ","Ming "};

void CreateList(int listSize, records **list);//����һ������ΪlistSize������Դ�����浽�ṹ������list��
void BubbleSort(int n, records **list);//��list�е�����ð������
void QuickSort(records **list, int i, int j);//��list�е����ݿ�������
/*������Ϊi��j�������У��Ի�׼Ԫ�عؼ���Ϊ��׼���ѽ�С��Ԫ�طŵ���࣬�ϴ��Ԫ�طŵ��Ҳ࣬�������Ǹ���׼Ԫ�ص������������Ϊ��������*/
int Partition(records **list, int i, int j, int pivotIndex);
int FindPivot(records **list, int i, int j);//�ҵ�һ����������Ļ�׼����
void Swap(records *a, records *b);//������������
void Menu();//�û��˵�
void CreateList(int listSize, records **list){
    int s;//һ���������
    records *temp;//���򽻻�ʱ����ʱ����
    int first,second,third;//��������ʱ���漴����
    int repeatChoice = 0, a, b, reverseChoice = 0, orderChoice = 0;
    srand(2018);
    /*������ɵ���������Ҫ���ظ����ݵģ�������һ��������*/
    printf("Do you want some repeat data?(yes:positive integer)\n");
    scanf("%d",&repeatChoice);getchar();
    if(repeatChoice > 0){
        /*�ظ����ݵ�ֵ��Ϊ(a,b]*/
        printf("Repeat interval (a,b]:\n");
        scanf("%d,%d",&a,&b);getchar();
    }else{
        /*�������ظ����ٴ�ѯ�ʣ��Ƿ�Ҫһϵ�з�������*/
        printf("Do you want a series of data in reverse order?(yes:positive integer)\n");
        scanf("%d",&reverseChoice);getchar();
    }
    /*�������ظ����������ٴ�ѯ�ʣ��Ƿ�Ҫһϵ����������*/
    if(reverseChoice <= 0){
        printf("Do you want a series of data in order?(yes:positive integer)\n");
        scanf("%d",&orderChoice);getchar();
    }
    for(int i=0;i<listSize;i++){
        if(repeatChoice <= 0)
            list[i]->key = i + 1;
        else
            list[i]->key = rand() % (b-a) + a + 1;//������(a,b]���ظ�
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
                /*������*/
                s = rand() % listSize;
                temp = list[s];
                list[s] = list[i];
                list[i] = temp;
            }else{
                /*������*/
                list[i]->key = listSize - i;
            }
        }
    }

}
void BubbleSort(int n, records **list){
    int changeFlag = 0;
    /*ð������ִ��n-1��*/
    for(int i=1;i<=n-1;i++){
        changeFlag - 0;
        /*ÿ�δ����ݵĵ׶����ϱ���������*/
        for(int j=n-1;j>=i;j--){
            if(list[j]->key < list[j-1]->key){
                Swap(list[j],list[j-1]);
                changeFlag = 1;
            }
        }
        /*�����һ�α����У���δ�����������������ź�˳�򣬾�û��Ҫ�ټ���������ȥ��*/
        if(changeFlag == 0)
            return;
    }
}
void QuickSort(records **list, int i, int j){
    int pivotIndex, partIndex;//��׼��������������Ԫ������
    pivotIndex = FindPivot(list,i,j);
    if(pivotIndex!=-1){
        partIndex = Partition(list,i,j,pivotIndex);
        /*Partitionִ�к󣬻�׼����λ�ڷ������������÷�������������ݹؼ��ֶ���С���Ҳ඼�ϴ�*/
        QuickSort(list,i,partIndex-1);//��������ݵݹ����
        QuickSort(list,partIndex+1,j);//���Ҳ����ݵݹ����
    }
}
int Partition(records **list, int i, int j, int pivotIndex){
    int l = i, r = j;//�����α�
    int pivot = list[pivotIndex]->key;//��׼�ؼ���
    records pivotRecord = *list[pivotIndex];//��׼����
    /*�ѻ�׼���ݵ��������ݶε���С�ˣ���ʾ�ѻ�׼������Ϊ��һ����*/
    if(pivotIndex!=i)
        Swap(list[pivotIndex],list[i]);
    do{
        /*�����α������ң��ҵ���һ���ؼ���С�ڻ�׼�ؼ��ֵģ�������ŵ����α����ݴ�*/
        /*����ǵ�һ�Σ�Ҳ���ǰѻ�׼�����ڸ���*/
        while(l < r && list[r]->key >= pivot)
            r--;
        *list[l] = *list[r];//���
        /*�����α������ң��ҵ���һ���ؼ��ִ��ڻ�׼�ؼ��ֵģ�������ŵ����α����ݴ�*/
        while(l < r && list[l]->key <= pivot)
            l++;
        *list[r] = *list[l];//���
    }while(l < r);
    *list[l] = pivotRecord;//���Ŀӣ����ʼ���ߵĻ�׼�������ϣ������l���Ƿ���������
    return l;
}
int FindPivot(records **list, int i, int j){
/* A possible bug */
/* �������ڳ�ʼ������listѰַ��ֵfirstkey�����i>j����ôlist[i]��Խ��� */
/* �û�׼Ԫ�صĲ��Ҳ����ǣ���С�˿�ʼ�������ҵ���һ��ͬ�����ݣ�ѡȡ�ϴ���*/
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
    records **list;//����Դ
    int listSize;//��������
    char choice;//�û�ѡ��
    clock_t start,stop;//��ʱװ��
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1'://ð������
            printf("How many records in the list?\n");
            scanf("%d",&listSize);getchar();
            /*Ϊ����Դ����ռ�*/
            list = (records**)malloc(sizeof(records*)*listSize);
            for(int i=0;i<listSize;i++)
                list[i] = (records*)malloc(sizeof(records));

            CreateList(listSize,list);

            start = clock();
            BubbleSort(listSize,list);
            stop = clock();
            /*���������*/
//            for(int i=0;i<listSize;i++){
//                printf("key: %d, name: %s\n",list[i]->key,list[i]->name);
//            }
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            /*�ͷ�list*/
            for(int i=0;i<listSize;i++)
                free(list[i]);
            free(list);
            break;
        case '2'://��������
            printf("How many records in the list?\n");
            scanf("%d",&listSize);getchar();
            /*Ϊ����Դ����ռ�*/
            list = (records**)malloc(sizeof(records*)*listSize);
            for(int i=0;i<listSize;i++)
                list[i] = (records*)malloc(sizeof(records));

            CreateList(listSize,list);

            start = clock();
            QuickSort(list,0,listSize-1);
            stop = clock();
            /*���������*/
//            for(int i=0;i<listSize;i++){
//                printf("key: %d, name: %s\n",list[i]->key,list[i]->name);
//            }
            printf("Run time: %lf\n",(double)(stop-start)/CLOCKS_PER_SEC);
            /*�ͷ�list*/
            for(int i=0;i<listSize;i++)
                free(list[i]);
            free(list);
            break;
        case '3'://�����õģ�����ǰ����������
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
