let result = recursiveFunction(5)
write(result, "\n")

func recursiveFunction(_ counter: Int) -> String {
    if counter <= 0 {
        return "Recursion stopped at counter\n"
    } else {
        let currentCall = "Recursive call with counter\n"
        var y = counter - 1
        let nextCall = recursiveFunction(y)
        return currentCall + nextCall
    }
}

write("\ndalsia funkcia:\n\n")

// funkcie bez navratovej hodnoty
func functionA(_ value: Double) {
    if value <= 0 {
        write("Base case in functionA. Stopping recursion.\n")
        return
    }
    else {}

    write("FunctionA call with value ",value,"\n")
    var y = value - 0.5
    functionB(y)
}

func functionB(_ value: Double) {
    if value <= 0 {
        write("Base case in functionB. Stopping recursion.\n")
        return
    }
    else {}

    write("FunctionB call with value ",value,"\n")
    var x = value - 0.5
    functionA(x)
}

functionA(3.0)
