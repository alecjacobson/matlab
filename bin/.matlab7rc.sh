#
# usage:        .matlab7rc.sh
#
# abstract:     This Bourne Shell script is sourced by the matlab script
#		to obtain certain site/user dependent information
#		as explained below. The first occurrence of this file
#		in the directory list:
#
#			. 		(current)
#			$HOME		(home)
#			matlab/bin	(default location)
#
#		is used. Most of the time this file in the default location
#		need not be modified at all and nothing needs to be done.
#		However, if MATLAB does not work properly with the default
#		values then this file may need to be modified and different
#		values inserted.
#
#		Currently, the following variables appear in the code below.
#
#		ARCH			(machine architecture)
#		DISPLAY			(DISPLAY variable for X Window System)
#		LDPATH_PREFIX		(path(s) that appear at the start of
#					 LD_LIBRARY_PATH)
#		LDPATH_SUFFIX		(path(s) that appear at the end of
#					 LD_LIBRARY_PATH)
#		LD_LIBRARY_PATH		(load library path - the name
#					 LD_LIBRARY_PATH is platform dependent)
#		MATLAB			(MATLAB root directory)
#		MATLABPATH		(MATLAB search path)
#		SHELL			(which shell to use for ! and unix
#					 command in MATLAB)
#		TOOLBOX			(toolbox path)
#
#		NOTE: Run matlab -n to get the values used to run MATLAB.
#		      MATLAB is NOT executed.
#
#		Additional variables are used in the MATLAB script, but
#		to affect their behavior from this file requires an
#		understanding first of how they are determined in the
#		MATLAB script and then making code modifications to this
#		file.
#
#		The strategy behind the use of this file is to keep
#		the site wide changes in the matlab/bin/.matlab7rc.sh version
#		and have the individual user start with a copy in their
#		$HOME directory and modify it for their special circumstances.
#
#		IMPORTANT: Please understand that The MathWorks cannot
#			   anticipate every possible installation. If
#			   your situation does not fit into the current
#			   model of using this .matlab7rc.sh file then
#			   we would like to hear from you. Please
#			   contact The MathWorks Technical Support.
#
# note(s):	1. The default values are
#
#		   ARCH			(machine architecture)
#
#			This is the machine architecture determined by
#			the arch utility script.
#
#		   DISPLAY		(DISPLAY variable for X Window System)
#
#			This is set to "$DISPLAY" where DISPLAY is
#		        taken from the environment.
#		        
#		   LDPATH_PREFIX	(path(s) that appear at the
#					 start of LD_LIBRARY_PATH)
#
#			Enclose in single quotes to defer evaluation
#			to the MATLAB script.
#
#		   LDPATH_SUFFIX	(path(s) that appear at the
#					  end of LD_LIBRARY_PATH)
#
#			Enclose in single quotes to defer evaluation
#			to the MATLAB script.
#
#		   LD_LIBRARY_PATH	(load library path - the name
#					 LD_LIBRARY_PATH is platform
#					 dependent)
#
#			TABLE:
#
#				platform        variable name
#				--------        -------------
#				glnxa64         LD_LIBRARY_PATH
#				maci64          DYLD_LIBRARY_PATH
#
#		   NOTE: The final load library path determined
#			 in the MATLAB startup script is composed
#			 of:
#			
#       ------------------------------------------------------------
#       LDPATH_PREFIX:<matlab_additions>:LD_LIBRARY_PATH:\
#		      <system_additions>:LDPATH_SUFFIX
#       ------------------------------------------------------------
#
#			 This means to add paths between:
#			 1. <matlab_additions> and LD_LIBRARY_PATH
#			    put them in front of LD_LIBRARY_PATH
#			 2. LD_LIBRARY_PATH and <system_additions>
#			    put them at the end of LD_LIBRARY_PATH
#
#		   MATLAB		(MATLAB root directory)
#
#		   MATLABPATH		(MATLAB search path)
#
#			This is set to "$MATLABPATH" where MATLABPATH is
#			taken from the environment.
#
#		   SHELL		(which shell to use for ! or
#					 unix command in MATLAB)
#
#		        This is set to "$SHELL" where SHELL is taken from
#			the environment. If SHELL is empty or not defined
#			then MATLAB uses /bin/sh internally.
#
#		   TOOLBOX		(toolbox path)
#
#			This is set to "$TOOLBOX" where TOOLBOX is
#			taken from the environment.
#
# Copyright 1986-2016 The MathWorks, Inc.
#----------------------------------------------------------------------------
#
# Determine the arch.
#
#   -------------------------------------------------------------
#
    MATLAB_UTIL_DIR=
#
#   -------------------------------------------------------------
#
    if [ ! "$MATLAB_UTIL_DIR" ]; then
	MATLAB_UTIL_DIR=$MATLAB_UTIL_DIRdefault
    fi
#
    MATLAB="$MATLABdefault"
#
    . "$MATLAB_UTIL_DIR/arch.sh"
    if [ "$ARCH" = "unknown" ]; then
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    echo ''
    echo '    Sorry! We could not determine the machine architecture for your'
    echo '           host. Please contact:'
    echo ''
    echo '               MathWorks Technical Support'
    echo ''
    echo '           for further assistance.'
    echo ''
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        trap ""
        exit 1
    fi
#
#	IMPORTANT! Modify ONLY if you don't like the defaults after running
#		   MATLAB.
#
    case "$ARCH" in
	glnx*)
#----------------------------------------------------------------------------
	    AUTOMOUNT_MAP=''
	    DISPLAY="$DISPLAY"
	    ARCH="$ARCH"
	    TOOLBOX="$TOOLBOX"
	    MATLABPATH="$MATLABPATH"
	    SHELL="$SHELL"
	    LDPATH_PREFIX=''
#
# To always use the OpenGL libraries shipped with MATLAB uncomment the next
# line.
#
#           LDPATH_PREFIX='$MATLAB/sys/opengl/lib/$ARCH'
#
	    LDPATH_SUFFIX=''
#
	    if [ "$LD_LIBRARY_PATH" != "" ]; then
                LD_LIBRARY_PATH=$LD_LIBRARY_PATH
	    else
                LD_LIBRARY_PATH=
	    fi
#----------------------------------------------------------------------------
	    ;;
	mac*)
	    AUTOMOUNT_MAP=''
	    DISPLAY="$DISPLAY"
	    ARCH="$ARCH"
	    TOOLBOX="$TOOLBOX"
	    MATLABPATH="$MATLABPATH"
	    SHELL="$SHELL"
	    LDPATH_PREFIX=''
#
# To always use the OpenGL libraries shipped with MATLAB uncomment the next
# line.
#
#           LDPATH_PREFIX='$MATLAB/sys/opengl/lib/$ARCH'
#
	    LDPATH_SUFFIX=''
#
	    if [ "$DYLD_LIBRARY_PATH" != "" ]; then
                DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH
	    else
                DYLD_LIBRARY_PATH=
	    fi
#----------------------------------------------------------------------------
	    ;;
	*)
#----------------------------------------------------------------------------
	    AUTOMOUNT_MAP=''
	    DISPLAY="$DISPLAY"
	    ARCH="$ARCH"
	    TOOLBOX="$TOOLBOX"
	    MATLABPATH="$MATLABPATH"
	    SHELL="$SHELL"
	    LDPATH_PREFIX=''
#
# To always use the OpenGL libraries shipped with MATLAB uncomment the next
# line.
#
#           LDPATH_PREFIX='$MATLAB/sys/opengl/lib/$ARCH'
#
	    LDPATH_SUFFIX=''
#
	    if [ "$LD_LIBRARY_PATH" != "" ]; then
                LD_LIBRARY_PATH=$LD_LIBRARY_PATH
	    else
                LD_LIBRARY_PATH=
	    fi
#----------------------------------------------------------------------------
	    ;;
    esac
