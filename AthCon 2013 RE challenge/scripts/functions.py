# -------------------------------------------------------------------------------------------------
#	Athcon 2013 reverse engineering challenge
#
# 	functions.py: Find the functions that correspond the checksums
#
# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
	print( "Starting...." )
	
	fn = 0x7C916788									# start from here
	
	while fn < 0x7C91CE40:							# for each function
		f = ""
		c = 0
		i = 0
		
		while Byte(fn) != 0:						# for each character
			f = f + chr(Byte(fn));
			b = Byte(fn) * 128;
				
			c = (c + b) & 0xffffffff
			c = (c << 7) | ((c >> 25) & 0x7f)
			c = (c ^ b) & 0xffffffff
			fn = fn + 1
			
		d = c * c
		chksum = ((d >> 32) + (d & 0xffffffff)) & 0xffffffff

		fn = fn + 1									# skip NULL byte
		
		# valid hash?
		if chksum == 0x946CE828 or chksum == 0x5F43B254 or chksum == 0xA7AFD948 or\
		   chksum == 0x848955AC or chksum == 0x67F17733:
			print( 'VM1', f, hex(chksum) )

		if chksum == 0x318A50B7 or chksum == 0x95AAF2E1 or chksum == 0xE27847F7 or\
		   chksum == 0x8AFA4D6D or chksum == 0xD2950638 or chksum == 0x25F2995D or\
		   chksum == 0x217A4264:
			print( 'VM2', f, hex(chksum) )			
		
# -------------------------------------------------------------------------------------------------
