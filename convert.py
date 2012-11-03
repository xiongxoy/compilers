#!/usr/bin/env python

def main():
    f_in = file('haneda_grammar.y')
    f_out = file('haneda_grammar.y.new', 'w')
    state = 0
    head = None
    while True:
	line = f_in.readline()
   	if 0 == state: 
	    if line.startswith("%%"):
		state = 1
		continue
	if 1 == state: 
	    if line.startswith("%%"):
		break
	    if line.split() == []:
		continue
	    else:
		head = line.split()[0]
		state = 2
		continue
	if 2 == state:
	    if line.split() == []:
		state = 1
		continue
            if line.split()[0] == ';':
		state = 1
		continue
	    else:
		tokens = line.split()
		#print 'head is ', head
		#print "tokens are ", tokens
		print '"'+head+':',
		for i in xrange(len(tokens)-1):
		    if tokens[i+1][0] == '\'':
			print tokens[i+1][1],
		    else:
		    	print tokens[i+1],
 		print '",',
		print 
    f_in.close()
    f_out.close()
    print 'hello',

if __name__ == "__main__":
    main()
