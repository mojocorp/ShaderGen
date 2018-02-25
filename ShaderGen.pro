######################################################################
# linux  : qmake -r -spec linux-g++
# macosx : qmake -r -spec macx-g++
# windows: qmake -r -tp vc
######################################################################
TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += source

