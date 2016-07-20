##########################################################################################
# Computer Engineering and Informatics Department, University of Patras                  #
# Operating Systems - Final Project                                                      #
# Fall 2011                                                                              #
# Kyriakos Ispoglou                                                                      #
#                                                                                        #
#                                                                                        #
#                               Makefile for my filesystem                               #
#                                                                                        #
##########################################################################################

CFLAGS = -g -Wall # set compiler flags
OBJS = main.o # set object files

# set 'make all' option
all: KiFS

#set 'make filesystem' option
KiFS: main.o
    g++ ${CFLAGS} -o KiFS main.cpp

#set 'make clean' option
clean:
    rm *.o        # delete object files
    rm KiFS # delete file system

# print the results
print:
    more Makefile main.cpp | enscript -2r -p listing.ps

##########################################################################################
#                                     End of Makefile                                    #
##########################################################################################
