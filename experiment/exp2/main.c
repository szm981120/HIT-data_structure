#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAX 64
//树的节点
struct node{
    struct node *lchild;
    struct node *rchild;
    unsigned data;
    int degree;//该成员只有在计算树的宽度的时候会用到
};
typedef struct node* binaryTree;
//做非递归树遍历时的节点栈
struct nodeStack{
    binaryTree bt;
    int flag;//该成员只有在非递归后序遍历时用到
};
//节点队列结构，层序遍历时使用
struct nodeQueue{
    int front;
    int rear;
    binaryTree elements[MAX];
};
int count = 0;//globel_value, 仅用于递归生成二叉树时，记录树的最大索引，以便文件写入
binaryTree CreateLayerOrder(struct node *layerNode[MAX]);
binaryTree recursionCreatePreOrder(struct node *layerNode[MAX]);
void recPreOrder(binaryTree bt);
void recInOrder(binaryTree bt);
void recPostOrder(binaryTree bt);
void preOrder(binaryTree bt);
void inOrder(binaryTree bt);
void postOrder(binaryTree bt);
void levelOrder(binaryTree bt);
int isComplete(binaryTree bt);
int treeWidth(struct node *layerRealNode[MAX], int k, int l, int width);
void getNodeDegree(binaryTree bt);
void levelOrderStoringNode(binaryTree bt, struct node *layerRealNode[MAX]);
void printTree(binaryTree bt);
void menu();

binaryTree CreateLayerOrder(struct node *layerNode[MAX]){
    int i = 0;//节点数组索引
    int j;//父节点索引
    int n;//节点最大索引
    int num;//节点数据
    FILE *fp;
    binaryTree root = NULL;//根节点
    binaryTree p = NULL;//临时节点

    printf("Please input the largest index of the nodes.\n");
    scanf("%d",&n);getchar();//输入节点最大索引
    printf("Please input the index and the data.\n");
    scanf("%d %d",&i,&num);getchar();//输入每个节点的索引及对应的数据
    while(i!=0){//当输入索引为0时退出
        p = (binaryTree)malloc(sizeof(struct node));
        if(p == NULL){
            printf("No more memory for the tree!\n");
            exit(0);
        }
        p->data = num;
        p->lchild = NULL;//initial
        p->rchild = NULL;//initial
        layerNode[i] = p;//把当前节点放到对应的节点数组位置上
        if(i==1){
            root = p;// initial root
        }else{
            j = i/2;
            if(i%2==0){//如果当前节点索引是偶数，则其是其父亲的左孩子
                layerNode[j]->lchild = p;
            }else{//否则是其父亲的右孩子
                layerNode[j]->rchild = p;
            }
        }
        scanf("%d %d",&i,&num);getchar();
    }
    //文件保存
    if((fp = fopen("aLayerOrderBtree.txt","w"))==NULL){
        printf("Fail to create the text!"); exit(0);
    }
    for(int k=1;k<=n;k++){
        if(layerNode[k]!=NULL){//该索引处有节点
            fprintf(fp,"%d ",layerNode[k]->data);
        }else{//否则
            fprintf(fp,"# ");
        }

    }
    fclose(fp);
    return root;
}
binaryTree recursionCreatePreOrder(struct node *layerNode[MAX]){
    int num;//用户输入数据
    binaryTree t = NULL;//临时节点
    scanf("%d",&num);getchar();
    count++;

    if(num < 0){//若数据为负数，表示该节点为空
        t == NULL;
        layerNode[count] = t;
    }else{//否则该节点存在
        t = (binaryTree)malloc(sizeof(struct node));
        if(t == NULL){
            printf("No more memory for the tree!\n");
            exit(0);
        }
        layerNode[count] = t;
        t->data = num;//initial data
        t->lchild = recursionCreatePreOrder(layerNode);//递归生成当前节点左子树
        t->rchild = recursionCreatePreOrder(layerNode);//生成左子树后，递归生成当前节点右子树
    }
    return t;//返回当前树的根节点
}
void recPreOrder(binaryTree bt){
    if(bt==NULL){//如果当前节点为空
        printf("# ");
    }else{//否则
        printf("%d ",bt->data);//先输出该节点
        recPreOrder(bt->lchild);//再遍历左子树
        recPreOrder(bt->rchild);//后遍历右子树
    }
}
void recInOrder(binaryTree bt){
    if(bt==NULL){//如果当前节点为空
        printf("# ");
    }else{//否则
        recInOrder(bt->lchild);//先遍历左子树
        printf("%d ",bt->data);//左子树遍历结束后，输出当前节点
        recInOrder(bt->rchild);//再遍历右子树
    }
}
void recPostOrder(binaryTree bt){
    if(bt==NULL){//如果当前节点为空
        printf("# ");
    }else{//否则
        recPostOrder(bt->lchild);//先遍历左子树
        recPostOrder(bt->rchild);//再遍历右子树
        printf("%d ",bt->data);//输出当前节点
    }
}
void preOrder(binaryTree bt){
    int top = -1;//栈顶索引
    struct nodeStack stack[MAX];

    while(bt!=NULL || top!=-1){//当前节点不为空，或者栈不为空
        while(bt!=NULL){//当前节点不为空，也就意味着一定是某棵树（或空树）的根
            printf("%d ",bt->data);//先输出
            stack[++top].bt = bt;//当前节点进栈
            bt = bt->lchild;//向左子树前进
        }
        printf("# ");//上述循环执行结束后说明遇到一个空节点
        //当左子树方向前进到尽头时，判断栈是否为空
        if(top!=-1){//若栈不为空
            bt = stack[top--].bt;//退栈回溯
            bt = bt->rchild;//向右子树前进
        }
    }
    printf("#");//最后要补一个最右下方的空节点
}
void inOrder(binaryTree bt){
    int top = -1;//栈顶索引
    struct nodeStack stack[MAX];

    while(bt!=NULL || top!=-1){//当前节点不为空，或者栈不为空
        while(bt!=NULL){//当前节点不为空，也就意味着一定是某棵树（或空树）的根
            stack[++top].bt = bt;//当前节点进栈
            bt = bt->lchild;//向左子树前进
            //注意前进过程中都不输出
        }
        printf("# ");//上述循环执行结束后说明遇到一个空节点
        //当左子树方向前进到尽头时，判断栈是否为空
        if(top!=-1){//若栈不为空
            bt = stack[top--].bt;//此时bt是空，应该回溯一个，再输出
            printf("%d ",bt->data);
            bt = bt->rchild;//向当前节点的右子树前进
        }
    }
    printf("#");//最后要补一个最右下方的空节点
}
void postOrder(binaryTree bt){
    int top = -1;//栈顶索引
    struct nodeStack stack[MAX];
    binaryTree root = bt;//记录下树根节点

    while(bt!=NULL || top!=-1){//当前节点不为空，或者栈不为空
        while(bt!=NULL){//当前节点不为空，也就意味着一定是某棵树（或空树）的根
            stack[++top].bt = bt;//当前节点进栈
            stack[top].flag = 1;//标记当前节点为1，假设当前节点仍有左孩子，回溯时不应直接输出
            bt = bt->lchild;//向左子树前进
            //注意前进过程中都不输出
        }
        printf("# ");//上述循环执行结束后说明遇到一个空节点
        //当左子树方向前进到尽头时，判断栈是否为空
        while(top!=-1 && stack[top].flag==2){//当栈不为空时，并且栈顶标记为2时
            bt = stack[top--].bt;//退栈
            printf("%d ",bt->data);//输出
        }
        if(top!=-1){//当前栈不为空
            stack[top].flag = 2;//当前节点再无左子树，也就是回溯过程中可以输出
            bt = stack[top].bt->rchild;//向右子树方向前进
        }
        if(bt == root)break;//如果回溯到根节点，直接退出
    }
    //由于是后根序遍历，最后无需补右下空节点
}
void levelOrder(binaryTree bt){
    //构建节点队列
    struct nodeQueue q;
    q.front = 0;
    q.rear = MAX-1;
    for(int i=0;i<MAX;i++)
        q.elements[i] = NULL;//队列初始化，有必要
    if(bt==NULL)return;//空树直接返回，无实际意义
    q.rear = (q.rear+1)%MAX;//initial queue
    q.elements[q.rear] = bt;//根节点进队
    while(((q.rear+1)%MAX)!=q.front){//当队不为空时
        if(q.elements[q.front]==NULL){//队首是空节点，输出一个'#'
            printf("# ");
        }else{//否则
            printf("%d ",q.elements[q.front]->data);//输出当前节点
            if(q.elements[q.front]->lchild!=NULL){//如果当前节点有左孩子，左孩子进队
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->lchild;
            }else{
                q.rear = (q.rear+1)%MAX;//空节点入队
            }
            if(q.elements[q.front]->rchild!=NULL){//如果当前节点有右孩子，右孩子进队
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->rchild;
            }else{
                q.rear = (q.rear+1)%MAX;//空节点入队
            }
        }
        q.front = (q.front+1)%MAX;//当前节点出队
    }
}
int isComplete(binaryTree bt){//该算法大部分与层序遍历相同，仅注释不同之处
    struct nodeQueue q;
    q.front = 0;
    q.rear = MAX-1;
    for(int i=0;i<MAX;i++)
        q.elements[i] = NULL;//初始化队列，有必要
    if(bt==NULL)return;//空树返回，无实际意义
    q.rear = (q.rear+1)%MAX;
    q.elements[q.rear] = bt;
    while(((q.rear+1)%MAX)!=q.front){
        if(q.elements[q.front]!=NULL){//只有在当前节点不是空节点时，才会去判断他的子节点
            if(q.elements[q.front]->lchild!=NULL){
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->lchild;
            }else{
                q.rear = (q.rear+1)%MAX;
            }
            if(q.elements[q.front]->rchild!=NULL){
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->rchild;
            }else{
                q.rear = (q.rear+1)%MAX;
            }
        }
        if(q.elements[q.front]==NULL && q.elements[q.front+1]!=NULL)//如果某一时刻，队首是空节点，而紧随其后的是非空节点，那么一定是非完全二叉树
            return 0;
        q.front = (q.front+1)%MAX;
    }
    return 1;//在任意时刻，队首是空节点的情况下，紧随其后的也是空节点，说明是完全二叉树
}
int treeWidth(struct node *layerRealNode[MAX], int k, int l, int width){//s是层序遍历得到额节点序列，k是当前节点索引，l是该层最后一个非空节点的索引，pm是截止到目前的最大宽度
    int m = 0;//用于计算当前层的宽度
    for(int i=k;i<=l;i++)
        m += layerRealNode[i]->degree;
    if(m>width)width = m;//更新最大宽度
    if(m!=0)width = treeWidth(layerRealNode,l+1,l+m,width);//当前层有节点，继续递归
    return width;//返回最大宽度
}
void getNodeDegree(binaryTree bt){//得到每个非空节点的度
    if(bt!=NULL){
        bt->degree = 0;
        if(bt->lchild!=NULL&&bt->lchild->data!=-1)bt->degree++;
        if(bt->rchild!=NULL&&bt->rchild->data!=-1)bt->degree++;
        getNodeDegree(bt->lchild);
        getNodeDegree(bt->rchild);
    }
}
void levelOrderStoringNode(binaryTree bt, struct node *layerRealNode[MAX]){
    struct nodeQueue q;
    int i = 0;
    q.front = 0;
    q.rear = MAX-1;
    if(bt==NULL)return;//空树直接返回，无实际意义
    q.rear = (q.rear+1)%MAX;//initial queue
    q.elements[q.rear] = bt;//根节点进队
    while(((q.rear+1)%MAX)!=q.front){//当队不为空时
        if(q.elements[q.front]->lchild!=NULL){//如果当前节点有左孩子，左孩子进队
            q.rear = (q.rear+1)%MAX;
            q.elements[q.rear] = q.elements[q.front]->lchild;
        }
        if(q.elements[q.front]->rchild!=NULL){//如果当前节点有右孩子，右孩子进队
            q.rear = (q.rear+1)%MAX;
            q.elements[q.rear] = q.elements[q.front]->rchild;
        }
        layerRealNode[++i] = q.elements[q.front];
        q.front = (q.front+1)%MAX;//当前节点出队
    }
}
void printTree(binaryTree bt){
    if(bt!=NULL){
        printf("[");//每一次向左子树前进之前都输出一个左括号
        printTree(bt->lchild);//先遍历左子树
        printf("%d",bt->data);//左子树遍历结束后，输出当前节点
        printTree(bt->rchild);//再遍历右子树
        printf("]");//在遍历右子树之后，和左括号匹配
    }
}
void menu(){
    printf("1. Create a layer-order binary tree.\n");
    printf("2. Create a pre-order binary tree with recursion method.\n");
    printf("3. (Recursion)Traversal the binary tree in pre-order.\n");
    printf("4. (Recursion)Traversal the binary tree in in-order.\n");
    printf("5. (Recursion)Traversal the binary tree in post-order.\n");
    printf("6. Traversal the binary tree in pre-order.\n");
    printf("7. Traversal the binary tree in in-order.\n");
    printf("8. Traversal the binary tree in post-order.\n");
    printf("9. Traversal the binary tree in level-order.\n");
    printf("a. Judge if the binary tree is complete.\n");
    printf("b. Print the shape of the tree.\n");
    printf("c. How wide is this tree?\n");
}
int main()
{
    binaryTree root = NULL;//二叉树根节点
    struct node *layerNode[MAX];//层序节点数组，包含空节点
    struct node *layerRealNode[MAX];//层序节点数组，不包含空节点
    FILE *fp;//文件指针
    char choice;//菜单选择
    int width = 0;//最大宽度
    int k = 1;

    while(1){
        menu();
        scanf("%c",&choice);
        getchar();
        switch(choice){
            case '1':
                for(int i=0;i<MAX;i++)//initial
                    layerNode[i] = NULL;
                root = CreateLayerOrder(layerNode);break;
            case '2':
                count = 0;
                for(int i=0;i<MAX;i++)//initial
                    layerNode[i] = NULL;
                root = recursionCreatePreOrder(layerNode);
                if((fp = fopen("aPreOrderBtree(Recursion).txt","w"))==NULL){
                    printf("Fail to create the text!"); exit(0);
                }
                for(int i=1;i<=count;i++){
                    if(layerNode[i]==NULL)
                        fprintf(fp,"# ");
                    else
                        fprintf(fp,"%d ",layerNode[i]->data);
                }
                fclose(fp);
                break;
            case '3':
                recPreOrder(root);printf("\n");break;
            case '4':
                recInOrder(root);printf("\n");break;
            case '5':
                recPostOrder(root);printf("\n");break;
            case '6':
                preOrder(root);printf("\n");break;
            case '7':
                inOrder(root);printf("\n");break;
            case '8':
                postOrder(root);printf("\n");break;
            case '9':
                levelOrder(root);printf("\n");break;
            case 'a':
            case 'A':
                if(isComplete(root))
                    printf("The binary tree is complete.\n");
                else
                    printf("The binary tree is not complete.\n");
                break;
            case 'b':
            case 'B':
                printTree(root);printf("\n");break;
            case 'c':
            case 'C':
                getNodeDegree(root);
                levelOrderStoringNode(root,layerRealNode);
                k = 1;
                width = treeWidth(layerRealNode,k,k,width);
                printf("The width of the binary tree is: %d\n",width);
                break;
            default:exit(0);
        }
    }
    return 0;
}
