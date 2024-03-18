#include <stdio.h>

#define amax  2047 //Maior endereço possível
#define levmax 3 //Profundidade máxima
#define cxmax 200 //Tamanho do array code

typedef enum { lit, opr, lod, sto, cal, inte, jmp, jpc } fct;

typedef struct 
{
    fct f;
    int l;
    int a;
}instruction;

instruction code[] = {
    {lit,0,100},//cria a variável de controle
    {lit,0,1},//cria o contador
    {lit,0,1},//cria o valor atual da soma
    {sto,0,7},//armazena o valor das variáveis
    {sto,0,5},
    {sto,0,8},
    {inte,0,3},
    {opr,0,2},//incrementa o contador
    {sto,0,6},//salva a variável
    {inte,0,-1},
              //inicio do loop
    {lod,0,8},//carrega as variáveis
    {lod,0,6},
    {lod,0,6},
    {opr,0,4},//gera contador ao quadrado
    {lod,0,5},
    {opr,0,2},//incrementa a soma
    {sto,0,5},//armazena o valor da soma
    {lod,0,6},//carrega o contador
    {opr,0,7},//verifica se é igual a variável de controle
    {inte,0,1},
    {lod,0,7},
    {opr,0,2},//incrementa contador
    {sto,0,6},
    {jpc,0,10},//verifica se a condição foi aceita
    {jmp,0,0}
};

void interpret(void);
int base(int n, int b,long int s[]);
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
    int stacksize = 500;
    int p, b, t; //programa, base, topo da pilha
    instruction i;
    long int s[stacksize];

    printf("Start\n");
    p = 0;
    b = 1;
    t = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
    s[4] = 0;
    s[5] = 0;

    do{
        i = code[p];
        p++;

        switch (i.f)
        {
        case lit:
            printf("Instrucao = lit %i %i\n",i.l,i.a);
            t++;
            s[t] = i.a;
            break;
        case opr:
            printf("Instrucao = opr %i %i\n",i.l,i.a);
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
        case lod:
            printf("Instrucao = lod %i %i\n",i.l,i.a);
            t++;
            s[t] = s[base(i.l,b,s) + i.a];
            break;
        case sto:
            printf("Instrucao = sto %i %i\n",i.l,i.a);
            s[base(i.l,b,s) + i.a] = s[t];
            t--;
            break;
        case cal:
            printf("Instrucao = cal %i %i\n",i.l,i.a);
            t+= i.a;
            s[t+1] = base(i.l,b,s);
            s[t+2] = b;
            s[t+3] = p;
            b = t+1;
            p = i.a;
            break;
        case inte:
            printf("Instrucao = int %i %i\n",i.l,i.a);
            t += i.a;
            break;
        case jmp:
            printf("Instrucao = jmp %i %i\n",i.l,i.a);
            p = i.a;
            break;
        case jpc:
            printf("Instrucao = jpc %i %i\n",i.l,i.a);
            if(s[t] == 0){
                p = i.a;
                t--;
            }
            break;
        default:
            break;
        }
        printf("P = %i   B = %i   T = %i\n",p,b,t);
        printf("Pilha:");
        for(int i = 1;i < 10; i++){
            printf(" %li //",s[i]);
        }
        printf("\n");
    }while(p != 0);
    printf("\nEnd pl\n");
}

int base(int l, int b,long int s[]){
    int b1 = b;

    while(l > 0){
        b1 = s[b1];
        l--;
    }
    return b1;
}