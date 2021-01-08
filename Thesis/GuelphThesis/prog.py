import os 
import sys 






if __name__ == "__main__":


    try:
        f = open("MinLadders.txt", "r")
        w = open("TestFile.txt", "w")
        s = f.read()
        lines = s.split("\n")
        for i in range(0, len(lines)-2, 4):
            print(i)
            s = lines[i] + lines[i+1] + lines[i+2]
            w.write(s)
            s = ""
            w.write("\n")
            i += 3
    except:
        print("An Error occurred")
    finally:
        f.close()
        w.close()
