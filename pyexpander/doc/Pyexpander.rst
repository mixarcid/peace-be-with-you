=================================================
pyexpander - a powerful macro processing language
=================================================

.. This text is RST (ReStructured Text), 
   see also http://docutils.sourceforge.net/rst.html

Introduction
------------

For some projects there is the need to replace macros in a text file with
values defined in other files or the command line. There are already other
macro replacement tools that can do this but if you want to calculations or
string operations and insert the results of this into your file, the list of
possible tools becomes much shorter.

Pyexpander combines the python programming language with a simple macro
replacement scheme in order to give the user both, ease of use and the
power of a full featured scripting language. 

Even without being familiar with python you can use pyexpander to perform
calculations or string operations and use the results in your macro
replacements.

Here is a very simple example, this is the content of file "letter.txt"::

  Dear $(salutation) $(surname),
  
  this is a simple pyexpander example.

Applying this command::

  expander.py --eval 'salutation="Mr";surname="Smith"' -f letter.txt

gives this result::

  Dear Mr Smith,
  
  this is a simple pyexpander example.


Here is more advanced example::

  $py(start=0; end=5)\
   x |  x**2
  ---|------
  $for(x in range(start,end+1))\
  $("%2d | %3d" % (x,x*x))
  $endfor\

Applying expander.py to a file with the content shown above gives the following
result::

   x |  x**2
  ---|------
   0 |   0
   1 |   1
   2 |   4
   3 |   9
   4 |  16
   5 |  25

And here we show how pyexpander compares with the well known m4 macro
processor. We have taken the 
`m4 <http://en.wikipedia.org/wiki/M4_(computer_language)>`_ example with small
modifications from Wikipedia::

  divert(-1)
  # This starts the count at ONE as the incr is a preincrement.
  define(`H2_COUNT', 0)
  # The H2_COUNT macro is redefined every time the H2 macro is used.
  define(`H2',
          `define(`H2_COUNT', incr(H2_COUNT))<h2>H2_COUNT. $1</h2>')
  divert(0)dnl Diversion to 0 means back to normal. dnl macro removes this line.
  H2(First Section)
  H2(Second Section)
  H2(Conclusion)

Here is the same example formulated in pyexpander::

  $py(
  # This starts the count at ONE as the incr is a preincrement.
  H2_COUNT=0
  # H2_COUNT is incremented each time H2 is called.
  def H2(st):
      global H2_COUNT
      H2_COUNT+=1
      return "<h2>%d. %s</h2>" % (H2_COUNT,st)
      )\
  $# the following makes H2 callable without another pair of enclosing brackets:
  $extend(H2)\
  $H2("First Section")
  $H2("Second Section")
  $H2("Conclusion")

Both produce this output::

  <h2>1. First Section</h2>
  <h2>2. Second Section</h2>
  <h2>3. Conclusion</h2>

The advantages of pyexpander are:

- simple syntax definition, all expander commands start with a dollar ("$")
  sign followed by word characters, parameters or python code enclosed in
  brackets or both.
- the full power of the python programming language can be used, all operators,
  functions and modules.
- *any* python expression can be used to insert text.
- There is also a python library, pyexpander.py, which you can use to develop
  other macro tools based on pyexpander.

If you are not familiar with the python programming language, I recommend that
you have a look at this `short python introduction <python.html>`_ *after* you
have read this manual.

Syntax of the pyexpander language
---------------------------------

The meaning of the dollar sign
++++++++++++++++++++++++++++++

Almost all elements of the language start with a dollar "$" sign. If a dollar
is preceded by a backslash "\\" it is escaped. The "\\$" is then replaced with
a simple dollar character "$" and the rules described further down do not
apply.

Here is an example::
 
  an escaped dollar: \$

This would produce this output::

  an escaped dollar: $

Comments
++++++++

A comment is started by a sequence "$#" where the dollar sign is not preceded
by a backslash (see above). All characters until and including the end of line
character(s) are ignored. Here is an example::

  This is ordinary text, $# from here it is a comment
  here the text continues.

Commands
++++++++

If the dollar sign, which is not preceded by a backslash, is followed by a
letter or an underline "_" and one or more alphanumeric characters, including
the underline "_", it is interpreted to be an expander command. 

The *name* of the command consists of all alphanumeric characters including "_"
that follow. In order to be able to embed commands into a sequence of letters,
as a variant of this, the *name* may be enclosed in curly brackets. This
variant is only allowed for commands that do not expect parameters.

If the command expects parameters, an opening round bracket "(" must
immediately (without spaces) follow the characters of the command name. The
parameters end with a closing round bracket ")".

Here are some examples::
 
  this is not a command due to escaping rules: \$mycommand
  a command: $begin
  a command within a sequence of letters abc${begin}def
  a command with parameters: $for(x in range(0,3))

Note that in the last line, since the parameter of the "for" command must be a
valid python expression, all opening brackets in that expression must match a
closing bracket. By this rule pyexpander is able to find the closing bracket
that belongs to the opening bracket of the parameter list.

Executing python statements
+++++++++++++++++++++++++++

A statement may be any valid python code. Statements usually do not return
values. All expressions are statements, but not all statements are 
expressions. In order to execute python statements, there is the "py" command.
"py" is an abbreviation of python. This command expects that valid python code
follows enclosed in brackets. Note that the closing bracket for "py" *must not*
be in the same line with a python comment, since a python comment would include
the bracket and all characters until the end of the line, leading to a
pyexpander parser error. The "py" command leads to the execution of the python
code but produces no output. It is usually used to define variables, but it can
also be used to execute python code of more complexity. Here are some
examples::

  Here we define the variable "x" to be 1: $py(x=1)
  Here we define two variables at a time: $py(x=1;y=2)
  Here we define a function, note that we have to keep
  the indentation that python requires intact:
  $py(
  def multiply(x,y):
      return x*y
      # here is a python comment
      # note that the closing bracket below
      # *MUST NOT* be in such a comment line
     )

Line continuation
+++++++++++++++++

Since the end of line character is never part of a command, commands placed on
a single line would produce an empty line in the output. Since this is
sometimes not wanted, the generation of an empty line can be suppressed by
ending the line with a single backslash "\\". Here is an example::

  $py(x=1;y=2)\
  The value of x is $(x), the value of y is $(y).
  Note that no leading empty line is generated in this example.

Substitutions
+++++++++++++

A substitution consists of a dollar "$" that is not preceded by a backslash and
followed by an opening round bracket "(" and a matching closing round bracket
")". The string enclosed by the pair of brackets must form a valid python
expression. Note that a python expression, in opposition to a python statement,
always has a value. This value is converted to a string and this string is
inserted in the text in place of the substitution command. Here is an example::

  $py(x=2) we set "x" to 2 here
  now we can replace "x" anywhere in the text
  like here $(x) since "x" alone is already a python expression.
  Note that the argument of "py" is a python statement.
  We can also insert x times 3 here like this: $(x*3). 
  We can even do calculations like: $(x*sin(x)).

There is also a mode called "simple vars" in the expander tool, where the round
brackets around variable names may be omitted. Note that this is not possible
for arbitrary python expressions, since pyexpander would not know where the
expression ends without the brackets. Here is an example::

  We define x: $py(x=1)
  In "simple vars" mode, we can use the variable as we know
  it: $(x) but also without brackets: $x. However, expressions that are
  not simple variable names must still use brackets: $(x*2).

Default values for variables
++++++++++++++++++++++++++++

When an undefined variable is encountered, pyexpander raises a python exception
and stops. Sometimes however, we want to take a default value for a variable
but only if it has not yet been set with a value. This can be achieved with the
"default" command.  This command must be followed by an opening bracket and an
arbitrary list of named python parameters. This means that each parameter
definition consists of an unquoted name, a "=" and a quoted string, several
parameter definitions must be separated by commas. The "default" command takes
these parameters and sets the variables of these names to the given values if
the variables are not yet set with different values. Here is an example::

  We define a: $py(a=1)
  Now we set a default for a and b: $default(a=10, b=20)
  Here, $(a) is 1 since is was already defined before
  and $(b) is 20, it's default value since it was not defined before.

Variable scopes
+++++++++++++++

By default, all variables defined in a "py" command are global. They exist from
the first time they are mentioned in the text and can be modified at any place
further below.  Sometimes however, it is desirable to set a variable in a
certain area of the text and restore it to it's old value below that area. In
order to do this, variable scopes are used. A variable scope starts with a
"begin" command and ends with an "end" command. All variable definitions and
changes between "begin" and "end" are reverted when the "end" command is
reached. Some commands like "for", "while" and "include" have a variant with a
"_begin" appended to their name, where they behave like "begin" and "end" and
define a variable scope additionally to their normal function. Here is an
example of "begin" and "end"::
  
  $py(a=1)
  a is now 1
  $begin
  $py(a=2)
  a is now 2
  $end
  here, a is 1 again

All variable modifications and definitions within a variable scope are isolated
from the rest of the text. However, sometimes we want to modify variables
outside the scope. This can be done by declaring a variable as non-local with
the command "nonlocal". The "nonlocal" command must be followed by a comma
separated list of variable names enclosed in brackets. When the end of the
scope is reached, all variables that were declared non-local are copied to the
outer scope. Here is an example::

  $py(a=1;b=2;c=3)
  a is now 1, b is 2 and c is 3
  $begin
  $nonlocal(a,b)
  $py(a=10;b=20;c=30)
  a is now 10, b is 20 and c is 30
  $end
  here, a is 10, b is 20 and c is 3 again

If scopes are nested, the "nonlocal" defines a variable to be non-local only in
the current scope. If the current scope is left, the variable is local again
unless it is defined non-local in that scope, too.

Extending the pyexpander language
+++++++++++++++++++++++++++++++++

All functions or variables defined in a "$py" command have to be applied in the
text by enclosing them in brackets and prepending a dollar sign like here::

  $(myvar)
  $(myfunction(parameters))

However, sometimes it would be nice if we could use these python objects a bit
easier. This can be achieved with the "extend" command. "extend" expects to be
followed by a comma separated list of identifiers enclosed in brackets. These
identifiers can then be used in the text without the need to enclose them in
brackets. Here is an example::

  $extend(myvar,myfunction)
  $myvar
  $myfunction(parameters)

Note that identifiers extend the pyexpander language local to their scope. Here
is an example for this::

  $py(a=1)
  $begin
  $extend(a)
  we can use "a" here directly like $a
  $end
  here the "extend" is unknown, a has always
  to be enclosed in brackets like $(a)

You should note that with respect to the "extend" command, there is a
difference between including a file with the "include" command or the
"include_begin" command (described further below). The latter one defines a
new scope, and the rule shown above applies here, too.

Conditionals
++++++++++++

A conditional part consists at least of an "if" and an "endif" command. Between
these two there may be an arbitrary number of "elif" commands. Before "endif"
and after the last "elif" (if present) there may be an "else" command. "if" and
"elif" are followed by a condition expression, enclosed in round brackets.
"else" and "endif" do not have parameters. If the condition after "if" is true,
this part is evaluated. If it is false, the next "elif" part is tested. If it
is true, this part is evaluated, if not, the next "elif" part is tested and so
on. If no matching condition was found, the "else" part is evaluated. All of
this is oriented on the python language which also has "if","elif" and "else".
"endif" has no counterpart in python since there the indentation shows where
the block ends. Here is an example::

  We set x to 1; $py(x=1)
  $if(x>2)
  x is bigger than 2
  $elif(x>1)
  x is bigger than 1
  $elif(x==1)
  x is equal to 1
  $else
  x is smaller than 1
  $endif
  here is a classical if-else-endif:
  $if(x>0)
  x is bigger than 0
  $else
  x is not bigger than 0
  $endif
  here is a simple if-endif:
  $if(x==0)
  x is zero
  $endif

While loops
+++++++++++

While loops are used to generate text that contains almost identical
repetitions of text fragments. The loop continues while the given loop
condition is true. A While loop starts with a "while" command followed by a
boolean expression enclosed in brackets. The end of the loop is marked by a
"endwhile" statement. Here is an example::

  $py(a=3)
  $while(a>0)
  a is now: $(a)
  $py(a-=1)
  $endwhile

In this example the loop runs 3 times with values of a ranging from 3 to 1. 

The command "while_begin" combines a while loop with a scope::

  $while_begin(condition)
  ...
  $endwhile
  
and::

  $while(condition)
  $begin
  ...
  $end
  $endwhile

are equivalent. 
  
For loops
+++++++++

For loops are a powerful tool to generate text that contains almost identical
repetitions of text fragments. A "for" command expects a parameter that is a
python expression in the form "variable(s) in iterable". For each run the
variable is set to another value from the iterable and the following text is
evaluated until "endfor" is found. At "endfor", pyexpander jumps back to the
"for" statement and assigns the next value to the variable. Here is an
example::

  $for(x in range(0,5))
  x is now: $(x)
  $endfor

The range function in python generates a list of integers starting with 0 and
ending with 4 in this example. 

You can also have more than one loop variable::

  $for( (x,y) in [(x,x*x) for x in range(0,3)])
  x:$(x) y:$(y)
  $endfor

or you can iterate over keys and values of a python dictionary::

  $py(d={"A":1, "B":2, "C":3})
  $for( (k,v) in d.items())
  key: $(k) value: $(v)
  $endfor

The command "for_begin" combines a for loop with a scope::

  $for_begin(loop expression)
  ...
  $endfor
  
and::

  $for(loop expression)
  $begin
  ...
  $end
  $endfor

are equivalent. 

Include files
+++++++++++++

The "include" command is used to include a file at the current position. It
must be followed by a string expression enclosed in brackets. The given file is
then interpreted until the end of the file is reached, then the interpretation
of the text continues after the "include" command in the original text.

Here is an example::

  $include("additional_defines.inc")

The command "include_begin" combines an include with a scope. It is equivalent
to the case when the include file starts with a "begin" command and ends with
an "end" command.

Here is an example::

  $include_begin("additional_defines.inc")

EPICS Substitution support
++++++++++++++++++++++++++

Pyexpander has been equipped with three more commands, "template", "subst" and
"pattern" that enable it to replace the `EPICS <http://www.aps.anl.gov/epics>`_
`msi <http://www.aps.anl.gov/epics/extensions/msi/index.php>`_ tool. These
commands, however, may also be useful for other applications.  The idea in msi
is to have a template file with macro placeholders in it and process this file
several times with different macro values at each run. In this mechanism, the
filename has only to be mentioned once. 

Here is a simple example, test.template has this content::

  record(calcout, "U3IV:$(name)") {
    field(CALC, "$(calc)")
    field(INPA, "U3IV:P4:rip:cvt CPP MS")
    field(OUT,  "U3IV:P4:rip:calcLRip.A PP MS")
  }

test.substitution has this content::

  $template("test.template")\
  $subst(
    name="set", 
    calc="A+B",
  )\
  $subst(
    name="set2",
    calc="C+D"
  )\

This is the result when test.template is processed::

  record(calcout, "U3IV:set") {
    field(CALC, "A+B")
    field(INPA, "U3IV:P4:rip:cvt CPP MS")
    field(OUT,  "U3IV:P4:rip:calcLRip.A PP MS")
  }
  record(calcout, "U3IV:set2") {
    field(CALC, "C+D")
    field(INPA, "U3IV:P4:rip:cvt CPP MS")
    field(OUT,  "U3IV:P4:rip:calcLRip.A PP MS")
  }

As you see, test.template was instantiated twice. In the pyexpander package
there is also a converter program, msi2pyexpander.py, which can be used to convert
substitution files from the EPICS msi format to the pyexpander format.

This is how the three commands work:

Setting the name of the template file
.....................................

The "template" command is used to define the name of an substitution file. It
must be followed by a string expression enclosed in brackets. Note that the
filename is only defined within the current scope (see "variable scopes"). 

Here is an example::

  $template("test.template")

The "subst" command
...................

This command is used to substitute macros in the file whose name was defined
with the "template" command before. This command must be followed by an
opening bracket and an arbitrary list of named python parameters. This means
that each parameter definition consists of an unquoted name, an "=" and a
quoted string, several parameter definitions must be separated by commas. The
"subst" command takes these parameters and defines the variables in a new
scope. It then processes the file that was previously set with the "template"
command. Here is an example::

  $subst(
          AMS= "ams_",
          BASE= "UE112ID7R:",
          BASE1= "UE112ID7R:",
          BASE2= "UE112ID7R:S",
          BaseStatMopVer= "9",
        )\

The "pattern" command
.....................

This command is an alternative way to substitute macros in a file. The pattern command must be followed by an opening round bracket, a list of python tuples and a closing round bracket. Each tuple is a comma separated list of quoted strings enclosed in round brackets. Tuples must be separated by commas. Here is an example::

  $pattern(
            ( "DEVN", "SIGNAL"),
            ( "PAHRP", "PwrCavFwd"),
            ( "PAHRP", "PwrCavRet"),
            ( "PAHRP", "PwrCircOut"),
          )\

The first tuple defines the names of the variables, all following tuples define
values these variables get. For each following tuple the file defined by
"template" is included once. In the example above, the variable "DEVN" has
always the value "PAHRP", the variable "SIGNAL" has the values "PwrCavFwd",
"PwrCavRet" and "PwrCircOut". The file defined by the previous "template"
command is instantiated 3 times.

Differences to the EPICS msi tool
.................................

These are differences to msi:

- The file format of substitution files is different, but you can use
  msi2pyexpander.py to convert them.
- Macros must always be defined. If a macro should be expanded and it is not
  defined at the time, the program stops with an exception. If you want the
  program to continue in this case, use the "default" command to provide
  default values for the macros that are sometimes not defined.
- Variables defined in a "subst" command are scoped, they are only defined for
  that single instantiation of the template file. 
- The template file commands "include" and "substitute" as they are known from
  msi are not implemented in this form. However, "include" in pyexpander
  has the same functionality as "include" in msi and "py" in pyexpander can be
  used to do the same as "substitute" in msi.

Here is an example how to convert a template file from msi to pyexpander. Note
that in pyexpander there is no principal difference between a template and a
substitution file, both have the same syntax. The msi template file is this::

  A variable with a default $(var=default value)
  Here we include a file:
  include "filename"
  Here we define a substitution:
  substitute "first=Joe,family=Smith"

Here is the same formulated for pyexpander::

  A variable with a default $default(var="default value")$(var)
  Here we include a file:
  $include("filename")
  Here we define a substitution:
  $py(first="Joe";family="Smith")
  
Internals
---------

This section describes how pyexpander works. 

pyexpander consists of the following parts:

expanderparser.py
+++++++++++++++++

A python module that implements a parser for expander files.  This is the
library that defines all functions and classes the are used for the
expanderparser interpreter.

Here is a link to the `embedded documentation of expanderparser
<expanderparser.html>`_.

pyexpander.py
+++++++++++++

A python module that implements all the functions needed to 
implement the pyexpander language.

Here is a link to the `embedded documentation of pyexpander
<pyexpander.html>`_.

Scripts provided by the package
-------------------------------

msi2pyexpander.py
+++++++++++++++++

This script can be used to convert `EPICS <http://www.aps.anl.gov/epics>`_ `msi
<http://www.aps.anl.gov/epics/extensions/msi/index.php>`_ template files to the
format of pyexpander. You only need this script when you have an `EPICS
<http://www.aps.anl.gov/epics>`_ application and want to start using pyexpander
for it.

Here is a link to the `command line options of msi2pyexpander.py
<msi2pyexpander.html>`_.

expander.py
+++++++++++

A python script with command line options for search paths and file
names which uses pyexpander to interpret the given text file.

You will probably just call this script for your application. However, you
could write a python program yourself that imports and uses the pyexpander.py
library.

Here is a link to the `command line options of expander.py
<expander.html>`_.

