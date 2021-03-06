# pylint: disable=C0302
import os
import os.path
import inspect

import sys # only for stderr-write

import expanderparser as EP

# pylint: disable=C0322,C0103,R0903

# ---------------------------------------------
# file search utilities
# ---------------------------------------------

# ---------------------------------------------
# dump utilities
# ---------------------------------------------

def _set2str(val):
    """convert an iterable to the repr string of a set."""
    elms= sorted(list(val))
    return "set(%s)" % repr(elms)

def _pr_set(val):
    """print an iterable as the repr string of a set."""
    print _set2str(val)

def find_file(filename, include_paths):
    """find a file in a list of include paths.

    include_paths MUST CONTAIN "" in order to search the
    local directory.
    """
    if include_paths is None:
        return None
    for path in include_paths:
        p= os.path.join(path, filename)
        if os.path.exists(p):
            if os.access(p, os.R_OK):
                return p
            print "warning: file \"%s\" found but it is not readable" % \
                  p
    return None

# ---------------------------------------------
# all
# ---------------------------------------------

def parseString(st):
    """parse a string."""
    return EP.parseAll(EP.IndexedString(st), 0)

def parseFile(filename):
    """parse a file."""
    f= open(filename, "r")
    st= f.read()
    f.close()
    return parseString(st)

# ---------------------------------------------
# process parse-list
# ---------------------------------------------

# pylint: disable=R0902,E0702

class Block(object):
    """class that represents a block in the expander language.

    Each block has a parent-pointer, which may be None. Blocks are used to
    represent syntactical blocks in the expander language. They manage the
    global variable dictionary and some more properties that are needed during
    execution of the program.
    """
    def posmsg(self, msg=None, pos=None):
        """return a message together with a position.

        This method is mainly used for error messages. We usually want to print
        filename and the line and column number together with a message. This
        method returns the given message string <msg> together with this
        additional information.
        """
        parts=[]
        if msg is not None:
            parts.append(msg)
        if self.filename is not None:
            parts.append("file \"%s\"" % self.filename)
        try:
            # posmsg is often called with pos==None. So we usually have to
            # determine the position ourselves. Usually posmsg is called when
            # an error occurs when processing self.parse_list[self.lst_pos]. So
            # if we call method rowcol(None) with this element (a ParsedItem
            # object) the *start* of the ParsedItem is calculated to a row and
            # a column and returned.
            p_elm= self.parse_list[self.lst_pos]
            parts.append("line %d, col %d" % p_elm.rowcol(pos))
            return " ".join(parts)
        except IndexError, _:
            return "unknown position"
    def _append(self, lst, name, val=None):
        """append a line with property information to a list.

        This method is used to create an object dump in method _strlist.
        """
        def elm2str(e):
            """convert an item to a "repr" like string."""
            if isinstance(e,set):
                return _set2str(e)
            return str(e)
        if val is not None:
            lst.append("    %-20s: %s" % (name,val))
        else:
            lst.append("    %-20s= %s" % (name,elm2str(getattr(self,name))))
    def _strlist(self):
        """utility for __str__.

        This function can be used by descendent classes in order to implement
        the __str__ method in a simple way.

        Tested by the testcode in __str__.
        """
        lst= []
        self._append(lst, "has parent",self.previous is not None)
        self._append(lst, "filename")
        self._append(lst, "template")
        self._append(lst, "template_path")
        self._append(lst, "has template_parselist_cache",
                          self.template_parselist_cache is not None)
        self._append(lst, "exported_syms")
        self._append(lst, "direct_vars")
        self._append(lst, "direct_funcs")
        self._append(lst, "new_scope")
        self._append(lst, "skip")
        self._append(lst, "new_parse_list")
        self._append(lst, "lst_pos")
        self._append(lst, "start_pos")
        if self.lst_pos>=0 and self.lst_pos<len(self.parse_list):
            self._append(lst, "current parse elm",
                         self.parse_list[self.lst_pos])
        return lst
    # pylint: disable=R0913,R0912,R0915
    def __init__(self,
                 previous= None,
                 new_scope= False,
                 filename= None,
                 parse_list=None,
                 external_definitions=None):
        """The Block constructor.

        Properties of a Block:
            parse_list    -- the list with parse objects
            lst_pos       -- the position in the parse_list
            start_pos     -- the start position of the block in the parse_list
            previous      -- the previous block
            new_scope     -- True if the block is also a new scope with
                             respect to global variables.
            skip          -- if True, skip a block in the text when the code
                             is interpreted
            globals_      -- a dictionary with the current global variables of
                             the interpreter.
            exported_syms -- a list of symbols (names in globals_) that are
                             copied to the globals_ dictionary of the parent
                             block when the pop() method is called.
            filename      -- name of the interpreted file, may be None
            template      -- name of the file that "$subst" would include,
                             usually None
            template_path -- complete path of template
                             usually None
            template_parselist_cache --
                             parselist of the template, usually None.
                             This is a kind of optimization

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> print b
        Block{
            has parent          : False
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = False
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        >>> b= Block(b,True,parse_list=[])
        >>> b.print_block_list()
        Block{
            has parent          : False
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = False
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        Block{
            has parent          : True
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = True
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        """
        if (filename is None) or (filename==""):
            self.filename= None
        else:
            self.filename= filename
        self.previous= previous
        if previous is None:
            self.new_scope= False
            self.skip= False
            self.globals_= dict()
            self.direct_vars=set()
            self.direct_funcs=set()
            self.exported_syms=[]
            if parse_list is None:
                raise AssertionError, "without previous, parse_list "+\
                                      "is mandatory"
            self.new_parse_list= True
            self.template= None
            self.template_path= None
            self.template_parselist_cache= None
            self.parse_list= parse_list
            self.lst_pos= -1
            self.start_pos= 0
        else:
            if not isinstance(previous,Block):
                raise AssertionError, "previous is not a block: %s" % \
                                      str(previous)
            if self.filename is None:
                self.filename= previous.filename
            if parse_list is None:
                self.new_parse_list= False
                self.parse_list= previous.parse_list
                self.lst_pos= previous.lst_pos
                if previous.lst_pos<0:
                    self.start_pos= 0
                else:
                    self.start_pos= previous.lst_pos
            else:
                self.new_parse_list= True
                self.parse_list= parse_list
                self.lst_pos= -1
                self.start_pos= 0
            self.template= previous.template
            self.template_path= previous.template_path
            self.template_parselist_cache= previous.template_parselist_cache
            self.new_scope= new_scope
            self.skip= previous.skip
            if new_scope:
                self.globals_= dict(previous.globals_)
                self.direct_vars= set(previous.direct_vars)
                self.direct_funcs= set(previous.direct_funcs)
                self.exported_syms=[]
            else:
                self.globals_= previous.globals_
                self.direct_vars= previous.direct_vars
                self.direct_funcs= previous.direct_funcs
                self.exported_syms= previous.exported_syms
        if external_definitions is not None:
            for (k,v) in external_definitions.items():
                self.globals_[k]= v
    # pylint: enable=R0913,R0912,R0915
    def parse_loop(self):
        """loop across the items in the Block.
        
        returns False if there is nothing more to parse in the current block.
        """
        self.lst_pos+= 1
        return self.lst_pos < len(self.parse_list)
    def parse_elm(self):
        """get the current parse element."""
        return self.parse_list[self.lst_pos]
    def eval_(self, st):
        """perform eval with the globals_ dictionary of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.exec_("a=2")
        >>> b.eval_("3*a")
        6
        """
        try:
            return eval(st, self.globals_)
        except SyntaxError, e:
            raise SyntaxError, "%s at %s" % (str(e), self.posmsg())
        except NameError, e:
            raise NameError, "%s at %s" % (str(e), self.posmsg())
        except IndexError, e:
            raise IndexError, "%s at %s" % (str(e), self.posmsg())
        except TypeError, e:
            raise TypeError, "%s at %s" % (str(e), self.posmsg())
        except Exception, e:
            sys.stderr.write("error at %s:\n" % self.posmsg())
            raise
    def str_eval(self, st):
        """perform eval with the globals_ dictionary of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.exec_("a=2")
        >>> b.str_eval("3*a")
        '6'
        """
        # pylint: disable=W0703
        val= self.eval_(st)
        try:
            return(str(val))
        except Exception, _:
            sys.stderr.write("error at %s:\n" % self.posmsg())
            raise
       # pylint: enable=W0703
    def exec_(self, st):
        """perform exec with the globals_ dictionary of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.exec_("a=1")
        >>> b["a"]
        1
        >>> b= Block(b,True)
        >>> b["a"]
        1
        >>> b.exec_("a=2")
        >>> b["a"]
        2
        >>> b.previous["a"]
        1
        """
        try:
            # pylint: disable=W0122
            exec st in self.globals_
            # pylint: enable=W0122
        except SyntaxError, e:
            raise SyntaxError, "%s at %s" % (str(e), self.posmsg())
        except NameError, e:
            raise NameError, "%s at %s" % (str(e), self.posmsg())
        except IndexError, e:
            raise IndexError, "%s at %s" % (str(e), self.posmsg())
        except TypeError, e:
            raise TypeError, "%s at %s" % (str(e), self.posmsg())
        except Exception, e:
            sys.stderr.write("error at %s:\n" % self.posmsg())
            raise
    def __getitem__(self, name):
        """looks up a value in the globals_ dictionary of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.globals_["a"]= 5
        >>> b["a"]
        5

        Here we show how symbols with dots '.' are treated. We misuse
        os.path to create a variable os.path.myvar and store a
        reference to the module "os" in the Block "globals_"
        dictionary:

        >>> import os.path
        >>> os.path.myvar=100
        >>> b.globals_["os"]= os
        >>> b["os.path.myvar"]
        100
        """
        try:
            return eval(name, self.globals_)
        except NameError, e:
            raise NameError, "%s at %s" % (str(e),self.posmsg())
    def __setitem__(self, name, val):
        """sets a value in the globals_ dictionary of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b["a"]= 5
        >>> b.globals_["a"]
        5

        Here we show how symbols with dots '.' are treated. We misuse os.path
        to create a variable os.path.myvar and store a reference to the module
        "os" in the Block "globals_" dictionary:

        >>> import os.path
        >>> b["os"]= os
        >>> b["os.path.myvar"]= 200
        >>> os.path.myvar
        200
        >>> b["os.path.myvar"]
        200
        """
        self.globals_["__pyexpander_buffer"]= val
        try:
            # pylint: disable=W0122
            exec "%s= __pyexpander_buffer" % name in self.globals_
            # pylint: enable=W0122
        except AttributeError, e:
            raise AttributeError, "%s at %s" % (str(e),self.posmsg())
    def __delitem__(self, name):
        """deletes a value in the globals_ dictionary of the block.

        Here is an example:

        >>> import pprint
        >>> b= Block(parse_list=[])
        >>> b.globals_.keys()
        []
        >>> b["a"]= 5
        >>> b.globals_.keys()
        ['__pyexpander_buffer', '__builtins__', 'a']
        >>> b["a"]
        5
        >>> del b["a"]
        >>> b.globals_.keys()
        ['__pyexpander_buffer', '__builtins__']
        >>> b["a"]
        Traceback (most recent call last):
            ...
        NameError: name 'a' is not defined at unknown position
        >>> del b["c"]
        Traceback (most recent call last):
            ...
        NameError: name 'c' is not defined at unknown position

        Here we show how symbols with dots '.' are treated. We misuse os.path
        to create a variable os.path.myvar and store a reference to the module
        "os" in the Block "globals_" dictionary:

        >>> import os.path
        >>> b= Block(parse_list=[])
        >>> os.path.myvar=200
        >>> b["os"]= os
        >>> b["os.path.myvar"]
        200
        >>> del b["os.path.myvar"]
        >>> b["os.path.myvar"]
        Traceback (most recent call last):
            ...
        AttributeError: 'module' object has no attribute 'myvar'
        >>> os.path.myvar
        Traceback (most recent call last):
            ...
        AttributeError: 'module' object has no attribute 'myvar'
        """
        try:
            # pylint: disable=W0122
            exec "del %s" % name in self.globals_
            # pylint: enable=W0122
        except NameError, e:
            raise NameError, "%s at %s" % (str(e),self.posmsg())
    def __len__(self):
        """returns the number of items in the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.globals_.keys()
        []
        >>> b["a"]= 5
        >>> b["b"]= 6
        >>> b.globals_.keys()
        ['__pyexpander_buffer', '__builtins__', 'b', 'a']
        >>> len(b)
        4
        """
        return len(self.globals_)
    def setdefault(self, name, val):
        """return a value from globals, set to a default if it's not defined.

        Here are some examples:
        >>> b= Block(parse_list=[])
        >>> b["a"]
        Traceback (most recent call last):
            ...
        NameError: name 'a' is not defined at unknown position
        >>> b.setdefault("a",10)
        10
        >>> b["a"]
        10
        >>> b["a"]=11
        >>> b.setdefault("a",10)
        11
        """
        try:
            return self.__getitem__(name)
        except NameError, _:
            self.__setitem__(name,val)
        return val

    def set_substfile(self, filename, tp):
        """set substitution filename.

        Used for the "$subst", "$template" and "$pattern" commands.
        """
        if not isinstance(filename, str):
            raise EP.ParseException(
                    self.posmsg("filename must be a string at",
                                  pos=tp.start()))
        if filename == self.template:
            return
        self.template_parselist_cache= None
        self.template_path= None
        self.template= filename
    def substfile_parselist(self, include_paths):
        """return the parselist for a given file.

        This method manages a cache of that assigns filenames to parselists.
        This can speed things up if a file is included several times.
        """
        if self.template_parselist_cache is not None:
            return (self.template_path,self.template_parselist_cache)
        if self.template is None:
            raise ValueError, \
                  self.posmsg("substitition file name missing at")
        self.template_path= find_file(self.template, include_paths)
        if self.template_path is None:
            raise ValueError, \
                  self.posmsg("file \"%s\" not found in" % \
                               self.template)
        else:
            self.template_parselist_cache= parseFile(self.template_path)
        return (self.template_path,self.template_parselist_cache)
    def export_symbols(self, lst):
        """appends items to the export_symbols list.

        This list is used by the pop() method in order to copy values from the
        current globals_ dictionary to the globals_ dictionary of the previous
        block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b.export_symbols(["a","b"])
        >>> b.exported_syms
        ['a', 'b']
        >>> b.export_symbols(["d","e"])
        >>> b.exported_syms
        ['a', 'b', 'd', 'e']
        """
        self.exported_syms.extend(lst)
    def extend(self, lst):
        """adds items to the list of expander functions or variables.

        Here is an example:
        >>> a=1
        >>> b=2
        >>> def t(x):
        ...   return x+1
        ...
        >>> block= Block(parse_list=[],external_definitions=globals())
        >>> block.extend(["a","b","t"])
        >>> _pr_set(block.direct_vars)
        set(['a', 'b'])
        >>> _pr_set(block.direct_funcs)
        set(['t'])
        """
        for elm in lst:
            obj= self.globals_[elm]
            if inspect.isbuiltin(obj):
                self.direct_funcs.add(elm)
                continue
            if inspect.isfunction(obj):
                self.direct_funcs.add(elm)
                continue
            # assume elm to be a variable:
            self.direct_vars.add(elm)
    def pop(self):
        """removes the current block and returns the previous one.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> b["a"]=1
        >>> b["b"]=2
        >>> b= Block(b,True)
        >>> b["a"]=10
        >>> b["b"]=20
        >>> b.export_symbols(["a"])
        >>> b= b.pop()
        >>> b["a"]
        10
        >>> b["b"]
        2
        """
        if self.previous is None:
            raise AssertionError,\
                  self.posmsg("block underflow (assertion) at")
        if self.new_scope:
            old= self.previous.globals_
            for elm in self.exported_syms:
                old[elm]= self.globals_[elm]
        # set the lst_pos in the parent Block:
        if not self.new_parse_list:
            self.previous.lst_pos= self.lst_pos
        return self.previous
    def __str__(self):
        """returns a string representation of the block.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> print b
        Block{
            has parent          : False
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = False
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        >>> b= Block(b,True)
        >>> print b
        Block{
            has parent          : True
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = True
            skip                = False
            new_parse_list      = False
            lst_pos             = -1
            start_pos           = 0
        }
        """
        lst=["%s{" % "Block"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)
    def get_block_list(self):
        """returns all blocks of the list.

        The list is returned with the oldest block first.
        """
        lst=[]
        block= self
        while block is not None:
            lst.append(block)
            block= block.previous
        lst.reverse()
        return lst
    def str_block_list(self):
        """returns a string representation of all blocks in the list.

        The list is returned with the oldest block first.
        """
        return [str(elm) for elm in self.get_block_list()]
    def print_block_list(self):
        """print all blocks in the list.

        The list is returned with the oldest block first.

        Here is an example:
        >>> b= Block(parse_list=[])
        >>> print b
        Block{
            has parent          : False
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = False
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        >>> b= Block(b,True)
        >>> print b
        Block{
            has parent          : True
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = True
            skip                = False
            new_parse_list      = False
            lst_pos             = -1
            start_pos           = 0
        }
        >>> b.print_block_list()
        Block{
            has parent          : False
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = False
            skip                = False
            new_parse_list      = True
            lst_pos             = -1
            start_pos           = 0
        }
        Block{
            has parent          : True
            filename            = None
            template            = None
            template_path       = None
            has template_parselist_cache: False
            exported_syms       = []
            direct_vars         = set([])
            direct_funcs        = set([])
            new_scope           = True
            skip                = False
            new_parse_list      = False
            lst_pos             = -1
            start_pos           = 0
        }
        """
        print "\n".join(self.str_block_list())

# pylint: enable=R0902,E0702

class IncludeBlock(Block):
    """implements a $include(filename) block.

    This block is simply a variable scope, so it is derived from Block where
    the constructor is called with new_scope=True.
    """
    def __init__(self,
                 previous= None,
                 new_scope= False,
                 filename= None,
                 include_paths=None):
        path= find_file(filename, include_paths)
        if path is None:
            parse_list= []
        else:
            parse_list= parseFile(path)
        Block.__init__(self, previous, new_scope,
                       path,
                       parse_list)
        if path is None:
            raise ValueError, \
                  self.posmsg("file \"%s\" not found in" % filename)
    def __str__(self):
        lst=["%s{" % "IncludeBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class SubstBlock(Block):
    """implements a $subst(parameters) block.

    This block is simply a variable scope, so it is derived from Block where
    the constructor is called with new_scope=True.
    """
    def __init__(self,
                 previous= None,
                 filename= None,
                 include_paths=None,
                 external_definitions=None):
        (path,parse_list)= previous.substfile_parselist(include_paths)
        Block.__init__(self, previous, True, # always new scope
                       path,
                       parse_list,
                       external_definitions)
        if path is None:
            raise ValueError, \
                  self.posmsg("file \"%s\" not found in" % filename)
    def __str__(self):
        lst=["%s{" % "SubstBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class PatternBlock(Block):
    """implements a $pattern(parameters) block.

    This block is simply a variable scope, so it is derived from Block where
    the constructor is called with new_scope=True.
    """
    def _strlist(self):
        lst= Block._strlist(self)
        self._append(lst, "heading")
        self._append(lst, "lines")
        self._append(lst, "curr_line")
        return lst
    # pylint: disable=R0913
    def __init__(self,
                 previous= None,
                 filename= None,
                 include_paths=None,
                 heading=None,
                 lines=None):
        (path,parse_list)= previous.substfile_parselist(include_paths)
        Block.__init__(self, previous, False, # no new scope
                       path, parse_list)
        if path is None:
            raise ValueError, \
                  self.posmsg("file \"%s\" not found in" % filename)
        if heading is None:
            heading= []
        if lines is None:
            lines= []
        self.heading= heading
        self.lines= lines
        if len(self.lines)<1:
            raise ValueError, \
                  self.posmsg("no instantiation data")
        self.curr_line= -1
        self.def_vars()
    # pylint: enable=R0913
    def pop(self):
        if not self.def_vars():
            return Block.pop(self)
        # reset position in file
        self.lst_pos= -1
        return self
    def def_vars(self):
        """define all the given variables in the PatternBlock."""
        self.curr_line+=1
        if self.curr_line >= len(self.lines):
            return False
        line= self.lines[self.curr_line]
        for i in xrange(0,len(self.heading)):
            self[self.heading[i]]= line[i]
        return True
    def __str__(self):
        lst=["%s{" % "PatternBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class BeginBlock(Block):
    """implements a $begin .. $end block.

    This block is simply a variable scope, so it is derived from Block where
    the constructor is called with new_scope=True.
    """
    def __init__(self,
                 previous= None,
                 filename= None):
        Block.__init__(self, previous, True)
        self.skip= previous.skip
    def __str__(self):
        lst=["%s{" % "BeginBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class IfBlock(Block):
    """implements a $if .. $else .. $endif block.

    An $if block never has a variable scope, so the base Block object is
    called with new_scope=False.
    """
    def _strlist(self):
        lst= Block._strlist(self)
        self._append(lst, "prev_skip")
        self._append(lst, "in_else_part")
        self._append(lst, "found")
        return lst
    def __init__(self,
                 previous= None,
                 condition= True):
        """constructs the $if block.

        condition is the boolean value of the $if condition.
        """
        Block.__init__(self, previous, False)
        self.prev_skip= previous.skip
        self.in_else_part= False
        if condition:
            self.found= True
            self.skip= self.prev_skip
        else:
            self.found= False
            self.skip= True
    def enter_elif(self, condition):
        """enter the "elif" part in the if..endif block."""
        if self.found:
            self.skip= True
        else:
            if condition:
                self.found= True
                self.skip= self.prev_skip
            else:
                self.skip= True
    def enter_else(self):
        """this should be called when $else is encountered.
        """
        if self.in_else_part:
            raise EP.ParseException(
                    self.posmsg("one \"else\" too many at"))
        self.in_else_part= True
        if not self.found:
            self.skip= self.prev_skip
        else:
            self.skip= True
    def __str__(self):
        lst=["%s{" % "IfBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class ForBlock(Block):
    """implements a $for .. $endfor block.
    """
    def _strlist(self):
        lst= Block._strlist(self)
        self._append(lst, "value_list")
        self._append(lst, "index")
        self._append(lst, "var_expr")
        self._append(lst, "jump_lst_pos")
        self._append(lst, "jump parse elm",self.parse_list[self.jump_lst_pos])
        return lst
    def __init__(self,
                 previous= None,
                 new_scope= False,
                 value_list=None,
                 var_expr=""):
        """constructor of the block.

        var_expr -- the expression that contains the loop variable or the
                    tuple with the loop variables.
        """
        Block.__init__(self, previous, new_scope)
        if value_list is None:
            value_list= []
        self.value_list= value_list
        self.index=0 # current index within self.value_list
        self.var_expr= var_expr
        self.jump_lst_pos= self.lst_pos
        if len(value_list)<=0:
            self.skip= True
        else:
            self.skip= previous.skip
    def set_loop_var(self):
        """set the loop variable to a new value."""
        if not self.skip:
            self.__setitem__(self.var_expr, self.value_list[self.index])
    def next_loop(self):
        """performs next loop.

        returns:
          True when the loop is not yet finished.
        """
        if self.skip:
            return False
        self.index+=1
        do_loop= self.index< len(self.value_list)
        if do_loop:
            self.lst_pos= self.jump_lst_pos
        return do_loop
    def __str__(self):
        lst=["%s{" % "ForBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

class WhileBlock(Block):
    """implements a $while .. $endwhile block.
    """
    def _strlist(self):
        lst= Block._strlist(self)
        self._append(lst, "while_expr")
        self._append(lst, "jump parse elm",self.parse_list[self.jump_lst_pos])
        return lst
    def __init__(self,
                 previous= None,
                 new_scope= False,
                 while_expr=""):
        """constructor of the block.

        while_expr -- the expression that contains the loop variable or the
                      tuple with the loop variables.
        """
        Block.__init__(self, previous, new_scope)
        self.while_expr= while_expr
        self.jump_lst_pos= self.lst_pos
        if while_expr=="":
            self.skip= True
        elif not self.eval_(self.while_expr):
            self.skip= True
        else:
            self.skip= previous.skip
    def next_loop(self):
        """performs next loop.

        returns:
          True when the loop is not yet finished.
        """
        if self.skip:
            return False
        do_loop= self.eval_(self.while_expr)
        if do_loop:
            self.lst_pos= self.jump_lst_pos
        return do_loop
    def __str__(self):
        lst=["%s{" % "WhileBlock"]
        lst.extend(self._strlist())
        lst.append("}")
        return "\n".join(lst)

def __pyexpander_helper(*args, **kwargs):
    """a helper function needed at runtime.

    This evaluates named arguments.
    """
    if len(args)==1:
        fn= args[0]
    elif len(args)>1:
        raise ValueError, "only one unnamed argument is allowed"
    else:
        fn= None
    return(fn, kwargs)

def __pyexpander_helper2(**kwargs):
    """a helper function needed at runtime.

    This evaluates named arguments.
    """
    return(kwargs)

# pylint: disable=R0914,R0912,R0915,E0702

def processToList(parse_list, filename=None,
                  external_definitions=None,
                  allow_nobracket_vars= False,
                  include_paths= None):
    """run pyexpander, return a list of strings.
    """
    # accept None for include_paths too:
    if include_paths is None:
        include_paths= []
    # prepend the cwd to the list of search paths:
    include_paths.insert(0,"")
    # The initial block:
    my_external_definitions= { "__pyexpander_helper": \
                                 globals()["__pyexpander_helper"],
                               "__pyexpander_helper2": \
                                 globals()["__pyexpander_helper2"]
                             }
    if external_definitions is not None:
        my_external_definitions.update(external_definitions)
    block= Block(filename= filename, parse_list=parse_list,
                 external_definitions= my_external_definitions)
    result= []
    while True:
        # print "-" * 40
        # block.print_block_list()
        if not block.parse_loop():
            # no more data in the current block:
            if isinstance(block, IncludeBlock):
                # if current block is an IncludeBlock, go back to previous
                # block:
                block= block.pop()
                continue
            elif isinstance(block, SubstBlock):
                # if current block is a SubstBlock, go back to previous block:
                block= block.pop()
                continue
            elif isinstance(block, PatternBlock):
                # if current block is a PatternBlock, go back to previous
                # block:
                block= block.pop()
                continue
            else:
                # end of data, leave the loop:
                break
        # get the current parse element (base class: ParsedItem)
        tp= block.parse_elm()
        if isinstance(tp, EP.ParsedComment):
            # comments are ignored:
            continue
        if isinstance(tp, EP.ParsedLiteral):
            # literals are only taken if skip mode is off:
            if not block.skip:
                result.append(tp.string())
            continue
        if isinstance(tp, EP.ParsedVar):
            # if skip mode is off, insert the current value of the variable.
            # The current block can be used like a dict in order to get values
            # of variables:
            if not block.skip:
                result.append(str(block[tp.string()]))
            continue
        if isinstance(tp, EP.ParsedEval):
            # if skip mode is off, evaluate the eval expression,
            # convert it to a string and insert the result:
            if not block.skip:
                result.append(block.str_eval(tp.string()))
            continue
        if isinstance(tp, EP.ParsedCommand):
            # if ParsedItem is a ParsedCommand:
            if tp.ident=="py":
                # $py(...) :
                # execute the string given within the brackets:
                if not block.skip:
                    block.exec_(tp.args())
            elif tp.ident=="include" or tp.ident=="include_begin":
                if not block.skip:
                    # $include(...) or $include_begin(...) :
                    # evaluate the filename of the file to include:
                    filename= block.str_eval(tp.args())
                    if not isinstance(filename, str):
                        raise EP.ParseException(
                                block.posmsg("filename must be a string at",
                                              pos=tp.start()))
                    # create an instance of an IncludeBlock:
                    block= IncludeBlock(previous= block,
                                        new_scope= (tp.ident=="include_begin"),
                                        filename= filename,
                                        include_paths= include_paths)
            elif tp.ident=="template":
                if not block.skip:
                    # $template(...) :
                    # evaluate the filename of substfile:
                    filename= block.str_eval(tp.args())
                    # remember this filename in the current block object:
                    block.set_substfile(filename, tp)
            elif tp.ident=="subst":
                if not block.skip:
                    # $subst(...) :
                    # evaluate all the named arguments:
                    args= block.eval_("__pyexpander_helper(%s)" % tp.args())
                    # first argument may be the name of the substfile,
                    # this is undocumented and usually not used:
                    fn= args[0]
                    if fn is not None:
                        block.set_substfile(fn, tp)
                    # create an instance of a SubstBlock:
                    block= SubstBlock(previous= block,
                                      filename= block.template,
                                      include_paths= include_paths,
                                      external_definitions= args[1])
            elif tp.ident=="pattern":
                if not block.skip:
                    # $pattern(...) :
                    # create a tuple of all arguments:
                    args= block.eval_("(%s)" % tp.args())
                    firstindex= 0
                    # first argument may be the name of the substfile,
                    # this is undocumented and usually not used:
                    if isinstance(args[0], str):
                        block.set_substfile(fn, tp)
                        firstindex= 1
                    block= PatternBlock(previous= block,
                                        filename= block.template,
                                        include_paths= include_paths,
                                        heading= args[firstindex],
                                        lines= args[firstindex+1:])
            elif tp.ident=="default":
                if not block.skip:
                    # $default(...) :
                    # evaluate all the named arguments:
                    args= block.eval_("__pyexpander_helper2(%s)" % tp.args())
                    # set these defaults in the current block:
                    for (k,v) in args.items():
                        block.setdefault(k, v)
            elif tp.ident=="if":
                # $if(...) :
                # evaluate the condition:
                if not block.skip:
                    condition= block.eval_(tp.args())
                else:
                    # fake a condition:
                    condition= True
                # create an instance of an IfBlock:
                block= IfBlock(previous= block,
                               condition= condition)
            elif tp.ident=="elif":
                # elif(...) :
                # current block must be an IfBlock:
                if not isinstance(block,IfBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched elif at",
                                          pos=tp.start()))
                # evaluate the condition:
                if not block.prev_skip:
                    condition= block.eval_(tp.args())
                else:
                    # fake a condition:
                    condition= False
                # enter the "elif" part of the if block by
                # calling enter_elif:
                # pylint: disable=E1103
                block.enter_elif(condition)
                # pylint: enable=E1103
            elif tp.ident=="for" or tp.ident=="for_begin":
                # $for(...) or $for_begin(...) :
                # assume the the parameters form a valid list comprehension
                try:
                    # try to parse the arguments of $for():
                    for_parts= EP.scanPyIn(tp.args())
                except EP.ParseException,_:
                    raise EP.ParseException(
                            block.posmsg("error in %s command at" % tp.ident,
                                         pos= tp.start()))
                # create a list of loop items by using pythons
                # list comprehension mechanism:
                if not block.skip:
                    for_list= block.eval_("list(%s)" % for_parts[2])
                else:
                    for_list= []
                # create an instance of a ForBlock:
                block= ForBlock(previous= block,
                                new_scope= (tp.ident=="for_begin"),
                                value_list= for_list,
                                var_expr= for_parts[0])
                block.set_loop_var()
            elif tp.ident=="while" or tp.ident=="while_begin":
                # $while(...) or $while_begin(...) :
                # create an instance of a WhileBlock:
                if not block.skip:
                    expr= tp.args()
                else:
                    expr= ""
                block= WhileBlock(previous= block,
                                  new_scope= (tp.ident=="while_begin"),
                                  while_expr= expr)
            elif tp.ident=="nonlocal":
                if not block.skip:
                    # $nonlocal(...) :
                    try:
                        # try to parse the arguments of $nonlocal():
                        identifiers= EP.scanPyIdentList(tp.args())
                    except EP.ParseException,_:
                        raise EP.ParseException(
                              block.posmsg("error in \"nonlocal\" command at",
                                           pos= tp.start()))
                    # mark them in the current block as exported symbols:
                    block.export_symbols(identifiers)
            elif tp.ident=="extend":
                if not block.skip:
                    # $extend(...) :
                    try:
                        # try to parse the arguments of $extend():
                        identifiers= EP.scanPyIdentList(tp.args())
                    except EP.ParseException,_:
                        raise EP.ParseException(
                              block.posmsg("error in \"extend\" command at",
                                           pos= tp.start()))
                    # mark them in the current block as "extended" identifiers:
                    block.extend(identifiers)
            elif tp.ident in block.direct_funcs:
                if not block.skip:
                    # $user-function-extended(...)
                    # apply the function directly:
                    result.append(block.str_eval("%s(%s)" % \
                                  (tp.ident,tp.args())))
            else:
                # everything else is a ParseException, this shouldn't happen
                # since all ParsedItem objects that the expanderparser can
                # create should be handled here.
                raise EP.ParseException(
                        block.posmsg("unknown command \"%s\" at",
                                      pos= tp.start()))
            continue
        if isinstance(tp, EP.ParsedPureCommand):
            # a "pure" command, a command without arguments:
            ident= tp.string()
            if ident=="else":
                # $else :
                # current block must be an IfBlock:
                if not isinstance(block,IfBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched else at",
                                          pos= tp.start()))
                # enter the "else" part of the if block by
                # calling enter_else:
                block.enter_else()
            elif ident=="endif":
                # $endif
                # current block must be an IfBlock:
                if not isinstance(block,IfBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched endif at",
                                          pos= tp.start()))
                # go back to previous block:
                block= block.pop()
            elif ident=="endfor":
                # $endfor
                # current block must be a ForBlock:
                if not isinstance(block,ForBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched endfor at",
                                          pos= tp.start()))
                # test if we have to perform the loop block again:
                if not block.next_loop():
                    # no further loops, go back to the previous block:
                    block= block.pop()
                else:
                    # further loops, give the loop variable a new value:
                    block.set_loop_var()
            elif ident=="endwhile":
                # $endwhile
                # current block must be a WhileBlock:
                if not isinstance(block,WhileBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched endwhile at",
                                          pos= tp.start()))
                # test if we have to loop again. next_loop also resets
                # the position, if we have to loop again:
                if not block.next_loop():
                    # if loop condition is False, go back to previous block,
                    block= block.pop()
            elif ident=="begin":
                # $begin
                # create an instance of a BeginBlock:
                block= BeginBlock(previous= block)
            elif ident=="end":
                # $end
                # current block must be a BeginBlock:
                if not isinstance(block,BeginBlock):
                    raise EP.ParseException(
                            block.posmsg("unmatched end at",
                                          pos= tp.start()))
                # go back to previous block:
                block= block.pop()
            elif ident in block.direct_vars:
                # $user-variable-extended
                # if skip mode is off, insert the current value of the
                # variable. The current block can be used like a dict in order
                # to get values of variables:
                if not block.skip:
                    result.append(str(block.__getitem__(ident)))
            else:
                # if we are not in nobracket_vars mode, we have an
                # unknown command without arguments here:
                if not allow_nobracket_vars:
                    raise EP.ParseException(
                            block.posmsg("unknown command \"%s\" at" % ident,
                                          pos= tp.start()))
                else:
                    # if skip mode is off, insert the current value of the
                    # variable. The current block can be used like a dict in
                    # order to get values of variables:
                    if not block.skip:
                        result.append(str(block.__getitem__(ident)))
            continue
    if block.previous is not None:
        raise EP.ParseException(
              block.posmsg("unclosed block at"))
    return (result,block.globals_)

# pylint: enable=R0914,R0912,R0915,E0702

def processToPrint(parse_list, filename=None,
                   external_definitions=None,
                   allow_nobracket_vars= False,
                   include_paths=None):
    """returns the global dict."""
    # debug:
    # for elm in parse_list:
    #     print elm
    (result,exp_globals)= processToList(parse_list, filename,
                                        external_definitions,
                                        allow_nobracket_vars,
                                        include_paths)
    print "".join(result),
    return exp_globals

def expand(st, filename=None,
           external_definitions=None,
           allow_nobracket_vars= False,
           include_paths=None):
    """returns the global dict."""
    return processToPrint(parseString(st), filename,
                          external_definitions,
                          allow_nobracket_vars,
                          include_paths)

def expandFile(filename,
               external_definitions=None,
               allow_nobracket_vars= False,
               include_paths=None):
    """returns the global dict."""
    return processToPrint(parseFile(filename), filename,
                          external_definitions,
                          allow_nobracket_vars,
                          include_paths)

def _test():
    """perform the doctest tests."""
    import doctest
    print "testing..."
    doctest.testmod()
    print "done"

if __name__ == "__main__":
    _test()
