#!/bin/mksh
#for 32-bit build is needed run the command from mingw32 comand prompt, otherwise no output file is created
#by calling build script without any parameters will source code compiled with all modules at once (takes more time if it's not fresh compilation) with given (default) settings
#by calling build script with parameters will they firest parsed (if something wrong script ends with printing help) and then is checked all if they are changed from last compilation 
#if the modele is changed from last compilation it is compiled into object file (.o file) and the variable SOURCE_IS_CHANGED is set to true
#if is variable SOURCE_IS_CHANGED set to true, then linking of all object file is run and generate output executable file

##########paths#########
WIN_MSYS2_PATH="/mingw64/bin"
WIN_CC_64="$WIN_MSYS2_PATH/gcc.exe -m64"	#msys2 Windows path
WIN_CC_32="$WIN_MSYS2_PATH/gcc.exe -m32"	#msys2 Windows path
LINUX_CC_32="gcc -m32"
LINUX_CC_64="gcc -m64"
CC=$LINUX_CC_64
GTK_LIBRARY_PATH="pkg-config --cflags gtk+-3.0"
GTK_LIBRARY="pkg-config --libs gtk+-3.0"
SQLITE_LIBRARY="pkg-config --libs sqlite3"
LIBGCRYPT="libgcrypt-config --libs --cflags"
LIB_PATH="$HOME/lib/"
DOXYCONF_FILE="doxyconf"
SRC_PATH="src"

OUTPUT="host-bk"
OUTPUT_DIR="bin"


##########modules########
MODUL[0]="main"
MODUL[1]="di"

####utility variable####
FORCE_BUILD=false
SOURCE_IS_CHANGED=false
LINK_ONLY=false
DOC_GENERATING_ONLY=false
DEBUG_MODE=false

function print_help
{
	echo "-h = print help"
	echo "-p win32/win64/linux32/linux64 = set build platform 32-bit or 64-bit for Linux or Windows operating system"
	echo "-c = clean, remove all build files"
	echo "-f = force build"
	echo "-l = link modules only without compilation" 
	echo "-do = generating doxygen documentation only"
	echo "-d = debug mode (add -g symbol inside gcc command)"
	exit 0
}


if [ $# -eq 0 ]; then
	#defailt compile command
	
	MODULES=""
	INDEX=0
    	while [ $INDEX -lt ${#MODUL[@]} ]; do
		MODULES="$MODULES $SRC_PATH/${MODUL[$INDEX]}.c"
		INDEX=$(($INDEX+1))
	done
#-mwindows - window only application
#-mconsole - consol and window application


	echo "Compiling with default settings (Linux 64-bit)."
	$($CC $MODULES -o $OUTPUT_DIR/$OUTPUT -g -Wall  -L$LIB_PATH I$LIB_PATH/include $GTK_LIBRARY_PATH $GTK_LIBRARY $SQLITE_LIBRARY $LIBGCRYPT -lc_string -lsettings_button -larray_list -lc_freq -lc_unit_lib -std=c11)

else
	#parse input parameters
	while [ $# -gt 0 ]; do
		if [ $1 = "-p" ]; then
			shift

			if [ $1 = "win32" ]; then
				CC=$WIN_CC_32
				GTK_LIBRARY_PATH=$WIN_MSYS2_PATH/$GTK_LIBRARY_PATH
				GTK_LIBRARY=$WIN_MSYS2_PATH/$GTK_LIBRARY
				SQLITE_LIBRARY=$WIN_MSYS2_PATH/$SQLITE_LIBRARY
				OUTPUT=$OUTPUT"_win_32.exe"
			elif [ $1 = "win64" ]; then
				CC=$WIN_CC_64
				GTK_LIBRARY_PATH=$WIN_MSYS2_PATH/$GTK_LIBRARY_PATH
				SQLITE_LIBRARY=$WIN_MSYS2_PATH/$SQLITE_LIBRARY
				GTK_LIBRARY=$WIN_MSYS2_PATH/$GTK_LIBRARY
				OUTPUT=$OUTPUT"_win_64.exe"
			elif [ $1 = "linux32" ]; then
				CC=$LINUX_CC_32
				OUTPUT=$OUTPUT"_linux_32"
			elif [ $1 = "linux64" ]; then
				CC=$LINUX_CC_64
				OUTPUT=$OUTPUT"_linux_64"
			else
				echo "Wrong input parameter -> $1"
				exit 1
			fi
		elif [ $1 = "-h" ]; then
			echo "Build script help:"
			print_help
		elif [ $1 = "-f" ]; then
			FORCE_BUILD=true
		elif [ $1 = "-l" ]; then
			LINK_ONLY=true
			FORCE_BUILD=false
		elif [ $1 = "-do" ]; then
			DOC_GENERATING_ONLY=true
		elif [ $1 = "-d" ]; then
			DEBUG_MODE=true
		elif [ $1 = "-c" ]; then

			if [ -d ./.src ]; then
				rm -r -v ./.src 
			fi
			
			if [ -d ./latex ]; then
				rm -r -v ./latex
			fi

			if [ -d ./html ]; then
				rm -r -v ./html
			fi 

			INDEX=0
			while [ $INDEX -lt ${#MODUL[@]} ]; do
				if [ -f ${MODUL[$INDEX]}.o ]; then
					rm ${MODUL[$INDEX]}.o -v
				fi


				INDEX=$(($INDEX+1))
			done

			if [ -f $OUTPUT_DIR/$OUTPUT"_win_32.exe" ]; then
				rm $OUTPUT_DIR/$OUTPUT"_win_32.exe" -v
			fi				

			if [ -f $OUTPUT_DIR/$OUTPUT"_win_64.exe" ]; then
				rm $OUTPUT_DIR/$OUTPUT"_win_64.exe" -v
			fi

			if [ -f $OUTPUT_DIR/$OUTPUT"_linux_32" ]; then
				rm $OUTPUT_DIR/$OUTPUT"_linux_32" -v
			fi

			if [ -f $OUTPUT_DIR/$OUTPUT"_linux_64" ]; then
				rm $OUTPUT_DIR/$OUTPUT"_linux_64" -v
			fi

			if [ -f $OUTPUT_DIR/$OUTPUT ]; then
				rm $OUTPUT_DIR/$OUTPUT -v
			fi
			
			exit 0
		else
			echo "Wrong input!"
			print_help
		fi

		shift
	done

	#compile only if is not only generated the doxygen documentation
	if [ $DOC_GENERATING_ONLY = false ]; then

		if [ ! -d ./.src ]; then
			mkdir ./.src
		fi
		
		BUILD_COMMAND="-L$LIB_PATH -I$LIB_PATH/include -lsettings_button -lc_freq -lc_unit_lib $($GTK_LIBRARY_PATH) $($GTK_LIBRARY) $($SQLITE_LIBRARY) $($LIBGCRYPT) -Wall -pthread -lconfig -std=c11 -lcomedi -lm  -larray_list_lib -lc_string"
		
		if [ $DEBUG_MODE = true ]; then
			BUILD_COMMAND="$BUILD_COMMAND -g"
		fi 


		#build moduels if they changed or in force build
		INDEX=0
		while [ $INDEX -lt ${#MODUL[@]} ]; do

			if [ $LINK_ONLY=false ]; then
				DIFFERENCE_H=""

				DIFFERENCE_C=$(diff $SRC_PATH/${MODUL[$INDEX]}.c ./.src/${MODUL[$INDEX]}.c)

				if [ -f $SRC_PATH/${MODUL[$INDEX]}.h ]; then 
					DIFFERENCE_H=$(diff $SRC_PATH/${MODUL[$INDEX]}.h ./.src/${MODUL[$INDEX]}.h)
				fi
		
				if [ ! -f ${MODUL[$INDEX]}.o ] || [ $FORCE_BUILD = true ] || [ ${#DIFFERENCE_C} -gt 0 ] || [ ${#DIFFERENCE_H} -gt 0 ]; then
					
					echo "Compiling modul ${MODUL[$INDEX]}.c"
					$($CC -c $SRC_PATH/${MODUL[$INDEX]}.c $BUILD_COMMAND)

					$(cp $SRC_PATH/${MODUL[$INDEX]}.c ./.src)

					if [ -f $SRC_PATH/${MODUL[$INDEX]}.h ]; then
						$(cp $SRC_PATH/${MODUL[$INDEX]}.h ./.src)
					fi

					SOURCE_IS_CHANGED=true
				fi
			fi

			BUILD_COMMAND="${MODUL[$INDEX]}.o $BUILD_COMMAND"
			INDEX=$(($INDEX+1))
		done
		
		#make output directory if not exists	
		if [ ! -d ./$OUTPUT_DIR ]; then
			mkdir ./$OUTPUT_DIR
		fi

		BUILD_COMMAND="-o $OUTPUT_DIR/$OUTPUT $BUILD_COMMAND"
	
		#execute the build command
		if [ $SOURCE_IS_CHANGED = true ] || [ $LINK_ONLY = true ]; then
		
			echo "Linking executable file."

			$($CC $BUILD_COMMAND)
	
		else
			echo "Modules was not changed. Build is not necessary. For force build use -f option.";
		fi
	fi

	#generating the doxygen documentation
	if [ $DOC_GENERATING_ONLY = true ]; then

		if [ ! -f $DOXYCONF_FILE ]; then
			#if tabu.sty not found install apt install texlive-latex-extra
			doxygen -g $DOXYCONF_FILE
		else
			echo "generating project documentation."
			doxygen $DOXYCONF_FILE && make pdf -C latex/
		fi

	fi
fi
