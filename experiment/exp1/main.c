#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct term{
    double coef;
    int exp;
    struct term *next;
};
typedef struct term* termnode;
//��������˵��������ע���еı�ͷ��ָ����ĵ�һ��
termnode createPolynomial(termnode termHead, int termN, FILE *fp);//���������¶���ʽ�����ر�ͷ
void printPolynomial(termnode termHead);//��ӡ��ǰ��������ʽ
termnode readFromFile(FILE *fp);//���ļ��ж�ȡ����ʽ�����ر�ͷ
double calPolynomial(termnode termHead, double x);//�����������ʽ��x����ֵ�����ؽ��
void sumPolynomial();//����������ʽ֮��
void subPolynomial();//����������ʽ֮��
void mulPolynomial();//����������ʽ֮��
void divPolynomial();//����������ʽ֮�̺���
termnode polyAdd(termnode termHead1, termnode termHead2);
termnode polySub(termnode termHead1, termnode termHead2);
termnode polyMul(termnode presentTerm, termnode termHead2);
termnode polyDiv(termnode dividend, termnode divisor);
termnode attach(double c, int e, termnode p);//����һ����cΪϵ������eΪָ�����½ڵ�������ӵ�p�ڵ�֮�󣬲�������һ���ָ��
termnode descend(termnode termHead);//�Բ�������ʽ��ָ���������򣬲����������ı�ͷ
void freePolynomial(termnode p);//�ͷŲ�������ʽ���ڴ�
void menu();//��ʾ�˵�


termnode createPolynomial(termnode termHead, int termN, FILE *fp){
    termnode newTerm = NULL;//�½ڵ���
    termnode lastTerm = NULL;//����β�ڵ�
    termnode p = NULL;//��ʱ����ָ��
    //�û���ʾ
    printf("Notes:\n");
    printf("Use whitespace to isolate every single number.\n");
    printf("Be sure to input in the order of coefficient and exponent for each term.\n");
    printf("For example:1 2 3 4(2 terms, 1x^2+3x^4)\n");
    //����һ����������������û��Ӽ�������
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
    termHead = descend(termHead);//�Զ���ʽ��ָ����������
    p = termHead;
    //������ʽд���ļ���
    while(p!=NULL){
        if(p==termHead){
            fprintf(fp,"%.2lfx^%d",p->coef,p->exp);
        }else{
            fprintf(fp,"+%.2lfx^%d",p->coef,p->exp);
        }
        p = p->next;
    }
    fclose(fp);//�ر��ļ�
    return termHead;
}
void printPolynomial(termnode termHead){
    termnode tempTerm = termHead;//��ʱ����ָ��
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
    termnode termHead = NULL;//����ʽ��ͷ
    termnode newTerm = NULL;//�����½ڵ���
    termnode lastTerm = NULL;//����ʽ��β
    termnode tempTerm = NULL;//��ʱ����ָ��
    char c;
    //���ļ��ж�ȡһ������ʽ��term
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
        c = fgetc(fp);//��һ���ַ�
    }while(c == '+');//���һ������ǼӺţ�˵����ǰ����ʽû�н���
    termHead = descend(termHead);//��������
    return termHead;
}
double calPolynomial(termnode termHead, double x){
    double result = 0;//���
    FILE *fp;
    //�����ͷΪ�գ���˵����ǰû�ж���ʽ
    if(termHead == NULL){
        printf("There is no polynomial at all!\n");
        return 0;
    }
    termnode presentTerm = termHead;
    result = termHead->coef;//���ݻ��ɹ���Horner������ʼ���������
    //�û��ɹ����ؾ����㷨���Զ���ʽ��������
    for(int i=termHead->exp-1;i>=0;i--){
        if(presentTerm->next != NULL && presentTerm->next->exp == i){
            result = result * x + presentTerm->next->coef;
            presentTerm = presentTerm->next;
        }else{
            result = result * x;
        }
    }
    //�����д���ļ�
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
    termnode termHead1 = NULL;//��һ������ʽ
    termnode termHead2 = NULL;//�ڶ�������ʽ
    termnode p = NULL;//��ʱ����ָ��
    termnode polySum = NULL;//����Ͷ���ʽ
    FILE *fp;
    char ch;
    //���ļ�
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    termHead1 = readFromFile(fp);//�����һ������ʽ
    termHead2 = readFromFile(fp);//����ڶ�������ʽ
    fclose(fp);//�ر��ļ�
    polySum = polyAdd(termHead1,termHead2);//����Ͷ���ʽ
    p = polySum;
    printf("The sum of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    //����������Ļ���ļ���
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
    fclose(fp);//�ر��ļ�
    //�ͷ��ڴ�
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    freePolynomial(polySum);
    printf("\n");
}
void subPolynomial(){
    termnode termHead1 = NULL, termHead2 = NULL;//����������ʽ����������ʽ
    termnode p = NULL;//��ʱ����ָ��
    termnode polyD = NULL;//�����ʽ
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    termHead1 = readFromFile(fp);//���뱻��������ʽ
    termHead2 = readFromFile(fp);//�����������ʽ
    polyD = polySub(termHead1,termHead2);//��������ʽ
    p = polyD;
    printf("The difference of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    //��������ʽ�ǿգ���ô�����0
    //����������Ļ���ļ���
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
    //�ر��ļ����ͷ��ڴ�
    fclose(fp);
    freePolynomial(polyD);
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    printf("\n");
}
void mulPolynomial(){
    termnode termHead1 = NULL, termHead2 = NULL;//�����˻�����ʽ
    termnode p = NULL;//��ʱ����ָ��
    termnode polyProduct = NULL;//���������ʽ
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    //���ļ����������˻�����ʽ
    termHead1 = readFromFile(fp);
    termHead2 = readFromFile(fp);
    p = termHead1;
    polyProduct = polyMul(p,termHead2);//�ȼ����һ������ʽ��ͷ�͵ڶ�������ʽ�Ļ�
    //������һ������ʽ
    p = p->next;
    while(p!=NULL){
        polyProduct = polyAdd(polyProduct,polyMul(p,termHead2));
        p = p->next;
        //ÿ�α��������ѽ������Ϊ��ǰ�����ڵ�͵ڶ�������ʽ�ĳ˻�������һ�εĽ��
        //�Ż�����
    }
    printf("The product of the two polynomials is:\n");
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    fprintf(fp,"The sum of the two polynomials in the two_polynomial.txt is:\n");
    p = polyProduct;
    //����������Ļ���ļ���
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
    //�ر��ļ����ͷ��ڴ�
    fclose(fp);
    freePolynomial(termHead1);
    freePolynomial(termHead2);
    freePolynomial(polyProduct);
    printf("\n");
}
void divPolynomial(){
    termnode dividendHead = NULL, divisorHead = NULL;//��������ʽ����������ʽ
    termnode biggestTerm = NULL;//��������ʽ������
    termnode qTerm = NULL;//�̶���ʽ��ÿһ��
    termnode quotientHead = NULL, quotientLast = NULL;//�̶���ʽ��ͷ���̶���ʽ��β
    termnode remainder = NULL;//�����ʽ
    termnode p = NULL;//��ʱ����ָ��
    FILE *fp;
    if((fp = fopen("two_polynomial.txt","r"))==NULL){
        printf("Failure to open polynomial.txt!\n");
        exit(0);
    }
    //����������ʽ
    dividendHead = readFromFile(fp);
    divisorHead = readFromFile(fp);
    //��ʼ����������ʽ�����
    biggestTerm = dividendHead;
    //��ʼ�ó������㷨������ֱ����ǰ��������ʽ��������С�ڳ�������ʽ������
    while(biggestTerm->exp >= divisorHead->exp){
        qTerm = polyDiv(biggestTerm,divisorHead);//�ñ�������������Գ�������ʽ��һ���Ϊ�̶���ʽ��һ��
        //�����̶���ʽ
        if(quotientHead == NULL){
            quotientHead = qTerm;
            quotientLast = quotientHead;
        }else{
            quotientLast->next = qTerm;
            quotientLast = qTerm;
        }
        remainder = polySub(dividendHead,polyMul(qTerm, divisorHead));//���������ʽ
        biggestTerm = remainder;//���±�����������
    }
    p = quotientHead;
    if((fp = fopen("result.txt","w"))==NULL){
        printf("Failure to open result.txt!\n");
        exit(0);
    }
    printf("The quotient of the two polynomials is:\n");
    fprintf(fp,"The quotient of the two polynomials is:\n");
    //�����
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
    //�����
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
    //�ر��ļ����ͷ��ڴ�
    freePolynomial(dividendHead);
    freePolynomial(divisorHead);
    freePolynomial(quotientHead);
    freePolynomial(remainder);
    fclose(fp);
    printf("\n");
}
termnode polyAdd(termnode termHead1, termnode termHead2){
    //��ʱ����ָ��
    termnode p1 = NULL;
    termnode p2 = NULL;
    termnode sumHead = NULL;//�¶���ʽ��ͷ
    termnode sumP = NULL;//����Ͷ���ʽʱ����ʱ����
    double sumCoef;//�����ϵ��
    //��ʼ��
    p1 = termHead1;
    p2 = termHead2;
    //����ռ�
    sumHead = (termnode)malloc(sizeof(struct term));
    if(sumHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    sumP = sumHead;
    //������ʽ����Ϊ��
    while((p1!=NULL) && (p2!=NULL)){
        if(p1->exp == p2->exp){//����ǰ��ָ����ͬ���������
            sumCoef = p1->coef + p2->coef;//ϵ�����
            if(sumCoef){
                sumP = attach(sumCoef, p1->exp, sumP);
            }
            //����
            p1 = p1->next;
            p2 = p2->next;
        //����ǰ��ָ����ͬ��ֻ��ָ�������һ��
        }else if(p1->exp > p2->exp){
            sumP = attach(p1->coef, p1->exp, sumP);
            p1 = p1->next;
        }else{
            sumP = attach(p2->coef, p2->exp, sumP);
            p2 = p2->next;
        }
    }
    //ĳ������ʽ�ȱ�����ȫ����һ������ʽ��ʣ�ֱ࣬�Ӽ�
    while(p1!=NULL){
        sumP = attach(p1->coef, p1->exp, sumP);
        p1 = p1->next;
    }
    while(p2!=NULL){
        sumP = attach(p2->coef, p2->exp, sumP);
        p2 = p2->next;
    }
    //ȡ���ձ�ͷ
    sumP->next = NULL;
    sumP = sumHead;
    sumHead = sumHead->next;
    free(sumP);
    return sumHead;
}
termnode polySub(termnode termHead1, termnode termHead2){
    //��ʱ����ָ��
    termnode p1 = NULL;
    termnode p2 = NULL;
    termnode differenceHead = NULL;//�����ʽ��ͷ
    termnode differenceP = NULL;//�����ʽ�½ڵ�
    double differenceCoef;//�����ʽ��ϵ��
    //��ʼ��
    p1 = termHead1;
    p2 = termHead2;
    differenceHead = (termnode)malloc(sizeof(struct term));
    if(differenceHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    differenceP = differenceHead;
    //������ʽ����Ϊ��
    while((p1!=NULL) && (p2!=NULL)){
        //����ָ����ͬ���������
        if(p1->exp == p2->exp){
            differenceCoef = p1->coef - p2->coef;
            if(differenceCoef){
                differenceP = attach(differenceCoef, p1->exp, differenceP);
            }
            p1 = p1->next;
            p2 = p2->next;
        //������ָ����ֱ�����ӱ�������
        }else if(p1->exp > p2->exp){
            differenceP = attach(p1->coef, p1->exp, differenceP);
            p1 = p1->next;
        //����ָ������Ҫ��ϵ��ȡ���������ӵ������
        }else{
            p2->coef = -(p2->coef);
            differenceP = attach(p2->coef, p2->exp, differenceP);
            p2 = p2->next;
        }
    }
    //ĳ������ʽ�ȱ�����ȫ����һ������ʽ��ʣ�ֱ࣬������
    while(p1!=NULL){
        differenceP = attach(p1->coef, p1->exp, differenceP);
        p1 = p1->next;
    }
    while(p2!=NULL){
        p2->coef = -(p2->coef);//ϵ��ȡ��
        differenceP = attach(p2->coef, p2->exp, differenceP);
        p2 = p2->next;
    }
    //ȡ���ձ�ͷ
    differenceP->next = NULL;
    differenceP = differenceHead;
    differenceHead = differenceHead->next;
    free(differenceP);
    return differenceHead;
}
termnode polyMul(termnode presentTerm, termnode termHead2){
    termnode p = NULL;//��ʱ����ָ��
    termnode productHead = NULL;//������ʽ��ͷ
    termnode productP = NULL;//������ʽ�½ڵ�
    double productCoef;//������ʽ��ϵ��
    int productExp;//������ʽ��ָ��
    //��ʼ��
    p = termHead2;
    productHead = (termnode)malloc(sizeof(struct term));
    if(productHead == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    productP = productHead;
    //������������ʽ
    while(p!=NULL){
        productCoef = p->coef * presentTerm->coef;//������ϵ��
        productExp = p->exp + presentTerm->exp;//������ָ��
        productP = attach(productCoef,productExp,productP);
        p = p->next;
    }
    //ȡ���ձ�ͷ
    productP->next = NULL;
    productP = productHead;
    productHead = productHead->next;
    free(productP);
    return productHead;
}
termnode polyDiv(termnode dividend, termnode divisor){
    termnode quotient = NULL;//�������ս���̶���ʽ�е�һ��
    //��ʼ��
    quotient = (struct term*)malloc(sizeof(struct term));
    if(quotient == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    quotient->coef = dividend->coef / divisor->coef;//��ϵ��
    quotient->exp = dividend->exp - divisor->exp;//��ָ��
    quotient->next = NULL;
    return quotient;
}
termnode attach(double c, int e, termnode p){
    termnode x = NULL;
    //�����ڴ�
    x = (termnode)malloc(sizeof(struct term));
    if(x == NULL){
        printf("No enough memory to allocate!\n");
        exit(0);
    }
    x->coef = c;//����ϵ��
    x->exp = e;//����ָ��
    p->next = x;//����
    return x;//���ص�ǰ�ڵ�
}
termnode descend(termnode termHead){
    termnode biggestTerm = NULL;//ָ�������
    termnode presentTerm = NULL;//��ǰ�����һ��ѭ��������
    termnode p = NULL;//�ڶ���ѭ��������
    double tempCoef;//��ʱϵ��
    int tempExp;//��ʱָ��

    presentTerm = termHead;//��ʼ��
    while(presentTerm->next!=NULL){//��һ�������ֱ�������ڶ���
        biggestTerm = presentTerm;//����α����е����ָ�����ʼ��Ϊ��ǰ��
        p = presentTerm->next;
        while(p!=NULL){//�ڶ������
            //�������ָ����
            if(p->exp > biggestTerm->exp){
                biggestTerm = p;
            }
            p = p->next;
        }
        //��������Ĳ���
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
    termnode q = NULL;//��ʱ����Ҫ���ͷ��ڴ�Ľڵ�
    while(p!=NULL){
        q = p;
        p = p->next;//����
        free(q);//�ͷ��ڴ�
    }
    free(p);//�ͷ�ĩ�ڵ��ڴ�
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
    char choice;//�û�ѡ��
    int termN;//����һ�������
    FILE *fp;//�ļ�ָ��
    termnode termHead = NULL;//����Ķ���ʽ��ͷ
    double x;//�������еĲ���ֵ
    double result = 0;//�������еĽ��ֵ
    while(1){//ѭ��ִ��
        menu();//�˵�
        scanf("%c",&choice);//ѡ��
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
                //�ͷ��������еĶ���ʽ�ڴ�
                freePolynomial(termHead);
                exit(0);
        }
        printf("\n");
    }
    return 0;
}
