#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _node_{
    char *palavra;
    struct _node_ *next;
    struct _node_ *prev;
    struct _list_ *sin;
}node;

typedef struct _list_{
    node *head;
    node *last;
}list;

list *createList(){
    list *l;
    l=(list*)malloc(sizeof(list));
    if(l!=NULL){
        l->head=NULL;
        l->last=NULL;
        return l;
    }
    return NULL;
}

node *createNode(){
    node *no;
    no=(node*)malloc(sizeof(node));
    if(no!=NULL){
        no->prev=NULL;
        no->next=NULL;
        no->palavra=NULL;
        no->sin=createList();
        return no;
    }
    return NULL;
}

void addPalavra(node *no,char *s){
    if(no!=NULL && s!=NULL){
        int tam;
        tam=strlen(s);
        no->palavra=(char*)malloc(tam * sizeof(char));
        strcpy(no->palavra,s);
    }
}

int addList(list *l, char *c){
    if(l!=NULL && c!=NULL){
        node *no;
        no=createNode();
        addPalavra(no,c);
        if(l->head==NULL){
            l->head=no;
            l->last=no;
        }else{
            no->prev=l->last;
            l->last->next=no;
            l->last=no;
        }
        return 1;
    }
    return 0;
}

node *compareWords(list *l, char *c){
    if(l!=NULL && c!=NULL){
        node *aux;
        int log;
        aux=l->head;
        while(aux!=NULL){
            if(strcmp(aux->palavra,c)==0){
                return aux;
            }
            aux=aux->next;
        }
        return NULL;
    }
}
int removeList(list *l, char *c){
    if(l!=NULL && c!=NULL){
        node *aux;
        aux=compareWords(l,c);
        if(aux!=NULL){
            if(aux->prev==NULL){
                aux->next->prev=NULL;
                l->head=l->head->next;
            }else if(aux->next==NULL){
                aux->prev->next=NULL;
                l->last=l->last->prev;
            }else{
                aux->prev->next=aux->next;
                aux->next->prev=aux->prev;
            }
            return 1;
        }
    }
    return 0;
}

int countLine(FILE *f, char *n_arq){
    f=fopen(n_arq,"r");
    char c;
    int count=0;

    while((c=fgetc(f))!=EOF){
        if(c=='\n')
            count++;
    }
    fclose(f);
    return count;
}

list *addSin(FILE *f){
    list *l;
    int lines,i;
    char *nome_arq;

    l=createList();
    nome_arq=(char*)malloc(100*sizeof(char));
    strcpy(nome_arq,"sinonimos.csv");
    lines=countLine(f,nome_arq);

    char *buffer,*end_bf,*an;
    buffer=(char*)malloc(500*sizeof(char));
    end_bf=buffer;
    int log;
    an=NULL;
    f=fopen(nome_arq,"r");

    for(i=0;i<lines;i++){
        fgets(buffer,500,f);

        an=strstr(buffer,";");
        *an=NULL;
        log=addList(l,buffer);
        buffer=an+1;

        while(buffer!=NULL){
            an=strstr(buffer,";");
            if(an!=NULL){

                *an=NULL;
                log=addList(l->last->sin,buffer);
                buffer=an+1;
            }else{
                an=strstr(buffer,"\n");
                *an=NULL;
                log=addList(l->head->sin,buffer);
                buffer=NULL;
            }
        }
        buffer=end_bf;
    }
    printf("fechou");
    fclose(f);
    return l;
}

int main(){
    FILE *sin;

    list *l;
    int log;
    l=addSin(l);
    printf("%i\n",log);
    char *a;
    a=(char*)malloc(100*sizeof(char));
    while(l->head!=NULL){
        strcpy(a,l->head->palavra);
        printf("%s\n",a);
        l->head=l->head->next;
    }
}
