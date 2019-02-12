#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_SIZE 60

typedef struct stackOp{//������ջ
    char elements[MAX_SIZE];
    int top;//ջ����־��ջ��Ԫ�ص�����+1�����ǿ�ջ����top=0
}stackOp;
typedef struct stackNum{//������ջ
    double elements[MAX_SIZE];
    int top;//ջ����־��ջ��Ԫ�ص�����+1�����ǿ�ջ����top=0
}stackNum;
void getPostfix();//���ļ��ж�ȡ��׺���ʽ����ת�ɺ�׺���ʽ�ַ������浽postfix�У�ͬʱ���ַ������浽�ļ���
double calValue();//���ļ��ж�ȡ��׺���ʽ�����ؽ��ֵ
char popOp(stackOp *stOp);//�Ӳ�����ջ��ջ
double popNum(stackNum *stNum);//�Ӳ�����ջ��ջ
void pushOp(char x, stackOp *stOp);//�������ջ��ջ
void pushNum(double x, stackNum *stNum);//�������ջ��ջ
int opLevel(char op);//��������ȼ�
int datatype(char c);//�ж��ַ��Ƿ������ֻ�С����

void getPostfix(){
    FILE *fp;
    int t = -1;//��׺�ַ���������������������׺���ʽ�ַ�����д���ַ�
    int k = 0;//��ʵ�����壬��ʾ���ʱ�����
    char op;//������ʱ����Ӳ�����ջ��ջ�Ĳ�����
    char ch;//������ʱ������ļ���ȡ���ַ�
    char previousCh;//���ڱ�����һ���ַ������жϡ�-���й�
    stackOp *stOp = NULL;//������ջ
    char *postfix;//��׺���ʽ�ַ���
    //��ʼ��ջ
    stOp = (stackOp*)malloc(sizeof(stackOp)*MAX_SIZE);
    if(stOp == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stOp->top = 0;
    //��ʼ���ַ���
    postfix = (char*)malloc(sizeof(char)*MAX_SIZE);
    if(postfix == NULL){
        printf("No memory for the string!\n");
    }
    //���ļ�
    if((fp = fopen("equation.txt","r")) == NULL){
        printf("Failure to open equation.txt!\n");
        exit(0);
    }
    //��ʼ����׺���ʽ���������ַ����и�ֵΪ�գ���Ϊ���ں�������׺���ʽ�Ĺ����У����ڷ���������Ͳ�����
    ch = fgetc(fp);
    while(ch!=EOF){
        if(!datatype(ch)){
            if(ch=='-'&&(t==-1||previousCh=='(')){//�ʼ��'-'��ǰ����'('��'-'�����ڸ��ţ������ִ���
                t++;postfix[t] = ch;
            }else if(stOp->top==0){//��һ�ζ�������
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else if(ch=='('){//������
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else if(ch==')'){//������
                op = popOp(stOp);
                while(op!='('){
                    t++;postfix[t] = ' ';
                    t++;postfix[t] = op;
                    op = popOp(stOp);
                }
            }else if(opLevel(ch)>=opLevel(stOp->elements[stOp->top-1])){//ջ�����������ȼ������ڵ�ǰ���������ȼ�
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else{//ջ�����������ȼ����ڵ�ǰ���������ȼ�
                op = popOp(stOp);
                t++;postfix[t] = ' ';
                t++;postfix[t] = op;
                while(opLevel(ch)<opLevel(stOp->elements[stOp->top-1])){
                    op = popOp(stOp);
                    t++;postfix[t] = ' ';
                    t++;postfix[t] = op;
                }
                pushOp(ch,stOp);
                t++;postfix[t] = ' ';
            }
        }else{//������
            t++;postfix[t] = ch;
        }
        printf("%d. The present character is:%c\tThe operation stack is:",++k,ch);
        printStackOp(stOp);//��ʾջ��״̬
        previousCh = ch;
        ch = fgetc(fp);
    }
    fclose(fp);//�ر��ļ�
    //���ʽ���꣬���ȫ����ջ
    do{
        op = popOp(stOp);
        t++;postfix[t] = ' ';
        t++;postfix[t] = op;
    }while(stOp->top!=0);
    t++;postfix[t] = '\0';
    for(int i=0;i<=t;i++){//��ʾ��׺���ʽ
        printf("%c",postfix[i]);
    }
    //���������ļ���
    if((fp = fopen("postfix_expression.txt","w")) == NULL){
        printf("Failure to open postfix_expression.txt!\n");
        exit(0);
    }
    for(int i=0;i<strlen(postfix);i++){
        fprintf(fp,"%c",postfix[i]);
    }
    printf("\nSave successfully!");//����ɹ�
    fclose(fp);//�ر��ļ�
    free(stOp);//�ͷ��ڴ�
    free(postfix);//�ͷ��ڴ�
    stOp = NULL;//��ֹҰָ��
    postfix = NULL;//��ֹҰָ��
    printf("\n\n");
}
double calValue(){
    double value = 0;//���ս��
    FILE *fp;
    stackNum *stNum = NULL;//������ջ
    stackOp *stOp = NULL;//������ջ
    char ch;//���ڱ�����ʱ��ȡ���ַ�
    double number;//���ڼ���ջ���������Ĳ������������ջ����
    double former;//����ջ����������ʱ�����ջ�Ĳ�����
    double latter;//����ջ����������ʱ���ȳ�ջ�Ĳ�����
    int k = 0;//��ʵ�����壬��ʾջ״̬ʱ�����
    //��ʼ��ջ
    stNum = (stackNum*)malloc(sizeof(stackNum)*MAX_SIZE);
    if(stNum == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stNum->top = 0;
    //��ʼ��ջ
    stOp = (stackOp*)malloc(sizeof(stackOp)*MAX_SIZE);
    if(stOp == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stOp->top = 0;
    //���ļ�
    if((fp = fopen("postfix_expression.txt","r")) == NULL){
        printf("Failure to open postfix_expression.txt!\n");
        exit(0);
    }
    //��ʼ����׺���ʽ
    ch = fgetc(fp);
    while(ch!=EOF){//���о���ʵ������Ķ�ȡȫ��д��if�����
        if(ch==' '){//�����ֽ��־
            ch = fgetc(fp);
            continue;
        }
        if(ch=='-' && stNum->top==0){//֮ǰδ�в���������������'-'��˵��һ���Ǹ��ţ����ݶ�ֵ
            fseek(fp,-1,SEEK_CUR);
            fscanf(fp,"%lf",&number);
            pushNum(number,stNum);
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            ch = fgetc(fp);
            continue;
        }
        if(ch=='-' && stNum->top >0){//�Ѿ���������������������'-'������һ��˵���Ǹ���
            ch = fgetc(fp);//�����ж�����һ���ַ�
            if(datatype(ch)){//�����һ���ַ������֣�˵��һ���Ǹ��ţ����ݶ�ֵ
                fseek(fp,-2,SEEK_CUR);
                fscanf(fp,"%lf",&number);
                pushNum(number,stNum);
                ch = fgetc(fp);
            }else{//�����ǲ�������ִ�м��Ų���
                latter = popNum(stNum);
                former = popNum(stNum);
                number = former - latter;
                pushNum(number,stNum);
            }
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            continue;
        }
        if(datatype(ch)){//���ַ������֣����ݶ�ֵ
            fseek(fp,-1,SEEK_CUR);
            fscanf(fp,"%lf",&number);
            pushNum(number,stNum);
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            ch = fgetc(fp);
            continue;
        }
        else{//���ַ��϶��ǳ�����֮��Ĳ���������Ϊ���ŵĿ����Ѿ���֮ǰ�жϹ�
            latter = popNum(stNum);
            former = popNum(stNum);
            switch(ch){
            case '+':
                number = latter+former;
                pushNum(number,stNum);
                break;
            case '*':
                number = former * latter;
                pushNum(number,stNum);
                break;
            case '/':
                number = former / latter;
                pushNum(number,stNum);
                break;
            case '^':
                number = pow(former,latter);
                pushNum(number,stNum);
                break;
            }
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
        }
        ch = fgetc(fp);//��������
    }
    fclose(fp);//�ر��ļ�
    value = stNum->elements[0];//�õ����
    //�ͷ��ڴ�
    free(stNum);
    free(stOp);
    //��ֹҰָ��
    stNum = NULL;
    stOp = NULL;
    return value;
}
void printStackOp(stackOp *stOp){
    for(int i=0;i<stOp->top;i++){
        printf("%c ",stOp->elements[i]);
    }
    printf("\n");
}
void printStackNum(stackNum *stNum){
    for(int i=0;i<stNum->top;i++){
        printf("%.2lf ",stNum->elements[i]);
    }
    printf("\n");
}
char popOp(stackOp *stOp){
    if(stOp->top == 0){
        printf("Stack is empty!\n");
        return -1;
    }else{
        stOp->top = stOp->top - 1;
        return stOp->elements[stOp->top];
    }
}
double popNum(stackNum *stNum){
    if(stNum->top == 0){
        printf("Stack is empty!\n");
        return 0;
    }else{
        stNum->top = stNum->top - 1;
        return stNum->elements[stNum->top];
    }
}
void pushOp(char x, stackOp *stOp){
    if(stOp->top == MAX_SIZE){
        printf("Stack is full!\n");
    }else{
        stOp->elements[stOp->top]= x;
        stOp->top++;
    }
}
void pushNum(double x, stackNum *stNum){
    if(stNum->top == MAX_SIZE){
        printf("Stack is full!\n");
    }else{
        stNum->elements[stNum->top] = x;
        stNum->top++;
    }
}
int opLevel(char op){
    if(op=='+')return 1;
    if(op=='-')return 2;
    if(op=='*')return 3;
    if(op=='/')return 4;
    if(op=='^')return 5;
    if(op=='(')return 0;
    if(op==')')return 6;
}
int datatype(char c){
    if((c>='0'&&c<='9')||c=='.'){
        return 1;
    }else{
        return 0;
    }
}
int main()
{
    double value;
    getPostfix();//�õ���׺���ʽ
    value = calValue();//�õ����
    printf("\nThe value of the expression is: %lf\n",value);
    return 0;
}
