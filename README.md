asterisk-svn-to-git
===================
This repository contains tools that can be used in the
[Asterisk](http://www.asterisk.org) migration from subversion to git.


Tools
=====

log-check
---------

log-check and log-check.py are two implementations of the same program. The
python version was used for a quick reference and the C++ version was written in
order to allow this functionality to be moved into a C++ converter that will
speak the git fast-import protocol from a dump of the Asterisk repository.

log-check are intended to be used to process the asterisk log file and pull out
any potential authors. This will be used to look for git-style "John Doe
<jdoe@example.com>" identity information from contributors who did not have
Subversion commit access but would now like to be listed as the author on the
git commit.

An example of running the cpp and python version of log-check on a sample
forms.txt file.

~~~bash
$ ./log-check.py < forms.txt 
A A
B B
C C
dd
EE
ee
F F
f f
George George
hal
$ make log-check
g++ -Wall -Os --std=c++11 -o log-check log-check.cpp -lboost_regex
$ ./log-check < forms.txt 
A A
B B
C C
dd
EE
ee
F F
f f
George George
hal
~~~

Or on the last 250 commits of the trunk of Asterisk:

~~~bash
$ svn log --limit 250 http://svn.asterisk.org/svn/asterisk/trunk | ./log-check
Alexander Traud
coreyfarrell
Dennis Guse
dennis.guse
dtryba
Elazar Broad
elguero
file
Gergely Domodi
gtjoseph
igorg
jkister
jrose
Kevin McCoy
kharwell
kmoore
Matt Jordan
Matthis Schmieder
may
Michael K
Michael L. Young
mjordan
mmichelson
newtonr
one47
opticron
Pavel Troller
qwell
rmudgett
rmudgetti
seanbright
sgalarneau
sgriepentrog
Steve Davies
Torrey Searle
tzafrir
wdoekes
wedhorn
~~~


