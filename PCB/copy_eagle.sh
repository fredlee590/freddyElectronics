#!/bin/bash

if [ $# -ne 1 ]
then
	echo "need one argument and one argument only"
	echo "enter a project name"
	exit -1
fi

proj=$1

eagledir="/c/Users/Frederick/Documents/eagle"
projdir="eagle_projects/$proj/eagle_files"
devdir="$projdir/dev"
proddir="$projdir/prod"

mkdir -p $proj/eagle_files/dev &> /dev/null
mkdir -p $proj/eagle_files/prod &> /dev/null

cp $eagledir/$proj/eagle.epf $devdir
cp $eagledir/$proj/$proj.sch $devdir
cp $eagledir/$proj/$proj.brd $devdir

# bottom layer, silkscreen, and solder mask
cp $eagledir/$proj/$proj\.GBL $proddir
cp $eagledir/$proj/$proj\.GBO $proddir
cp $eagledir/$proj/$proj\.GBS $proddir

# top layer, silkscreen, and solder mask
cp $eagledir/$proj/$proj\.GTL $proddir
cp $eagledir/$proj/$proj\.GTO $proddir
cp $eagledir/$proj/$proj\.GTS $proddir

# drill information and outline
cp $eagledir/$proj/$proj\.GML $proddir
cp $eagledir/$proj/$proj\.TXT $proddir
cp $eagledir/$proj/$proj\.dri $proddir
cp $eagledir/$proj/$proj\.dxf $proddir

# specific to OSHPark
cp $eagledir/$proj/$proj\.GKO $proddir
cp $eagledir/$proj/$proj\.XLN $proddir
