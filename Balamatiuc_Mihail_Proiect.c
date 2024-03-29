#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_LEN 256

typedef struct sala {
    char nume[100];
    int capacitate;
} sala;

//nume sala ora1 ora2 data_string
typedef struct rezerva {
    char nume[100];
    char sala_rez[100];
    int ora1;
    int ora2;
    char data[20];
} rezerva;

void adauga_sala(sala *s, char add[]) {
    int ind = 0;
    int ok = 0;
    int nr = 0;
    for (int i = 0; i < strlen(add); i++) {
        if (ok && add[i] != '\n') {
            nr *= 10;
            nr += add[i] - '0';
        }
        else {
            if (add[i] == ' ')
                ok = 1;
            else
                s->nume[ind++] = add[i];
        }
    }
    s->nume[ind] = '\0';
    s->capacitate = nr;
}

void adauga_rez(rezerva* s, char add[]) {
    int ind = 0;
    int sp = 0;//cate spatii am intalnit
    for (int i = 0; i < strlen(add); i++) {
        if (add[i] == ' ') {
            if (sp == 0) {
                s->nume[ind] = '\0';
                sp++;
                ind = 0;
                continue;
            }
            if (sp == 1) {
                s->sala_rez[ind] = '\0';
                sp++;
                ind = 0;
                continue;
            }
            sp++;
            ind = 0;
            continue;
        }
        if (!sp) 
            s->nume[ind++] = add[i];
        if (sp == 1) 
            s->sala_rez[ind++] = add[i];
        if (sp == 2) {
            ind *= 10;
            ind += add[i] - '0';
            s->ora1 = ind;
        }
        if (sp == 3) {
            ind *= 10;
            ind += add[i] - '0';
            s->ora2 = ind;
        }
        if (sp == 4) 
            s->data[ind++] = add[i];
    }
    s->data[ind] = '\0';
}

void afisare_tot(sala s[], int ind) {
    for (int i = 0; i < ind; i++) 
        printf("%d. %s, capacitate %d pers.\n", i + 1, s[i].nume, s[i].capacitate);
    printf("\n");
}

void afisare_nume(sala s[], int ind) {
    for (int i = 0; i < ind; i++)
        printf("%d. %s\n", i + 1, s[i].nume);
    printf("\n");
}

void afisare_rez(rezerva s[], int ind) {
    for (int i = 0; i < ind; i++)
        printf("%s a rezervat %s de la %d pana la %d la data %s\n", s[i].nume, s[i].sala_rez, s[i].ora1, s[i].ora2, s[i].data);
    printf("\n");
}

int main() {
    //sali
    sala all[1000];
    int ind = 0;

    FILE* fp;
    fp = fopen("in.txt", "r");
    if (fp == NULL) {
        perror("Failed: ");
        return 1;
    }

    char buffer[MAX_LEN];
    while (fgets(buffer, MAX_LEN, fp)){
        buffer[strcspn(buffer, "\n")] = 0;
        adauga_sala(&all[ind++], buffer);
    }

    fclose(fp);

    //rezervari
    rezerva rez[1000];
    int ind_rez = 0;

    FILE* rez_p;
    rez_p = fopen("rezervari.txt", "r");
    if (rez_p == NULL) {
        perror("Failed: ");
        return 1;
    }

    char buffer_rez[MAX_LEN];
    while (fgets(buffer_rez, MAX_LEN, rez_p)) {
        buffer_rez[strcspn(buffer_rez, "\n")] = 0;
        adauga_rez(&rez[ind_rez++], buffer_rez);
    }

    fclose(rez_p);
    
    int admin = 0;
    char parola[] = "1234";

    while (true) {
        printf("Tastati 1 pentru admin sau 2 pentru utilizator : ");
        char optiune[100]; scanf("%s", &optiune);
        if (strlen(optiune) > 1 || (optiune[0] - '0' != 1 && optiune[0] - '0' != 2)) {
            printf("#####Optiune nevalida#####\n");
            continue;
        }
        admin = optiune[0] - '0';
        if (admin == 2) {
            admin = 0;
            break;
        }
        else {
            printf("Introdu parola: ");
            char prl[10]; scanf("%s", &prl);
            if (strlen(prl) != strlen(parola)) {
                printf("#####Parola incorecta#####\n");
                continue;
            }
            int bad = 0;
            for (int i = 0; i < strlen(prl); i++) {
                if (prl[i] != parola[i]) {
                    printf("#####Parola incorecta#####\n");
                    bad = 1;
                    break;
                }
            }
            if (bad)
                continue;
            else {
                admin = 1;
                break;
            }
        }
    }

    if (admin) {
        system("cls");
        while (true) {
            printf("Meniu admin:\n");
            printf("1 - Vizualizati sali\n");
            printf("2 - Adaugarea salii dupa capacitate si nume\n");
            printf("3 - Stergerea salii dupa nume\n");
            printf("4 - Afisarea tuturor salilor si rezervarilor\n");
            printf("0 - Iesire\n\n");
            printf("*Tastati oricand ctrl+c pentru iesire\n\n");

            printf("Tastati optiunea:");
            int option; scanf("%d", &option);
            while (option > 4 || option < 0) {
                printf("#####Optiune nevalida#####\n");
                printf("Tastati optiunea din nou:");
                scanf("%d", &option);
            }

            if (option == 1) {
                printf("Salile disponibile sunt: \n\n");

                afisare_tot(all, ind);
                printf("\n");

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 2) {
                printf("Tastati numele salii: ");
                char nume[31];
                scanf("%s", &nume);
                printf("Tastati capacitatea salii: ");
                int capacitate;
                scanf("%d", &capacitate);
                printf("Ati adaugat sala cu succes\n");

                //adaugare sala
                FILE* fptr;

                fptr = fopen("in.txt", "a");

                if (fptr == NULL) {
                    printf("Error!");
                    exit(1);
                }

                fprintf(fptr, "\n%s %d", nume, capacitate);
                strcpy(all[ind].nume, nume);
                all[ind++].capacitate = capacitate;

                fclose(fptr);

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 3) {
                printf("Tastati numele salii care doriti sa o stergeti: ");
                char nume[31];
                scanf("%s", &nume);
                printf("Ati sters sala cu succes\n");

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 4) {
                printf("Salile sunt: \n\n");
                afisare_nume(all, ind);
                printf("Rezervarile sunt: \n\n");
                afisare_rez(rez, ind_rez);

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (!option)
                break;
        }
    }
    else {
        system("cls");
        while (true) {
            printf("Optiuni:\n");
            printf("1 - Vizualizati sali disponibile\n");
            printf("2 - Rezervati o sala\n");
            printf("3 - Anulati rezervarea\n");
            printf("4 - Cautare dupa nume sau capacitate\n");
            printf("0 - Iesire\n\n");
            printf("*Tastati oricand ctrl+c pentru iesire\n\n");

            printf("Tastati optiunea:");
            int option; scanf("%d", &option);
            while (option > 4 || option < 0) {
                printf("#####Optiune nevalida#####\n");
                printf("Tastati optiunea din nou:");
                scanf("%d", &option);
            }

            if (option == 1) {
                printf("Salile disponibile sunt: \n\n");

                afisare_tot(all, ind);
                printf("\n");

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 2) {
     
                printf("Salile disponibile pentru rezervare sunt: \n");
                afisare_tot(all, ind);

                printf("Tastati numarul salii care doriti sa o rezervati: ");
                int sala; scanf("%d", &sala);
                while (sala < 1 || sala > ind) {
                    printf("###Sala inexistenta###\n");
                    printf("Tastati numarul salii care doriti sa o rezervati: ");
                    scanf("%d", &sala);
                }

                printf("Date si ore indisponibile pentru aceasta sala: \n\n");
                for (int i = 0; i < ind_rez; i++) {
                    int ok = 1;
                    if (strlen(rez[i].sala_rez) != strlen(all[sala - 1].nume))
                        continue;
                    for (int j = 0; j < strlen(rez[i].sala_rez); j++)
                        if (rez[i].sala_rez[j] != all[sala - 1].nume[j])
                            ok = 0;
                    if (ok)
                        printf("orele %d-%d pe data %s\n", rez[i].ora1, rez[i].ora2, rez[i].data);
                }
                printf("\n");

                printf("Tastati ziua lunii: ");
                int ziua; scanf("%d", &ziua);
                while (ziua < 1 || ziua > 31) {
                    printf("###Zi nevalida###\n");
                    printf("Tastati ziua lunii: ");
                    scanf("%d", &ziua);
                }

                printf("Tastati luna: ");
                int luna; scanf("%d", &luna);
                while (luna < 1 || luna > 12) {
                    printf("###Luna nevalida###\n");
                    printf("Tastati luna: ");
                    scanf("%d", &luna);
                }

                printf("Tastati anul: ");
                int anul; scanf("%d", &anul);
                while (anul != 2023) {
                    printf("###An nevalid###\n");
                    printf("Tastati anul: ");
                    scanf("%d", &anul);
                }

                printf("Tastati ora de inceput: ");
                int ora1, ora2; scanf("%d", &ora1);
                while (ora1 > 24 || ora1 < 1) {
                    printf("###Ora incorecta###\n");
                    printf("Tastati ora de inceput: ");
                    scanf("%d", &ora1);
                }
                printf("Tastati ora de sfarsit: ");
                scanf("%d", &ora2);
                while (ora2 > 24 || ora2 < 1 || ora2 < ora1) {
                    printf("###Ora incorecta###\n");
                    printf("Tastati ora de sfarsit: ");
                    scanf("%d", &ora2);
                }
                printf("Introduceti numele dvs: ");
                char nume[31]; scanf("%s", nume);
                // scriere fisier rezervari
                FILE* fptr;

                fptr = fopen("rezervari.txt", "a");

                if (fptr == NULL){
                    printf("Error!");
                    exit(1);
                }
                char data_rez[30];
                int data_ind = 0;
                if (ziua > 9) {
                    data_rez[data_ind++] = ziua / 10 % 10 + '0';
                    data_rez[data_ind++] = ziua % 10 + '0';
                }
                else
                    data_rez[data_ind++] = ziua + '0';

                data_rez[data_ind++] = '.';

                if (luna > 9) {
                    data_rez[data_ind++] = luna / 10 % 10 + '0';
                    data_rez[data_ind++] = luna % 10 + '0';
                }
                else
                    data_rez[data_ind++] = luna + '0';

                data_rez[data_ind++] = '.';
                data_rez[data_ind++] = anul / 1000 % 10 + '0';
                data_rez[data_ind++] = anul / 100 % 10 + '0';
                data_rez[data_ind++] = anul / 10 % 10 + '0';
                data_rez[data_ind++] = anul % 10 + '0';
                data_rez[data_ind++] = '\0';

                fprintf(fptr, "\n%s %s %d %d %s", nume, all[sala - 1].nume, ora1, ora2, data_rez);
                fclose(fptr);

                strcpy(rez[ind_rez].nume, nume);
                strcpy(rez[ind_rez].sala_rez, all[sala - 1].nume);
                rez[ind_rez].ora1 = ora1;
                rez[ind_rez].ora2 = ora2;
                strcpy(rez[ind_rez++].data, data_rez);

                // sfarsit scriere rezervari


                printf("Ati rezervat cu succes\n");
                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 3) {
                printf("Tastati numele dvs: ");
                char nume[31];
                scanf("%s", &nume);
                printf("Rezervarile dvs sunt:\n");
                printf("Tastati nr rezervarii ce doriti sa o anulati: \n");
                int rez; scanf("%d", &rez);
                printf("ati anulat rezervarea cu succes\n");

                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (option == 4) {
                printf("Dupa ce doriti sa cautati ?\n1 - Nume\n2 - Capacitate\n");
                int suboption; scanf("%d", &suboption);
                while (suboption != 1 && suboption != 2) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &suboption);
                }
                if (suboption == 1) {
                    printf("Tastati numele: ");
                    char nume[31]; scanf("%s", &nume);
                }
                if (suboption == 2) {
                    printf("Tastati capacitatea minima: ");
                    int capacitate; scanf("%d", &capacitate);
                }
                printf("Salile disponibile sunt:\n");
                printf("1 - back\n0 - iesire\n");
                printf("Tastati optiunea:");
                int exitoption; scanf("%d", &exitoption);
                while (exitoption != 1 && exitoption != 0) {
                    printf("#####Optiune nevalida#####\n");
                    printf("Tastati optiunea:");
                    scanf("%d", &exitoption);
                }
                if (exitoption == 1) {
                    system("cls");
                    continue;
                }
                else
                    break;
            }
            if (!option)
                break;
        }
    }

    return 0;
}
