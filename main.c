#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

typedef struct _node_{
    char *palavra;
    struct _node_ *next;
    struct _node_ *prev;
    struct _list_ *sin;
    int rep;
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
        no->rep=0;
        return no;
    }
    return NULL;
}

void addPalavra(node *no,char *s){
    if(no!=NULL && s!=NULL){
        int tam;
        tam=strlen(s)+1;
        no->palavra=(char*)malloc(tam  * sizeof(char));
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

int countLine( char *n_arq){
    FILE *f;
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

list *addSin(char *n_arq){
    FILE *f;
    f=fopen(n_arq,"r");

    char *buffer;
    //buffer=(char*)malloc(500*sizeof(char));
    char *an;

    int lines,i;

    list *l;

    l=createList();
    an=NULL;
    lines=countLine(n_arq);

    for(i=0;i<lines;i++){
        buffer=(char*)malloc(500*sizeof(char));
        fgets(buffer,500,f);

        an=strstr(buffer,";");
        *an='\0';
        addList(l,buffer);
        buffer=an+1;

        an=strstr(buffer,";");
        while(an!=NULL){
            *an='\0';
            addList(l->last->sin,buffer);
            buffer=an +1;
            an=strstr(buffer,";");
        }

    }
    fclose(f);
    return l;
}

int countCarct( char *n_arq){
    FILE *f;
    f=fopen(n_arq,"r");
    char c;
    int count=0;

    while((c=fgetc(f))!=EOF){

            count++;
    }
    fclose(f);
    return count;
}

list *storeText(char *text){
    FILE * f;
    f=fopen(text,"r");
    int tam;
    char *buffer,*an;
    list *l;
    l=createList();
    tam=countCarct(text)+100;
    buffer=(char*)malloc(tam *sizeof(char));

    fgets(buffer,tam,f);
    an=strtok(buffer," ,.-;!?");

    while(an!=NULL){
        addList(l,an);
        an=strtok(NULL," ,.-;!?");
    }
    return l;
}

int main(){
    setlocale(LC_ALL,"");
    FILE *sin;
    char *nome_arq;
    list *l,*t_l;
    int log;

    nome_arq=(char*)malloc(100*sizeof(char));
    strcpy(nome_arq,"sinonimos.csv");
    l=addSin(nome_arq);
    log=countCarct(nome_arq);
    printf("%i\n",log);
    strcpy(nome_arq,"text.txt");
    t_l=storeText(nome_arq);

    char *a;
    a=(char*)malloc(100*sizeof(char));
    while(l->head!=NULL){
        strcpy(a,l->head->palavra);
        printf("%s\n",a);
        while(l->head->sin->head!=NULL){
            strcpy(a,l->head->sin->head->palavra);
            printf("%s, ",a);
            l->head->sin->head=l->head->sin->head->next;

        }
        printf("\n");
        l->head=l->head->next;
    }
    printf("\n\n");
    while(t_l->head!=NULL){
        strcpy(a,t_l->head->palavra);
        printf("%s ",a);
        t_l->head=t_l->head->next;
    }

}
