cd ..
DESTHOST=goetzpf,pyexpander@frs.sourceforge.net
DESTPATH=/home/frs/project/p/py/pyexpander
scp dist/*.tar.gz $DESTHOST:$DESTPATH
scp dist/*.zip $DESTHOST:$DESTPATH
scp README $DESTHOST:$DESTPATH
