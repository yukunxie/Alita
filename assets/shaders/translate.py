import os


fd = open("shader.frag.spv", "rb")

chars = []
count = 0
for k in fd.read():
    count = count + 1
    chars.append("0x%X" % ord(k))

print ("const int shader_frag_length = %d;" % count )
data = "const char data [shader_frag_length] = [" + ", ".join(chars) + "];"

print (data)



fd = open("shader.vert.spv", "rb")

chars = []
count = 0
for k in fd.read():
    count = count + 1
    chars.append("0x%X" % ord(k))

print ("const int shader_vert_length = %d;" % count )
data = "const char data [shader_vert_length] = [" + ", ".join(chars) + "];"

print (data)