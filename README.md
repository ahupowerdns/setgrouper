# setgrouper
lists for n files which lines are part of which files from those n. Also
groups this output so you get list of all lines part of file one but not
file two etc.

As an example, say you have email addresses which appear on three lists, and
you want to find out who is on the first and second lists, but not the
third, this is your tool.

Easily scales to millions of lines.

This tool has previously been described in the blogpost [I'm a C++ dinosaur
but I'm
ok](http://bert-hubert.blogspot.nl/2012/10/im-c-dinosaur-but-im-ok.html).
Since C++ 2011 and 2014, I no longer feel like a dinosaur using C++ though!

# Example

Three files, 'een':
```
aap
noot
mies
wim
```

'twee':
```
wim
zus 
jet
gys
```

'drie':
```
aap
NOOT
mies
wim
zus
jet
gys
```

Output:
```
$ grouper -i een twee drie
	een	twee	drie	
aap	1	0	1	
gys	0	1	1	
jet	0	1	1	
mies	1	0	1	
noot	1	0	1	
wim	1	1	1	
zus	0	1	1	

Per group output	

Group (size=3): 	1	0	1	
aap	
mies	
noot	

Group (size=3): 	0	1	1	
gys	
jet	
zus	

Group (size=1): 	1	1	1	
wim	
```
