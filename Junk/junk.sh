#!/bin/bash
###############################################################################
# Author: Julia Nelson
# Date: May 26, 2021
# Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
# Description: Simple bash script to provide the basic functionality of a recycle bin.
#				Move files into the recycle bin directory, must be able to list and
#				purge the files that have been placed into the recycle bin.
#				Substitute for the rm command (able to recover files deleted)
###############################################################################


readonly JUNK="/home/user/.junk"

help_flag=0
list_flag=0
purge_flag=0

flag_count=0

usage_message(){
	cat << ENDOFFILE
Usage: $(basename $0) [-hlp] [list of files]
   -h: Display help.
   -l: List junked files.
   -p: Purge all files.
   [list of files] with no other arguments to junk those files.
ENDOFFILE
}

#test_message(){
#   cat << ENDOFFILE
#test: $0 [-hlp] [list of files]
#ENDOFFILE
#}

# getopts for flags hlp
while getopts ":hpl :" option; do                                    #for some reason i need a space after the l for it to be counted as an option
    case "$option" in
      h) help_flag=1;;
      l) list_flag=1;;
      p) purge_flag=1;;
      ?) echo "Error: Unknown option '-$OPTARG'.\n" >&2		
         usage_message
         exit 1
         ;;
    esac
done



# How many flags are there?
flag_count=$(( help_flag + list_flag + purge_flag ))

### use for more than one valid flag entered
### display error mesage and usage
if [[ $flag_count > 1 ]]; then                                       # possibly change to -gt
    echo "Error: Too many options enabled."
    usage_message
    exit 1           
fi



#Possibly change to $JUNK
if [[ ! -d "$JUNK" ]] ; then                                         #### CHECKs IF .junk directory IS ALREADY MADE
   #cd ..
   mkdir "/home/user/.junk"
fi


# captures all files provided as arguments.
for FILE in ${@}
do
   if [[ $flag_count -eq 0 ]] ; then
      if [[ ! -f $FILE ]] && [[ ! -d $FILE ]] ; then                 # added if there are no flags specified and the other arg is not a file or directory, give warning
            echo "Warning: $FILE not found."
      fi
      if [[ -f $FILE ]] || [[ -d $FILE ]]; then                      # if arg IS a file or directory and no flags move that file to .junk
         mv "$FILE" "$JUNK"
         exit 0
      fi
   fi
   if [[ $flag_count -gt 0 ]] ; then                                 #flags and files error
      if [[ -f $FILE ]] ; then
            echo "Error: Too many options enabled."
            usage_message
            exit 1     
      fi
      
   fi
done



### FLAGS 

if [[ $help_flag -eq 1 ]] && [[ $flag_count -eq 1 ]] ; then 
   usage_message 
   exit 0 
fi


if [[ $purge_flag -eq 1 ]] && [[ $flag_count -eq 1 ]] ; then
   cd ..
   rm -rf "$JUNK"    # f necessary to skip prompts 
   mkdir "$JUNK"
   exit 0
fi


if [[ $list_flag -eq 1 ]] && [[ $flag_count -eq 1 ]] ; then  
   ls -lAF ../.junk
   exit 0
fi




