#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAX 101
//Definuji maximalni hodnotu

char hledani_cisla(char cislo_napsane[], char cislo_soubor[MAX]);
char hledani_jmena(char cislo_napsane[], char jmeno_soubor[MAX]);
void jeden_radek(char jmeno_soubor [], char cislo_soubor []);
//Deklaruji funkce


void jeden_radek(char jmeno_soubor [], char cislo_soubor [])
{
    jmeno_soubor[strcspn(jmeno_soubor, "\n")] = 0; 
    cislo_soubor[strcspn(cislo_soubor, "\n")] = 0;
    //Davam oba retezce na jeden radek
}

char hledani_jmena(char cislo_napsane[], char jmeno_soubor[MAX])
{
int cislo_n = strlen(cislo_napsane);
int jmeno_s = strlen(jmeno_soubor);
//Merim delku retezcu
for (int i = 0; i < jmeno_s; i++)
{
    if (jmeno_soubor[i] >= 'A' && jmeno_soubor[i] <= 'Z')
    {
        jmeno_soubor[i] = jmeno_soubor[i] + 32;
    }
    //Menim velka pismena na mala
}
int hledani_shody = 0;
char x[MAX];
int j = 0;
for (int i = 0; i < jmeno_s; i++)
{
    if(jmeno_soubor[i] >= 'a' && jmeno_soubor[i] <= 'c')
    {
        x[j] = '2';
        j++;
    }
    else if (jmeno_soubor[i] >= 'd' && jmeno_soubor[i] <= 'f')
    {
        x[j] = '3';
        j++;
    }
    else if (jmeno_soubor[i] >= 'g' && jmeno_soubor[i] <= 'i')
    {
        x[j] = '4';
        j++;
    }
    else if (jmeno_soubor[i] >= 'j' && jmeno_soubor[i] <= 'l')
    {
        x[j] = '5';
        j++;
    }
    else if (jmeno_soubor[i] >= 'm' && jmeno_soubor[i] <= 'o')
    {
        x[j] = '6';
        j++;
    }
    else if (jmeno_soubor[i] >= 'p' && jmeno_soubor[i] <= 's')
    {
        x[j] = '7';
        j++;
    }
    else if (jmeno_soubor[i] >= 't' && jmeno_soubor[i] <= 'v')
    {
        x[j] = '8';
        j++;
    }
    else if (jmeno_soubor[i] >= 'w' && jmeno_soubor[i] <= 'z')
    {
        x[j] = '9';
        j++;
    }
    //Prohledavam pole v souboru a pokud zde plati rovnost, tak
    //zapisuji honotu do noveho vytvoreneho pole prvku
}
//Projizdim oba retezce, pokud se splni podminka, pridam 1 do hledani shody,
//pokud se hledani shody shoduje s cislem co jsem napsal, tak hodnota plati
//a string se printne
for (int j = 0; j < jmeno_s; j++)
{
    for(int i = 0; i < cislo_n; i++)
    {
        if (cislo_napsane[0] == x[j] && cislo_n < 2)
    {
        hledani_shody++;
    }
    else if (cislo_napsane[i] == x[j] && cislo_n > 1)
    {
        hledani_shody++;
        j++;
    }
    else
    {
        hledani_shody = 0;
    }
    if (hledani_shody == cislo_n)
    {
        return 0;
    }
    }
}
return 1;
}

char hledani_cisla(char cislo_napsane[], char cislo_soubor[MAX])
{
int cislo_n = strlen(cislo_napsane);
int cislo_s = strlen(cislo_soubor);
//Zjistuji delku cisla, ktere jsem napsal jako argument a cisla, ktere je v souboru

//To same jak u jmen, hledam shody ve 2 retezcich, pokud se shoduje, tak se zvyssi
//hledani shody a pokud se hledani shody rovna s delkou zadaneho retezce, tak se
//dany retezec printne
int hledani_shody = 0;
for (int i = 0; i < cislo_n; i++)
{
    for (int j = 0;j < cislo_s && i < cislo_n; j++)
    {
    if (cislo_napsane[0] == '0' && cislo_soubor[0] == '+')
    {
        hledani_shody++;
        i++;
    }
    else if (cislo_napsane[0] == cislo_soubor[j] && cislo_n < 2)
    {
        hledani_shody++;
        i++;
    }
    else if(cislo_napsane[i] == cislo_soubor[j] && cislo_n > 1)
    {
        hledani_shody++;
        i++;
    }
    }

    if (hledani_shody == cislo_n)
    {
    return 0;
    }
}
return 1;
}

int main(int argc, char *argv[])
{
char cislo_soubor[MAX];
char jmeno_soubor[MAX];
int pocitadlo = 0;
//cyklus pokdu je pocet argumentu 2


if (argc == 2)
{
    while (fgets(jmeno_soubor, MAX, stdin)  && fgets(cislo_soubor,MAX, stdin))
{
    jeden_radek(jmeno_soubor, cislo_soubor);
    hledani_jmena(argv[1], jmeno_soubor);
    hledani_cisla(argv[1], cislo_soubor);
    if ((hledani_jmena(argv[1], jmeno_soubor) == 0) || hledani_cisla(argv[1], cislo_soubor) == 0)
    {
        printf("%s, %s\n", jmeno_soubor, cislo_soubor);
        pocitadlo++;
        //Mam pocitadlo, ktere pocita pocet radku
    }
}
//Pokud je pocet radku, ktere splnuji podminku 0, tak program vytiskne not found na stderr
if (pocitadlo == 0)
    {
    fprintf(stderr, "Not found\n");
    return 1;
    }
}
if (argc == 1)
//Pokud je pocet argumentu 1, tak printnu vsechny kontakty
{
    while(fgets(jmeno_soubor, 101, stdin) && fgets(cislo_soubor,101, stdin))
    {
        jeden_radek(jmeno_soubor, cislo_soubor);
        printf("%s, %s\n", jmeno_soubor, cislo_soubor);
    }
}
if (argc >= 3)
//Pokud je pocet argumentu vetsi nez 2, tak na stderr se printnne chyba
{
    fprintf(stderr ,"Prilis mnoho argumentu\n");
    return 1;
}
if (argc == 0)
//Pokud je pocet argumentu vetsi nez 2, tak na stderr se printnne chyba
{
    fprintf(stderr ,"Prilis malo argumentu\n");
    return 1;
}
return 0;
}