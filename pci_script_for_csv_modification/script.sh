#/bin/sh

echo "script run"


#while [ true ] 
#do
	FILE_IN="/media/stc/q_hot/data_out/$(ls /media/stc/q_hot/data_out/ | grep camera.csv)"
	
	cp -v $FILE_IN $HOME/tempdir

	FILE_OUT="$HOME/tempdir/$(ls $HOME/tempdir | grep camera.csv)"

	
	sed -i 's/\"//g' $FILE_OUT
	sed -i 's/č/c/g' $FILE_OUT

	cp -v $FILE_OUT /media/stc/q_hot/data_out/
	rm -v $FILE_OUT

#	sleep 1
#	echo "next iteration"
#done
