import os
import hashlib
import sys, getopt

def getFileType(f):
    ending = f.rsplit('.', 1)[1].lower()
    if ending == 'obj':
        return ('OBJ', 'M')
    elif ending == 'png':
        return ('PNG', 'T')
    else:
        return ('UNK', 'U')



def main(argv):
    input_path = './example'
    output_path = 'res.custom_package'
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
    except getopt.GetoptError:
          print ('main.py -i <inputfile> -o <outputfile>')
          sys.exit(2)

    for opt, arg in opts:
          if opt == '-h':
             print ('main.py -i <inputfile> -o <outputfile>')
             sys.exit()
          elif opt in ("-i", "--ifile"):
             input_path = arg
          elif opt in ("-o", "--ofile"):
             output_path = arg

    with open(output_path, 'wb') as output_file:

        arr = os.listdir(input_path)
        offset = 0
        output_file.write(bytes(str(len(arr)) + '\n', 'utf-8'))
        for f in arr:

            p = os.path.join(input_path, f)
            if os.path.isfile(p):
                size = os.path.getsize(p)
                asset_type = getFileType(f)
                output_file.write(bytes(p + ':' + str(abs(hash(p)))[0:9] + ':' + asset_type[0] + ':' + asset_type[1] + ':' + str(size) + ':' + str(offset) + '\n', 'utf-8'))
                offset += size

        for f in arr:
            with open(os.path.join(input_path, f), 'rb') as read_file:
                data = read_file.read()
                output_file.write(data)

if __name__ == "__main__":
   main(sys.argv[1:])
