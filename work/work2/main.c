#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TOTAL 98
/*�ַ���charCollection�е�ӳ���ϵ
    �ַ�32~126ӳ�䵽����1~95
    \t >> 96
    \r >> 97
    \n >> 98
    ��98���ַ�
    ����0�д�������ַ�����*/
//Huffman�ڵ�ṹ
typedef struct{
    int weight;
    int lchild;
    int rchild;
    int parent;
}huffmanNode;
//Huffman�ַ�����ṹ
typedef struct{
    char ch;
    char bits[TOTAL+1];
}codeNode;
//��ʵ�֣�Huffman�ڵ�ṹ
struct tree{
    int frequency;
    char ch;
    struct tree *lchild;
    struct tree *rchild;
    struct tree *parent;
};
typedef struct tree* HEAPhuffmanNode;
//�ѽṹ
struct heap{
    HEAPhuffmanNode array[TOTAL+1];
    int n;
};
typedef struct heap* heapNode;
//��ʵ�֣��ַ�����ṹ
typedef struct {
    HEAPhuffmanNode leaf;
    char bits[TOTAL+1];
}CODE;
clock_t start,stop;//���Լ�������ʱ��
double duration;//����ʱ��
int count = 0;//����ʵ�֣����ֵ��ַ�����
//����ʵ��
void readFromFile(int charCollection[TOTAL+1]);//���ļ�
void createHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1], int charCollection[TOTAL+1]);//�����������ַ���Ƶ�ʣ�����һ��Huffman��
void initHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1]);//initial Huffman tree
void initWeight(huffmanNode huffmanTree[2*TOTAL-1], int charColletion[TOTAL+1]);//��ʼ��Huffman����Ҷ�ӽڵ�Ƶ��
void selectMin(huffmanNode huffmanTree[2*TOTAL-1], int n, int *p1, int *p2);//�ӵ�ǰhuffm����ѡ������Ƶ����С��Ҷ�ӽڵ㣬������Ϊp1,p2
void charSetHuffmanEncoding(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL], char charColletion[TOTAL+1]);//����Huffman������������ַ��ı�������
char ascii2char(int k);//���������ַ�ӳ���
void compression(codeNode codeArray[TOTAL]);//���ݱ������У�����ѹ���ļ�
void huffmanDecode(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL]);//���ݱ������У�����ѹ���ļ�
void writeFrequency(int charCollection[TOTAL+1]);//��¼�����ַ���Ƶ��
void writeMapping(codeNode codeArray[TOTAL]);//��¼�����ַ��ı�������
void getRate();
//��ʵ��
void createEmptyMinHeap(heapNode minheap);//�����ն�
int isHeapEmpty(heapNode minheap);//�ж϶ѿ�
int isHeapFull(heapNode minheap);//�ж϶���
void insert(heapNode minheap, HEAPhuffmanNode newNode);//����С�Ѳ���һ��Huffman�ڵ�
HEAPhuffmanNode deleteMin(heapNode minheap);//ɾ����С�Ѷ��ڵ㣬���������Huffman�ڵ�
void HEAPreadFromFile(int charCollection[TOTAL+1]);//��ʵ�֣����ļ�
void HEAPwriteFrequency(int charCollection[TOTAL+1]);//��ʵ�֣���¼�ַ�Ƶ��
void initHeap_initLeafArray(heapNode minheap, CODE leafArray[TOTAL+1], int charCollection[TOTAL+1]);//�����ַ�Ƶ�ʣ���ʼ����С�ѣ���ʼ��Ҷ�ӽڵ�����
void HEAPcreateHuffmanTree(heapNode minheap);//�ڶѵĻ����Ϲ���Huffman���ṹ
void buildLeafArray(CODE leafArray[TOTAL+1]);//����Ҷ�ӽڵ����У����ݵõ������ַ�����
void HEAPwriteMapping(CODE leafArray[TOTAL+1]);//��¼�ַ�����
void HEAPcompression(CODE leafArray[TOTAL+1]);//����ѹ���ļ�
void HEAPdecompression(heapNode minheap, CODE leafArray[TOTAL+1]);//����ѹ���ļ�
void getHEAPrate();

void readFromFile(int charCollection[TOTAL+1]){
    FILE *fp;
    char ch;
    if((fp=fopen("text.txt","r"))==NULL)exit(0);
    while((ch=fgetc(fp))!=EOF){
        if(ch=='\t'){
            charCollection[96]++;
        }else if(ch=='\r'){
            charCollection[97]++;
        }else if(ch=='\n'){
            charCollection[98]++;
        }else{
            charCollection[ch-31]++;
        }
        charCollection[0]++;
    }
    fclose(fp);
}
void createHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1], int charCollection[TOTAL+1]){
    int p1 = -1;
    int p2 = -1;
    int i;

    initHuffmanTree(huffmanTree);//initial Huffman tree
    initWeight(huffmanTree,charCollection);//��ʼ��Huffman����Ҷ�ӽڵ�Ƶ��
    //�õ����ֵ��ַ�����
    for(i=0;i<TOTAL;i++){
        if(huffmanTree[i].weight!=0)count++;
    }
    //ѭ���ַ�����-1��
    for(i=TOTAL;i<TOTAL+count-1;i++){
        p1 = p2 = -1;
        selectMin(huffmanTree,i-1,&p1,&p2);
        //�������ӹ�ϵ
        huffmanTree[p1].parent = i;
        huffmanTree[p2].parent = i;
        huffmanTree[i].lchild = p1;
        huffmanTree[i].rchild = p2;
        huffmanTree[i].weight = huffmanTree[p1].weight + huffmanTree[p2].weight;//���¸��ڵ�Ƶ��
    }
}
void initHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1]){
    for(int i=0;i<TOTAL*2-1;i++){
        huffmanTree[i].weight = 0;
        huffmanTree[i].lchild = -1;
        huffmanTree[i].rchild = -1;
        huffmanTree[i].parent = -1;
    }
}
void initWeight(huffmanNode huffmanTree[2*TOTAL-1], int charColletion[TOTAL+1]){
    for(int i=0;i<TOTAL;i++){
        huffmanTree[i].weight = charColletion[i+1];
    }
}
void selectMin(huffmanNode huffmanTree[2*TOTAL-1], int n, int *p1, int *p2){
    for(int i=0;i<=n;i++){
        if(huffmanTree[i].weight!=0 && huffmanTree[i].parent==-1){//ǰ���ǣ�����ַ����ֹ�����������Ҷ�ӽڵ�
            if(*p1==-1){//��һ��������0Ƶ���ַ�����Ϊ��СƵ�ʣ������´�ѭ��
                *p1 = i;continue;
            }
            if(*p2==-1){//�ڶ���������0Ƶ���ַ�
                if(huffmanTree[i].weight<huffmanTree[*p1].weight){//����СƵ�ʱȣ�����С���������СƵ�������ʹ�СƵ������
                    *p2 = *p1;
                    *p1 = i;
                }else{//����ֱ�Ӽ�Ϊ��СƵ������
                    *p2 = i;
                }
                continue;
            }
            if(huffmanTree[i].weight<huffmanTree[*p1].weight){//����ǰ����Ƶ�ʱ���СֵС���������С�ʹ�СƵ������
                *p2 = *p1;
                *p1 = i;
            }else if(huffmanTree[i].weight<huffmanTree[*p2].weight){//����ǰ������С����Сֵ����С�ڴ�Сֵ������´�СƵ������
                *p2 = i;
            }
        }
    }
}
void charSetHuffmanEncoding(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL], char charColletion[TOTAL+1]){
    int childPosition;//���ݹ��̵��ӽڵ�
    int parentPosition;//���ݹ��̵ĸ��ڵ�
    char code[TOTAL+1];//�����洢��ǰ�ı�������
    int start;//��ǰ�������е��׸����������
    code[TOTAL] = '\0';//��ʼ����ǰ��������
    for(int i=0;i<TOTAL;i++){//��Ҷ�ӽڵ����
        codeArray[i].ch = ascii2char(i+1);
        start = TOTAL;
        childPosition = i;
        while((parentPosition=huffmanTree[childPosition].parent)>=0){//������ѭ�����Ľڵ�û�и��ף�˵������ַ�δ���֣�ֱ������
            code[--start] = (huffmanTree[parentPosition].lchild==childPosition) ? '0' : '1';//������ӵļ�Ϊ0���Ҷ��ӵļ�Ϊ1
            childPosition = parentPosition;//����
        }
        strcpy(codeArray[i].bits, &code[start]);//��¼�±�������
    }
}
char ascii2char(int k){
    if(k==96)return '\t';
    if(k==97)return '\r';
    if(k==98)return '\n';
    return (char)(k+31);
}
void compression(codeNode codeArray[TOTAL]){
    FILE *compressionTXT;
    FILE *textTXT;
    char ch;
    if((textTXT=fopen("text.txt","r"))==NULL)exit(0);
    if((compressionTXT=fopen("compression.txt","w"))==NULL)exit(0);
    while((ch=fgetc(textTXT))!=EOF){
        for(int i=0;i<TOTAL;i++){//��Huffman��Ҷ�ӽڵ���Ѱ������ַ�
            if(ch == codeArray[i].ch){//��ʵ�ϣ�ֻ�г��ֹ����ַ������ı������в��ܱ�д��
                fputs(codeArray[i].bits,compressionTXT);
            }
        }
    }
//    fprintf(compressionTXT,"\0");
    fclose(compressionTXT);
    fclose(textTXT);
}
void huffmanDecode(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL]){
    FILE *compressionTXT;
    FILE *decompressionTXT;
    char ch;
    int present;
    if((compressionTXT=fopen("compression.txt","r"))==NULL)exit(0);
    if((decompressionTXT=fopen("decompression.txt","w"))==NULL)exit(0);
    present = TOTAL+count-2;//����Huffman���ĸ�����
    if(huffmanTree[present].lchild==-1&&huffmanTree[present].rchild==-1){
        printf("There is no huffman tree.\n");exit(0);
    }
    do{
        ch=fgetc(compressionTXT);
        if(ch=='0'){//����0��1���Ӹ��ڵ����±���
            present = huffmanTree[present].lchild;
        }else{
            present = huffmanTree[present].rchild;
        }
        if(huffmanTree[present].lchild==-1&&huffmanTree[present].rchild==-1){//����һ��Ҷ�ڵ㣬д���Ӧ���ַ�
            fprintf(decompressionTXT,"%c",codeArray[present].ch);
            present = TOTAL+count-2;//�ص����ڵ㣬׼���´α���
        }
    }while(ch!=EOF);
    fclose(compressionTXT);
    fclose(decompressionTXT);
}
void writeFrequency(int charCollection[TOTAL+1]){
    FILE *frequencyTXT;
    if((frequencyTXT=fopen("frequency.txt","w"))==NULL)exit(0);
    fprintf(frequencyTXT,"%d\n",charCollection[0]);
    for(int i=1;i<TOTAL-2;i++){
        fprintf(frequencyTXT,"%c: %d\n",(char)(i+31),charCollection[i]);
    }
    fprintf(frequencyTXT,"\t: %d\n",charCollection[TOTAL-2]);
    fprintf(frequencyTXT,"\r: %d\n",charCollection[TOTAL-1]);
    fprintf(frequencyTXT,"\n: %d\n",charCollection[TOTAL]);
    fclose(frequencyTXT);
}
void writeMapping(codeNode codeArray[TOTAL]){
    FILE *mappingTXT;
    if((mappingTXT=fopen("mapping.txt","w"))==NULL)exit(0);
    for(int i=0;i<TOTAL;i++){
        fprintf(mappingTXT,"%c:\t",codeArray[i].ch);
        for(int j=0;j<strlen(codeArray[i].bits);j++){
            fprintf(mappingTXT,"%c",codeArray[i].bits[j]);
        }
        fprintf(mappingTXT,"\n");
    }
    fclose(mappingTXT);
}
void getRate(){
    FILE *fp,*rate,*HEAPrate;
    long sourceSize,compressionSize;
    double compressionRate;
    if((fp=fopen("text.txt","r"))==NULL)exit(0);
    fseek(fp,0L,SEEK_END);
    sourceSize = ftell(fp);
    fclose(fp);
    if((fp=fopen("compression.txt","r"))==NULL)exit(0);
    fseek(fp,0L,SEEK_END);
    compressionSize = ftell(fp);
    fclose(fp);
    compressionRate = (double)compressionSize/(double)sourceSize/8;
    printf("The compression rate: %lf%%\n",compressionRate*100);
    printf("Note: x/y/8\tx is the size of compression.txt, y is the size of the text.txt\n");
    if((rate=fopen("rate.txt","w"))==NULL)exit(0);
    fprintf(rate,"The compression rate: %lf%%\n",compressionRate*100);
    fprintf(rate,"Note: x/y/8\tx is the size of compression.txt, y is the size of the text.txt\n");
    fclose(rate);
}

void createEmptyMinHeap(heapNode minheap){
    minheap->n = 0;//����Ԫ�ظ���Ϊ0
}
int isHeapEmpty(heapNode minheap){
    return (!minheap->n);
}
int isHeapFull(heapNode minheap){
    return (minheap->n==(TOTAL+1));//����Ԫ�ص�������
}
void insert(heapNode minheap, HEAPhuffmanNode newNode){
    int i;
    if(!isHeapFull(minheap)){//�����δ��
        i = minheap->n + 1;//�����򣬼������Ԫ�������һ��������
        while((i!=1) && (newNode->frequency<minheap->array[i/2]->frequency)){//������Ԫ��û���ε���������������ڵ��ȨֵС���丸�ڵ�
            minheap->array[i] = minheap->array[i/2];//���ڵ�����
            i/=2;//��Ԫ�����ε���ǰ�Ľڵ�ĸ���λ�ã������ж��Ƿ�����
        }
    }
    minheap->n++;//Ԫ������+1
    minheap->array[i] = newNode;//��ǰλ��Ϊ����Ԫ��
}
HEAPhuffmanNode deleteMin(heapNode minheap){
    int parent = 1;//���¹����У���ʼ�����ڵ�Ϊ��
    int child = 2;//���¹����У���ʼ���ӽڵ�Ϊ������
    HEAPhuffmanNode minNode;
    HEAPhuffmanNode tempNode;
    if(!isHeapEmpty(minheap)){//��ǰ�ѷǿ�
        minNode = minheap->array[1];//��С�ѵĸ�������Ȩ����С��Ԫ�أ�����Ҫ���ѵ�Ԫ��
        tempNode = minheap->array[minheap->n--];//���¶��а���������һ��Ԫ�أ�ͬʱԪ�ظ���-1
        while(child <= minheap->n){//���¹����У���֤�ӽڵ��Ƕ���Ԫ�أ�˼���Ǽ������һ��Ԫ��ȡ����ǰparent��λ��
            if((child<minheap->n) && (minheap->array[child]->frequency>minheap->array[child+1]->frequency)){//�ӽڵ��Ƕ��У����ң���ǰ�ڵ�Ȩֵ�����ֵ�Ȩֵ
                child++;//���ֵ���Ҫ���ε�
            }
            if(tempNode->frequency <= minheap->array[child]->frequency) break;//���������һ��Ԫ�ز��������Ҷ�������С���Ǹ�����ô������ѭ�������������һ��Ԫ�ط���parentλ��
            minheap->array[parent] = minheap->array[child];//����
            parent = child;//���������ж�
            child *= 2;
        }
        minheap->array[parent] = tempNode;//�����һ��Ԫ�ط���parentλ�þͺ���
        return minNode;//������С�ڵ�
    }
}
void HEAPreadFromFile(int charCollection[TOTAL+1]){
    FILE *HEAPtextTXT;
    char ch;
    if((HEAPtextTXT=fopen("HEAPtext.txt","r"))==NULL)exit(0);
        while((ch = fgetc(HEAPtextTXT))!=EOF){
            if(ch == '\t'){
                charCollection[96]++;
            }else if(ch == '\r'){
                charCollection[97]++;
            }else if(ch == '\n'){
                charCollection[98]++;
            }else{
                charCollection[ch-31]++;
            }
            charCollection[0]++;
        }
    fclose(HEAPtextTXT);
}
void HEAPwriteFrequency(int charCollection[TOTAL+1]){
    FILE *HEAPfrequencyTXT;
    if((HEAPfrequencyTXT=fopen("HEAPfrequency.txt","w"))==NULL)exit(0);
    fprintf(HEAPfrequencyTXT,"Total of characters: %d\n",charCollection[0]);
    for(int i=1;i<TOTAL-2;i++){
        fprintf(HEAPfrequencyTXT,"%c: %d\n",(char)(i+31),charCollection[i]);
    }
    fprintf(HEAPfrequencyTXT,"\t: %d\n",charCollection[TOTAL-2]);
    fprintf(HEAPfrequencyTXT,"\r: %d\n",charCollection[TOTAL-1]);
    fprintf(HEAPfrequencyTXT,"\n: %d\n",charCollection[TOTAL]);
    fclose(HEAPfrequencyTXT);
}
void initHeap_initLeafArray(heapNode minheap, CODE leafArray[TOTAL+1], int charCollection[TOTAL+1]){
    HEAPhuffmanNode newNode;
    for(int i=1;i<=TOTAL;i++){
        if(charCollection[i]!=0){//�ѳ��ֹ����ַ����������
            newNode = (HEAPhuffmanNode)malloc(sizeof(struct tree));
            newNode->ch = ascii2char(i);
            newNode->frequency = charCollection[i];
            newNode->lchild = newNode->rchild = newNode->parent = NULL;
            leafArray[i].leaf = newNode;
            insert(minheap,newNode);//ͬʱ������С��
        }
    }
}
void HEAPcreateHuffmanTree(heapNode minheap){
    HEAPhuffmanNode minNode,secNode;
    HEAPhuffmanNode parentNode;
    while(minheap->n != 1){//��������ʣһ��Ԫ�أ�����Huffman���ĸ��ڵ�
        minNode = deleteMin(minheap);//�ó�һ����С��
        secNode = deleteMin(minheap);//�ó�һ����С��
        parentNode = (HEAPhuffmanNode)malloc(sizeof(struct tree));//�����ڵ�����һ����
        parentNode->frequency = minNode->frequency + secNode->frequency;//��������Ȩֵ
        //�������ӹ�ϵ
        parentNode->lchild = minNode;
        parentNode->rchild = secNode;
        parentNode->parent = NULL;
        minNode->parent = secNode->parent = parentNode;
        insert(minheap,parentNode);//������������У�������С�ѣ���������
    }
}
void buildLeafArray(CODE leafArray[TOTAL+1]){
    char tempCode[TOTAL+1];
    int start;
    HEAPhuffmanNode recallChild, recallParent;
    tempCode[TOTAL] = '\0';
    for(int i=1;i<=TOTAL;i++){
        if(leafArray[i].leaf!=NULL){//���г��ֵ��ַ������������
            start = TOTAL;
            recallChild = leafArray[i].leaf;//��Ҷ�ӽڵ����
            while(recallChild->parent!=NULL){//ֱ�����ݵ����ڵ�
                recallParent = recallChild->parent;
                tempCode[--start] = (recallChild==recallParent->lchild) ? '0' : '1';//�������Ϊ0���Һ������Ϊ1
                recallChild = recallParent;
            }
            strcpy(leafArray[i].bits,&tempCode[start]);//��¼��������
        }
    }
}
void HEAPwriteMapping(CODE leafArray[TOTAL+1]){
    FILE *HEAPmappingTXT;
    if((HEAPmappingTXT = fopen("HEAPmapping.txt","w"))==NULL)exit(0);
    for(int i=1;i<=TOTAL;i++){
        if(leafArray[i].leaf!=NULL){
            fprintf(HEAPmappingTXT,"%c:\t",leafArray[i].leaf->ch);
            fputs(leafArray[i].bits,HEAPmappingTXT);
            fprintf(HEAPmappingTXT,"\n");
        }
    }
    fclose(HEAPmappingTXT);
}
void HEAPcompression(CODE leafArray[TOTAL+1]){
    FILE *HEAPcompressionTXT, *HEAPtextTXT;
    char readCh;
    if((HEAPtextTXT=fopen("HEAPtext.txt","r"))==NULL)exit(0);
    if((HEAPcompressionTXT = fopen("HEAPcompression.txt","w"))==NULL)exit(0);
    while((readCh=fgetc(HEAPtextTXT))!=EOF){//��ȡ�ı�
        for(int i=1;i<=TOTAL;i++){
            if(leafArray[i].leaf!=NULL && readCh == leafArray[i].leaf->ch){//���ַ��г���
                fputs(leafArray[i].bits,HEAPcompressionTXT);//������ַ���Ӧ�ı�������
            }
        }
    }
    fclose(HEAPtextTXT);
    fclose(HEAPcompressionTXT);
}
void HEAPdecompression(heapNode minheap, CODE leafArray[TOTAL+1]){
    FILE *HEAPcompressionTXT, *HEAPdecompressionTXT;
    HEAPhuffmanNode decodeNode;
    char ch;
    if((HEAPcompressionTXT=fopen("HEAPcompression.txt","r"))==NULL)exit(0);
    if((HEAPdecompressionTXT=fopen("HEAPdecompression.txt","w"))==NULL)exit(0);
    decodeNode = minheap->array[1];//�Ӹ��ڵ㿪ʼ����Ѱ��
    do{
        ch=fgetc(HEAPcompressionTXT);//��01
        if(ch=='0'){//�����0����������Ѱ��
            decodeNode = decodeNode->lchild;
        }else{//�����1�������Һ���Ѱ��
            decodeNode = decodeNode->rchild;
        }
        if(decodeNode->lchild==NULL && decodeNode->rchild==NULL){//ֱ��������Ҷ�ӽڵ�
            fprintf(HEAPdecompressionTXT,"%c",decodeNode->ch);//�����Ӧ���ַ�
            decodeNode = minheap->array[1];//�ص����ڵ㣬׼����һ������
        }
    }while(ch!=EOF);
    fclose(HEAPcompressionTXT);
    fclose(HEAPdecompressionTXT);
}
void getHEAPrate(){
    FILE *fp,*rate,*HEAPrate;
    long sourceSize,compressionSize;
    double compressionRate;
    if((fp=fopen("HEAPtext.txt","r"))==NULL)exit(0);
    fseek(fp,0L,SEEK_END);
    sourceSize = ftell(fp);
    fclose(fp);
    if((fp=fopen("HEAPcompression.txt","r"))==NULL)exit(0);
    fseek(fp,0L,SEEK_END);
    compressionSize = ftell(fp);
    fclose(fp);
    compressionRate = (double)compressionSize/(double)sourceSize/8;
    printf("The compression rate: %lf%%\n",compressionRate*100);
    printf("Note: x/y/8\tx is the size of compression.txt, y is the size of the text.txt\n");
    if((HEAPrate=fopen("HEAPrate.txt","w"))==NULL)exit(0);
    fprintf(HEAPrate,"The compression rate: %lf%%\n",compressionRate*100);
    fprintf(HEAPrate,"Note: x/y/8\tx is the size of compression.txt, y is the size of the text.txt\n");
    fclose(HEAPrate);
}

int main()
{
    int charCollection[TOTAL+1];//����һ��Ƶ�����飬��������Ƶ�ʣ�ʣ��TOTAL=98�������п��ܳ��ֵ��ַ�

    huffmanNode huffmanTree[2*TOTAL-1];//����ʵ�ֵ�Huffman������
    codeNode codeArray[TOTAL];//����ʵ�ֵı�������
    heapNode minheap;//��С��
    CODE leafArray[TOTAL+1];//��ʵ�ֵ�Ҷ�ӽڵ����У�������������

    char choice;
    //��ʼ��Ƶ������
    for(int i=0;i<TOTAL+1;i++){
        charCollection[i] = 0;
    }
    printf("1. The old way.\n");
    printf("2. The heap way.\n");
    printf("Whi schway!\n");
    scanf("%c",&choice);getchar();
    if(choice=='1'){//����ʵ��
        start = clock();
        readFromFile(charCollection);//���ļ�
        createHuffmanTree(huffmanTree,charCollection);//�����������ַ���Ƶ�ʣ�����һ��Huffman��
        charSetHuffmanEncoding(huffmanTree,codeArray,charCollection);//����Huffman������������ַ��ı�������
        writeMapping(codeArray);//��¼�ַ�����
        compression(codeArray);//���ݱ������У�����ѹ���ļ�
        huffmanDecode(huffmanTree,codeArray);//���ݱ������У�����ѹ���ļ�
        stop = clock();
        duration = (double)(stop-start)/CLK_TCK;
        printf("Running time: %lf\n",duration);
        getRate();
    }
    if(choice=='2'){//��ʵ��
        start = clock();
        minheap = (heapNode)malloc(sizeof(struct heap));
        createEmptyMinHeap(minheap);//�����ն�
        HEAPreadFromFile(charCollection);//��ʵ�֣����ļ�
        HEAPwriteFrequency(charCollection);//��ʵ�֣���¼�ַ�Ƶ��
        initHeap_initLeafArray(minheap,leafArray,charCollection);//�����ַ�Ƶ�ʣ���ʼ����С�ѣ���ʼ��Ҷ�ӽڵ�����
        HEAPcreateHuffmanTree(minheap);//�ڶѵĻ����Ϲ���Huffman���ṹ
        buildLeafArray(leafArray);//����Ҷ�ӽڵ����У����ݵõ������ַ�����
        HEAPwriteMapping(leafArray);//��¼�ַ�����
        HEAPcompression(leafArray);//����ѹ���ļ�
        HEAPdecompression(minheap,leafArray);//����ѹ���ļ�
        stop = clock();
        duration = (double)(stop-start)/CLK_TCK;
        printf("Running time: %lf\n",duration);
        getHEAPrate();
    }
    return 0;
}
