s = {}
with open("hoge_data.txt", "r") as f1:
    while True:
        data = f1.readline().split(" ")
        if data == ['']:
            break
        s[data[1]] = data[0]

with open("lib_data.txt", "r") as f2:
    while True:
        d2 = f2.readline().split(" ")
        if d2 == ['']:
            break
        if d2[1] in s:
            print(f"{s[d2[1]]}   {d2[0]}")
