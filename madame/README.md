## Madame De Maintenon’s Cryptographic Pursuit – Unmasking the Traitors

The challenge description can be found
[here](https://hex-rays.com/blog/madame-de-maintenons-cryptographic-pursuit-unmasking-the-traitors/).

___

This is a brief solution. Challenge consists of **5** stages.
On each stage we are asked to provide an order on "what to do next". 
If this is correct, program decrypts (using `AES` the text for the next stage.

**Stage 1:** Compares input directly: `memcmp("Head to the library", inp, 0x13uLL)`

**Stage 2:** Function 0x402560 XORs input with a constant, random key and compares it against a
known ciphertext. We XOR ciphertext and key to get the plaintext: `Check books on the next shelf`

**Stage 3:** Function `0x401300` applies a random operation (`+`, `-`, `^`) to each character of
the input with a random number and compares the result (for example `if ( (a1_inp[8] + 9) == 113 )`.
We parse the decompiled code line by line to extract the operator and the constants.
Then we invert the operations (`+` becomes `-`, `-` becomes `+` and `^` remains the same and we
recover the input: `Search the book for clues`

**Stage 4:** Function `0x4016F0` uses the answer from the previous stage (`#3`) as a **256**-bit
**AES** in **ECB** mode to decrypt a ciphertext and compares it against our input. We simply set a breakpoint at `strncmp` at `0x40179E` to grab the result: `Turn over the page`

**Stage 5:** This is the trickiest of all. After each stage is passed successfully,
function `0x4021E4` is invoked to encrypt (using `RSA`) the input from the first stage
(`"Head to the library"`). The exponent `e` is **3** and the modulo `n` is also known.
If the encrypted result matches a given ciphertext a global flag at `0x408488` is set.
On the next stage, before we read the input in function `0x402370`, we assign a different
value to `n`: `if ( glo_flag_C ) qmemcpy(glo_rsa_n, a678dcc64ccf7c2, sizeof(glo_rsa_n));` 
After we read the input, we also assign a new value to the ciphertext `c`, and we repeat the same process. 
In total we encrypt the first input with **3** different modulus (`n`) and we check it against **3**
different ciphertexts. Our goal is to find the correct input. To solve this, we use
**CRT (Chinese Remainder Theorem)** and we quickly recover `message^3`. Then we get the
**cubic root** of it and get the final message which is:
`Head to the library. Upon entering, politely ask the librarian if they are aware of any extra documents refering to Madame De Maintenon.`. This is actually the input we have to give to stage #1.


The above inputs give us **5** pages from the dairy. Each page contains a binary number:
`01000010`, `00110111`, `10110010`, `00000101`, `01000000110111000011011000000000`.
The hint says these are floating point numbers and correspond to coordinates.
We concatenate the first four and we convert them into a float (we convert it to `int`,
we pack it and then we unpack it as float).
We do the same for the second: **(45.92384719848633, 6.881591796875)**.
Finally, we truncate the coordinates to 4 decimals: `45.9238, 06.8815` and we ensure its MD5 hash is
`fe72f3730186f24d983a1c2ed1bc1da7`.

Below is the complete program output:
```
┌─[00:21:26]─[ispo@ispo-glaptop2]─[~/ctf/madame]
└──> ./madame
You have heard rumours that the diary of Madame de Maintenon contained the secrets to a legendary plot.
Good luck on your journey to uncovering the truth behind this mystery!
You have heard that a rival historian recently discovered a copy of a chapter of the diary of Madame de Maintenon at the local library. But being unable to solve the mystery, returned it in frustration. Having long been fascinated by her history, you can't wait to investigate. What do you do?
Head to the library. Upon entering, politely ask the librarian if they are aware of any extra documents refering to Madame De Maintenon.

You locate the section of the library where the diary was rumoured to have been stored, but its place is empty. After a few minutes browsing, you find it! A single page, but one that holds the key to a fascinating mystery.

The page reads:
_______________
21 October 1684

Dear Diary,

Today, an unsettling discovery came my way. A letter, it was, with ominous tidings of a plot against our cherished Louis XIV. The message was unlike any other, its meaning hidden behind unfamiliar symbols.

Within the letter lay a clue, 01000010, a piece of the puzzle. It hinted at more secrets, and I felt compelled to uncover them. But where to find the next piece?

Yours in devotion,

Madame De Maintenon
_______________

The page was lying on the shelf in the open, maybe it fell from somewhere. You see a few more loose pages sticking out of some other books around you. What happened here?


What do you do?

Check books on the next shelf

What luck! While going through the books on the next shelf over, you find another page stuck under them, similarly weathered to the first one. The message is hard to decipher due to it's age, but after some careful analysis you manage to decode it.

It reads:
_______________
24 October 1684

Beloved Diary,
 
As I delved into the code, a new piece surfaced, 00110111. It whispered of hidden truths, yet it also hinted that there was more to uncover. The puzzle remained incomplete.

Yours eternally,

Madame De Maintenon
_______________

Another clue, what could it mean? And where are the rest?

What do you do?

Search the book for clues

From the lack of dust on the book you found, it's clear these were recently borrowed. Maybe the pages got mixed up with the books when being reshelved?

You look up the name of the last borrower, and look up what other books they may have checked out. There you find the diary records mentioned, as well as one other book. 
Finding that book on the shelves yields another page!

_______________
30 October 1684

Dearest Diary,

Another fragment emerged, 10110010. It was a step closer to the full picture, but it also held a hint. The rest of the location, it suggested, was not far away.

Yours eternally,

Madame De Maintenon
_______________



What do you do?

Turn over the page

Turning the page over, you find the final entry to the diary!

_______________
9 November 1684

Beloved Diary,

Today, the last piece fell into place, 00000101. With it came the realization that the remaining location lay elsewhere, a mystery yet to be unraveled. Our mission is clear, my dear diary; we must decipher the rest to protect our homeland.

Yours in devotion,

Madame de Maintenon
_______________

What does this mean? You've worked so hard but yet still don't have the information you seek? What now?
You have all four pages your rival claimed to have found, and yet are no closer to the truth.
After several hours of fruitlessly searching for meaning in the messages, you give up and turn to leave in defeat.


As you move to leave, the librarian comes running!

'I found this in the back room for you, it was a page we found lying around after procesing the most recent batch of new books but we weren't sure what it was for! But look at the signature!'

She hands you a fifth, almost completely blank new page. The aging of the paper looks near identical to the other four pages you found from the diary!

All the page says on it is:
_______________

The other key:

01000000110111000011011000000000

M d. M
_______________

You thank the librarian, and take your leave. You have much to think on. All these 1's and 0's, how do they encode the location of the final target???

#########################

Congratulations! If you've found all 5 pages of the diary you have everything you need! Convert the values you found into coordinates, (hint: IEEE-754 Floating Point), and send those coordinates in an email to marketing@hex-rays.com!
To locally verify your coordinates, the md5 of the coordinates, with 4 decimal places, (including potential leading zeros) in the form:
xx.xxxx, yy.yyyy
Has an md5 of fe72f3730186f24d983a1c2ed1bc1da7 when pasted as a 16 character string into https://www.md5hashgenerator.com/
```
___
