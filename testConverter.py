
with open("test_in_converted", "w") as output_file:
    output_file.write("#include \"library2.h\"\n")
    output_file.write("int main() {\n")
    with open("test.in.txt", "r") as file_1:
        for line in file_1:
            line = line.split()
            if len(line) > 1:
                args_array = line[1:]
                args = ",".join(args_array)
                code_line = line[0] + "(" + args + ");"
            else:
                code_line = line[0] + "();"
            output_file.write(" " + code_line + "\n")
    output_file.write("}")

file_1.close()
output_file.close()
