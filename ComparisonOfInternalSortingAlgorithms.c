/*
*Author : @liangtaichi
*Date : 2019/11/20 11:44
*LastEditTime : 2019/12/17 23：12
*/
//基本定义
#include<stdio.h>      
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
 
typedef int Status;
typedef int KeyType;   
typedef struct{
    KeyType key;        
 
}RecordType,RcdType;    
typedef struct{
    RcdType *rcd;    
    int length;      
    int size;        
}RcdSqList;
typedef struct{
    RcdType *rcd ;  //基址 ，0单元闲置
    int n ;      //长度
    int size ;  //容量
    int tag ;  //tag = 0 ,小顶堆 ；tag = 1 ， 大顶堆 
} Heap ;//这里不能加int ( * prior )( KeyType  , KeyType ) ;系统不支持结构体里面定义函数指针
//大顶堆
int greatPrior(KeyType x,KeyType y){    
    return x>=y;
}
//小顶堆 
int lessPrior(KeyType x,KeyType y){     
    return x<=y;
}
 
//初始化一个最大容量为size的空堆H，tag决定是大顶还是小顶
Status InitHeap(Heap &H,int size,int tag){
    H.rcd=NULL;
    H.size=size;
    H.n=0;
    H.tag=tag;
    return OK;
}
 
// 交换H中i结点与j结点
Status swapHeapElem(Heap &H,int i,int j){
    RcdType temp;
    if(i<=0||i>H.n||j<=0||j>H.n){
        return ERROR;
    }
    temp=H.rcd[i];
    H.rcd[i]=H.rcd[j];
    H.rcd[j]=temp;
    return OK;
 
}
//对堆H中位置为pos的结点做筛选，将以pos为根的子树调整为子堆
void ShiftDown(Heap &H,int pos){
    int c,rc;
    int (*prior)(KeyType,KeyType);  //试着换成在这里加函数指针，还真的行了
    if(H.tag==1)    prior=greatPrior;//大顶堆
    else    prior=lessPrior;         //小顶堆
 
    while(pos<=H.n/2){  //此时是叶子结点
        c=pos*2;        //左
        rc=pos*2+1;     //右
        if(rc<=H.n&&prior(H.rcd[rc].key,H.rcd[c].key))
            c=rc;       //t为pos结点的左右孩子中较优先者
        if(prior(H.rcd[pos].key,H.rcd[c].key))
            return;     //t为较优先，结束
        swapHeapElem(H,pos,c); //否者pos与较优先者t进行交换
        pos=c;          //继续向下调整
    }
}
 
//用E建立长度为n的堆H，容量为size，当tag=0，小顶堆，tag=1，大顶堆
void MakeHeap(Heap &H,RcdType *E,int n,int size,int tag){
    int i;
    H.rcd=E;            //E[1...n]是堆的结点，0号单元闲置
    H.n=n;
    H.size=size;
    H.tag=tag;
    int (*prior)(KeyType,KeyType);
    if(H.tag==1)    prior=greatPrior;
    else    prior=lessPrior;
    for(i=H.n/2;i>0;i--)    //对所有非叶子结点筛选
        ShiftDown(H,i);
}
 
//销毁堆H
Status DestroyHeap(Heap &H){
    free(H.rcd);
    H.rcd=NULL;
    H.n=0;
    H.size=0;
    return OK;
}
 
//将e从堆尾插入
Status InsertHeap(Heap &H,RcdType e){
    int curr;
    if(H.n>=H.size-1||H.size==0) return ERROR;  //堆已满，插入失败
    //问题点
    if(H.n==0){
        H.rcd=(RcdType*)malloc(sizeof(RcdType)*H.size);
        H.rcd[1]=e;
    }
    curr=++H.n;
    H.rcd[curr]=e;
    int (*prior)(KeyType,KeyType);
    if(H.tag==1)    prior=greatPrior;
    else    prior=lessPrior;
    while(1!=curr&&prior(H.rcd[curr].key,H.rcd[curr/2].key)){
        swapHeapElem(H,curr,curr/2);
        curr/=2;
    }
    return OK;
}
 
//删除堆H的堆顶结点，并用e将其返回
Status RemoveFirstHeap(Heap &H,RcdType &e){
    if(H.n<=0) return ERROR;
    e=H.rcd[1];
    swapHeapElem(H,1,H.n);
    H.n--;            
    if(H.n>1) ShiftDown(H,1);
    return OK;
}
 
//删除位置pos的结点，用e返回
Status RemoveHeap(Heap &H,int pos,RcdType &e){
    if(H.n<=0||pos<=0||pos>H.n) return ERROR;
    e=H.rcd[pos];
    swapHeapElem(H,pos,H.n);
    H.n--;
    if(H.n>1) ShiftDown(H,pos);
    return OK;
}
 
//堆排序
void HeapSort(RcdSqList &L,int tag){
    Heap H;
    int i;
    RcdType e;
    MakeHeap(H,L.rcd,L.length,L.size,tag);
 
    for(i=H.n;i>0;i--){
        RemoveFirstHeap(H,e);
    }
}
 
//求深度 （书本说堆n个结点的完全二叉树的深度为[log2(n) + 1]
int Depth(Heap H){
    float depth=log(H.n)/log(2)+1;
    return (int)depth;
}
 
//打印堆
void printHeap(Heap H){
    if(H.rcd==NULL){
        printf("NULL\n");
        return;
    }
    int i,j;
    printf("顺序存储结构:\n");
    for(i=1;i<=H.n;i++){
        printf("%d ",H.rcd[i].key);
    }
    printf("\n\n树形:\n");
    for(i=1,j=1;i<=H.n;i++){
        printf("%d ",H.rcd[i].key);
        if(i==(int)pow(2,j)-1){
            printf("\n");
            j++;
        }
    }
    printf("\nDepth%d\n\n",(Depth(H)));
}
 
 
 
int main(){
    Heap H; 
    int size=40,i,n;
    RcdSqList L;    //存用来堆排序的记录
    RcdType E[40];                 
    RcdType R[40];  //堆排序专用                 
 
    printf("\n--------begin------------\n\n\n");
    printf("You can do :\n");
    printf("1.初始化空堆H\n");
    printf("2.创建有记录的堆H\n");
    printf("3.删除堆H顶的结点\n");
    printf("4.删除堆H指定pos位置的结点\n");
    printf("5交换第i个结点和第j个结点\n");
    printf("6.销毁H\n");
    printf("7.将e插入堆H\n");
    printf("8.对位置pos左筛选\n");
    printf("9.打印堆H\n");
    printf("10.堆排序\n");
    printf("11.测试大顶堆函数\n");
    printf("12.测试小顶堆函数\n");
    printf("13.退出\n");
    printf("14.再看一次选项\n\n");
    int choice=0;
    int tag,pos,t1,t2;
    KeyType x,y;
    RcdType a;
    while(choice!=13){
        printf("你的选择是:\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                    //初始化   Status InitHeap(Heap &H,int size,int tag)
                    printf("0：小顶堆；1：大顶堆\n");
                    scanf("%d",&tag);
                    if(tag!=0&&tag!=1) {
                        printf("\n");
                        break;
                    }
                    if(InitHeap(H,size,tag))
                        printf("\n");
                    else
                        printf("?\n");
                    printf(":\n\n");
                    printHeap(H);
                    break;
 
 
            case 2:
                    //建长度为n的堆H，容量为size，当tag=0或1时分别表示小顶堆和大顶堆
                    printf("0小顶堆  1大顶堆\n");
                    scanf("%d",&tag);
                    if(tag!=0&&tag!=1) {
                        printf("ERROR\n");
                        break;
                    }
                    printf("堆的长度\n");
                    printf("输入后回车(小于%d)\n\n",size);
                    printf("长度 n=");
                    scanf("%d",&n);
                    if(n>=size||n<0){
                        printf("ERROR\n");
                        break;
                    }
                    if(n==0){
                        InitHeap(H,size,tag);
                        printf("你创建的堆为:\n\n");
                        printHeap(H);
                        break;
                    }
                    printf("\n输入%d个数字\n",n);
                    for(i=1;i<=n;i++){
                        printf("第%d个记录的关键字:",i);
                        scanf("%d",&E[i].key);
                    }
                    printf("\n----------输入结束----------\n\n");
                    printf("你创建的堆为:\n\n");
                    MakeHeap(H,E,n,size,tag);
                    printHeap(H);
                    break;
 
 
            case 3:
                    //删除堆顶结点的函数   Status RemoveFirstHeap(Heap &H,RcdType &e)
                    if(RemoveFirstHeap(H,a)){
                        printf("堆顶结点的记录为:%d\n",a.key);
                        printf(":\n");
                        printHeap(H);
 
                    }else{
                        printf("ERROR , IT IS  A NULL HEAP\n");
                    }
                    break;
 
            case 4:
                    //删除pos位置结点的函数   Status RemoveHeap(Heap &H,int pos,RcdType &e)
                    printf("结点位置pos=");
                    scanf("%d",&pos);
                    if(RemoveHeap(H,pos,a)){
                    printf("删除了%d位置的记录为:%d\n",pos,a.key);
                    printf("删除了%d位置的堆:\n",pos);
                    printHeap(H);
                    }else{
                        printf("ERROR\n");
                    }
                    break;
 
 
            case 5:
                    //交换函数   Status swapHeapElem(Heap &H,int i,int j)
                    printf("输入你想要交换的两个位置:(中间用','隔开)\n");
                    scanf("%d,%d",&t1,&t2);
                    if(swapHeapElem(H,t1,t2)){
                        printf("成功\n");
                        printHeap(H);
                    }
                    else
                    printf("ERROR\n");
                    break;
 
 
            case 6:
                    //销毁函数  DestroyHeap(Heap &H)
                    DestroyHeap(H);
                    printf("销毁成功\n");
                    break;
 
 
            case 7:
                    //插入函数   Status InsertHeap(Heap &H,RcdType e)
                    printf("输入你要插入的关键字RcdType a=");
                    scanf("%d",&a.key);
                    if(InsertHeap(H,a)){
                        printf("插入后的堆\n");
                        printHeap(H);
                    }
                    else printf("ERROR\n");
                    break;
 
 
 
            case 8:
                    //对堆H中位置为pos的结点筛选，以pos为跟的子树调整为子堆
                    printf("输入你要左筛选的位置pos=");
                    scanf("%d",&pos);
                    if(pos<=0||pos>H.n) {
                        printf("ERROR\n");
                        break;
                    }
                    ShiftDown(H,pos);
                    printf("成功\n");
                    printHeap(H);
                    break;
 
 
            case 9:
                    printHeap(H);
                    break;
 
 
            case 10:
                    //推排序  void HeapSort(RcdSqList &L)
                    printf("  0降序 1升序\n");
                    scanf("%d",&tag);
                    if(tag!=0&&tag!=1) {
                        printf("ERROR\n");
                        break;
                    }
                    printf("排序的记录有多少个？\n");
                    scanf("%d",&L.length);
                    if(L.length>=size){printf("ERROR\n");break;}
                    L.size=size;
                    for(i=1;i<=L.length;i++){
                        printf("第%d个记录关键字:",i);
                        scanf("%d",&R[i].key);
                    }
                    L.rcd=R;
                    HeapSort(L,tag);
                    printf("结果\n");
                    for(i=1;i<=L.length;i++){
                        printf("%d ",L.rcd[i].key);
                    }
                    printf("\n");
                    break;
 
 
            case 11:
                    //大顶堆优先函数int greatPrior(KeyType x,KeyType y)
                    printf("输入要测试的两个数\n");
                    printf("第一个:");
                    scanf("%d",&x);
                    printf("第二个:");
                    scanf("%d",&y);
                    if(greatPrior(x,y)){
                        printf("%d优先于%d\n",x,y);
                    }else{
                        printf("%d优先于%d\n",y,x);
                    }
                    break;
 
            case 12:
                    //小顶堆优先函数int lessPrior(KeyType x,KeyType y)
                    printf("输入要测试的两个数\n");
                    printf("第一个:");
                    scanf("%d",&x);
                    printf("第二个:");
                    scanf("%d",&y);
                    if(lessPrior(x,y)){
                        printf("%d优先于%d\n",x,y);
                    }else{
                        printf("%d优先于%d\n",y,x);
                    }
                    break;
 
            case 13:
                    printf("退出\n");
                    break;
 
 
            case 14:
                printf("You can do :\n");
                printf("1.初始化空堆H\n");
                printf("2.创建有记录的堆H\n");
                printf("3.删除堆H顶的结点\n");
                printf("4.删除堆H指定pos位置的结点\n");
                printf("5交换第i个结点和第j个结点\n");
                printf("6.销毁H\n");
                printf("7.将e插入堆H\n");
                printf("8.对位置pos左筛选\n");
                printf("9.打印堆H\n");
                printf("10.堆排序\n");
                printf("11.测试大顶堆函数\n");
                printf("12.测试小顶堆函数\n");
                printf("13.退出\n\n");
                    break;
 
 
            default:printf("ERROR\n");break;
 
        }
        printf("输入14可以再次查看选项\n\n");
    }
 
 
}
