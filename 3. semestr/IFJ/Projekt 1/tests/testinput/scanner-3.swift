func scitaj(_ cislo1: Int, _ cislo2: Int) -> Int {
    write("vykonava sa funkcia scitaj(", cislo1,",", cislo2,")\n")
    let sucet : Int
    sucet = cislo1 + cislo2
    return sucet
}

// Použitie funkcie
let vysledok = scitaj(5, 3)
write("Vysledok scitania: ", vysledok, "\n")
var VYSLEDOK = scitaj(3, 5)
write("Výsledok sčítania: ", VYSLEDOK, "\n")

if vysledok != VYSLEDOK
{
    write("vysledok == VYSLEDOK a sme vo vetve if\n")

    let prvyRetazec = "Ahoj, "
    write("prvyRetazec: Ahoj, \n")
    let druhyRetazec = "svet!"
    write("druhyRetazec: svet! \n")
    let spojenyVysledok = spoj(prvyRetazec, druhyRetazec)
    write("funkcia spoj vracia vysledok spojenyVysledok: ", spojenyVysledok, "\n")
}
else
{
    write("vysledok != VYSLEDOK a sme vo vetve else\n")
}

write("\n")

VYSLEDOK = scitaj(5, 5)
write("VYSLEDOK sčítania: ", VYSLEDOK, "\n")

if vysledok == VYSLEDOK
{
    write("vysledok == VYSLEDOK a sme vo vetve if\n")
}
else
{
    write("vysledok != VYSLEDOK a sme vo vetve else\n")

}

func spoj(_ retazec1: String, _ retazec2: String) -> String {
     write("vykonava sa funkcia spoj(", retazec1,",", retazec2,")\n")
    let spojenyRetazec = retazec1 + retazec2
    return spojenyRetazec
}

var haha = spoj("su"," vianoce")
write(haha,"\n")

// volanie return funkcie bez priradenia vysledku
spoj("neulozi","vysledok")

var sucet=10+5
vyhodnot(blabla: 5, sucet)
sucet = vyhodnot(blabla: 5, sucet)
write("sucet je: ",sucet,"\n")


func vyhodnot(blabla cislo1: Int, _ cislo2: Int) -> Int {
    write("vykonava sa funkcia vyhodnot(with ", cislo1,",", cislo2,")\n")
    return (cislo1+cislo2)*2
}
