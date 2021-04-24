#!/bin/sh
FLOPPY=/dev/fd0
DESTDIR=/home/sdisks
DISKNAME='disk%03d'
num=1

test -d $DESTDIR || mkdir $DESTDIR
while true; do
	read -p "Another disk y/n? " ans
	if [ "$ans" = "n" ]; then break; fi
	outfile=$DESTDIR/`printf $DISKNAME $num`
	dd if=$FLOPPY of=$outfile.img 
	./s550util -l $outfile.img >$outfile.txt
	let num=num+1
done	
