#include "gvar.h"
#include "opcodes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
int mov(){
    int idx = arg2[0] - 'a';
    values[idx] = atoi(arg3);
}
int add(){
    int idx = arg2[0] - 'a';
    values[idx] = values[idx] + atoi(arg3);
}
int INT_print(){
    int idx = arg2[0] - 'a';
    printf("%d\n",values[idx]);
}
int sub(){
    int idx = arg2[0] - 'a';
    values[idx] = values[idx] - atoi(arg3);
}
int add_num() {
    int result = atoi(arg2) + atoi(arg3);
    printf("%d\n", result);
}
int subt_num(){
    int result = atoi(arg2) - atoi(arg3);
    printf("%d\n", result);
}
int clear() {
    int idx = arg2[0] - 'a';
    values[idx] = 0;
}
int mul_num() {
    int result = atoi(arg2) * atoi(arg3);
    printf("%d\n", result);
}
int div_num() {
    int result = atoi(arg2) / atoi(arg3);
    printf("%d\n", result);
}
void raz() {
    arg2[0] = 0; arg3[0] = 0; arg4[0] = 0; arg5[0] = 0;
    if (sscanf(s, "%89s %89s %89s %89s %89s", arg1,arg2,arg3,arg4,arg5) < 1) return;
    int num = -1;
    if (strcmp(arg1,"mov")==0) { num = OP_MOV;
    }else if(strcmp(arg1,"add")==0){ num = OP_ADD;
    }else if(strcmp(arg1,"sub")==0){ num = OP_SUB;
    }else if(strcmp(arg1,"int_print")==0){ num = OP_INT_PRINT;
    }else if(strcmp(arg1,"print_c")==0){ num = OP_PRINT_C;
    }else if(strcmp(arg1,"add_num")==0){ num = OP_ADD_NUM;
    }else if(strcmp(arg1,"subt_num")==0){ num = OP_SUBT_NUM;
    }else if(strcmp(arg1,"clear")==0){ num = OP_CLEAR;
    }else if(strcmp(arg1,"mul_num")==0){ num = OP_MUL_NUM;
    }else if(strcmp(arg1,"div_num")==0){ num = OP_DIV_NUM;
    }else if(strcmp(arg1,"str")==0){ num = OP_STR;
    }else if(strcmp(arg1,"print_str")==0){ num = OP_PRINT_STR;
    }else if(strcmp(arg1,"fmov")==0){ num = OP_FMOV;
    }else if(strcmp(arg1,"printfl")==0){ num = OP_PRINTFL;
    }else if(strcmp(arg1,"movC")==0){ num = OP_MOVC;
    }else if(strcmp(arg1,"printc")==0){ num = OP_PRINTC;
    }else if(strcmp(arg1,"strlen")==0){ num = OP_STRLEN;
    }else if(strcmp(arg1,"fadd")==0){ num = OP_FADD;
    }else if(strcmp(arg1,"input_int")==0){ num = OP_INPUT_INT;
    }else if(strcmp(arg1,"input_str")==0){ num = OP_INPUT_STR;
    }else if(strcmp(arg1,"dec")==0){ num = OP_DEC;
    }else if(strcmp(arg1,"inc")==0){ num = OP_INC;
    }else if(strcmp(arg1,"copy")==0){ num = OP_COPY;
    }else if(strcmp(arg1,"savev")==0){ num = OP_SAVEV;
    }else if(strcmp(arg1,"loadv")==0){ num = OP_LOADV;
    }else if(strcmp(arg1,"inputf")==0){ num = OP_INPUTF;
    }else if(strcmp(arg1,"inputc")==0){ num = OP_INPUTC;
    }else if(strcmp(arg1,"if")==0){ num = OP_IF;
    }else if(strncmp(arg1,"//",2)==0){ num = OP_CM;
    }else{printf("unknown: %s",arg1);}
    code[cs++] = num;
    code[cs++] = 0;
    code[cs++] = 0;
    strcpy(sa[arg_i],arg2);strcpy(sb[arg_i],arg3);arg_i++;
}
void load_sbc(const char *name) {
    FILE *f = fopen(name, "r");
    if (!f) { puts("Файл не найден"); return; }
    cs = 0;
    int n;
    fscanf(f, "%d", &cs);
    for (int i = 0; i < cs; i++) fscanf(f, "%d", &code[i]);
    fscanf(f,"%d",&arg_i);
    for(int i=0;i<arg_i;i++) {
        fscanf(f,"%s",sa[i]);
        fgetc(f);
        fgets(sb[i],100,f);
        sb[i][strcspn(sb[i],"\n")]=0;
        if(sb[i][0]==' ')memmove(sb[i],sb[i]+1,strlen(sb[i]));
    }
    fclose(f);
}

void save_sbc(const char *name) {
    FILE *f = fopen(name, "w");
    if (!f) { puts("Не удалось создать файл"); return; }
    fprintf(f, "%d\n", cs);
    for (int i = 0; i < cs; i++) {
        fprintf(f, "%d\n", code[i]);
    }
    fprintf(f,"%d\n",arg_i);
    for(int i=0;i<arg_i;i++) fprintf(f,"%s %s\n",sa[i],sb[i]);
    fclose(f);
    printf("Байт-код записан в %s\n", name);
}
int run(){
    pc = 0;
    while (pc < cs) {
        int o = code[pc];
        pc += 3;
        strcpy(arg2, sa[pc/3-1]);
        strcpy(arg3,sb[pc/3-1]);

        if(o == OP_MOV){mov();
        }else if(o == OP_ADD){ add();
        }else if(o == OP_SUB){ sub();
        }else if(o == OP_INT_PRINT){ INT_print();
        }else if(o == OP_PRINT_C){ printf("%s\n",arg2);
        }else if(o == OP_ADD_NUM) { add_num();
        }else if(o == OP_SUBT_NUM) { subt_num();
        }else if(o == OP_CLEAR) { clear();
        }else if(o == OP_MUL_NUM) { mul_num();
        }else if(o == OP_DIV_NUM) { div_num();
        }else if(o == OP_STR) {
            int idx = arg2[0] - 'a';
            strcpy(strings[idx], arg3);
        }else if(o == OP_PRINT_STR) {
            int idx = arg2[0] - 'a';
            printf("%s\n", strings[idx]);
        }else if(o == OP_FMOV) {
            int idx = arg2[0] - 'a';
            fval[idx] = atof(arg3);
        }else if(o == OP_PRINTFL) {
            int idx = arg2[0] - 'a';
            printf("%f\n",fval[idx]);
        }else if(o == OP_MOVC) {
            int idx = arg2[0] - 'a';
            fval[idx] = fval[idx] - atof(arg3);
        }else if(o == OP_PRINTC) {
            int idx = arg2[0] - 'a';
            cval[idx] = arg3[0];
        }else if(o == OP_STRLEN) {
            int idx = arg2[0] - 'a';
            printf("%c\n",cval[idx]);
        }else if(o == OP_FADD) {
            int idx = arg2[0] - 'a';
            printf("%zu\n",strlen(strings[idx]));
        }else if(o == OP_INPUT_INT) {
            int idx = arg2[0] - 'a';
            scanf("%d", &values[idx]);
            getchar();
        }else if(o == OP_INPUT_STR) {
            int idx = arg2[0] - 'a';
            scanf("%s", strings[idx]);
            getchar();
        }else if(o == OP_DEC) {
            int idx = arg2[0] - 'a';
            values[idx] = values[idx] - 1;
        }else if(o == OP_INC) {
            int idx = arg2[0] - 'a';
            values[idx] = values[idx] + 1;
        }else if(o == OP_COPY) {
            int idx1 = arg2[0] - 'a';
            int idx2 = arg3[0] - 'a';
            values[idx2] = values[idx1];
        }else if(o == OP_SAVEV) {
            int idx = arg2[0] - 'a';
            FILE *f = fopen("data.txt", "w");
            fprintf(f, "%d", values[idx]);
            fclose(f);
        }else if(o == OP_LOADV) {
            int idx = arg2[0] - 'a';
            FILE *f = fopen("data.txt", "r");
            if (f) {
                fscanf(f, "%d", &values[idx]);
                fclose(f);
            }
        }else if(o == OP_INPUTF) {
            int idx = arg2[0] - 'a';
            scanf("%f", &fval[idx]);
            getchar();
        }else if(o == OP_INPUTC) {
            int idx = arg2[0] - 'a';
            scanf("%c", &cval[idx]);
            getchar();
        }else if(o == OP_IF) {
            int idx = arg2[0] - 'a';
            if (values[idx] != 0) {
                char buf[90];
                snprintf(buf, sizeof(buf), "%s %s %s", arg3, arg4, arg5);
                strcpy(s, buf);
                raz();
            }
        }else if(o == OP_CM) {;}
    }

}
void main() {
    int ch;
    puts("Напишите цифру 3 и увидите информацию");
    puts("о использования приложения.");
    puts("\n1 - REPL interpretator");
    puts("2 - to run code");
    puts("3 - help");
    puts("4 - compile");
    puts("5 - run\n");
    while (1) {
        scanf("%d",&ch);
        getchar();
        switch (ch) {
            case 1:
                puts("———————————————————");
                puts("\nSF interpretator");
                while (1) {
                    printf("> ");
                    fgets(s, sizeof(s), stdin);
                    s[strcspn(s, "\n")] = 0;
                    if (strcmp(s, "exit") == 0) break;
                    cs=0;arg_i=0;raz();run();
                }
                break;
            case 2:
                int fi;
                puts("Файл должен называтся programm.sf\n");
                scanf("%d",&fi);
                if(fi==1){
                    system("clear");
                    printf("------output-------\n\n");
    			    FILE *f = fopen("programm.sf","r");
                    printf("\n-------------------\n");
                    if(!f){
                    puts("Файл пустой или не найден");
                    puts("Нужен файл с названием programm.sf");
                    break;
                    }
                    cs = 0;
                    arg_i = 0;
    			    while (fgets(s, sizeof(s), f)) { 
        		    s[strcspn(s, "\n")] = 0;
        		    raz();
                    }
                    printf("\n-------------------\n");
                    run();
                    fclose(f);
                    break;
                }
                return;
            case 3:
                printf("\nЕсли выбираете два, то нужен файл с нужным\n");
                printf("кодом и с названием programm.sf\n\n");
                break;
            case 4:
                cs = 0;
                arg_i = 0;
                FILE *f4 = fopen("programm.sf", "r");
                if (!f4) { puts("Файл не найден"); break; }
                while (fgets(s, sizeof(s), f4)) {
                    s[strcspn(s, "\n")] = 0;
                    raz();
                }
                fclose(f4);
                save_sbc("programm.f");
                break;
            case 5:
                load_sbc("programm.f");
                run();
                break;
            default:puts("Неизвестная команда");   
        }    
    }    
    return;
}
