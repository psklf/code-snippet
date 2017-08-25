#!usr/bin/python

import sys
import os
import re

def main():
    pattern = re.compile("\!\[\]\(http.*?psklf\/")

    for md in os.listdir(path):
        md_path = os.path.join(path, md)
        tmp_file_path = os.path.join(path, "tmp.txt")

        file_info = os.path.splitext(md_path)
        if file_info[1] != ".md":
            continue

        print "\nfile path ", md_path

        each_file = open(md_path, "r")
        tmp_file = open(tmp_file_path, 'w')
        for line in each_file:
            string = ""
            string += line
            # result = pattern.search(string)
            result = re.sub(pattern, "![](http://the-new-url", string)
            tmp_file.write(result)

        tmp_file.close()
        each_file.close()

        os.remove(md_path)

        os.rename(tmp_file_path, md_path)
            

if __name__ == "__main__":
    main()
