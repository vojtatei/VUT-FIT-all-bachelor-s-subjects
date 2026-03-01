var name: String?
name = "Fero"
let num: Int = 100

if name == "Fero" {
    name = "PEPIK"
} else {
    name = "HONZIK"
}
write("num is ", num, "\n")

var x = 2.2
var y = 3.33
var z = 5.0
var sum: Double

let i = 1

if i == 1 {
    sum = x + y + z
    write("sum is ", sum, "\n")
}
else {

}

let greeting = "Hello, " + (name ?? "friend") + "!\n"
write(greeting)
// Prints "Hello, friend!"


