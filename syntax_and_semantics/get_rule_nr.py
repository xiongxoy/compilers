#!/usr/bin/env python

def main():
    f_in = file("temp.txt")
    rules = []
    while True:
        line = f_in.readline()
	rules.append(line)
        if line.split() == []:
	    break

    f_in.close()
    f_in = file("used_rule.txt")
    li = []
    while True:
        line = f_in.readline()
	tokens = line.split()
	if len(tokens) < 4:
	    break
	print tokens
	li.append(int(tokens[3]))
    #print li
    li = set(li)
    li = list(li)
    li.sort()
    print li
    print len(li)
    for i in li:
	print "//"+rules[i] +"if( "+str(i)+" == act_num ) { \n}\n"

if __name__ == "__main__":
    main()
