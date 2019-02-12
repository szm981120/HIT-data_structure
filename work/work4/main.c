#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAXSIZE 3000//AVL树的最大数据容量
//结构体声明
struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;
struct AvlNode{
    int data;
    AvlTree left;
    AvlTree right;
    int height;
};

AvlTree CreateAvl();//生成一个AVL树，这个树的数据是n个自然数，n由标准输入 数据源乱序，返回AVL树
AvlTree Insert(int data, AvlTree tree);//向AVL树中插入数据data，返回AVL树
Position SingleRotateWithLeft(Position k2);//对节点满足LL旋转的节点k2，执行一次LL顺时针旋转，返回旋转后的根节点
Position SingleRotateWithRight(Position k2);//对节点满足RR旋转的节点k2，执行一次RR逆时针旋转，返回旋转后的根节点
Position DoubleRotateWithLeft(Position k3);//对节点满足LR旋转的节点k2，先逆时针转，再顺时针转，返回旋转后的根节点
Position DoubleRotateWithRight(Position k3);//对节点满足RL旋转的节点k2，先顺时针转，再逆时针转，返回旋转后的根节点
int Height(Position p);//计算节点P的高度，返回高度值
int Max(int a, int b);//返回a和b的较大值
AvlTree Delete(int data, AvlTree tree);//在AVL中删除关键字为data的节点，并返回AVL树
void GetOrder(AvlTree tree);//得到AVL中数据的排序
Position SearchAvl(int k, AvlTree tree);//搜索关键字为k的节点，并返回这个节点
/*测试函数声明*/
void InsertLLTest();void InsertLRTest();void InsertRLTest();void InsertRRTest();
void DeleteLLTest();void DeleteLRTest();void DeleteRLTest();void DeleteRRTest();
void fprintTree(AvlTree tree,FILE *fp);//向文件写入树的形状，用于测试结果展示
void Menu();//用户菜单界面


AvlTree CreateAvl(){
    AvlTree tree = NULL;
    int n, a[MAXSIZE], temp, s;
    srand(2018);
    printf("How many records?\n");
    scanf("%d",&n);getchar();
    /*生成n个自然数，并乱序处理*/
    for(int i=1;i<=n;i++)
        a[i-1] = i;
    for(int i=1;i<=n;i++){
        s = rand() % n;
        temp = a[i-1];
        a[i-1] = a[s];
        a[s] = temp;
    }
    /*把乱序的n个自然数插入到AVL树中，开始构建AVL树*/
    for(int i=0;i<n;i++)
        tree = Insert(a[i],tree);
    return tree;
}
AvlTree Insert(int data, AvlTree tree){
    /*AVL树是空树，插入的是根节点*/
    if(tree == NULL){
        if((tree = (AvlTree)malloc(sizeof(struct AvlNode)))==NULL){
            printf("Out of space!!!");
        }else{
            /*根节点数据初始化*/
            tree->data = data;
            tree->height = 0;
            tree->left = NULL;
            tree->right = NULL;
        }
    }
    /*插入的数据关键字小于当前节点*/
    else if(data < tree->data){
        tree->left = Insert(data,tree->left);//向左子树递归插入
        /*插入后，判断是否平衡*/
        if(Height(tree->left) - Height(tree->right) == 2){//不平衡
            if(data < tree->left->data){
                tree = SingleRotateWithLeft(tree);//LL旋转
            }else{
                tree = DoubleRotateWithLeft(tree);//LR旋转
            }
        }
    }
    /*插入的数据关键字大于当前节点*/
    else if(data > tree->data){
        tree->right = Insert(data,tree->right);//向右子树递归插入
        if(Height(tree->right) - Height(tree->left) == 2){//不平衡
            if(data > tree->right->data){
                tree = SingleRotateWithRight(tree);//RR旋转
            }else{
                tree = DoubleRotateWithRight(tree);//RL旋转
            }
        }
    }
    /*插入的数据关键字等于当前节点，说明有这个数据了，不再插入了*/
    else{
        printf("The data has already existed!\n");
    }
    tree->height = Max(Height(tree->left),Height(tree->right)) + 1;//平衡处理后，更新节点高度
    return tree;
}
Position SingleRotateWithLeft(Position k2){
    Position k1;
    /*左孩子顺时针旋转，节点继承*/
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    /*节点高度更新*/
    k2->height = Max(Height(k2->left),Height(k2->right)) + 1;
    k1->height = Max(Height(k1->left),k2->height) + 1;
    return k1;
}
Position SingleRotateWithRight(Position k2){
    Position k1;
    /*右孩子逆时针旋转，节点继承*/
    k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    /*节点高度更新*/
    k2->height = Max(Height(k2->left),Height(k2->right)) + 1;
    k1->height = Max(k2->height,Height(k1->right)) + 1;
    return k1;
}
Position DoubleRotateWithLeft(Position k3){
    k3->left = SingleRotateWithRight(k3->left);//先逆时针转
    return SingleRotateWithLeft(k3);//再顺时针转
}
Position DoubleRotateWithRight(Position k3){
    k3->right = SingleRotateWithLeft(k3->right);//先顺时针转
    return SingleRotateWithRight(k3);//再逆时针转
}
int Height(Position p){
    if(p == NULL)
        return -1;
    else
        return p->height;
}
int Max(int a, int b){
    if(a > b)
        return a;
    else
        return b;
}
AvlTree Delete(int data, AvlTree tree){
    Position temp = NULL;
    if(tree == NULL)//空树返回
        return tree;
    /*要删除的数据关键字大于当前关键字，并且当前节点右子树不为空，向右子树递归删除*/
    if(data > tree->data && tree->right != NULL){
        tree->right = Delete(data,tree->right);
    }
    /*要删除的数据关键字小于当前关键字，并且当前节点左子树不为空，向左子树递归删除*/
    else if(data < tree->data && tree->left != NULL){
        tree->left = Delete(data,tree->left);
    }
    /*找到了要删除的节点*/
    else if(data == tree->data){
        /*被删除的节点最多只有一个孩子*/
        if(tree->right == NULL || tree->left == NULL){
            temp = (tree->left) ? tree->left : tree->right;//temp表示这个节点的那个孩子，也可能是NULL
            free(tree);//删除
            return temp;//节点继承
        /*被删除的节点有两个孩子*/
        }else{
            /*找到这个节点的右子树的最左下节点，记为temp*/
            temp = tree->right;
            while(temp->left != NULL)
                temp = temp->left;

            tree->data = temp->data;//节点继承
            tree->right = Delete(temp->data,tree->right);//当前节点已经更新为其后继节点，故删除其后继节点
        }
    }
    /*无要删除数据*/
    else{
        printf("There is no such data!\n");
    }
    /*删除完成后，检查是否平衡*/
    if(Height(tree->left) - Height(tree->right) >= 2){
        tree = (Height(tree->left->left) > Height(tree->left->right)) ? SingleRotateWithLeft(tree) : DoubleRotateWithLeft(tree);
    }else if(Height(tree->right) - Height(tree->left) >= 2){
        tree = (Height(tree->right->right) > Height(tree->right->left)) ? SingleRotateWithRight(tree) : DoubleRotateWithRight(tree);
    }
    /*更新当前节点高度*/
    tree->height = Max(Height(tree->left),Height(tree->right)) + 1;
    return tree;
}
void GetOrder(AvlTree tree){
    /*中序遍历，输出排序的数据*/
    if(tree == NULL)
        return;
    else{
        GetOrder(tree->left);
        printf("%d\n",tree->data);
        GetOrder(tree->right);
    }
}
Position SearchAvl(int k, AvlTree tree){
    Position p = tree;
    if(p == NULL)//空节点
        return p;//返回NULL
    else{
        if(k < p->data)
            return (SearchAvl(k,p->left));//要搜索的关键字小于当前节点关键字，向左递归搜索
        else if(k > p->data)
            return (SearchAvl(k,p->right));//要搜索的关键字大于当前节点关键字，向右递归搜索
        else
            return p;//找到了要搜索的数据
    }
}

void InsertLLTest(){
    AvlTree tree = NULL;
    int LLtest[6] = {8,4,12,2,6,1};//插入过程，LL旋转的测试数据
    FILE *fp;
    if((fp=fopen("InsertLLtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<6;i++){
        tree = Insert(LLtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[[1]2]4[6]]8[12]\nAfter the rotation: ");
    fprintTree(tree,fp);
    fclose(fp);
}
void InsertLRTest(){
    AvlTree tree = NULL;
    int LRtest[6] = {8,4,12,2,6,5};//插入过程，LR旋转的测试数据
    FILE *fp;
    if((fp=fopen("InsertLRtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<6;i++){
        tree = Insert(LRtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[2]4[[5]6]]8[12]\nAfter the rotation: ");
    fprintTree(tree,fp);
    fclose(fp);
}
void InsertRLTest(){
    AvlTree tree = NULL;
    int RLtest[6] = {8,4,12,10,14,9};//插入过程，RL旋转的测试数据
    FILE *fp;
    if((fp=fopen("InsertRLtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<6;i++){
        tree = Insert(RLtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [4]8[[[9]10]12[14]]\nAfter the rotation: ");
    fprintTree(tree,fp);
    fclose(fp);
}
void InsertRRTest(){
    AvlTree tree = NULL;
    int RRtest[6] = {8,4,12,10,14,13};//插入过程，RR旋转的测试数据
    FILE *fp;
    if((fp=fopen("InsertRRtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<6;i++){
        tree = Insert(RRtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [4]8[[10]12[[13]14]]\nAfter the rotation: ");
    fprintTree(tree,fp);
    fclose(fp);
}
void DeleteLLTest(){
    AvlTree tree = NULL;
    int LLtest[7] = {8,4,12,2,6,13,1};//删除过程，LL旋转测试数据
    FILE *fp;
    if((fp=fopen("DeleteLLtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<7;i++){
        tree = Insert(LLtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[[1]2]4[6]]8[12[13]]\nAfter the rotation: ");
    tree = Delete(13,tree);
    fprintTree(tree,fp);
    fclose(fp);
}
void DeleteLRTest(){
    AvlTree tree = NULL;
    int LRtest[7] = {8,4,12,2,6,13,5};//删除过程，LR旋转测试数据
    FILE *fp;
    if((fp=fopen("DeleteLRtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<7;i++){
        tree = Insert(LRtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[2]4[[5]6]]8[12[13]]\nAfter the rotation: ");
    tree = Delete(13,tree);
    fprintTree(tree,fp);
    fclose(fp);
}
void DeleteRLTest(){
    AvlTree tree = NULL;
    int RLtest[7] = {8,4,12,2,10,14,9};//删除过程，RL旋转测试数据
    FILE *fp;
    if((fp=fopen("DeleteRLtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<7;i++){
        tree = Insert(RLtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[2]4]8[[[9]10]12[14]]\nAfter the rotation: ");
    tree = Delete(2,tree);
    fprintTree(tree,fp);
    fclose(fp);
}
void DeleteRRTest(){
    AvlTree tree = NULL;
    int RRtest[7] = {8,4,12,2,10,14,13};//删除过程，RR旋转测试数据
    FILE *fp;
    if((fp=fopen("DeleteRRLtest.txt","w"))==NULL) exit(0);
    for(int i=0;i<7;i++){
        tree = Insert(RRtest[i],tree);
    }
    fprintf(fp,"The tree is supposed to be: [[2]4]8[[10]12[[13]14]]\nAfter the rotation: ");
    tree = Delete(2,tree);
    fprintTree(tree,fp);
    fclose(fp);
}
void fprintTree(AvlTree tree,FILE *fp){
    if(tree!=NULL){
        /*中序遍历，向文件中写入树的形状，作为测试结果*/
        fprintf(fp,"[");//每一次向左子树前进之前都输出一个左括号
        fprintTree(tree->left,fp);//先遍历左子树
        fprintf(fp,"%d",tree->data);//左子树遍历结束后，输出当前节点
        fprintTree(tree->right,fp);//再遍历右子树
        fprintf(fp,"]");//在遍历右子树之后，和左括号匹配
    }
}
void Menu(){
    printf("\t1. Create a ASL tree.\n");
    printf("\t2. Search the assigned data from ASL.\n");
    printf("\t3. Delete the assigned data from ASL.\n");
    printf("\t4. Get the order of data in ASL.\n");
    printf("\t5. Insert a data into the ASL tree.\n");
    printf("\t6. Insert test.\n");
    printf("\t7. Delete test.\n");
}
int main()
{
    AvlTree tree = NULL;//AVL树
    int searchKey, deleteKey, insertKey;//搜索、删除、插入的关键字
    Position searchResult;//搜索结果数据
    char choice;//用户选择
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1'://生成一个AVL树，这个树的数据是n个自然数，n由标准输入 数据源乱序
            tree = CreateAvl();break;
        case '2'://搜索关键字为k的节点
            printf("What's the key of the data you want to search?\n");
            scanf("%d",&searchKey);getchar();
            searchResult = SearchAvl(searchKey,tree);
            if(searchResult!=NULL)
                printf("%d\n",searchResult->data);//找到了
            else
                printf("There is no such data!\n");//没找到
            break;
        case '3'://在AVL中删除关键字为data的节点
            printf("What's the key of the data you want to delete?\n");
            scanf("%d",&deleteKey);getchar();
            tree = Delete(deleteKey,tree);
            break;
        case '4'://得到AVL中数据的排序
            GetOrder(tree);break;
        case '5'://向AVL树中插入数据data
            printf("What's the key of the data you want to insert?\n");
            scanf("%d",&insertKey);getchar();
            tree = Insert(insertKey,tree);
            break;
        case '6'://插入过程的旋转测试
            InsertLLTest();InsertLRTest();InsertRLTest();InsertRRTest();
            printf("Test success!\n");break;
        case '7'://删除过程的旋转测试
            DeleteLLTest();DeleteLRTest();DeleteRLTest();DeleteRRTest();
            printf("Test success!\n");break;
        default:exit(0);
        }
    }
    return 0;
}
