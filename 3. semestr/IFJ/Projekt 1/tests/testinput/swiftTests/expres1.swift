var retI: Int
var retD: Double
var retS: String

let xI = 10
let yI = 678
let zI = 199

let vD = 10e+2
let xD = 3.89
let yD = 100.11111
let zD = 5E-2

let vS = "Monika"
let xS = "Doubi"
let yS = "Vojta"
let zS = "Kubi"

// TEST +
retI = xI + yI + zI
write("retI je: ", retI, "\n")
retD = vD + xD + yD + zD
write("retD je: ", retD, "\n")
retS = vS + xS + yS + zS
write("retS je: ", retS, "\n")

write("\n")

//TEST INT DOUBLE -
retI=xI-yI-zI
write("retI je: ", retI, "\n")
retD=vD-xD-zD-yD
write("retD je: ", retD, "\n")

write("\n")

//TEST INT DOUBLE * / ()
retI=xI*yI/zI*zI
write("retI je: ", retI, "\n")
retD=vD*xD/zD*yD*yD
write("retD je: ", retD, "\n")
write("\n")
retI=(xI*yI*zI*zI)/98
write("retI je: ", retI, "\n")
retD=(vD)/(5.55)/zD*yD*yD*2.908
write("retD je: ", retD, "\n")
write("\n")

//TEST INT DOUBLE * / + - ()
retI=9/xI-xI*yI*zI-134+xI*2-zI*zI
write("retI je: ", retI, "\n")
retD=zD-987-12*xD-12+yD/vD*vD+zD/2
write("retD je: ", retD, "\n")
write("\n")
retI=(9/xI)-(yI*zI-134)*(xI+2)-(zI)
write("retI je: ", retI, "\n")
retD=zD-987-(12*xD)-12+(yD/vD*vD+zD)/2
write("retD je: ", retD, "\n")
write("\n")

//TEST vela tokenov
retI = 9+2-1-10*4/1-6*6*1/4-0-1+6*2*1
write("retI je: ", retI, "\n")
retI = (xI+yI-zI-xI)*xI/(zI-xI)*xI*xI/yI-yI-(zI+zI*yI)
write("retI je: ", retI, "\n")

