#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct term{
    double coef;
    int exp;
    struct term *next;
};
typedef struct term* termnode;
//若无特殊说明，以下注释中的表头均指链表的第一项
termnode createPolynomial(termnode termHead, int termN, FILE *fp);//创建单个新多项式，返回表头
void printPolynomial(termnode termHead);//打印当前单个多项式
termnode readFromFile(FILE *fp);//从文件中读取多项式，返回表头
double calPolynomial(termnode termHead, double x);//计算参数多项式在x处的值，返回结果
void sumPolynomial();//计算两多项式之和
void subPolynomial();//计算两多项式之差
void mulPolynomial();//计算两多项式之积
void divPolynomial();//计算两多项式之商和余
termnode polyAdd(termnode termHead1, termnode termHead2);
termnode polySub(termnode termHead1, termnode termHead2);
termnode polyMul(termnode presentTerm, termnode termHead2);
termnode polyDiv(termnode dividend, termnode divisor);
termnode attach(double c, int e, termnode p);//构建一个以c为系数，以e为指数的新节点项，并链接到p节点之后，并返回这一项的指针
termnode descend(termnode termHead);//对参数多项式按指数降幂排序，并返回排序后的表头
void freePolynomial(termnode p);//释放参数多项式的内存
void menu();//显示菜单


termnode createPolynomial(termnode termHead, int termN, FILE *fp){
    termnode newTerm = NULL;//新节点项
    termnode lastTerm = NULL;//链表尾节点
    termnode p = NULL;//临时遍历指针
    //用户提示
    printf("Notes:\n");
    printf("Use whitespace to isolate every single number.\n");
    printf("Be sure to input in the order of coefficient and exponent for each term.\n");
    printf("For example:1 2 3 4(2 terms, 1x^2+3x^4)\n");
    //构建一个链表，各项参数由用户从键盘输入
    for(int i=0;i<termN;i++){
        newTerm = (struct term*)malloc(sizeof(struct term));
        if(newTerm == NULL){
            printf("No enough memory to allocate!\n");
            exit(0);
        }else if(termHead == NULL){
            termHead = newTerm;
            scanf("%lf %d",&newTerm->coef,&newTerm->exp);
            getchar();
            lastTerm = termHead;
        }else{
            lastTerm->next = newTerm;
            scanf("%lf %d",&newTerm->coef,&newTerm->exp);
            getchar();
            lastTerm = newTerm;
        }
    }
    termHead = descend(termHead);//对多项式按指数降幂排列
    p = termHead;
    //将多项式写入文件中
    while(p!=NULL){
        if(p==termHead){
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    fclose(fp);//关闭文件
    return termHead;
}
void printPolynomial(termnode termHead){
    termnode tempTerm = termHead;//临时遍历指针
    while(tempTerm!=NULL){
        if(tempTerm == termHead){
            printf("%.2lfx^%d",tempTerm->coef,tempTerm->exp);
        }else{
            printf("+%.2lfx^%d",tempTerm->coef,tempTerm->exp);
        }
        tempTerm = tempTerm->next;
    }
    printf("\n");
}
termnode readFromFile(FILE *fp){
    termnode termHead = NULL;//多项式表头
    termnode newTerm = NULL;//构建新节点项
    termnode lastTerm = NULL;//多项式表尾
    termnode tempTerm = NULL;//临时遍历指针
    char c;
    //从文件中读取一个多项式到term
    do{

        newTerm = (termnode)malloc(sizeof(struct term));
        if(newTerm == NULL){
            printf("No enough memory to allocate!\n");
            exit(0);
        }
        if(termHead == NULL){
            termHead = newTerm;
            lastTerm = termHead;
        }else{
            lastTerm->next = newTerm;
            lastTerm = newTerm;
        }
        fscanf(fp,"%lfx^%d",&newTerm->coef,&newTerm->exp);
        c = fgetc(fp);//读一个字符
    }while(c == '+');//如果一项后面是加号，说明当前多项式没有结束
    termHead = descend(termHead);//降幂排列
    return termHead;
}
double calPolynomial(termnode termHead, double x){
    double result = 0;//结果
    FILE *fp;
    //如果表头为空，就说明当前没有多项式
    if(termHead == NULL){
        printf("There is no polynomial at all!\n");
        return 0;
    }
    termnode presentTerm = termHead;
    result = termHead->coef;//根据霍纳规则（Horner），初始化结果变量
    //用霍纳规则（秦九韶算法）对多项式遍历运算
    for(int i=termHead->exp-1;i>=0;i--){
        if(presentTerm->next != NULL && presentTerm->next->exp == i){
            result = result * x + presentTerm->next->coef;
            presentTerm = presentTerm->next;
        }else{
            result = result * x;
        }
    }
    //将结果写入文件
    if((fp = fopen("value.txt","w"))==NULL){
        printf("Failure to open value.txt!\n");
        exit(0);
    }
    fprintf(fp,"The value of the polynomial in polynomial.txt when x equals to %.2lf is:\n",x);
    fprintf(fp,"%lf",result);
    fclose(fp);
    return result;
}
void sumPolynomial(){
    termnode termHead1 = NULL;//第一个多项式
    termnode termHead2 = NULL;//第二个多项式
    termnode p = NULL;//临时遍历指针
    termnode polySum = NULL;//结果和多项式
    FILE *fp;
    char ch;
    //打开文件
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    termHead1 = readFromFile(fp);//导入第一个多项式
    termHead2 = readFromFile(fp);//导入第二个多项式
    fclose(fp);//关闭文件
    polySum = polyAdd(termHead1,termHead2);//计算和多项式
    p = polySum;
    printf("The sum of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    //输出结果到屏幕和文件中
    while(p!=NULL){
        if(p == polySum){
            printf("%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            printf("+%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    fclose(fp);//关闭文件
    //释放内存
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    freePolynomial(polySum);
    printf("\n");
}
void subPolynomial(){
    termnode termHead1 = NULL, termHead2 = NULL;//被减数多项式，减数多项式
    termnode p = NULL;//临时遍历指针
    termnode polyD = NULL;//差多项式
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    termHead1 = readFromFile(fp);//导入被减数多项式
    termHead2 = readFromFile(fp);//导入减数多项式
    polyD = polySub(termHead1,termHead2);//计算差多项式
    p = polyD;
    printf("The difference of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    //如果差多项式是空，那么结果是0
    //输出结果到屏幕和文件中
    if(p == NULL){
        printf("0");
        fprintf(fp,"0");
        return;
    }
    while(p!=NULL){
        if(p == polyD){
            printf("%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            printf("+%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    //关闭文件，释放内存
    fclose(fp);
    freePolynomial(polyD);
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    printf("\n");
}
void mulPolynomial(){
    termnode termHead1 = NULL, termHead2 = NULL;//两个乘积多项式
    termnode p = NULL;//临时遍历指针
    termnode polyProduct = NULL;//结果积多项式
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    //从文件导入两个乘积多项式
    termHead1 = readFromFile(fp);
    termHead2 = readFromFile(fp);
    p = termHead1;
    polyProduct = polyMul(p,termHead2);//先计算第一个多项式表头和第二个多项式的积
    //遍历第一个多项式
    p = p->next;
    while(p!=NULL){
        polyProduct = polyAdd(polyProduct,polyMul(p,termHead2));
        p = p->next;
        //每次遍历，都把结果更新为当前遍历节点和第二个多项式的乘积加上上一次的结果
        //优化部分
    }
    printf("The product of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    p = polyProduct;
    //结果输出到屏幕和文件中
    while(p!=NULL){
        if(p == polyProduct){
            printf("%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            printf("+%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    //关闭文件，释放内存
    fclose(fp);
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    freePolynomial(polyProduct);
    printf("\n");
}
void divPolynomial(){
    termnode dividendHead = NULL, divisorHead = NULL;//被除多项式，除数多项式
    termnode biggestTerm = NULL;//被除多项式最大次项
    termnode qTerm = NULL;//商多项式的每一项
    termnode quotientHead = NULL, quotientLast = NULL;//商多项式表头，商多项式表尾
    termnode remainder = NULL;//余多项式
    termnode p = NULL;//临时遍历指针
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    //导入两多项式
    dividendHead = readFromFile(fp);
    divisorHead = readFromFile(fp);
    //初始化被除多项式最大项
    biggestTerm = dividendHead;
    //开始用长除法算法遍历，直到当前被除多项式的最大次数小于除数多项式最大次数
    while(biggestTerm->exp >= divisorHead->exp){
        qTerm = polyDiv(biggestTerm,divisorHead);//用被除数最大次项除以除数多项式第一项，记为商多项式的一项
        //构造商多项式
        if(quotientHead == NULL){
            quotientHead = qTerm;
            quotientLast = quotientHead;
        }else{
            quotientLast->next = qTerm;
            quotientLast = qTerm;
        }
        remainder = polySub(dividendHead,polyMul(qTerm, divisorHead));//更新余多项式
        biggestTerm = remainder;//更新被除数最大次项
    }
    p = quotientHead;
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    printf("The quotient of the two polynomials is:\n");
    fprintf(fp,"The quotient of the two polynomials is:\n");
    //输出商
    while(p!=NULL){
        if(p == quotientHead){
            printf("%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            printf("+%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    printf("\n");
    fprintf(fp,"\n");
    p = remainder;
    printf("The remainder of the two polynomials is:\n");
    fprintf(fp,"The quotient of the two polynomials is:\n");
    //输出余
    while(p!=NULL){
        if(p == remainder){
            printf("%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            printf("+%.2lfx^%d",p->coef,p->exp);
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    //关闭文件，释放内存
    freePolynomial(dividendHead);
    freePolynomial(divisorHead);
    freePolynomial(quotientHead);
    freePolynomial(remainder);
    fclose(fp);
    printf("\n");
}
termnode polyAdd(termnode termHead1, termnode termHead2){
    //临时遍历指针
    termnode p1 = NULL;
    termnode p2 = NULL;
    termnode sumHead = NULL;//新多项式表头
    termnode sumP = NULL;//构造和多项式时的临时新项
    double sumCoef;//新项的系数
    //初始化
    p1 = termHead1;
    p2 = termHead2;
    //申请空间
    sumHead = (termnode)malloc(sizeof(struct term));
    if(sumHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    sumP = sumHead;
    //两多项式都不为空
    while((p1!=NULL) && (p2!=NULL)){
        if(p1->exp == p2->exp){//两当前项指数相同，可以相加
            sumCoef = p1->coef + p2->coef;//系数相加
            if(sumCoef){
                sumP = attach(sumCoef, p1->exp, sumP);
            }
            //遍历
            p1 = p1->next;
            p2 = p2->next;
        //两当前项指数不同，只加指数大的那一项
        }else if(p1->exp > p2->exp){
            sumP = attach(p1->coef, p1->exp, sumP);
            p1 = p1->next;
        }else{
            sumP = attach(p2->coef, p2->exp, sumP);
            p2 = p2->next;
        }
    }
    //某个多项式先遍历完全，另一个多项式有剩余，直接加
    while(p1!=NULL){
        sumP = attach(p1->coef, p1->exp, sumP);
        p1 = p1->next;
    }
    while(p2!=NULL){
        sumP = attach(p2->coef, p2->exp, sumP);
        p2 = p2->next;
    }
    //取消空表头
    sumP->next = NULL;
    sumP = sumHead;
    sumHead = sumHead->next;
    free(sumP);
    return sumHead;
}
termnode polySub(termnode termHead1, termnode termHead2){
    //临时遍历指针
    termnode p1 = NULL;
    termnode p2 = NULL;
    termnode differenceHead = NULL;//差多项式表头
    termnode differenceP = NULL;//差多项式新节点
    double differenceCoef;//差多项式项系数
    //初始化
    p1 = termHead1;
    p2 = termHead2;
    differenceHead = (termnode)malloc(sizeof(struct term));
    if(differenceHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    differenceP = differenceHead;
    //两多项式都不为空
    while((p1!=NULL) && (p2!=NULL)){
        //两项指数相同，可以相减
        if(p1->exp == p2->exp){
            differenceCoef = p1->coef - p2->coef;
            if(differenceCoef){
                differenceP = attach(differenceCoef, p1->exp, differenceP);
            }
            p1 = p1->next;
            p2 = p2->next;
        //被减数指数大，直接链接被减数项
        }else if(p1->exp > p2->exp){
            differenceP = attach(p1->coef, p1->exp, differenceP);
            p1 = p1->next;
        //减数指数大，需要把系数取负，再链接到结果中
        }else{
            p2->coef = -(p2->coef);
            differenceP = attach(p2->coef, p2->exp, differenceP);
            p2 = p2->next;
        }
    }
    //某个多项式先遍历完全，另一个多项式有剩余，直接链接
    while(p1!=NULL){
        differenceP = attach(p1->coef, p1->exp, differenceP);
        p1 = p1->next;
    }
    while(p2!=NULL){
        p2->coef = -(p2->coef);//系数取负
        differenceP = attach(p2->coef, p2->exp, differenceP);
        p2 = p2->next;
    }
    //取消空表头
    differenceP->next = NULL;
    differenceP = differenceHead;
    differenceHead = differenceHead->next;
    free(differenceP);
    return differenceHead;
}
termnode polyMul(termnode presentTerm, termnode termHead2){
    termnode p = NULL;//临时遍历指针
    termnode productHead = NULL;//积多项式表头
    termnode productP = NULL;//积多项式新节点
    double productCoef;//积多项式项系数
    int productExp;//积多项式项指数
    //初始化
    p = termHead2;
    productHead = (termnode)malloc(sizeof(struct term));
    if(productHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    productP = productHead;
    //遍历参数多项式
    while(p!=NULL){
        productCoef = p->coef * presentTerm->coef;//求新项系数
        productExp = p->exp + presentTerm->exp;//求新项指数
        productP = attach(productCoef,productExp,productP);
        p = p->next;
    }
    //取消空表头
    productP->next = NULL;
    productP = productHead;
    productHead = productHead->next;
    free(productP);
    return productHead;
}
termnode polyDiv(termnode dividend, termnode divisor){
    termnode quotient = NULL;//这是最终结果商多项式中的一项
    //初始化
    quotient = (struct term*)malloc(sizeof(struct term));
    if(quotient == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    quotient->coef = dividend->coef / divisor->coef;//求系数
    quotient->exp = dividend->exp - divisor->exp;//求指数
    quotient->next = NULL;
    return quotient;
}
termnode attach(double c, int e, termnode p){
    termnode x = NULL;
    //申请内存
    x = (termnode)malloc(sizeof(struct term));
    if(x == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    x->coef = c;//定义系数
    x->exp = e;//定义指数
    p->next = x;//链接
    return x;//返回当前节点
}
termnode descend(termnode termHead){
    termnode biggestTerm = NULL;//指数最大项
    termnode presentTerm = NULL;//当前项，即第一层循环遍历项
    termnode p = NULL;//第二层循环遍历项
    double tempCoef;//临时系数
    int tempExp;//临时指数

    presentTerm = termHead;//初始化
    while(presentTerm->next!=NULL){//第一层遍历，直到倒数第二项
        biggestTerm = presentTerm;//令这次遍历中的最大指数项初始化为当前项
        p = presentTerm->next;
        while(p!=NULL){//第二层遍历
            //更新最大指数项
            if(p->exp > biggestTerm->exp){
                biggestTerm = p;
            }
            p = p->next;
        }
        //交换两项的参数
        tempCoef = biggestTerm->coef;
        tempExp = biggestTerm->exp;
        biggestTerm->coef = presentTerm->coef;
        biggestTerm->exp = presentTerm->exp;
        presentTerm->coef = tempCoef;
        presentTerm->exp = tempExp;
        presentTerm = presentTerm->next;
    }
    return termHead;
}
void freePolynomial(termnode p){
    termnode q = NULL;//临时保存要被释放内存的节点
    while(p!=NULL){
        q = p;
        p = p->next;//遍历
        free(q);//释放内存
    }
    free(p);//释放末节点内存
}
void menu(){
    printf("Choices:\n");
    printf("1. Create a polynomial.\n");
    printf("2. Print the polynomial.\n");
    printf("3. Read the polynomial from the file.\n");
    printf("4. Calculate the value of the polynomial at x.\n");
    printf("5. Calculate the sum of the two polynomials.\n");
    printf("6. Calculate the sub of the two polynomials.\n");
    printf("7. Calculate the product of the two polynomials.\n");
    printf("8. Calculate the quotient and the remainder of the two polynomials.\n");
    printf("To quit, please press anything but 1 to 8.\n");
}
int main()
{
    char choice;//用户选择
    int termN;//功能一的项个数
    FILE *fp;//文件指针
    termnode termHead = NULL;//整体的多项式表头
    double x;//功能四中的参数值
    double result = 0;//功能四中的结果值
    while(1){//循环执行
        menu();//菜单
        scanf("%c",&choice);//选择
        getchar();
        switch(choice){
            case '1':
                printf("How many terms in this polynomial?\n");
                scanf("%d",&termN);
                getchar();
                if((fp = fopen("polynomial.txt","w")) == NULL){
                    printf("Failure to open polyminal.txt!\n");
                    exit(0);
                }
                termHead = createPolynomial(termHead,termN,fp);
                fclose(fp);
                break;
            case '2':printPolynomial(termHead);break;
            case '3':
                if((fp = fopen("polynomial.txt","r"))==NULL){
                    printf("Failure to open polynomial.txt!\n");
                    exit(0);
                }
                termHead = readFromFile(fp);
                fclose(fp);break;
            case '4':
                printf("Give the x: ");
                scanf("%lf",&x);
                getchar();
                result = calPolynomial(termHead,x);
                printf("The value of the polynomial at x is: %.2lf.\n",result);
                break;
            case '5':sumPolynomial();exit(0);
            case '6':subPolynomial();exit(0);
            case '7':mulPolynomial();exit(0);
            case '8':divPolynomial();exit(0);
            default:
                //释放主函数中的多项式内存
                freePolynomial(termHead);
                exit(0);
        }
        printf("\n");
    }
    return 0;
}
