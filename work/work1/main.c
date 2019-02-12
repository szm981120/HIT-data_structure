#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_SIZE 60

typedef struct stackOp{//操作符栈
    char elements[MAX_SIZE];
    int top;//栈顶标志是栈顶元素的索引+1，若是空栈，则top=0
}stackOp;
typedef struct stackNum{//操作数栈
    double elements[MAX_SIZE];
    int top;//栈顶标志是栈顶元素的索引+1，若是空栈，则top=0
}stackNum;
void getPostfix();//从文件中读取中缀表达式，并转成后缀表达式字符串保存到postfix中，同时将字符串保存到文件中
double calValue();//从文件中读取后缀表达式，返回结果值
char popOp(stackOp *stOp);//从操作符栈出栈
double popNum(stackNum *stNum);//从操作数栈出栈
void pushOp(char x, stackOp *stOp);//向操作符栈入栈
void pushNum(double x, stackNum *stNum);//向操作数栈入栈
int opLevel(char op);//运算符优先级
int datatype(char c);//判断字符是否是数字或小数点

void getPostfix(){
    FILE *fp;
    int t = -1;//后缀字符串的最大索引，用于向后缀表达式字符串中写入字符
    int k = 0;//无实际意义，显示结果时的序号
    char op;//用于临时保存从操作符栈出栈的操作符
    char ch;//用于临时保存从文件读取的字符
    char previousCh;//用于保存上一个字符，与判断‘-’有关
    stackOp *stOp = NULL;//操作符栈
    char *postfix;//后缀表达式字符串
    //初始化栈
    stOp = (stackOp*)malloc(sizeof(stackOp)*MAX_SIZE);
    if(stOp == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stOp->top = 0;
    //初始化字符串
    postfix = (char*)malloc(sizeof(char)*MAX_SIZE);
    if(postfix == NULL){
        printf("No memory for the string!\n");
    }
    //打开文件
    if((fp = fopen("equation.txt","r")) == NULL){
        printf("Failure to open equation.txt!\n");
        exit(0);
    }
    //开始读中缀表达式，其中在字符串中赋值为空，是为了在后续读后缀表达式的过程中，便于分离操作数和操作符
    ch = fgetc(fp);
    while(ch!=EOF){
        if(!datatype(ch)){
            if(ch=='-'&&(t==-1||previousCh=='(')){//最开始的'-'，前面是'('的'-'都属于负号，按数字处理
                t++;postfix[t] = ch;
            }else if(stOp->top==0){//第一次读操作符
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else if(ch=='('){//左括号
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else if(ch==')'){//右括号
                op = popOp(stOp);
                while(op!='('){
                    t++;postfix[t] = ' ';
                    t++;postfix[t] = op;
                    op = popOp(stOp);
                }
            }else if(opLevel(ch)>=opLevel(stOp->elements[stOp->top-1])){//栈顶操作符优先级不大于当前操作符优先级
                t++;postfix[t] = ' ';
                pushOp(ch,stOp);
            }else{//栈顶操作符优先级大于当前操作符优先级
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
        }else{//是数字
            t++;postfix[t] = ch;
        }
        printf("%d. The present character is:%c\tThe operation stack is:",++k,ch);
        printStackOp(stOp);//显示栈的状态
        previousCh = ch;
        ch = fgetc(fp);
    }
    fclose(fp);//关闭文件
    //表达式读完，最后全部出栈
    do{
        op = popOp(stOp);
        t++;postfix[t] = ' ';
        t++;postfix[t] = op;
    }while(stOp->top!=0);
    t++;postfix[t] = '\0';
    for(int i=0;i<=t;i++){//显示后缀表达式
        printf("%c",postfix[i]);
    }
    //保存结果到文件中
    if((fp = fopen("postfix_expression.txt","w")) == NULL){
        printf("Failure to open postfix_expression.txt!\n");
        exit(0);
    }
    for(int i=0;i<strlen(postfix);i++){
        fprintf(fp,"%c",postfix[i]);
    }
    printf("\nSave successfully!");//保存成功
    fclose(fp);//关闭文件
    free(stOp);//释放内存
    free(postfix);//释放内存
    stOp = NULL;//防止野指针
    postfix = NULL;//防止野指针
    printf("\n\n");
}
double calValue(){
    double value = 0;//最终结果
    FILE *fp;
    stackNum *stNum = NULL;//操作数栈
    stackOp *stOp = NULL;//操作符栈
    char ch;//用于保存临时读取的字符
    double number;//用于计算栈顶两个数的操作结果，更新栈顶数
    double former;//计算栈顶两操作数时，后出栈的操作数
    double latter;//计算栈顶两操作数时，先出栈的操作数
    int k = 0;//无实际意义，显示栈状态时的序号
    //初始化栈
    stNum = (stackNum*)malloc(sizeof(stackNum)*MAX_SIZE);
    if(stNum == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stNum->top = 0;
    //初始化栈
    stOp = (stackOp*)malloc(sizeof(stackOp)*MAX_SIZE);
    if(stOp == NULL){
        printf("Can't allocate memory for the stack!\n");
    }
    stOp->top = 0;
    //打开文件
    if((fp = fopen("postfix_expression.txt","r")) == NULL){
        printf("Failure to open postfix_expression.txt!\n");
        exit(0);
    }
    //开始读后缀表达式
    ch = fgetc(fp);
    while(ch!=EOF){//所有具有实际意义的读取全部写入if语句中
        if(ch==' '){//遇到分界标志
            ch = fgetc(fp);
            continue;
        }
        if(ch=='-' && stNum->top==0){//之前未有操作数，而遇到了'-'，说明一定是负号，回溯读值
            fseek(fp,-1,SEEK_CUR);
            fscanf(fp,"%lf",&number);
            pushNum(number,stNum);
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            ch = fgetc(fp);
            continue;
        }
        if(ch=='-' && stNum->top >0){//已经遇到操作数，而遇到了'-'，不能一定说明是负号
            ch = fgetc(fp);//继续判断其下一个字符
            if(datatype(ch)){//如果下一个字符是数字，说明一定是负号，回溯读值
                fseek(fp,-2,SEEK_CUR);
                fscanf(fp,"%lf",&number);
                pushNum(number,stNum);
                ch = fgetc(fp);
            }else{//否则是操作符，执行减号操作
                latter = popNum(stNum);
                former = popNum(stNum);
                number = former - latter;
                pushNum(number,stNum);
            }
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            continue;
        }
        if(datatype(ch)){//该字符是数字，回溯读值
            fseek(fp,-1,SEEK_CUR);
            fscanf(fp,"%lf",&number);
            pushNum(number,stNum);
            printf("%d. The present character is:%c\tThe number stack is:",++k,ch);
            printStackNum(stNum);
            ch = fgetc(fp);
            continue;
        }
        else{//该字符肯定是除减号之外的操作符，因为减号的可能已经在之前判断过
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
        ch = fgetc(fp);//其他可能
    }
    fclose(fp);//关闭文件
    value = stNum->elements[0];//得到结果
    //释放内存
    free(stNum);
    free(stOp);
    //防止野指针
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
    getPostfix();//得到后缀表达式
    value = calValue();//得到结果
    printf("\nThe value of the expression is: %lf\n",value);
    return 0;
}
