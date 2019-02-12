#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAXSIZE 3000//AVL���������������
//�ṹ������
struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;
struct AvlNode{
    int data;
    AvlTree left;
    AvlTree right;
    int height;
};

AvlTree CreateAvl();//����һ��AVL�����������������n����Ȼ����n�ɱ�׼���� ����Դ���򣬷���AVL��
AvlTree Insert(int data, AvlTree tree);//��AVL���в�������data������AVL��
Position SingleRotateWithLeft(Position k2);//�Խڵ�����LL��ת�Ľڵ�k2��ִ��һ��LL˳ʱ����ת��������ת��ĸ��ڵ�
Position SingleRotateWithRight(Position k2);//�Խڵ�����RR��ת�Ľڵ�k2��ִ��һ��RR��ʱ����ת��������ת��ĸ��ڵ�
Position DoubleRotateWithLeft(Position k3);//�Խڵ�����LR��ת�Ľڵ�k2������ʱ��ת����˳ʱ��ת��������ת��ĸ��ڵ�
Position DoubleRotateWithRight(Position k3);//�Խڵ�����RL��ת�Ľڵ�k2����˳ʱ��ת������ʱ��ת��������ת��ĸ��ڵ�
int Height(Position p);//����ڵ�P�ĸ߶ȣ����ظ߶�ֵ
int Max(int a, int b);//����a��b�Ľϴ�ֵ
AvlTree Delete(int data, AvlTree tree);//��AVL��ɾ���ؼ���Ϊdata�Ľڵ㣬������AVL��
void GetOrder(AvlTree tree);//�õ�AVL�����ݵ�����
Position SearchAvl(int k, AvlTree tree);//�����ؼ���Ϊk�Ľڵ㣬����������ڵ�
/*���Ժ�������*/
void InsertLLTest();void InsertLRTest();void InsertRLTest();void InsertRRTest();
void DeleteLLTest();void DeleteLRTest();void DeleteRLTest();void DeleteRRTest();
void fprintTree(AvlTree tree,FILE *fp);//���ļ�д��������״�����ڲ��Խ��չʾ
void Menu();//�û��˵�����


AvlTree CreateAvl(){
    AvlTree tree = NULL;
    int n, a[MAXSIZE], temp, s;
    srand(2018);
    printf("How many records?\n");
    scanf("%d",&n);getchar();
    /*����n����Ȼ������������*/
    for(int i=1;i<=n;i++)
        a[i-1] = i;
    for(int i=1;i<=n;i++){
        s = rand() % n;
        temp = a[i-1];
        a[i-1] = a[s];
        a[s] = temp;
    }
    /*�������n����Ȼ�����뵽AVL���У���ʼ����AVL��*/
    for(int i=0;i<n;i++)
        tree = Insert(a[i],tree);
    return tree;
}
AvlTree Insert(int data, AvlTree tree){
    /*AVL���ǿ�����������Ǹ��ڵ�*/
    if(tree == NULL){
        if((tree = (AvlTree)malloc(sizeof(struct AvlNode)))==NULL){
            printf("Out of space!!!");
        }else{
            /*���ڵ����ݳ�ʼ��*/
            tree->data = data;
            tree->height = 0;
            tree->left = NULL;
            tree->right = NULL;
        }
    }
    /*��������ݹؼ���С�ڵ�ǰ�ڵ�*/
    else if(data < tree->data){
        tree->left = Insert(data,tree->left);//���������ݹ����
        /*������ж��Ƿ�ƽ��*/
        if(Height(tree->left) - Height(tree->right) == 2){//��ƽ��
            if(data < tree->left->data){
                tree = SingleRotateWithLeft(tree);//LL��ת
            }else{
                tree = DoubleRotateWithLeft(tree);//LR��ת
            }
        }
    }
    /*��������ݹؼ��ִ��ڵ�ǰ�ڵ�*/
    else if(data > tree->data){
        tree->right = Insert(data,tree->right);//���������ݹ����
        if(Height(tree->right) - Height(tree->left) == 2){//��ƽ��
            if(data > tree->right->data){
                tree = SingleRotateWithRight(tree);//RR��ת
            }else{
                tree = DoubleRotateWithRight(tree);//RL��ת
            }
        }
    }
    /*��������ݹؼ��ֵ��ڵ�ǰ�ڵ㣬˵������������ˣ����ٲ�����*/
    else{
        printf("The data has already existed!\n");
    }
    tree->height = Max(Height(tree->left),Height(tree->right)) + 1;//ƽ�⴦��󣬸��½ڵ�߶�
    return tree;
}
Position SingleRotateWithLeft(Position k2){
    Position k1;
    /*����˳ʱ����ת���ڵ�̳�*/
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    /*�ڵ�߶ȸ���*/
    k2->height = Max(Height(k2->left),Height(k2->right)) + 1;
    k1->height = Max(Height(k1->left),k2->height) + 1;
    return k1;
}
Position SingleRotateWithRight(Position k2){
    Position k1;
    /*�Һ�����ʱ����ת���ڵ�̳�*/
    k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    /*�ڵ�߶ȸ���*/
    k2->height = Max(Height(k2->left),Height(k2->right)) + 1;
    k1->height = Max(k2->height,Height(k1->right)) + 1;
    return k1;
}
Position DoubleRotateWithLeft(Position k3){
    k3->left = SingleRotateWithRight(k3->left);//����ʱ��ת
    return SingleRotateWithLeft(k3);//��˳ʱ��ת
}
Position DoubleRotateWithRight(Position k3){
    k3->right = SingleRotateWithLeft(k3->right);//��˳ʱ��ת
    return SingleRotateWithRight(k3);//����ʱ��ת
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
    if(tree == NULL)//��������
        return tree;
    /*Ҫɾ�������ݹؼ��ִ��ڵ�ǰ�ؼ��֣����ҵ�ǰ�ڵ���������Ϊ�գ����������ݹ�ɾ��*/
    if(data > tree->data && tree->right != NULL){
        tree->right = Delete(data,tree->right);
    }
    /*Ҫɾ�������ݹؼ���С�ڵ�ǰ�ؼ��֣����ҵ�ǰ�ڵ���������Ϊ�գ����������ݹ�ɾ��*/
    else if(data < tree->data && tree->left != NULL){
        tree->left = Delete(data,tree->left);
    }
    /*�ҵ���Ҫɾ���Ľڵ�*/
    else if(data == tree->data){
        /*��ɾ���Ľڵ����ֻ��һ������*/
        if(tree->right == NULL || tree->left == NULL){
            temp = (tree->left) ? tree->left : tree->right;//temp��ʾ����ڵ���Ǹ����ӣ�Ҳ������NULL
            free(tree);//ɾ��
            return temp;//�ڵ�̳�
        /*��ɾ���Ľڵ�����������*/
        }else{
            /*�ҵ�����ڵ���������������½ڵ㣬��Ϊtemp*/
            temp = tree->right;
            while(temp->left != NULL)
                temp = temp->left;

            tree->data = temp->data;//�ڵ�̳�
            tree->right = Delete(temp->data,tree->right);//��ǰ�ڵ��Ѿ�����Ϊ���̽ڵ㣬��ɾ�����̽ڵ�
        }
    }
    /*��Ҫɾ������*/
    else{
        printf("There is no such data!\n");
    }
    /*ɾ����ɺ󣬼���Ƿ�ƽ��*/
    if(Height(tree->left) - Height(tree->right) >= 2){
        tree = (Height(tree->left->left) > Height(tree->left->right)) ? SingleRotateWithLeft(tree) : DoubleRotateWithLeft(tree);
    }else if(Height(tree->right) - Height(tree->left) >= 2){
        tree = (Height(tree->right->right) > Height(tree->right->left)) ? SingleRotateWithRight(tree) : DoubleRotateWithRight(tree);
    }
    /*���µ�ǰ�ڵ�߶�*/
    tree->height = Max(Height(tree->left),Height(tree->right)) + 1;
    return tree;
}
void GetOrder(AvlTree tree){
    /*���������������������*/
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
    if(p == NULL)//�սڵ�
        return p;//����NULL
    else{
        if(k < p->data)
            return (SearchAvl(k,p->left));//Ҫ�����Ĺؼ���С�ڵ�ǰ�ڵ�ؼ��֣�����ݹ�����
        else if(k > p->data)
            return (SearchAvl(k,p->right));//Ҫ�����Ĺؼ��ִ��ڵ�ǰ�ڵ�ؼ��֣����ҵݹ�����
        else
            return p;//�ҵ���Ҫ����������
    }
}

void InsertLLTest(){
    AvlTree tree = NULL;
    int LLtest[6] = {8,4,12,2,6,1};//������̣�LL��ת�Ĳ�������
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
    int LRtest[6] = {8,4,12,2,6,5};//������̣�LR��ת�Ĳ�������
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
    int RLtest[6] = {8,4,12,10,14,9};//������̣�RL��ת�Ĳ�������
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
    int RRtest[6] = {8,4,12,10,14,13};//������̣�RR��ת�Ĳ�������
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
    int LLtest[7] = {8,4,12,2,6,13,1};//ɾ�����̣�LL��ת��������
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
    int LRtest[7] = {8,4,12,2,6,13,5};//ɾ�����̣�LR��ת��������
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
    int RLtest[7] = {8,4,12,2,10,14,9};//ɾ�����̣�RL��ת��������
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
    int RRtest[7] = {8,4,12,2,10,14,13};//ɾ�����̣�RR��ת��������
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
        /*������������ļ���д��������״����Ϊ���Խ��*/
        fprintf(fp,"[");//ÿһ����������ǰ��֮ǰ�����һ��������
        fprintTree(tree->left,fp);//�ȱ���������
        fprintf(fp,"%d",tree->data);//���������������������ǰ�ڵ�
        fprintTree(tree->right,fp);//�ٱ���������
        fprintf(fp,"]");//�ڱ���������֮�󣬺�������ƥ��
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
    AvlTree tree = NULL;//AVL��
    int searchKey, deleteKey, insertKey;//������ɾ��������Ĺؼ���
    Position searchResult;//�����������
    char choice;//�û�ѡ��
    while(1){
        Menu();
        scanf("%c",&choice);getchar();
        switch(choice){
        case '1'://����һ��AVL�����������������n����Ȼ����n�ɱ�׼���� ����Դ����
            tree = CreateAvl();break;
        case '2'://�����ؼ���Ϊk�Ľڵ�
            printf("What's the key of the data you want to search?\n");
            scanf("%d",&searchKey);getchar();
            searchResult = SearchAvl(searchKey,tree);
            if(searchResult!=NULL)
                printf("%d\n",searchResult->data);//�ҵ���
            else
                printf("There is no such data!\n");//û�ҵ�
            break;
        case '3'://��AVL��ɾ���ؼ���Ϊdata�Ľڵ�
            printf("What's the key of the data you want to delete?\n");
            scanf("%d",&deleteKey);getchar();
            tree = Delete(deleteKey,tree);
            break;
        case '4'://�õ�AVL�����ݵ�����
            GetOrder(tree);break;
        case '5'://��AVL���в�������data
            printf("What's the key of the data you want to insert?\n");
            scanf("%d",&insertKey);getchar();
            tree = Insert(insertKey,tree);
            break;
        case '6'://������̵���ת����
            InsertLLTest();InsertLRTest();InsertRLTest();InsertRRTest();
            printf("Test success!\n");break;
        case '7'://ɾ�����̵���ת����
            DeleteLLTest();DeleteLRTest();DeleteRLTest();DeleteRRTest();
            printf("Test success!\n");break;
        default:exit(0);
        }
    }
    return 0;
}
