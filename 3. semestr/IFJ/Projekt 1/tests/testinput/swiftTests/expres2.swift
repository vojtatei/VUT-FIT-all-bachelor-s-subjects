var retI: Int
var retD: Double
var retS: String

let vS = "Monika"
let xS = "Doubi"
let yS = "Vojta"
let zS = "Kubi"

retS = vS+""
write("retS je: ", retS, "\n")
retS = zS+" "
write("retS je: ", retS, "\n")
retS = xS+" . "
write("retS je: ", retS, "\n")
retS = yS+"\n"
write("retS je: ", retS, "\n")

if vS > xS
{
    write("vacsia je ", vS, "\n")
}
else
{
    write("vacsia je ", xS, "\n")
}

if vS<xS{write("vacsia je ",vS,"\n")}
else{write("vacsia je ",xS,"\n")}

if vS==xS{write("rovnaju sa ",vS," a ",xS,"\n")}
else{write("nerovnaju sa ",vS," a ",xS,"\n")}

if yS==yS{write("rovnaju sa ",yS," a ",yS,"\n")}
else{write("nerovnaju sa ",yS," a ",yS,"\n")}

if yS != yS{write("nerovnaju sa ",yS," a ",yS,"\n")}
else{write("rovnaju sa ",yS," a ",yS,"\n")}

