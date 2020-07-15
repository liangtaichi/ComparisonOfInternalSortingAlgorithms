/*
*Author : @liangtaichi
*Date : 2019/11/20 11:44
*LastEditTime : 2019/12/17 23��12
*/
//��������
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
    RcdType *rcd ;  //��ַ ��0��Ԫ����
    int n ;      //����
    int size ;  //����
    int tag ;  //tag = 0 ,С���� ��tag = 1 �� �󶥶� 
} Heap ;//���ﲻ�ܼ�int ( * prior )( KeyType  , KeyType ) ;ϵͳ��֧�ֽṹ�����涨�庯��ָ��
//�󶥶�
int greatPrior(KeyType x,KeyType y){    
    return x>=y;
}
//С���� 
int lessPrior(KeyType x,KeyType y){     
    return x<=y;
}
 
//��ʼ��һ���������Ϊsize�Ŀն�H��tag�����Ǵ󶥻���С��
Status InitHeap(Heap &H,int size,int tag){
    H.rcd=NULL;
    H.size=size;
    H.n=0;
    H.tag=tag;
    return OK;
}
 
// ����H��i�����j���
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
//�Զ�H��λ��Ϊpos�Ľ����ɸѡ������posΪ������������Ϊ�Ӷ�
void ShiftDown(Heap &H,int pos){
    int c,rc;
    int (*prior)(KeyType,KeyType);  //���Ż���������Ӻ���ָ�룬���������
    if(H.tag==1)    prior=greatPrior;//�󶥶�
    else    prior=lessPrior;         //С����
 
    while(pos<=H.n/2){  //��ʱ��Ҷ�ӽ��
        c=pos*2;        //��
        rc=pos*2+1;     //��
        if(rc<=H.n&&prior(H.rcd[rc].key,H.rcd[c].key))
            c=rc;       //tΪpos�������Һ����н�������
        if(prior(H.rcd[pos].key,H.rcd[c].key))
            return;     //tΪ�����ȣ�����
        swapHeapElem(H,pos,c); //����pos���������t���н���
        pos=c;          //�������µ���
    }
}
 
//��E��������Ϊn�Ķ�H������Ϊsize����tag=0��С���ѣ�tag=1���󶥶�
void MakeHeap(Heap &H,RcdType *E,int n,int size,int tag){
    int i;
    H.rcd=E;            //E[1...n]�ǶѵĽ�㣬0�ŵ�Ԫ����
    H.n=n;
    H.size=size;
    H.tag=tag;
    int (*prior)(KeyType,KeyType);
    if(H.tag==1)    prior=greatPrior;
    else    prior=lessPrior;
    for(i=H.n/2;i>0;i--)    //�����з�Ҷ�ӽ��ɸѡ
        ShiftDown(H,i);
}
 
//���ٶ�H
Status DestroyHeap(Heap &H){
    free(H.rcd);
    H.rcd=NULL;
    H.n=0;
    H.size=0;
    return OK;
}
 
//��e�Ӷ�β����
Status InsertHeap(Heap &H,RcdType e){
    int curr;
    if(H.n>=H.size-1||H.size==0) return ERROR;  //������������ʧ��
    //�����
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
 
//ɾ����H�ĶѶ���㣬����e���䷵��
Status RemoveFirstHeap(Heap &H,RcdType &e){
    if(H.n<=0) return ERROR;
    e=H.rcd[1];
    swapHeapElem(H,1,H.n);
    H.n--;            
    if(H.n>1) ShiftDown(H,1);
    return OK;
}
 
//ɾ��λ��pos�Ľ�㣬��e����
Status RemoveHeap(Heap &H,int pos,RcdType &e){
    if(H.n<=0||pos<=0||pos>H.n) return ERROR;
    e=H.rcd[pos];
    swapHeapElem(H,pos,H.n);
    H.n--;
    if(H.n>1) ShiftDown(H,pos);
    return OK;
}
 
//������
void HeapSort(RcdSqList &L,int tag){
    Heap H;
    int i;
    RcdType e;
    MakeHeap(H,L.rcd,L.length,L.size,tag);
 
    for(i=H.n;i>0;i--){
        RemoveFirstHeap(H,e);
    }
}
 
//����� ���鱾˵��n��������ȫ�����������Ϊ[log2(n) + 1]
int Depth(Heap H){
    float depth=log(H.n)/log(2)+1;
    return (int)depth;
}
 
//��ӡ��
void printHeap(Heap H){
    if(H.rcd==NULL){
        printf("NULL\n");
        return;
    }
    int i,j;
    printf("˳��洢�ṹ:\n");
    for(i=1;i<=H.n;i++){
        printf("%d ",H.rcd[i].key);
    }
    printf("\n\n����:\n");
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
    RcdSqList L;    //������������ļ�¼
    RcdType E[40];                 
    RcdType R[40];  //������ר��                 
 
    printf("\n--------begin------------\n\n\n");
    printf("You can do :\n");
    printf("1.��ʼ���ն�H\n");
    printf("2.�����м�¼�Ķ�H\n");
    printf("3.ɾ����H���Ľ��\n");
    printf("4.ɾ����Hָ��posλ�õĽ��\n");
    printf("5������i�����͵�j�����\n");
    printf("6.����H\n");
    printf("7.��e�����H\n");
    printf("8.��λ��pos��ɸѡ\n");
    printf("9.��ӡ��H\n");
    printf("10.������\n");
    printf("11.���Դ󶥶Ѻ���\n");
    printf("12.����С���Ѻ���\n");
    printf("13.�˳�\n");
    printf("14.�ٿ�һ��ѡ��\n\n");
    int choice=0;
    int tag,pos,t1,t2;
    KeyType x,y;
    RcdType a;
    while(choice!=13){
        printf("���ѡ����:\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                    //��ʼ��   Status InitHeap(Heap &H,int size,int tag)
                    printf("0��С���ѣ�1���󶥶�\n");
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
                    //������Ϊn�Ķ�H������Ϊsize����tag=0��1ʱ�ֱ��ʾС���Ѻʹ󶥶�
                    printf("0С����  1�󶥶�\n");
                    scanf("%d",&tag);
                    if(tag!=0&&tag!=1) {
                        printf("ERROR\n");
                        break;
                    }
                    printf("�ѵĳ���\n");
                    printf("�����س�(С��%d)\n\n",size);
                    printf("���� n=");
                    scanf("%d",&n);
                    if(n>=size||n<0){
                        printf("ERROR\n");
                        break;
                    }
                    if(n==0){
                        InitHeap(H,size,tag);
                        printf("�㴴���Ķ�Ϊ:\n\n");
                        printHeap(H);
                        break;
                    }
                    printf("\n����%d������\n",n);
                    for(i=1;i<=n;i++){
                        printf("��%d����¼�Ĺؼ���:",i);
                        scanf("%d",&E[i].key);
                    }
                    printf("\n----------�������----------\n\n");
                    printf("�㴴���Ķ�Ϊ:\n\n");
                    MakeHeap(H,E,n,size,tag);
                    printHeap(H);
                    break;
 
 
            case 3:
                    //ɾ���Ѷ����ĺ���   Status RemoveFirstHeap(Heap &H,RcdType &e)
                    if(RemoveFirstHeap(H,a)){
                        printf("�Ѷ����ļ�¼Ϊ:%d\n",a.key);
                        printf(":\n");
                        printHeap(H);
 
                    }else{
                        printf("ERROR , IT IS  A NULL HEAP\n");
                    }
                    break;
 
            case 4:
                    //ɾ��posλ�ý��ĺ���   Status RemoveHeap(Heap &H,int pos,RcdType &e)
                    printf("���λ��pos=");
                    scanf("%d",&pos);
                    if(RemoveHeap(H,pos,a)){
                    printf("ɾ����%dλ�õļ�¼Ϊ:%d\n",pos,a.key);
                    printf("ɾ����%dλ�õĶ�:\n",pos);
                    printHeap(H);
                    }else{
                        printf("ERROR\n");
                    }
                    break;
 
 
            case 5:
                    //��������   Status swapHeapElem(Heap &H,int i,int j)
                    printf("��������Ҫ����������λ��:(�м���','����)\n");
                    scanf("%d,%d",&t1,&t2);
                    if(swapHeapElem(H,t1,t2)){
                        printf("�ɹ�\n");
                        printHeap(H);
                    }
                    else
                    printf("ERROR\n");
                    break;
 
 
            case 6:
                    //���ٺ���  DestroyHeap(Heap &H)
                    DestroyHeap(H);
                    printf("���ٳɹ�\n");
                    break;
 
 
            case 7:
                    //���뺯��   Status InsertHeap(Heap &H,RcdType e)
                    printf("������Ҫ����Ĺؼ���RcdType a=");
                    scanf("%d",&a.key);
                    if(InsertHeap(H,a)){
                        printf("�����Ķ�\n");
                        printHeap(H);
                    }
                    else printf("ERROR\n");
                    break;
 
 
 
            case 8:
                    //�Զ�H��λ��Ϊpos�Ľ��ɸѡ����posΪ������������Ϊ�Ӷ�
                    printf("������Ҫ��ɸѡ��λ��pos=");
                    scanf("%d",&pos);
                    if(pos<=0||pos>H.n) {
                        printf("ERROR\n");
                        break;
                    }
                    ShiftDown(H,pos);
                    printf("�ɹ�\n");
                    printHeap(H);
                    break;
 
 
            case 9:
                    printHeap(H);
                    break;
 
 
            case 10:
                    //������  void HeapSort(RcdSqList &L)
                    printf("  0���� 1����\n");
                    scanf("%d",&tag);
                    if(tag!=0&&tag!=1) {
                        printf("ERROR\n");
                        break;
                    }
                    printf("����ļ�¼�ж��ٸ���\n");
                    scanf("%d",&L.length);
                    if(L.length>=size){printf("ERROR\n");break;}
                    L.size=size;
                    for(i=1;i<=L.length;i++){
                        printf("��%d����¼�ؼ���:",i);
                        scanf("%d",&R[i].key);
                    }
                    L.rcd=R;
                    HeapSort(L,tag);
                    printf("���\n");
                    for(i=1;i<=L.length;i++){
                        printf("%d ",L.rcd[i].key);
                    }
                    printf("\n");
                    break;
 
 
            case 11:
                    //�󶥶����Ⱥ���int greatPrior(KeyType x,KeyType y)
                    printf("����Ҫ���Ե�������\n");
                    printf("��һ��:");
                    scanf("%d",&x);
                    printf("�ڶ���:");
                    scanf("%d",&y);
                    if(greatPrior(x,y)){
                        printf("%d������%d\n",x,y);
                    }else{
                        printf("%d������%d\n",y,x);
                    }
                    break;
 
            case 12:
                    //С�������Ⱥ���int lessPrior(KeyType x,KeyType y)
                    printf("����Ҫ���Ե�������\n");
                    printf("��һ��:");
                    scanf("%d",&x);
                    printf("�ڶ���:");
                    scanf("%d",&y);
                    if(lessPrior(x,y)){
                        printf("%d������%d\n",x,y);
                    }else{
                        printf("%d������%d\n",y,x);
                    }
                    break;
 
            case 13:
                    printf("�˳�\n");
                    break;
 
 
            case 14:
                printf("You can do :\n");
                printf("1.��ʼ���ն�H\n");
                printf("2.�����м�¼�Ķ�H\n");
                printf("3.ɾ����H���Ľ��\n");
                printf("4.ɾ����Hָ��posλ�õĽ��\n");
                printf("5������i�����͵�j�����\n");
                printf("6.����H\n");
                printf("7.��e�����H\n");
                printf("8.��λ��pos��ɸѡ\n");
                printf("9.��ӡ��H\n");
                printf("10.������\n");
                printf("11.���Դ󶥶Ѻ���\n");
                printf("12.����С���Ѻ���\n");
                printf("13.�˳�\n\n");
                    break;
 
 
            default:printf("ERROR\n");break;
 
        }
        printf("����14�����ٴβ鿴ѡ��\n\n");
    }
 
 
}
