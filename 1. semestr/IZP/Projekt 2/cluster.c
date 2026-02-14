/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

        c->size = 0;

    //pokud je pozadovana kapacita shluku 0, tak mohu shluk
    //inicializovat na prazdny
    if (cap == 0)
    {
        c->obj = NULL;
        c->capacity = 0;
    }
    //alokuji pamet pro pole objektu
    else
    {
        c->obj = malloc(sizeof(struct obj_t) * cap);
    }

    c->capacity = cap;


    return;

    // TODO

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    //pokud nejsou ve shluku zadne objekty nemusim uvolnovat pamet
    if (c->obj != NULL)
    free(c->obj);

    //inizializace na prazdny shluk
    c->size = 0;
    c->capacity = 0;
    c->obj = NULL;

    return;

    // TODO
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    //pokud je velikost stejna nebo vetsi nez kapacita, tak zavolam funkci
    //pro zmenu kapacity shluku
    if (c->size >= c->capacity)
    {
        //rozsiruji kapacitu shluku o doporucenou velikost
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }
    //pridam objekt na prvni volnou pozici
    c->obj[c->size] = obj;
    //zvyseni poctu ve shluku
    c->size++;

    return;


    // TODO
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    //pridani objektu shluku c2 do c1
    for (int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }
    //serazeni chluku c1
    sort_cluster(c1);



    // TODO
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&(carr[idx]));

    //posunuti shluku od idx doleva
    for (int i = idx; i < narr; i++)
    {
        carr[i] = carr[i+1];
    }

    return narr-1;

    // TODO
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf((o1->x - o2->x)*(o1->x - o2->x)+(o1->y - o2->y)*(o1->y - o2->y));

    // TODO
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float vzdalenost = 0.0;

    //pocitani vzdalenosti vsech objketu mezi shluky c1 a c2
    for(int i = 0; i < c1->size; i++)
    {
        for(int j = 0; j < c2->size; j++)
        {
            vzdalenost = obj_distance(&c1->obj[i], &c2->obj[j]);
        }
    }

    return vzdalenost;

    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    //pokud je v poli jen jeden shluk, tak uklada do pameti 0
    if (narr == 1)
    {
        *c1  = 0;
        *c2 = 0;
        return;
    }

    float vzdalenost;
    float min_vzdalenost = -1;

    //Projizdim polem shluku
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            //pocitam vzdalenost
            vzdalenost = cluster_distance(&carr[i], &carr[j]);
            //pokud je vzdalenost -1 tak jeste neni nastavena
            if (min_vzdalenost == -1 || vzdalenost < min_vzdalenost)
            {
                min_vzdalenost = vzdalenost;
                *c1 = i;
                *c2 = j;
            }
        }
    }

    // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    //nacitam ze souboru objekty
    FILE *objekty;

    objekty = fopen(filename, "r");


    // Otevreni souboru se nezdarilo
    if (objekty == NULL)
    {
        fprintf(stderr, "Soubor se nepodarilo otevrit\n");
        *arr = NULL;
        return -1;
    }

    // Prvni radek - 'count=<pocet>'
    char test1[7];
    char test2[20];
    int test3;
    int count = 0; // Pocet objektu ktere bude treba nacist
    int line = 1; // aktualni radek v souboru
    if (fgets(test1, 7, objekty) == NULL)
    {
        fprintf(stderr, "Soubor je prazdny\n");
        *arr = NULL;
        return -1;
    }

    if (fgets(test2, 20, objekty) == NULL)
    {
        fprintf(stderr, "Chyba pri cteni souboru, neni uvedena hodnota\n");
        *arr = NULL;
        return -1;
    }

    test3 = sscanf(test2, "%d", &count);

    if (test3 == EOF || test3 < 1)
    {
        fprintf(stderr, "Chybejici nebo chybna hodnota\n");
        *arr = NULL;
        return -1;
    }
    if (count <= 0)
    {
        fprintf(stderr, "Chyba, pocet objektu musi byt vic nez 0\n");
        *arr = NULL;
        return -1;
    }
    //kontrola dalsich radku
    for (int i = 0; i < count; i++)
    {
        line++;
        char test4[20];
        int test5;
        int id;
        int x;
        int y;
        if (fgets(test4, 20, objekty) == NULL)
        {
            fprintf(stderr, "Chyba pri cteni souboru, nedostatek radku\n");
            *arr = NULL;
            return -1;
        }
        test5 = sscanf(test4, "%d %d %d", &id, &x, &y);
        if (test5 == EOF || test5 < 3)
        {
            fprintf(stderr, "Chyba na radku %d, chybejici nebo chybne hodnoty\n", line);
            *arr = NULL;
            return -1;
        }
        if (x < 0 || x > 1000 || y < 0 || y > 1000)
        {
            fprintf(stderr, "Chyba na radku %d, hodnota je mimo rozsah\n", line);
            *arr = NULL;
            return -1;
        }
    }

    //vrátím ukazatel na zacatek souboru
    rewind(objekty);

    struct cluster_t *clusters = malloc(sizeof(struct cluster_t)*count);

    //preskocim prvni radek
    while (0 < 1)
    {
        int r = fgetc(objekty);
        if (r == '\n')
            break;
    }

    // Prochazime dalsi radky souboru a zapisuji hodnoty
    for (int i = 0; i < count; i++)
    {
        char line[20];
        int id;
        int x;
        int y;

        fgets(line, 20, objekty);
        sscanf(line, "%d %d %d", &id, &x, &y);
        struct obj_t obj;
        obj.id = id;
        obj.x = x;
        obj.y = y;
        struct cluster_t c;
        init_cluster(&c, 1);
        append_cluster(&c, obj);
        clusters[i] = c;
    }
    
    fclose(objekty);
    //ukazatel na prvni polozku pole
    *arr = &clusters[0];
    //vracim pocet
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters; // Pole s clustery
    int count; // Pocet shluku v poli
    int need; // Cilovy pocet shluku

    if (argc < 2)
    {
        fprintf(stderr, "Chybejici argumenty\n");
        return 1;
    }
    if (argc > 2)
    {
        int potrebny_pocet;
        potrebny_pocet = strtol(argv[2], NULL, 10); // kvuli preteceni
        need = potrebny_pocet;
    }
    else
    {
        need = 1;
    }

    //Nacteni clusteru ze souboru

    count = load_clusters(argv[1], &clusters);
    // Nacteni clusteru se nepovedlo
    if (count < need)
    {
        fprintf(stderr, "Pocet nactenych objektu je nedostacujici\n");
        return 1;
    }

    while(count != need)
    {
        int c1;
        int c2; // Obsahuje ID daneho shluku v poli shluku
        find_neighbours(clusters, count, &c1, &c2);
        merge_clusters(&(clusters[c1]), &(clusters[c2]));
        count = remove_cluster(clusters, count, c2);
    }

    //tisknu vysledek
    print_clusters(clusters, count);
    free(clusters);

    return 0;
}