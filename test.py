

x = {}
y = {
    "x" : x 
}

print(id(x))
x[1] = 2
print(id(x))
print(id(y))
print(id(y["x"]))

