#!/usr/bin/env python2
# --------------------------------------------------------------------------------------------------
# Capitalization Steganography
#
# This is a little program to insert and extract secret in a message. The idea is to use the case
# of the letters to encode one bit of information. For example from the word "kYRiakOs" we can
# extract the number "01100010" (0x62) which is 'b' in ASCII. I did this program as a part of a
# challenge in Software Security class (I know it's not stealthy at all).
#
# - ispo
# --------------------------------------------------------------------------------------------------
import sys
import argparse
import string

# --------------------------------------------------------------------------------------------------
def read_file( filename ):								# read a file into a buffer
	try: 
		fp   = open(filename, 'r')
		buf  = fp.read()		
		fp.close()

	except IOError:										# handle errors
		print "Could not read file:", filename
		sys.exit( -1 )									# abort

	return buf 											# return buffer

# --------------------------------------------------------------------------------------------------
def write_file( filename, buf ):						# write a buffer to a file
	try: 
		fp = open(filename, 'w')
		fp.write( buf )		
		fp.close()

	except IOError:										# handle errors
		print "Could not write file:", filename
		sys.exit( -1 )									# abort

# --------------------------------------------------------------------------------------------------
if __name__ == "__main__":

	''' =======================================================================
		parse arguments first 
	======================================================================= '''
	parser    = argparse.ArgumentParser()
	subparser = parser.add_subparsers()					# we need subparsers

	p_i = subparser.add_parser('insert',  help='Insert a secret into a message')
	p_e = subparser.add_parser('extract', help='Extract a secret from a message')
 	
	p_i.add_argument('-i', action='store', dest='file_in',   help='Input file',  required=True)
	p_i.add_argument('-o', action='store', dest='file_out',  help='Output file', required=True)
	p_i.add_argument('-s', action='store', dest='file_secr', help='Secret file', required=True)
	p_e.add_argument('-f', action='store', dest='file',      help='Input file',  required=True)

	p_i. set_defaults(action='insert')					# defaults for each subparser
	p_e. set_defaults(action='extract')

	result = parser.parse_args()						# do the parsing


	''' =======================================================================
		run program in "extract" mode
	======================================================================= '''
	if result.action == 'extract':						# extract mode?
		print 'Extracting secret message from %s...' % result.file
		print 
		
		secret = []

		for ch in read_file(result.file):				# process input
			if ch in string.ascii_lowercase:			# if character is lowercase
				secret.append(0)						# append zero

			elif ch in string.ascii_uppercase:			# if character is uppercase
				secret.append(1)						# append one


		# secret is a binary stream. Convert back to text as follows:
		#	[1]. Split it into (sub)lists of 8 digits (each digit is a string)
		#	[2]. Join (sub)lists to a binary string
		#	[3]. Convert binary string to integer then to character
		#	[4]. Join the remaining list to get the secret
		secret = ''.join( [ chr(int(''.join(str(x) for x in secret[i:i + 8]), 2)) 		\
												   for i in range(0, len(secret), 8)] )

		print '----- BEGIN SECRET MESSAGE -----'		
		print secret
		print '----- END SECRET MESSAGE -----'

		sys.exit( 0 )									# success!


	''' =======================================================================
		run program in "insert" mode
	======================================================================= '''
	text   = list(read_file(result.file_in).lower())	# cast to lowercase and convert it to list
	secret = ''.join(format(ord(x), '08b') for x in read_file(result.file_secr))
	j      = 0
		
	# secret is the binary strean of all characters in secret file

	print 'Inserting secret message to %s...' % result.file_in

	for i in range(len(text)):							# for each character
		if text[i] in string.ascii_letters:				# if it's a letter
			
			if secret[j] == '1': 						# and bit is set
				text[i] = text[i].upper()				# flip letter to uppercase

			j = j + 1									# move on the next secret bit
			if j >= len(secret):						# prevent overflows
				break

	if j < len(secret):									# input not big enough?
		print 'Input is not big enough (%d < %d)'	 % (j, len(secret))
		sys.exit( -1 )									# abort
	
	write_file(result.file_out, ''.join(text))			# write new message to output file
	print 'Done.'

	sys.exit( 0 )										# success!

# --------------------------------------------------------------------------------------------------
'''
* * * INSERT A MESSAGE * * *
ispo@xrysa:~/stego$ cat inp.txt 
	This is a sample text (from me) that we're going to insert our hidden message!
ispo@xrysa:~/stego$ cat secr.txt 
	_ispo_
ispo@xrysa:~/stego$ ./capitalization_stego.py insert -i inp.txt -s secr.txt -o out.txt
	Inserting secret message to inp.txt...
	Done.
ispo@xrysa:~/stego$ cat out.txt 
	tHiS IS A SaMPlE teXt (FROm mE) ThAT We're goINg TO INsErT OUR HiddeN mEssage!


* * * EXTRACT A MESSAGE * * *
ispo@xrysa:~/stego$ ./capitalization_stego.py extract -f out.txt 
	Extracting secret message from out.txt...

	----- BEGIN SECRET MESSAGE -----
	_ispo_

	----- END SECRET MESSAGE -----
'''
# --------------------------------------------------------------------------------------------------
