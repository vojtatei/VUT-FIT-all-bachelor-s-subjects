// DONT EDIT THIS FILE, IT IS USED FOR TESTING
let    var sranda = 10.
func fa#ctorial(_ n : Int) -> Int {
    var result : Int?
    if (n < 2) {
        result = 1

    } else {
        let decremented#_n = decrement(of: n, by: 1)
        let temp_result = factorial(decremented_n)
        result = n * temp_result
    }
    return result!
    

    
}