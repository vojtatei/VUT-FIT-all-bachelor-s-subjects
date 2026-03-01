let i1: Int? = 10
let i2: Int? = nil
let d1: Double? = nil
let d2: Double? = nil

if let i1   //nie je nil
{ write("if: i1 je ", i1, "\n")}
else
{ write("else:  je ", i1, "\n")}

if let i2   //nil
{ write("if: i1 je ", i2, "\n")}
else
{ write("else: i1 je ", i2, "\n")}

if (d1==d2)
{ write("if: ",d1, " sa rovna ", d2, "\n")}
else
{ write("else: ",d1, " sa nerovna ", d2, "\n")}

if i1 == i2
{ write("if: ",i1, " sa rovna ", i2, "\n")}
else
{ write("else: ",i1, " sa nerovna ", i2, "\n")}