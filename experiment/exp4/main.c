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
/*测试计数器*/
int success = 0,fail = 0;
/*姓氏表*/
char firstname[20][4] = {"赵","钱","孙","李","周","吴","郑","王","冯","陈","褚","卫","蒋","沈","韩","杨","朱","秦","尤","许"};
char secondname[20][4] = {"伟","强","冰","猛","啸","明","威","媛","维","峰","帅","歌","宝","嫣","芳","艳","军","静","英","丽"};

BSTNode* CreateBST();/*创建一棵容量最大为MAXSIZE的BST树，数据源是混乱无序的，返回BST树*/
BSTNode* InsertBST(struct records record, BSTNode* BSTtree);/*向BST树中添加数据，返回BST树*/
BSTNode* SearchBST(int k, BSTNode* BSTtree);/*按关键字搜索，返回结果节点*/
void DeleteBSTNode(int k, BSTNode** BSTtree);/*在BST中删除一个数据，返回BST树*/
struct records DeleteMin(BSTNode** BSTtree);/*删除一棵树的最左下角节点，并返回这个节点*/
void GetOrder(BSTNode* BSTtree);/*中序输出BST中数据*/
BSTNode* TestOrder();/*按序喂给BST一组容量为TESTSIZE的数据，构建一棵BST树，并返回BST*/
BSTNode* TestDisorder();/*喂给BST一组容量为TESTSIZE的混乱无序数据，构建一棵BST树，并返回BST*/
void TestBinsearch(struct records* list, int testSuccess[TESTSIZE], int testFail[TESTSIZE]);/*测试折半查找的成败性能*/
void BSTtestResult(BSTNode* BSTtree, int testSuccess[TESTSIZE], int testFail[TESTSIZE]);/*测试BST查找的成败性能*/
int BinSearch(int k, int last, struct records* list);/*在数组中运用非递归折半查找*/
int recBinSearch(int k, int low, int up, struct records* list);/*在数据数组中应用递归折半查找*/
void Menu();/*菜单*/

BSTNode* CreateBST(){
    BSTNode* BSTtree = NULL;
    int first, second, third, n;
    struct records record;
    int a[MAXSIZE],temp,s;
    srand(2019);
    printf("How many records?\n");
    scanf("%d",&n);getchar();
    /*数据源，大小由标准输入*/
    for(int i=1;i<=n;i++)
        a[i-1] = i;
    /*对数据源混乱处理*/
    for(int i=1;i<=n;i++){
        s = rand() % n;
        temp = a[i-1];
        a[i-1] = a[s];
        a[s] = temp;
    }
    /*根据数据，构建BST*/
    for(int i=0;i<n;i++){
        /*随机生成姓氏*/
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
    if(BSTtree==NULL){/*递归到叶子节点的子节点（空），创建一个新节点*/
        BSTtree = (BSTNode*)malloc(sizeof(BSTNode));
        BSTtree->data = record;
        BSTtree->lchild = NULL;
        BSTtree->rchild = NULL;
    }else if(record.key < BSTtree->data.key){//关键字小于当前节点的值，向当前节点的左子树插入
        BSTtree->lchild = InsertBST(record,BSTtree->lchild);
    }else if(record.key > BSTtree->data.key){//关键字大于当前节点的值，向当前节点的右子树插入
        BSTtree->rchild = InsertBST(record,BSTtree->rchild);
    }else{//关键字等于当前节点的值，说明已经存在了！
        printf("The key has already existed!\n");
    }
    return BSTtree;
}
BSTNode* SearchBST(int k, BSTNode* BSTtree){
    BSTNode *p = BSTtree;
    if(p==NULL){
        return p;
    }else{
        if(k < p->data.key){//关键字比当前小，在左子树中找
            success++;fail++;//未知成败，每找一次，两计数器加一
            return (SearchBST(k,p->lchild));
        }else if(k > p->data.key){//关键字比当前大，在右子树中找
            success++;fail++;//未知成败，每找一次，两计数器加一
            return (SearchBST(k,p->rchild));
        }else{
            success++;
            return p;
        }
    }
}
void DeleteBSTNode(int k, BSTNode** BSTtree){
    if(BSTtree!=NULL){//当前节点非空，才能执行删除，否则就返回了
        /*这里是四个互斥条件分支*/
        if(k < (*BSTtree)->data.key && (*BSTtree)->lchild!=NULL){//关键字比当前小，且当前有左子树，在左子树中删除
            DeleteBSTNode(k,&((*BSTtree)->lchild));
        }else if(k > (*BSTtree)->data.key && (*BSTtree)->rchild!=NULL){//关键字比当前大，且当前有右子树，在右子树中删除
            DeleteBSTNode(k,&((*BSTtree)->rchild));
        }else if(k == (*BSTtree)->data.key){//关键字等于当前值，应该把当前节点删除
            /*删除当前节点，实则是把其后继节点删除，并把这个后继节点放到当前节点的位置*/
            if((*BSTtree)->rchild==NULL){//其无后继节点，直接由左子树继承
                (*BSTtree) = (*BSTtree)->lchild;
            }else if((*BSTtree)->lchild==NULL){//其无前驱节点，直接由右子树继承
                (*BSTtree) = (*BSTtree)->rchild;
            }else{//既有前驱也有后继，改为删除其后继节点，并返回后继节点，来继承当前节点
                (*BSTtree)->data = DeleteMin(&((*BSTtree)->rchild));
            }
        }else{//有如下可能，说明找不到删除节点
            /*关键字小，但无左子树*/
            /*关键字大，但无右子树*/
            /*关键字不大不小也不等，事实上不可能存在*/
            printf("There is no such key of data!\n");
        }
    }

}
struct records DeleteMin(BSTNode** BSTtree){
    struct records temp;
    /*递推找到最左下节点，先保存下来，再删除，再返回*/
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
    /*按树的中序递归遍历输出*/
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
    /*构建一个容量为TESTSIZE的有序奇数序列作为测试样本，并生成BST树*/
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
    /*构建一个容量为TESTSIZE的混乱无序奇数序列作为测试样本，并生成BST树*/
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
    /*容量为TESTSIZE的有序奇数列表*/
    for(int i=0;i<TESTSIZE;i++)
        list[i].key = 2*i+1;
    /*执行折半查找*/
    for(int i=1;i<=TESTSIZE;i++){
        success = 0;
        BinSearch(2*i-1,TESTSIZE,list);
        testSuccess[i-1] = success;
        fail = 0;
        BinSearch(2*i,TESTSIZE,list);
        testFail[i-1] = fail;
    }
    /*统计*/
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
    /*执行BST查找*/
    for(int i=1;i<=TESTSIZE;i++){
        success = 0;
        SearchBST(2*i-1,BSTtree);
        testSuccess[i-1] = success;
        fail = 0;
        SearchBST(2*i,BSTtree);
        testFail[i-1] = fail;
    }
    /*统计*/
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
        mid = (low + up) / 2;//执行折半
        if(list[mid].key==k){//找到
            return mid;
        }else if(list[mid].key>k){//寻找过程，再循环，成功失败均加一
            up = mid - 1;
        }else{
            low = mid + 1;
        }
        //寻找失败了，还要再进一次循环，也是失败加一
    }
    return 0;
}
int recBinSearch(int k, int low, int up, struct records* list){
    int mid;
    /*递归折半查找未设置计数器*/
    if(low > up){
        return 0;
    }else{
        mid = (low + up) / 2;
        if(k < list[mid].key){
            return recBinSearch(k,low,mid-1,list);//递归折半
        }else if(k > list[mid].key){
            return recBinSearch(k,mid+1,up,list);//递归折半
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
    BSTNode* BSTtree = NULL;//BST树
    struct records list[MAXSIZE];//折半查找数据
    struct records insertRecord;//BST插入数据
    int deleteKey, searchKey;//BST删除和搜索的关键字
    BSTNode* searchResult;//BST搜索结果
    int testSuccess[TESTSIZE],testFail[TESTSIZE];//统计数组
    char choice;
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1':/*创建一棵容量最大为MAXSIZE的BST树，数据源是混乱无序的，返回BST树*/
            BSTtree = CreateBST();break;
        case '2'://按删除关键字删除
            printf("What's the key of the data that you want to delete?\n");
            scanf("%d",&deleteKey);getchar();
            DeleteBSTNode(deleteKey,&BSTtree);
            break;
        case '3'://按搜索关键字搜索
            printf("What's the key of the data that you want to search?\n");
            scanf("%d",&searchKey);getchar();
            searchResult = SearchBST(searchKey,BSTtree);
            /*判定搜索结果，并给出信息*/
            if(searchResult!=NULL){
                printf("Key: %d\n",searchResult->data.key);
                printf("Name: %s\n",searchResult->data.name);
            }else{
                printf("There is no such data!\n");
            }
            break;
        case '4'://得到BST排序结果
            GetOrder(BSTtree);printf("\n");break;
        case '5'://向BST中插入数据
            printf("What's the key and the name of the data that you want to insert?\n");
            scanf("%d,%s",&insertRecord.key,insertRecord.name);getchar();
            BSTtree = InsertBST(insertRecord,BSTtree);
            break;
        case '6'://测试折半查找的成败性能
            TestBinsearch(list,testSuccess,testFail);break;
        case '7'://测试BST有序数据查找的成败性能
            BSTtree = TestOrder();
            BSTtestResult(BSTtree,testSuccess,testFail);
            break;
        case '8'://测试BST无序数据查找的成败性能
            BSTtree = TestDisorder();
            BSTtestResult(BSTtree,testSuccess,testFail);
            break;
        default:exit(0);
        }
    }
    return 0;
}
