cd ..
DESTHOST=goetzpf,pyexpander@frs.sourceforge.net
DESTPATH=/home/frs/project/p/py/pyexpander/rpm
scp dist/*.rpm $DESTHOST:$DESTPATH
