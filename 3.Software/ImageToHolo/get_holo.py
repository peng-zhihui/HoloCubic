import os, sys
from convertor.core import Convertor

if __name__ == '__main__':
    if len(sys.argv) == 2:
        INPUT_FILE = sys.argv[1]
    else:
        print("usage:\n\t python " + sys.argv[0] + " input_file.jpg")
        sys.exit(0)

    c = Convertor(INPUT_FILE)

    c.get_bin_file()
    c.get_c_code_file()
