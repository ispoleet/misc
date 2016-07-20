## Capitalization Steganography

This is a little program to insert and extract secret in a message. The idea is to use the case
of the letters to encode one bit of information. For example from the word "kYRiakOs" we can
extract the number "01100010" (0x62) which is 'b' in ASCII. I did this program as a part of a
challenge in Software Security class (I know it's not stealthy at all).

___

## Examples

```
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
```

___