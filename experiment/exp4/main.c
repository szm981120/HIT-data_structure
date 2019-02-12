#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAXSIZE 10000
#define TESTSIZE 8096
struct records{
    int key;
    char name[20];
};
typedef struct BSTcelltype{
    struct records data;
    struct BSTcelltype *lchild;
    struct BSTcelltype *rchild;
}BSTNode;
/*���Լ�����*/
int success = 0,fail = 0;
/*���ϱ�*/
char firstname[20][4] = {"��","Ǯ","��","��","��","��","֣","��","��","��","��","��","��","��","��","��","��","��","��","��"};
char secondname[20][4] = {"ΰ","ǿ","��","��","Х","��","��","��","ά","��","˧","��","��","��","��","��","��","��","Ӣ","��"};

BSTNode* CreateBST();/*����һ���������ΪMAXSIZE��BST��������Դ�ǻ�������ģ�����BST��*/
BSTNode* InsertBST(struct records record, BSTNode* BSTtree);/*��BST����������ݣ�����BST��*/
BSTNode* SearchBST(int k, BSTNode* BSTtree);/*���ؼ������������ؽ���ڵ�*/
void DeleteBSTNode(int k, BSTNode** BSTtree);/*��BST��ɾ��һ�����ݣ�����BST��*/
struct records DeleteMin(BSTNode** BSTtree);/*ɾ��һ�����������½ǽڵ㣬����������ڵ�*/
void GetOrder(BSTNode* BSTtree);/*�������BST������*/
BSTNode* TestOrder();/*����ι��BSTһ������ΪTESTSIZE�����ݣ�����һ��BST����������BST*/
BSTNode* TestDisorder();/*ι��BSTһ������ΪTESTSIZE�Ļ����������ݣ�����һ��BST����������BST*/
void TestBinsearch(struct records* list, int testSuccess[TESTSIZE], int testFail[TESTSIZE]);/*�����۰���ҵĳɰ�����*/
void BSTtestResult(BSTNode* BSTtree, int testSuccess[TESTSIZE], int testFail[TESTSIZE]);/*����BST���ҵĳɰ�����*/
int BinSearch(int k, int last, struct records* list);/*�����������÷ǵݹ��۰����*/
int recBinSearch(int k, int low, int up, struct records* list);/*������������Ӧ�õݹ��۰����*/
void Menu();/*�˵�*/

BSTNode* CreateBST(){
    BSTNode* BSTtree = NULL;
    int first, second, third, n;
    struct records record;
    int a[MAXSIZE],temp,s;
    srand(2019);
    printf("How many records?\n");
    scanf("%d",&n);getchar();
    /*����Դ����С�ɱ�׼����*/
    for(int i=1;i<=n;i++)
        a[i-1] = i;
    /*������Դ���Ҵ���*/
    for(int i=1;i<=n;i++){
        s = rand() % n;
        temp = a[i-1];
        a[i-1] = a[s];
        a[s] = temp;
    }
    /*�������ݣ�����BST*/
    for(int i=0;i<n;i++){
        /*�����������*/
        first = rand()%20 - 1;
        second = rand()%20 - 1;
        third = rand()%20 - 1;
        strcpy(record.name,firstname[first]);
        strcat(record.name,secondname[second]);
        strcat(record.name,secondname[third]);
        record.key = a[i];
        BSTtree = InsertBST(record,BSTtree);
    }
    return BSTtree;
}
BSTNode* InsertBST(struct records record, BSTNode* BSTtree){
    if(BSTtree==NULL){/*�ݹ鵽Ҷ�ӽڵ���ӽڵ㣨�գ�������һ���½ڵ�*/
        BSTtree = (BSTNode*)malloc(sizeof(BSTNode));
        BSTtree->data = record;
        BSTtree->lchild = NULL;
        BSTtree->rchild = NULL;
    }else if(record.key < BSTtree->data.key){//�ؼ���С�ڵ�ǰ�ڵ��ֵ����ǰ�ڵ������������
        BSTtree->lchild = InsertBST(record,BSTtree->lchild);
    }else if(record.key > BSTtree->data.key){//�ؼ��ִ��ڵ�ǰ�ڵ��ֵ����ǰ�ڵ������������
        BSTtree->rchild = InsertBST(record,BSTtree->rchild);
    }else{//�ؼ��ֵ��ڵ�ǰ�ڵ��ֵ��˵���Ѿ������ˣ�
        printf("The key has already existed!\n");
    }
    return BSTtree;
}
BSTNode* SearchBST(int k, BSTNode* BSTtree){
    BSTNode *p = BSTtree;
    if(p==NULL){
        return p;
    }else{
        if(k < p->data.key){//�ؼ��ֱȵ�ǰС��������������
            success++;fail++;//δ֪�ɰܣ�ÿ��һ�Σ�����������һ
            return (SearchBST(k,p->lchild));
        }else if(k > p->data.key){//�ؼ��ֱȵ�ǰ��������������
            success++;fail++;//δ֪�ɰܣ�ÿ��һ�Σ�����������һ
            return (SearchBST(k,p->rchild));
        }else{
            success++;
            return p;
        }
    }
}
void DeleteBSTNode(int k, BSTNode** BSTtree){
    if(BSTtree!=NULL){//��ǰ�ڵ�ǿգ�����ִ��ɾ��������ͷ�����
        /*�������ĸ�����������֧*/
        if(k < (*BSTtree)->data.key && (*BSTtree)->lchild!=NULL){//�ؼ��ֱȵ�ǰС���ҵ�ǰ��������������������ɾ��
            DeleteBSTNode(k,&((*BSTtree)->lchild));
        }else if(k > (*BSTtree)->data.key && (*BSTtree)->rchild!=NULL){//�ؼ��ֱȵ�ǰ���ҵ�ǰ��������������������ɾ��
            DeleteBSTNode(k,&((*BSTtree)->rchild));
        }else if(k == (*BSTtree)->data.key){//�ؼ��ֵ��ڵ�ǰֵ��Ӧ�ðѵ�ǰ�ڵ�ɾ��
            /*ɾ����ǰ�ڵ㣬ʵ���ǰ����̽ڵ�ɾ�������������̽ڵ�ŵ���ǰ�ڵ��λ��*/
            if((*BSTtree)->rchild==NULL){//���޺�̽ڵ㣬ֱ�����������̳�
                (*BSTtree) = (*BSTtree)->lchild;
            }else if((*BSTtree)->lchild==NULL){//����ǰ���ڵ㣬ֱ�����������̳�
                (*BSTtree) = (*BSTtree)->rchild;
            }else{//����ǰ��Ҳ�к�̣���Ϊɾ�����̽ڵ㣬�����غ�̽ڵ㣬���̳е�ǰ�ڵ�
                (*BSTtree)->data = DeleteMin(&((*BSTtree)->rchild));
            }
        }else{//�����¿��ܣ�˵���Ҳ���ɾ���ڵ�
            /*�ؼ���С������������*/
            /*�ؼ��ִ󣬵���������*/
            /*�ؼ��ֲ���СҲ���ȣ���ʵ�ϲ����ܴ���*/
            printf("There is no such key of data!\n");
        }
    }

}
struct records DeleteMin(BSTNode** BSTtree){
    struct records temp;
    /*�����ҵ������½ڵ㣬�ȱ�����������ɾ�����ٷ���*/
    if((*BSTtree)->lchild==NULL){
        temp = (*BSTtree)->data;
        (*BSTtree) = (*BSTtree)->rchild;
        free(*BSTtree);
        return temp;
    }else{
        temp = DeleteMin(&((*BSTtree)->lchild));
        return temp;
    }
}
void GetOrder(BSTNode* BSTtree){
    /*����������ݹ�������*/
    if(BSTtree==NULL){
        return;
    }else{
        GetOrder(BSTtree->lchild);
        printf("Key: %d, Name: %s\n",BSTtree->data.key,BSTtree->data.name);
        GetOrder(BSTtree->rchild);
    }
}
BSTNode* TestOrder(){
    BSTNode* BSTtree = NULL;
    struct records record;
    /*����һ������ΪTESTSIZE����������������Ϊ����������������BST��*/
    for(int i=1;i<=TESTSIZE;i++){
        record.key = 2 * i - 1;
        BSTtree = InsertBST(record,BSTtree);
    }
    return BSTtree;
}
BSTNode* TestDisorder(){
    BSTNode* BSTtree = NULL;
    int a[TESTSIZE], temp, s;
    struct records record;
    /*����һ������ΪTESTSIZE�Ļ�����������������Ϊ����������������BST��*/
    for(int i=1;i<=TESTSIZE;i++)
        a[i-1] = 2 * i - 1;
    for(int i=1;i<=TESTSIZE;i++){
        s = rand()%TESTSIZE - 1;
        temp = a[i-1];
        a[i-1] = a[s];
        a[s] = temp;
    }
    for(int i=1;i<=TESTSIZE;i++){
        record.key = a[i-1];
        BSTtree = InsertBST(record,BSTtree);
    }
    return BSTtree;
}
void TestBinsearch(struct records* list, int testSuccess[TESTSIZE], int testFail[TESTSIZE]){
    int successAverage = 0;
    int failAverage = 0;
    /*����ΪTESTSIZE�����������б�*/
    for(int i=0;i<TESTSIZE;i++)
        list[i].key = 2*i+1;
    /*ִ���۰����*/
    for(int i=1;i<=TESTSIZE;i++){
        success = 0;
        BinSearch(2*i-1,TESTSIZE,list);
        testSuccess[i-1] = success;
        fail = 0;
        BinSearch(2*i,TESTSIZE,list);
        testFail[i-1] = fail;
    }
    /*ͳ��*/
    for(int i=0;i<TESTSIZE;i++)
        successAverage += testSuccess[i];
    for(int i=0;i<TESTSIZE;i++)
        failAverage += testFail[i];
    printf("Success Total: %d\tSuccess Average: %lf\n",successAverage,(double)successAverage/TESTSIZE);
    printf("Fail Total: %d\tFail Average: %lf\n",failAverage,(double)failAverage/TESTSIZE);
}
void BSTtestResult(BSTNode* BSTtree, int testSuccess[TESTSIZE], int testFail[TESTSIZE]){
    int successAverage = 0;
    int failAverage = 0;
    /*ִ��BST����*/
    for(int i=1;i<=TESTSIZE;i++){
        success = 0;
        SearchBST(2*i-1,BSTtree);
        testSuccess[i-1] = success;
        fail = 0;
        SearchBST(2*i,BSTtree);
        testFail[i-1] = fail;
    }
    /*ͳ��*/
    successAverage = 0;
    for(int i=0;i<TESTSIZE;i++)
        successAverage += testSuccess[i];
    failAverage = 0;
    for(int i=0;i<TESTSIZE;i++)
        failAverage += testFail[i];
    printf("Success Total: %d\tSuccess Average: %lf\n",successAverage,(double)successAverage/TESTSIZE);
    printf("Fail Total: %d\tFail Average: %lf\n",failAverage,(double)failAverage/TESTSIZE);
}
int BinSearch(int k, int last, struct records* list){
    int low, up, mid;
    low = 1;
    up = last;
    while(low <= up){
        success++;fail++;
        mid = (low + up) / 2;//ִ���۰�
        if(list[mid].key==k){//�ҵ�
            return mid;
        }else if(list[mid].key>k){//Ѱ�ҹ��̣���ѭ�����ɹ�ʧ�ܾ���һ
            up = mid - 1;
        }else{
            low = mid + 1;
        }
        //Ѱ��ʧ���ˣ���Ҫ�ٽ�һ��ѭ����Ҳ��ʧ�ܼ�һ
    }
    return 0;
}
int recBinSearch(int k, int low, int up, struct records* list){
    int mid;
    /*�ݹ��۰����δ���ü�����*/
    if(low > up){
        return 0;
    }else{
        mid = (low + up) / 2;
        if(k < list[mid].key){
            return recBinSearch(k,low,mid-1,list);//�ݹ��۰�
        }else if(k > list[mid].key){
            return recBinSearch(k,mid+1,up,list);//�ݹ��۰�
        }else{
            return mid;
        }
    }
}
void Menu(){
    printf("\t1. Build a BST.\n");
    printf("\t2. Delete the assigned data from the BST.\n");
    printf("\t3. Search the assigned data from the BST.\n");
    printf("\t4. Get the order about the BST.\n");
    printf("\t5. Insert a data into the BST.\n");
    printf("\t6. Test binsearch with ordered data.\n");
    printf("\t7. Test the BST with ordered data.\n");
    printf("\t8. Test the BST with disordered data.\n");
}

int main()
{
    BSTNode* BSTtree = NULL;//BST��
    struct records list[MAXSIZE];//�۰��������
    struct records insertRecord;//BST��������
    int deleteKey, searchKey;//BSTɾ���������Ĺؼ���
    BSTNode* searchResult;//BST�������
    int testSuccess[TESTSIZE],testFail[TESTSIZE];//ͳ������
    char choice;
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1':/*����һ���������ΪMAXSIZE��BST��������Դ�ǻ�������ģ�����BST��*/
            BSTtree = CreateBST();break;
        case '2'://��ɾ���ؼ���ɾ��
            printf("What's the key of the data that you want to delete?\n");
            scanf("%d",&deleteKey);getchar();
            DeleteBSTNode(deleteKey,&BSTtree);
            break;
        case '3'://�������ؼ�������
            printf("What's the key of the data that you want to search?\n");
            scanf("%d",&searchKey);getchar();
            searchResult = SearchBST(searchKey,BSTtree);
            /*�ж������������������Ϣ*/
            if(searchResult!=NULL){
                printf("Key: %d\n",searchResult->data.key);
                printf("Name: %s\n",searchResult->data.name);
            }else{
                printf("There is no such data!\n");
            }
            break;
        case '4'://�õ�BST������
            GetOrder(BSTtree);printf("\n");break;
        case '5'://��BST�в�������
            printf("What's the key and the name of the data that you want to insert?\n");
            scanf("%d,%s",&insertRecord.key,insertRecord.name);getchar();
            BSTtree = InsertBST(insertRecord,BSTtree);
            break;
        case '6'://�����۰���ҵĳɰ�����
            TestBinsearch(list,testSuccess,testFail);break;
        case '7'://����BST�������ݲ��ҵĳɰ�����
            BSTtree = TestOrder();
            BSTtestResult(BSTtree,testSuccess,testFail);
            break;
        case '8'://����BST�������ݲ��ҵĳɰ�����
            BSTtree = TestDisorder();
            BSTtestResult(BSTtree,testSuccess,testFail);
            break;
        default:exit(0);
        }
    }
    return 0;
}
