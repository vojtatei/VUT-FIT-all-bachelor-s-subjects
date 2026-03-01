func fibonacciPo(limit limitek: Int) {
    var predchadzajuce = 0
    var aktualne = 1

    write("Fibonacciho postupnost do ",limitek,":\n")
    write(predchadzajuce)

    // createframe
    var sucet = predchadzajuce + aktualne
    while aktualne <= limitek {
        write(aktualne)
        sucet = predchadzajuce + aktualne
        predchadzajuce = aktualne
        aktualne = sucet
    }
}

// Volanie funkcie
fibonacciPo(limit: 20)
write("\n")
