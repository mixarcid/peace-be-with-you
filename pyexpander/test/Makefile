#
# test1.in : simply macro replacement

PYPATH=`pwd`/..

EXPANDER=PYTHONPATH=$(PYPATH) ../expander.py

.PRECIOUS: %.out

# set this only to "yes" in order to create the 
# test[n].ok files. These files are later compared to
# the test[n].out files.
# In order for the tests to really test pyexpander, 
# the variable MUST BE set to "no".
CREATE_TESTS=no

testlist= test1 test2 test3 test4 test5 test6 test7 test8 test9 \
	  test10 test11 test12 test13 test14 test15 test16 test17 \
	  test18 test19 test20 test21 test22 test23 test24 test25 \
	  test26 test27 test28 test29


all: pure_module_test external_module_test $(addsuffix .tst, $(testlist))

clean:
	rm -f $(addsuffix .out, $(testlist))

_clean: clean
	rm -f $(addsuffix .ok, $(testlist))

pure_module_test:
	PYTHONPATH=$(PYPATH) python ../expanderparser.py
	PYTHONPATH=$(PYPATH) python ../pyexpander.py

external_module_test:
	PYTHONPATH=$(PYPATH) python pyexpander_test.py

ok: $(addsuffix .ok, $(testlist))

%.out: %.in
	$(EXPANDER) -f $< > $@

test3.out: test3.in
	$(EXPANDER) -f $< --eval 'x=1' > $@

test4.out: test4.in
	($(EXPANDER) -f $< || true) 2>&1 | \
	    sed -e 's/.*\//  "/' -e 's/line [0-9]\+/line xxx/' > $@

test7.out: test7.in
	$(EXPANDER) -s -f $< > $@

test19.out: test19.in
	mkdir x
	echo "\$$py(a+=1)" > x/test.inc
	$(EXPANDER) -f $< -I x > $@
	rm -rf x

ifeq "$(CREATE_TESTS)" "yes"
%.ok: %.out
	cp $< $@
endif

%.tst: %.in %.out %.ok
	diff $(wordlist 2, 3, $+) 


