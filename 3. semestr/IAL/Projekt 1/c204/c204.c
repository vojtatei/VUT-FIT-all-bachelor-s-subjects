/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char c;

	while ( !Stack_IsEmpty(stack) && (Stack_Top(stack, &c), c) != '(')
	{
		postfixExpression[(*postfixExpressionLength)++] = c;
		Stack_Pop(stack);
	}
	
	Stack_Pop(stack);
	//solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	//solved = false; /* V případě řešení, smažte tento řádek! */

	char top;
	if (!Stack_IsEmpty(stack))
	{
		Stack_Top(stack, &top);
	}

	if (Stack_IsEmpty(stack) || (top == '(') || ((top == '+' || top == '-') && (c == '*' || c == '/')))
	{
		Stack_Push(stack, c);
	}
	else
	{
			postfixExpression[(*postfixExpressionLength)] = top;
			Stack_Pop(stack);
			(*postfixExpressionLength)++;
			doOperation(stack, c, postfixExpression, postfixExpressionLength);
	}
	//solved = false; /* V případě řešení, smažte tento řádek! */)

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	//solved = false; /* V případě řešení, smažte tento řádek! */
	
	char *postfixExpression = (char *)malloc(MAX_LEN * sizeof(char));

    if (postfixExpression == NULL || infixExpression == NULL) {
        return NULL;  // Chyba při alokaci paměti
    }

	unsigned i = 0; //Index pro vstupní infixový výraz
	unsigned j = 0; //Index pro výsledný postfixový výraz

	Stack stack;
	Stack_Init(&stack);

	while(0 < 1)
	{
		char c = infixExpression[i];

		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			postfixExpression[j] = c;
			j++;
		}
		else if (c == '(')
		{
			Stack_Push(&stack, c);
		}
		else if (c == ')')
		{
            // Funkce untilLeftPar pro zpracování uzávorkovaného výrazu
			untilLeftPar(&stack, postfixExpression, &j);
		}
		else if (c == '=')
		{
            // Funkce untilLeftPar pro zpracování zbývajících operátorů
			untilLeftPar(&stack, postfixExpression, &j);
			break;
		}
		else
		{
            // Funkce doOperation pro zpracování operátorů a jejich priorit
			doOperation(&stack, c, postfixExpression, &j);
		}

		i++;
	}


    Stack_Dispose(&stack);

	postfixExpression[j] = '=';
	postfixExpression[j+1] = '\0';
	return postfixExpression;
}



/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

    char bytes[4];
    bytes[3] = (char)(value & 0xFF);
    bytes[2] = (char)((value >> 8) & 0xFF);
    bytes[1] = (char)((value >> 16) & 0xFF);
    bytes[0] = (char)((value >> 24) & 0xFF);

    // Postupné vložení hodnoty na zásobník
    Stack_Push(stack, bytes[3]);
    Stack_Push(stack, bytes[2]);
    Stack_Push(stack, bytes[1]);
    Stack_Push(stack, bytes[0]);
	
    //solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	
    *value = 0;

    // Postupné extrakce a složení hodnoty ze zásobníku
    for (int i = 0; i < 4; i++) 
    {
        char byte;
        Stack_Top(stack, &byte);
        Stack_Pop(stack);
        // Přetypování znaku na byte a uložení do hodnoty
        *value = (*value << 8) | (unsigned char)byte;
    }
    
    //solved = false; /* V případě řešení, smažte tento řádek! */
}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */


bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
    // Převedení infixového výrazu na postfixový
    char *postfixExpression = infix2postfix(infixExpression);

    if (postfixExpression == NULL) 
    {
        return false; // Chyba při převodu
    }

    // Inicializace zásobníku pro vyhodnocení postfixového výrazu
    Stack operand_stack;
    Stack_Init(&operand_stack);

    unsigned i = 0;

    // Vyhodnocení postfixového výrazu
    while (postfixExpression[i] != '=') 
    {
        char c = postfixExpression[i];
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) 
        {
            // Hodnota nebo proměnná, vložit na zásobník
            int num;
            if (c >= '0' && c <= '9') 
            {
                num = c - '0'; // Převod znaku na číslo
            } else 
            {
                // Najít hodnotu proměnné v poli variableValues
                bool found = false;

                for (int j = 0; j < variableValueCount; j++) 
                {
                    if (variableValues[j].name == c) 
                    {
                        num = variableValues[j].value;
                        found = true;
                        break;
                    }
                }
                if (!found) 
                {
                    // Proměnná nenalezena
                    free(postfixExpression);
                    return false;
                }
            }
            // Vložení hodnoty na zásobník
            expr_value_push(&operand_stack, num);
        } else 
        {
            // Operátor, provést operaci
            int operand2, operand1;
            expr_value_pop(&operand_stack, &operand2);
            expr_value_pop(&operand_stack, &operand1);

            switch (c) 
            {
                case '+':
                    expr_value_push(&operand_stack, operand1 + operand2);
                    break;
                case '-':
                    expr_value_push(&operand_stack, operand1 - operand2);
                    break;
                case '*':
                    expr_value_push(&operand_stack, operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) 
                    {
                        // Chyba: Dělení nulou
                        free(postfixExpression);
                        return false;
                    }
                    expr_value_push(&operand_stack, operand1 / operand2);
                    break;
                default:
                    // Neznámý operátor
                    free(postfixExpression);
                    return false;
            }
        }
        i++;
    }

    // Získání výsledné hodnoty z vrcholu zásobníku
    expr_value_pop(&operand_stack, value);

    // Uvolnění paměti
    free(postfixExpression);

    // Zkontrolovat, zda zásobník operand_stack je prázdný (měl by být)
    if (!Stack_IsEmpty(&operand_stack)) 
    {
        return false; // Chyba - zásobník není prázdný
    }

    Stack_Dispose(&operand_stack);
    return true;
}
/* Konec c204.c */
