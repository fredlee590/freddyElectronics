#!/bin/bash

if [ $# -ne 1 ]
then
	echo "need one argument and one argument only"
	echo "enter a project name"
	exit -1
fi

proj=$1

eagledir="/c/Users/Frederick/Documents/eagle"

mkdir -p $proj/eagle_files/dev &> /dev/null
mkdir -p $proj/eagle_files/prod &> /dev/null

cp $eagledir/$proj/eagle.epf $proj/eagle_files/dev
cp $eagledir/$proj/$proj.sch $proj/eagle_files/dev
cp $eagledir/$proj/$proj.brd $proj/eagle_files/dev

# bottom layer, silkscreen, and solder mask
cp $eagledir/$proj/$proj\.GBL $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.GBO $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.GBS $proj/eagle_files/prod

# top layer, silkscreen, and solder mask
cp $eagledir/$proj/$proj\.GTL $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.GTO $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.GTS $proj/eagle_files/prod

# drill information and outline
cp $eagledir/$proj/$proj\.GML $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.TXT $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.dri $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.dxf $proj/eagle_files/prod

# specific to OSHPark
cp $eagledir/$proj/$proj\.GKO $proj/eagle_files/prod
cp $eagledir/$proj/$proj\.XLN $proj/eagle_files/prod
