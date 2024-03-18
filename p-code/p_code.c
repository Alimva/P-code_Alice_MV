#include <stdio.h>

#define amax  2047 //Maior endereço possível
#define levmax 3 //Profundidade máxima
#define cxmax 200 //Tamanho do array code

typedef enum { LIT, OPR, LOD, STO, CAL, INT, JMP, JPC} fct;

typedef struct 
{
    fct f;
    int l;
    int a;
}instruction;

int stacksize = 500;
int p, b, t; //programa, base, topo da pilha
p = 0;
b = 0;
t = 0;

instruction code[] = {{JMP,0,0}};

void interpret(void);
int base(int n, long int s[]);
void include_code(fct f, int l, int a, instruction code[],int n);

int main(void){
    interpret();
    return 0;
}

void include_code(fct f, int l, int a, instruction code[], int n){
    code[n].f = f;
    code[n].l = l;
    code[n].a = a;
}

void interpret(void){
    long int s[stacksize];
    instruction i;

    printf("Start\n");
    printf("p | (i.f i.l i.a) | (p b t) | s[0] s[1] s[2] s[3] s[4] s[5] s[6] s[7]\n");
    printf("-------------------------------------------------------------------------\n\n");
    
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
    s[4] = 0;
    s[5] = 0;

    do{
        printf("%i | ",p);
        i = code[p];
        p++;

        switch (i.f)
        {
        case LIT:
            printf("(LIT %i %i) | ",i.l,i.a);
            t++;
            s[t] = i.a;
            break;
        case OPR:
            printf("(OPR %i %i) | ",i.l,i.a);
            switch (i.a)
            {
            case 0:
                t = b-1;
                p = s[t+3];
                b = s[t+2];
                break;
            case 1:
                s[t] = (s[t])*-1;
                break;
            case 2:
                t--;
                s[t] += s[t+1];
                break;
            case 3:
                t--;
                s[t] -= s[t+1];
                break;
            case 4:
                t--;
                s[t] = s[t] * s[t+1];
                break;
            case 5:
                t--;
                s[t] = s[t]/s[t+1];
                break;
            case 6:
                t--;
                if(s[t] % 2 == 0){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 7:
                t--;
                if(s[t] == s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 8:
                t--;
                if(s[t] != s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 9:
                t--;
                if(s[t] < s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 10:
                t--;
                if(s[t] >= s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 11:
                t--;
                if(s[t] > s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            case 12:
                t--;
                if(s[t] <= s[t+1]){
                    s[t] = 1;
                }else{
                    s[t] = 0;
                }
                break;
            default: 
                break;
            }
            break;
        case LOD:
            printf("(LOD %i %i) | ",i.l,i.a);
            t++;
            s[t] = s[base(i.l,s) + i.a];
            break;
        case STO:
            printf("(STO %i %i) | ",i.l,i.a);
            s[base(i.l,s) + i.a] = s[t];
            t--;
            break;
        case CAL:
            printf("(CAL %i %i) | ",i.l,i.a);
            t+= i.a;
            s[t+1] = base(i.l,s);
            s[t+2] = b;
            s[t+3] = p;
            b = t+1;
            p = i.a;
            break;
        case INT:
            printf("(INT %i %i) | ",i.l,i.a);
            t += i.a;
            break;
        case JMP:
            printf("(JMP %i %i) | ",i.l,i.a);
            p = i.a;
            break;
        case JPC:
            printf("(JPC %i %i) | ",i.l,i.a);
            if(s[t] == 0){
                p = i.a;
                t--;
            }
            break;
        default:
            break;
        }
        printf("(%i %i %i) |  ",p,b,t);
        for(int i = 0;i < 10; i++){
            printf(" %li ",s[i]);
        }
        printf("\n");
    }while(p != 0);
    
    printf("\n-------------------------------------------------------------------------\n");
    printf("p | (i.f i.l i.a) | (p b t) | s[0] s[1] s[2] s[3] s[4] s[5] s[6] s[7]\n");
    printf("End pl\n");
}

int base(int l, long int s[]){
    int b1 = b;

    while(l > 0){
        b1 = s[b1];
        l--;
    }
    return b1;
}