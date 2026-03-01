func concat(_ z : String, with y : String) -> String {
    let x = z + y
    return z + " " + y
}
let a = "ahoj "
var ct : String
ct = concat(a, with: "svete")
write(ct, a, "\n")


//funkcia bez zoznamu parametrov
func helloWorld() -> String {
    return "Hello, World!"
}
let message = helloWorld()
write(message, "\n")


//prazdna funkcia
func prazdnaFunkcia(parameter1 ahoj: Int, parameter2 _: String) {

}
prazdnaFunkcia(parameter1: 42, parameter2: "Hello")




