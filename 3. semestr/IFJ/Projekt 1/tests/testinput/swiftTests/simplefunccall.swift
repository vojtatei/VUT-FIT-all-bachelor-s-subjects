
var x : Int?
write(x)

var y : Int?

write(y)

write("\n\n\n")


func scitaj(_ cislo1: Int, _ cislo2: Int) -> Int {
    write("vykonava sa funkcia scitaj(", cislo1,",", cislo2,")\n")
    let sucet : Int
    sucet = cislo1 + cislo2
    return sucet
}


x = scitaj(5, 3)

write(x)
