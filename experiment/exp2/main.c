#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAX 64
//���Ľڵ�
struct node{
    struct node *lchild;
    struct node *rchild;
    unsigned data;
    int degree;//�ó�Աֻ���ڼ������Ŀ�ȵ�ʱ����õ�
};
typedef struct node* binaryTree;
//���ǵݹ�������ʱ�Ľڵ�ջ
struct nodeStack{
    binaryTree bt;
    int flag;//�ó�Աֻ���ڷǵݹ�������ʱ�õ�
};
//�ڵ���нṹ���������ʱʹ��
struct nodeQueue{
    int front;
    int rear;
    binaryTree elements[MAX];
};
int count = 0;//globel_value, �����ڵݹ����ɶ�����ʱ����¼��������������Ա��ļ�д��
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
    int i = 0;//�ڵ���������
    int j;//���ڵ�����
    int n;//�ڵ��������
    int num;//�ڵ�����
    FILE *fp;
    binaryTree root = NULL;//���ڵ�
    binaryTree p = NULL;//��ʱ�ڵ�

    printf("Please input the largest index of the nodes.\n");
    scanf("%d",&n);getchar();//����ڵ��������
    printf("Please input the index and the data.\n");
    scanf("%d %d",&i,&num);getchar();//����ÿ���ڵ����������Ӧ������
    while(i!=0){//����������Ϊ0ʱ�˳�
        p = (binaryTree)malloc(sizeof(struct node));
        if(p == NULL){
            printf("No more memory for the tree!\n");
            exit(0);
        }
        p->data = num;
        p->lchild = NULL;//initial
        p->rchild = NULL;//initial
        layerNode[i] = p;//�ѵ�ǰ�ڵ�ŵ���Ӧ�Ľڵ�����λ����
        if(i==1){
            root = p;// initial root
        }else{
            j = i/2;
            if(i%2==0){//�����ǰ�ڵ�������ż�����������丸�׵�����
                layerNode[j]->lchild = p;
            }else{//�������丸�׵��Һ���
                layerNode[j]->rchild = p;
            }
        }
        scanf("%d %d",&i,&num);getchar();
    }
    //�ļ�����
    if((fp = fopen("aLayerOrderBtree.txt","w"))==NULL){
        printf("Fail to create the text!"); exit(0);
    }
    for(int k=1;k<=n;k++){
        if(layerNode[k]!=NULL){//���������нڵ�
            fprintf(fp,"%d ",layerNode[k]->data);
        }else{//����
            fprintf(fp,"# ");
        }

    }
    fclose(fp);
    return root;
}
binaryTree recursionCreatePreOrder(struct node *layerNode[MAX]){
    int num;//�û���������
    binaryTree t = NULL;//��ʱ�ڵ�
    scanf("%d",&num);getchar();
    count++;

    if(num < 0){//������Ϊ��������ʾ�ýڵ�Ϊ��
        t == NULL;
        layerNode[count] = t;
    }else{//����ýڵ����
        t = (binaryTree)malloc(sizeof(struct node));
        if(t == NULL){
            printf("No more memory for the tree!\n");
            exit(0);
        }
        layerNode[count] = t;
        t->data = num;//initial data
        t->lchild = recursionCreatePreOrder(layerNode);//�ݹ����ɵ�ǰ�ڵ�������
        t->rchild = recursionCreatePreOrder(layerNode);//�����������󣬵ݹ����ɵ�ǰ�ڵ�������
    }
    return t;//���ص�ǰ���ĸ��ڵ�
}
void recPreOrder(binaryTree bt){
    if(bt==NULL){//�����ǰ�ڵ�Ϊ��
        printf("# ");
    }else{//����
        printf("%d ",bt->data);//������ýڵ�
        recPreOrder(bt->lchild);//�ٱ���������
        recPreOrder(bt->rchild);//�����������
    }
}
void recInOrder(binaryTree bt){
    if(bt==NULL){//�����ǰ�ڵ�Ϊ��
        printf("# ");
    }else{//����
        recInOrder(bt->lchild);//�ȱ���������
        printf("%d ",bt->data);//���������������������ǰ�ڵ�
        recInOrder(bt->rchild);//�ٱ���������
    }
}
void recPostOrder(binaryTree bt){
    if(bt==NULL){//�����ǰ�ڵ�Ϊ��
        printf("# ");
    }else{//����
        recPostOrder(bt->lchild);//�ȱ���������
        recPostOrder(bt->rchild);//�ٱ���������
        printf("%d ",bt->data);//�����ǰ�ڵ�
    }
}
void preOrder(binaryTree bt){
    int top = -1;//ջ������
    struct nodeStack stack[MAX];

    while(bt!=NULL || top!=-1){//��ǰ�ڵ㲻Ϊ�գ�����ջ��Ϊ��
        while(bt!=NULL){//��ǰ�ڵ㲻Ϊ�գ�Ҳ����ζ��һ����ĳ��������������ĸ�
            printf("%d ",bt->data);//�����
            stack[++top].bt = bt;//��ǰ�ڵ��ջ
            bt = bt->lchild;//��������ǰ��
        }
        printf("# ");//����ѭ��ִ�н�����˵������һ���սڵ�
        //������������ǰ������ͷʱ���ж�ջ�Ƿ�Ϊ��
        if(top!=-1){//��ջ��Ϊ��
            bt = stack[top--].bt;//��ջ����
            bt = bt->rchild;//��������ǰ��
        }
    }
    printf("#");//���Ҫ��һ�������·��Ŀսڵ�
}
void inOrder(binaryTree bt){
    int top = -1;//ջ������
    struct nodeStack stack[MAX];

    while(bt!=NULL || top!=-1){//��ǰ�ڵ㲻Ϊ�գ�����ջ��Ϊ��
        while(bt!=NULL){//��ǰ�ڵ㲻Ϊ�գ�Ҳ����ζ��һ����ĳ��������������ĸ�
            stack[++top].bt = bt;//��ǰ�ڵ��ջ
            bt = bt->lchild;//��������ǰ��
            //ע��ǰ�������ж������
        }
        printf("# ");//����ѭ��ִ�н�����˵������һ���սڵ�
        //������������ǰ������ͷʱ���ж�ջ�Ƿ�Ϊ��
        if(top!=-1){//��ջ��Ϊ��
            bt = stack[top--].bt;//��ʱbt�ǿգ�Ӧ�û���һ���������
            printf("%d ",bt->data);
            bt = bt->rchild;//��ǰ�ڵ��������ǰ��
        }
    }
    printf("#");//���Ҫ��һ�������·��Ŀսڵ�
}
void postOrder(binaryTree bt){
    int top = -1;//ջ������
    struct nodeStack stack[MAX];
    binaryTree root = bt;//��¼�������ڵ�

    while(bt!=NULL || top!=-1){//��ǰ�ڵ㲻Ϊ�գ�����ջ��Ϊ��
        while(bt!=NULL){//��ǰ�ڵ㲻Ϊ�գ�Ҳ����ζ��һ����ĳ��������������ĸ�
            stack[++top].bt = bt;//��ǰ�ڵ��ջ
            stack[top].flag = 1;//��ǵ�ǰ�ڵ�Ϊ1�����赱ǰ�ڵ��������ӣ�����ʱ��Ӧֱ�����
            bt = bt->lchild;//��������ǰ��
            //ע��ǰ�������ж������
        }
        printf("# ");//����ѭ��ִ�н�����˵������һ���սڵ�
        //������������ǰ������ͷʱ���ж�ջ�Ƿ�Ϊ��
        while(top!=-1 && stack[top].flag==2){//��ջ��Ϊ��ʱ������ջ�����Ϊ2ʱ
            bt = stack[top--].bt;//��ջ
            printf("%d ",bt->data);//���
        }
        if(top!=-1){//��ǰջ��Ϊ��
            stack[top].flag = 2;//��ǰ�ڵ�������������Ҳ���ǻ��ݹ����п������
            bt = stack[top].bt->rchild;//������������ǰ��
        }
        if(bt == root)break;//������ݵ����ڵ㣬ֱ���˳�
    }
    //�����Ǻ���������������貹���¿սڵ�
}
void levelOrder(binaryTree bt){
    //�����ڵ����
    struct nodeQueue q;
    q.front = 0;
    q.rear = MAX-1;
    for(int i=0;i<MAX;i++)
        q.elements[i] = NULL;//���г�ʼ�����б�Ҫ
    if(bt==NULL)return;//����ֱ�ӷ��أ���ʵ������
    q.rear = (q.rear+1)%MAX;//initial queue
    q.elements[q.rear] = bt;//���ڵ����
    while(((q.rear+1)%MAX)!=q.front){//���Ӳ�Ϊ��ʱ
        if(q.elements[q.front]==NULL){//�����ǿսڵ㣬���һ��'#'
            printf("# ");
        }else{//����
            printf("%d ",q.elements[q.front]->data);//�����ǰ�ڵ�
            if(q.elements[q.front]->lchild!=NULL){//�����ǰ�ڵ������ӣ����ӽ���
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->lchild;
            }else{
                q.rear = (q.rear+1)%MAX;//�սڵ����
            }
            if(q.elements[q.front]->rchild!=NULL){//�����ǰ�ڵ����Һ��ӣ��Һ��ӽ���
                q.rear = (q.rear+1)%MAX;
                q.elements[q.rear] = q.elements[q.front]->rchild;
            }else{
                q.rear = (q.rear+1)%MAX;//�սڵ����
            }
        }
        q.front = (q.front+1)%MAX;//��ǰ�ڵ����
    }
}
int isComplete(binaryTree bt){//���㷨�󲿷�����������ͬ����ע�Ͳ�֮ͬ��
    struct nodeQueue q;
    q.front = 0;
    q.rear = MAX-1;
    for(int i=0;i<MAX;i++)
        q.elements[i] = NULL;//��ʼ�����У��б�Ҫ
    if(bt==NULL)return;//�������أ���ʵ������
    q.rear = (q.rear+1)%MAX;
    q.elements[q.rear] = bt;
    while(((q.rear+1)%MAX)!=q.front){
        if(q.elements[q.front]!=NULL){//ֻ���ڵ�ǰ�ڵ㲻�ǿսڵ�ʱ���Ż�ȥ�ж������ӽڵ�
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
        if(q.elements[q.front]==NULL && q.elements[q.front+1]!=NULL)//���ĳһʱ�̣������ǿսڵ㣬�����������Ƿǿսڵ㣬��ôһ���Ƿ���ȫ������
            return 0;
        q.front = (q.front+1)%MAX;
    }
    return 1;//������ʱ�̣������ǿսڵ������£���������Ҳ�ǿսڵ㣬˵������ȫ������
}
int treeWidth(struct node *layerRealNode[MAX], int k, int l, int width){//s�ǲ�������õ���ڵ����У�k�ǵ�ǰ�ڵ�������l�Ǹò����һ���ǿսڵ��������pm�ǽ�ֹ��Ŀǰ�������
    int m = 0;//���ڼ��㵱ǰ��Ŀ��
    for(int i=k;i<=l;i++)
        m += layerRealNode[i]->degree;
    if(m>width)width = m;//���������
    if(m!=0)width = treeWidth(layerRealNode,l+1,l+m,width);//��ǰ���нڵ㣬�����ݹ�
    return width;//���������
}
void getNodeDegree(binaryTree bt){//�õ�ÿ���ǿսڵ�Ķ�
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
    if(bt==NULL)return;//����ֱ�ӷ��أ���ʵ������
    q.rear = (q.rear+1)%MAX;//initial queue
    q.elements[q.rear] = bt;//���ڵ����
    while(((q.rear+1)%MAX)!=q.front){//���Ӳ�Ϊ��ʱ
        if(q.elements[q.front]->lchild!=NULL){//�����ǰ�ڵ������ӣ����ӽ���
            q.rear = (q.rear+1)%MAX;
            q.elements[q.rear] = q.elements[q.front]->lchild;
        }
        if(q.elements[q.front]->rchild!=NULL){//�����ǰ�ڵ����Һ��ӣ��Һ��ӽ���
            q.rear = (q.rear+1)%MAX;
            q.elements[q.rear] = q.elements[q.front]->rchild;
        }
        layerRealNode[++i] = q.elements[q.front];
        q.front = (q.front+1)%MAX;//��ǰ�ڵ����
    }
}
void printTree(binaryTree bt){
    if(bt!=NULL){
        printf("[");//ÿһ����������ǰ��֮ǰ�����һ��������
        printTree(bt->lchild);//�ȱ���������
        printf("%d",bt->data);//���������������������ǰ�ڵ�
        printTree(bt->rchild);//�ٱ���������
        printf("]");//�ڱ���������֮�󣬺�������ƥ��
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
    binaryTree root = NULL;//���������ڵ�
    struct node *layerNode[MAX];//����ڵ����飬�����սڵ�
    struct node *layerRealNode[MAX];//����ڵ����飬�������սڵ�
    FILE *fp;//�ļ�ָ��
    char choice;//�˵�ѡ��
    int width = 0;//�����
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
