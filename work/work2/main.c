#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TOTAL 98
/*字符在charCollection中的映射关系
    字符32~126映射到索引1~95
    \t >> 96
    \r >> 97
    \n >> 98
    共98个字符
    索引0中储存的是字符总数*/
//Huffman节点结构
typedef struct{
    int weight;
    int lchild;
    int rchild;
    int parent;
}huffmanNode;
//Huffman字符编码结构
typedef struct{
    char ch;
    char bits[TOTAL+1];
}codeNode;
//堆实现，Huffman节点结构
struct tree{
    int frequency;
    char ch;
    struct tree *lchild;
    struct tree *rchild;
    struct tree *parent;
};
typedef struct tree* HEAPhuffmanNode;
//堆结构
struct heap{
    HEAPhuffmanNode array[TOTAL+1];
    int n;
};
typedef struct heap* heapNode;
//堆实现，字符编码结构
typedef struct {
    HEAPhuffmanNode leaf;
    char bits[TOTAL+1];
}CODE;
clock_t start,stop;//用以计算运行时间
double duration;//运行时间
int count = 0;//常规实现，出现的字符总数
//常规实现
void readFromFile(int charCollection[TOTAL+1]);//读文件
void createHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1], int charCollection[TOTAL+1]);//根据所读的字符和频率，创建一个Huffman树
void initHuffmanTree(huffmanNode huffmanTree[2*TOTAL-1]);//initial Huffman tree
void initWeight(huffmanNode huffmanTree[2*TOTAL-1], int charColletion[TOTAL+1]);//初始化Huffman树的叶子节点频率
void selectMin(huffmanNode huffmanTree[2*TOTAL-1], int n, int *p1, int *p2);//从当前huffm树中选择两个频率最小的叶子节点，索引记为p1,p2
void charSetHuffmanEncoding(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL], char charColletion[TOTAL+1]);//根据Huffman树，构造各个字符的编码序列
char ascii2char(int k);//索引——字符映射表
void compression(codeNode codeArray[TOTAL]);//根据编码序列，编码压缩文件
void huffmanDecode(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL]);//根据编码序列，译码压缩文件
void writeFrequency(int charCollection[TOTAL+1]);//记录各个字符的频率
void writeMapping(codeNode codeArray[TOTAL]);//记录各个字符的编码序列
void getRate();
//堆实现
void createEmptyMinHeap(heapNode minheap);//建立空堆
int isHeapEmpty(heapNode minheap);//判断堆空
int isHeapFull(heapNode minheap);//判断堆满
void insert(heapNode minheap, HEAPhuffmanNode newNode);//向最小堆插入一个Huffman节点
HEAPhuffmanNode deleteMin(heapNode minheap);//删除最小堆顶节点，并返回这个Huffman节点
void HEAPreadFromFile(int charCollection[TOTAL+1]);//堆实现，读文件
void HEAPwriteFrequency(int charCollection[TOTAL+1]);//堆实现，记录字符频率
void initHeap_initLeafArray(heapNode minheap, CODE leafArray[TOTAL+1], int charCollection[TOTAL+1]);//根据字符频率，初始化最小堆，初始化叶子节点序列
void HEAPcreateHuffmanTree(heapNode minheap);//在堆的基础上构建Huffman树结构
void buildLeafArray(CODE leafArray[TOTAL+1]);//根据叶子节点序列，回溯得到各个字符编码
void HEAPwriteMapping(CODE leafArray[TOTAL+1]);//记录字符编码
void HEAPcompression(CODE leafArray[TOTAL+1]);//编码压缩文件
void HEAPdecompression(heapNode minheap, CODE leafArray[TOTAL+1]);//译码压缩文件
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
    initWeight(huffmanTree,charCollection);//初始化Huffman树的叶子节点频率
    //得到出现的字符总数
    for(i=0;i<TOTAL;i++){
        if(huffmanTree[i].weight!=0)count++;
    }
    //循环字符总数-1次
    for(i=TOTAL;i<TOTAL+count-1;i++){
        p1 = p2 = -1;
        selectMin(huffmanTree,i-1,&p1,&p2);
        //构建父子关系
        huffmanTree[p1].parent = i;
        huffmanTree[p2].parent = i;
        huffmanTree[i].lchild = p1;
        huffmanTree[i].rchild = p2;
        huffmanTree[i].weight = huffmanTree[p1].weight + huffmanTree[p2].weight;//更新父节点频率
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
        if(huffmanTree[i].weight!=0 && huffmanTree[i].parent==-1){//前提是，这个字符出现过，并且它是叶子节点
            if(*p1==-1){//第一次遇到非0频率字符，记为最小频率，跳到下次循环
                *p1 = i;continue;
            }
            if(*p2==-1){//第二次遇到非0频率字符
                if(huffmanTree[i].weight<huffmanTree[*p1].weight){//和最小频率比，若更小，则更新最小频率索引和次小频率索引
                    *p2 = *p1;
                    *p1 = i;
                }else{//否则，直接记为次小频率索引
                    *p2 = i;
                }
                continue;
            }
            if(huffmanTree[i].weight<huffmanTree[*p1].weight){//若当前索引频率比最小值小，则更新最小和次小频率索引
                *p2 = *p1;
                *p1 = i;
            }else if(huffmanTree[i].weight<huffmanTree[*p2].weight){//若当前索引不小于最小值，但小于次小值，则更新次小频率索引
                *p2 = i;
            }
        }
    }
}
void charSetHuffmanEncoding(huffmanNode huffmanTree[2*TOTAL-1], codeNode codeArray[TOTAL], char charColletion[TOTAL+1]){
    int childPosition;//回溯过程的子节点
    int parentPosition;//回溯过程的父节点
    char code[TOTAL+1];//用来存储当前的编码序列
    int start;//当前编码序列的首个编码的索引
    code[TOTAL] = '\0';//初始化当前编码序列
    for(int i=0;i<TOTAL;i++){//对叶子节点遍历
        codeArray[i].ch = ascii2char(i+1);
        start = TOTAL;
        childPosition = i;
        while((parentPosition=huffmanTree[childPosition].parent)>=0){//如果这次循环到的节点没有父亲，说明这个字符未出现，直接跳过
            code[--start] = (huffmanTree[parentPosition].lchild==childPosition) ? '0' : '1';//是左儿子的记为0，右儿子的记为1
            childPosition = parentPosition;//回溯
        }
        strcpy(codeArray[i].bits, &code[start]);//记录下编码序列
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
        for(int i=0;i<TOTAL;i++){//在Huffman的叶子节点中寻找这个字符
            if(ch == codeArray[i].ch){//事实上，只有出现过的字符，它的编码序列才能被写入
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
    present = TOTAL+count-2;//这是Huffman树的根索引
    if(huffmanTree[present].lchild==-1&&huffmanTree[present].rchild==-1){
        printf("There is no huffman tree.\n");exit(0);
    }
    do{
        ch=fgetc(compressionTXT);
        if(ch=='0'){//根据0和1来从根节点往下遍历
            present = huffmanTree[present].lchild;
        }else{
            present = huffmanTree[present].rchild;
        }
        if(huffmanTree[present].lchild==-1&&huffmanTree[present].rchild==-1){//到达一个叶节点，写入对应的字符
            fprintf(decompressionTXT,"%c",codeArray[present].ch);
            present = TOTAL+count-2;//回到根节点，准备下次遍历
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
    minheap->n = 0;//堆中元素个数为0
}
int isHeapEmpty(heapNode minheap){
    return (!minheap->n);
}
int isHeapFull(heapNode minheap){
    return (minheap->n==(TOTAL+1));//堆中元素到达容量
}
void insert(heapNode minheap, HEAPhuffmanNode newNode){
    int i;
    if(!isHeapFull(minheap)){//如果堆未满
        i = minheap->n + 1;//按层序，假设插入元素在最后一个索引处
        while((i!=1) && (newNode->frequency<minheap->array[i/2]->frequency)){//如果这个元素没有游到根处，并且这个节点的权值小于其父节点
            minheap->array[i] = minheap->array[i/2];//父节点下移
            i/=2;//该元素已游到当前的节点的父亲位置，继续判断是否上游
        }
    }
    minheap->n++;//元素总数+1
    minheap->array[i] = newNode;//当前位置为插入元素
}
HEAPhuffmanNode deleteMin(heapNode minheap){
    int parent = 1;//更新过程中，初始化父节点为根
    int child = 2;//更新过程中，初始化子节点为根左孩子
    HEAPhuffmanNode minNode;
    HEAPhuffmanNode tempNode;
    if(!isHeapEmpty(minheap)){//当前堆非空
        minNode = minheap->array[1];//最小堆的根，就是权重最小的元素，就是要出堆的元素
        tempNode = minheap->array[minheap->n--];//记下堆中按层序的最后一个元素，同时元素个数-1
        while(child <= minheap->n){//更新过程中，保证子节点是堆中元素，思想是假设最后一个元素取代当前parent的位置
            if((child<minheap->n) && (minheap->array[child]->frequency>minheap->array[child+1]->frequency)){//子节点是堆中，并且，当前节点权值大于兄弟权值
                child++;//其兄弟是要上游的
            }
            if(tempNode->frequency <= minheap->array[child]->frequency) break;//如果这个最后一个元素不大于左右儿子里最小的那个，那么就跳出循环，并把这最后一个元素放在parent位置
            minheap->array[parent] = minheap->array[child];//更新
            parent = child;//继续向下判断
            child *= 2;
        }
        minheap->array[parent] = tempNode;//把最后一个元素放在parent位置就好了
        return minNode;//返回最小节点
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
        if(charCollection[i]!=0){//把出现过的字符都放入堆中
            newNode = (HEAPhuffmanNode)malloc(sizeof(struct tree));
            newNode->ch = ascii2char(i);
            newNode->frequency = charCollection[i];
            newNode->lchild = newNode->rchild = newNode->parent = NULL;
            leafArray[i].leaf = newNode;
            insert(minheap,newNode);//同时整理最小堆
        }
    }
}
void HEAPcreateHuffmanTree(heapNode minheap){
    HEAPhuffmanNode minNode,secNode;
    HEAPhuffmanNode parentNode;
    while(minheap->n != 1){//堆中最少剩一个元素，就是Huffman树的根节点
        minNode = deleteMin(minheap);//拿出一个最小的
        secNode = deleteMin(minheap);//拿出一个次小的
        parentNode = (HEAPhuffmanNode)malloc(sizeof(struct tree));//两个节点生成一个树
        parentNode->frequency = minNode->frequency + secNode->frequency;//更新新树权值
        //构建父子关系
        parentNode->lchild = minNode;
        parentNode->rchild = secNode;
        parentNode->parent = NULL;
        minNode->parent = secNode->parent = parentNode;
        insert(minheap,parentNode);//把新树放入堆中，整理最小堆，依次类推
    }
}
void buildLeafArray(CODE leafArray[TOTAL+1]){
    char tempCode[TOTAL+1];
    int start;
    HEAPhuffmanNode recallChild, recallParent;
    tempCode[TOTAL] = '\0';
    for(int i=1;i<=TOTAL;i++){
        if(leafArray[i].leaf!=NULL){//对有出现的字符构造编码序列
            start = TOTAL;
            recallChild = leafArray[i].leaf;//从叶子节点回溯
            while(recallChild->parent!=NULL){//直到回溯到根节点
                recallParent = recallChild->parent;
                tempCode[--start] = (recallChild==recallParent->lchild) ? '0' : '1';//左孩子则记为0，右孩子则记为1
                recallChild = recallParent;
            }
            strcpy(leafArray[i].bits,&tempCode[start]);//记录编码序列
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
    while((readCh=fgetc(HEAPtextTXT))!=EOF){//读取文本
        for(int i=1;i<=TOTAL;i++){
            if(leafArray[i].leaf!=NULL && readCh == leafArray[i].leaf->ch){//该字符有出现
                fputs(leafArray[i].bits,HEAPcompressionTXT);//输出该字符相应的编码序列
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
    decodeNode = minheap->array[1];//从根节点开始向下寻找
    do{
        ch=fgetc(HEAPcompressionTXT);//读01
        if(ch=='0'){//如果是0，则向左孩子寻找
            decodeNode = decodeNode->lchild;
        }else{//如果是1，则向右孩子寻找
            decodeNode = decodeNode->rchild;
        }
        if(decodeNode->lchild==NULL && decodeNode->rchild==NULL){//直到搜索到叶子节点
            fprintf(HEAPdecompressionTXT,"%c",decodeNode->ch);//输出对应的字符
            decodeNode = minheap->array[1];//回到根节点，准备下一次搜索
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
    int charCollection[TOTAL+1];//这是一个频率数组，首项是总频率，剩下TOTAL=98项是所有可能出现的字符

    huffmanNode huffmanTree[2*TOTAL-1];//常规实现的Huffman树数组
    codeNode codeArray[TOTAL];//常规实现的编码序列
    heapNode minheap;//最小堆
    CODE leafArray[TOTAL+1];//堆实现的叶子节点序列，包含编码序列

    char choice;
    //初始化频率数组
    for(int i=0;i<TOTAL+1;i++){
        charCollection[i] = 0;
    }
    printf("1. The old way.\n");
    printf("2. The heap way.\n");
    printf("Whi schway!\n");
    scanf("%c",&choice);getchar();
    if(choice=='1'){//常规实现
        start = clock();
        readFromFile(charCollection);//读文件
        createHuffmanTree(huffmanTree,charCollection);//根据所读的字符和频率，创建一个Huffman树
        charSetHuffmanEncoding(huffmanTree,codeArray,charCollection);//根据Huffman树，构造各个字符的编码序列
        writeMapping(codeArray);//记录字符编码
        compression(codeArray);//根据编码序列，编码压缩文件
        huffmanDecode(huffmanTree,codeArray);//根据编码序列，译码压缩文件
        stop = clock();
        duration = (double)(stop-start)/CLK_TCK;
        printf("Running time: %lf\n",duration);
        getRate();
    }
    if(choice=='2'){//堆实现
        start = clock();
        minheap = (heapNode)malloc(sizeof(struct heap));
        createEmptyMinHeap(minheap);//建立空堆
        HEAPreadFromFile(charCollection);//堆实现，读文件
        HEAPwriteFrequency(charCollection);//堆实现，记录字符频率
        initHeap_initLeafArray(minheap,leafArray,charCollection);//根据字符频率，初始化最小堆，初始化叶子节点序列
        HEAPcreateHuffmanTree(minheap);//在堆的基础上构建Huffman树结构
        buildLeafArray(leafArray);//根据叶子节点序列，回溯得到各个字符编码
        HEAPwriteMapping(leafArray);//记录字符编码
        HEAPcompression(leafArray);//编码压缩文件
        HEAPdecompression(minheap,leafArray);//译码压缩文件
        stop = clock();
        duration = (double)(stop-start)/CLK_TCK;
        printf("Running time: %lf\n",duration);
        getHEAPrate();
    }
    return 0;
}
