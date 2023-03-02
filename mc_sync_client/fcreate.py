long_size = 2147483647

def write_char():
    f = open("long_max.txt", "w")
    f.write(str('C'*200000000))
    f.close()

def write_size_file(size:int, filepath:str):
    f = open(filepath,"wb")   # Erstelle long_max große Datei
    f.seek(size-1)
    f.write(b"\0")
    f.close()

def append_size_file(size:int, filepath:str):
    f = open(filepath,"a")   # Erstelle long_max große Datei
    for i in range(size):
        f.write(b"\0")
    f.close()

write_size_file(long_size + 1, 'max.txt')
#write_char()
#write_size_file(1, 'long_max.txt')