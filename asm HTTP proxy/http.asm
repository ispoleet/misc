; -------------------------------------------------------------------------------------- ;
; HTTP proxy - Version 1.1                                                               ;
;                                                                                        ;
;   COMPILE & LINK:                                                                      ;
;       nasm -f elf -l sock.lst sock.asm                                                 ;
;       gcc -o sock sock.o                                                               ;
;                                                                                        ;
;                                                                                        ;
;   This program sends HTTP requests, and stores the response into a file.               ;
;   First, program attempts to open a file with the HTTP request. HTTProxy               ;
;   reads packet and extracts IP address from host domain. If packet file                ;
;   does not exists, then program sends the default packet to google with                ;
;   a crafted query (my name).                                                           ; 
;                                                                                        ;
;   HTTProxy first receives HTTP response header, and then response body.                ;
;   There are 2 ways to find the length of the response body:                            ;
;   1. Content-Length field in response header contains the length of the                ;
;      body.                                                                             ;
;   2. HTTP 1.1 protocol supports chunked data.If Transfer-Encoding field                ;
;      in response header is equal to chunked, then we start receiving                   ;
;      chunks of the response. First we receive a hexadecimal which shows                ;
;      the length of chunked packet, followed from a newline character. Then             ;
;      follows the actual packet. The last chunk contains a zero number,                 ;
;      followed by a newline, and does not contains body.                                ;
;                                                                                        ;
;   IMPORTANT WARNING:                                                                   ;
;                                                                                        ;
;   Response file contains FULL HTTP response and NOT HTML code. This means that         ;
;   response needs additional processing (we have to remove http response header         ;
;   and numbers between chunks). We don't implement this processing here, to keep        ;
;   keep things simple. Also this job could be done by the end-user.                     ;
;                                                                                        ;
;   (NOTE 2: If page contains unicode characters, editor may failed to display)          ;
;                                                                                        ;
;                                                                                        ;
; 05/10/2012 - ispo                                                                      ;
;                                                                                        ;
; -------------------------------------------------------------------------------------- ;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                             ;
;                 data segment --> Initialized global data                    ;
;                                                                             ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .data
    ; ---------------------- info messages ---------------------
    msg1: db 72,84,84,80,32,112,114,111,120,121,32,119,114,105,116,116,101,110,
          db 32,97,115,115,101,109,98,108,121,32,102,114,111,109,32,97,110,32,
          db 105,110,115,97,110,101,32,99,48,100,51,114,46,13,10,13,10
    msg1len equ $-msg1  

    msg2: db 72,84,84,80,32,114,101,115,112,111,110,115,101,32,114,101,99,101,
          db 105,118,101,114,32,115,117,99,99,101,115,115,102,117,108,108,121,
          db 46,13,10,72,84,84,80,32,114,101,115,112,111,110,115,101,32,115,
          db 116,111,114,101,100,32,105,110,32,102,105,108,101,58,32
    msg2len equ $-msg2

    msg3: db 13,10,13,10,72,97,118,101,32,97,32,110,105,99,101,32,100,97,121,33,
          db 32,58,41,13,10,13,10
    msg3len equ $-msg3
    
    msg4: db 68,101,102,97,117,108,116,32,114,101,113,117,101,115,116,32,115,101,
          db 110,100,46,46,46,10
    msg4len equ $-msg4
    
    msg5: db 85,115,101,114,32,114,101,113,117,101,115,116,32,115,101,110,100,46,
          db 46,46,10
    msg5len equ $-msg5

    ; ---------------------- error message ---------------------
    error: db 'Something gone wrong... :(', 10, 10
    errlen equ $-error

    ; ------------------- matching substrings ------------------
    s1: db 'Content-Length: ', 0    
    s2: db 'Transfer-Encoding: chunked', 0
    s3: db 13,10,48,13,10,0
    s4: db 'Host: ',0

    ; ------------------- HTTP request packet ------------------
    HTTPreq: db 'GET /search?q=ispo HTTP/1.1',13,10
             db 'Host: www.google.com',13,10
             db 'User-Agent: Mozilla/5.0 (X11; Linux i686; rv:5.0.1) Gecko/20100101 Firefox/5.0.1',13,10
             db 'Proxy-Connection: keep-alive',13,10
             db 'DNT: 1',13,10,13,10
    HTTPlen: equ $-HTTPreq  

    ; ------------------- server information -------------------
    IP_ADDR: db '173.194.67.105',0  ; IP address to connect
    PORT   : dw 80                  ; port to connect

    ; ------------------- file information -------------------
    filein : db 'request.txt',0     ; file name to read request
    fileout: db 'response.txt',0    ; file name to store response
    foutlen equ $-fileout


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                             ;
;                 bss segment --> Unitialized global data                     ;
;                                                                             ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .bss
    buffer  : resb 4097             ; reserve 4096+1 bytes for buffer
    host    : resb 512              ; reserve 512 bytes for host name
    sockd   : resd 1                ; reserve a DWORD for socket descriptor
    filed   : resd 1                ; reserve a DWORD for file  descriptor


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                             ;
;                         text segment --> Main code                          ;
;                                                                             ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .text

    global main                     ; set entry point (main)

    extern htons                    ; host to network byte order
    extern inet_addr                ; IP address string to number
    extern memset                   ; memory initialization
    extern strstr                   ; substring searching
    extern atoi                     ; string to integer
    extern gethostbyname            ; convert domain to inet address


; -------------------------------------------------------------------- ;
;                                                                      ;
;   servconnect( inet_address ):                                       ;
;       This function connects socket to server.                       ;
;                                                                      ;
;   Return Value (in eax): 0 on success. -1 if an error occured        ; 
;                                                                      ;
; -------------------------------------------------------------------- ;
servconnect:
    push ebp                        ; store previous value of ebp
    mov  ebp, esp                   ; set up a new stack frame
    push ebx                        ; push registers to use
    push ecx
    push edx
    sub  esp, 20h                   ; reserve 32B in stack for local vars

    ; -------------------------------------------------------------------- ;
    ;                   connect socket to server                           ;
    ; -------------------------------------------------------------------- ;
    
    ; ------------------- call htons( PORT ) -------------------
    mov  ebx, [PORT]                ; ebx: port to connect
    push ebx                        ; port in stack 
    call htons                      ; host byte to network byte order
    mov  ebx, eax                   ; ebx: port to connect in big-endian
    add  esp, 04h                   ; pop argument from stack

    mov eax, [ebp+08h]              ; get inet address argument 
    mov ecx, 02h                    ; ecx = AF_INET = 2
    xor edx, edx                    ; clear edx
    
    ; -------------- set up sockaddr_in structure --------------
    ;   struct in_addr {
    ;       uint32_ t s_ addr;             /* Internet address */
    ;   };
    ;
    ;   struct sockaddr_in {
    ;       short            sin_family;   // e.g. AF_INET, AF_INET6
    ;       unsigned short   sin_port;     // e.g. htons(3490)
    ;       struct in_addr   sin_addr;     // see struct in_addr
    ;       char             sin_zero[8];  // zero this
    ;   };
    
    mov  [esp]    , cx              ; sin_family    = AF_INET 
    mov  [esp+02h], bx              ; sin_port      = htons(PORT)
    mov  [esp+04h], eax             ; sin_addr      = inet_addr(IP_ADDR)
    mov  [esp+08h], edx             ; sin_zero[0-3] = 0x00000000
    mov  [esp+0ch], edx             ; sin_zero[4-7] = 0x00000000
    
    ; ----------------------- connect() ------------------------
    mov  eax, [sockd]               ; eax = socket descriptor   
    mov  [esp+10h], eax             ; sockd in a[0] for connect()
    mov  [esp+14h], esp             ; sockaddr_in in a[1]
    mov  edx, 10h                   ; edx = 10h
    mov  [esp+18h], edx             ; sizeof(sockaddr_in) in a[2]
    mov  eax, 66h                   ; system call number --> sys_socketcall
    mov  ebx, 03h                   ; call = SYS_CONNECT
    lea  ecx, [esp+10h]             ; ecx  = address of a[]
    int  80h                        ; go to kernel
    nop                             ; eax contains return value of connect()

    pop edx                         ; pop registers from stack
    pop ecx
    pop ebx
    leave                           ; destruct stack frame. pop ebp
    ret                             ; return from function


; -------------------------------------------------------------------- ;
;                      Here's the main function                        ;
; -------------------------------------------------------------------- ;
main:   
    sub esp, 20h                    ; reserve 8 slots in stack for local vars

    ; ------------------- print init message -------------------
    mov  eax, 04h                   ; system call number  --> sys_write
    mov  ebx, 01h                   ; file descriptor = 1 --> stdout
    mov  ecx, msg1                  ; message pointer
    mov  edx, msg1len               ; message length
    int  80h                        ; go to kernel
    
    ; -------------------------------------------------------------------- ;
    ;                              create socket                           ;
    ; -------------------------------------------------------------------- ;
    mov  ebx, 02h                   ; ebx = 2
    mov  [esp+10h], ebx             ; a0 = PF_INET = 2;
    mov  ebx, 01h                   ; ebx = 1
    mov  [esp+14h], ebx             ; a1 = SOCK_STREAM = 1; 
    xor  ebx, ebx                   ; ebx = 0
    mov  [esp+18h], ebx             ; a2 = protocol = 0
    
    mov  eax, 66h                   ; system call number --> sys_socketcall
    mov  ebx, 01h                   ; call = SYS_SOCKET
    lea  ecx, [esp+10h]             ; ecx = address of a[]
    int  80h                        ; go to kernel
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().
    mov  [sockd], eax               ; Otherwise this is the socket descriptor

    ; -------------------------------------------------------------------- ;
    ;                     read HTTP request from file                      ;
    ; -------------------------------------------------------------------- ;

    ; ---------------------- open file -------------------------
    mov  eax, 05h                   ; system call number --> here sys_open
    mov  ebx, filein                ; socket descriptor
    xor  ecx, ecx                   ; enable O_RDONLY flag (read)
    mov  edx, 400                   ; permissions: r for owner, nothing for others
    int  80h                        ; go to kernel
    cmp  eax, 00h                   ; negative value returned?
    jl   @DEFPACKET                 ; If yes, send default packet
    mov  [filed], eax               ; else, store file descirptor

    ; ---------------------- read data -------------------------
    mov  eax, 03h                   ; system call number --> here sys_read
    mov  ebx, [filed]               ; socket descriptor
    mov  ecx, buffer                ; buffer
    mov  edx, 1000h                 ; buffer maximum length
    int  80h                        ; go to kernel
    mov  [esp+1ch], eax             ; message length

    ; --------------------- close file -------------------------
    mov  eax, 06h                   ; system call number --> sys_close
    mov  ebx, [filed]               ; set file descriptor
    int  80h                        ; go to kernel

    ; -------------- search for host signature -----------------
    push s4                         ; push substring to search for
    push buffer                     ; push buffer
    call strstr                     ; call strstr(buffer, s1)
    add  esp, 8h                    ; pop arguments from stack
    test eax, eax                   ; zero value returned?
    je   @DEFPACKET                 ; If yes, send default packet
    
    ; ------------------ extract host name ---------------------
    lea  esi, [eax+6h]              ; esi = host address (bypass host signature)
    mov  edi, host                  ; edi = start of empty buffer

@EXTRACTCHAR:
    mov  al, [esi]                  ; get 1 char from source
        
    cmp  al, 13                     ; is character a new line character?
    jz   @EXTRACTEND                ; if yes, stop
    cmp  al, 10                     ; is character a new line character?
    jz   @EXTRACTEND                ; if yes, stop
    
    mov  [edi], al                  ; otherwise store char to host name
    inc  esi                        ; move to the next source character
    inc  edi                        ; move to the next destination character
    jmp  @EXTRACTCHAR               ; continue
    
@EXTRACTEND:
    ; ------------------ host name extracted -------------------
    push host                       ; push host name in stack       
    call gethostbyname              ; call gethostbyname()
    add  esp, 4h                    ; pop argument from stack
    cmp  eax, 00h                   ; NULL pointer returned?
    je   @ERROR                     ; function failed. Abort()

    ; eax = pointer to hostent structure
    ;
    ; struct hostent {
    ;           char  *h_name;            /* official name of host */
    ;           char **h_aliases;         /* alias list */
    ;           int    h_addrtype;        /* host address type */
    ;           int    h_length;          /* length of address */
    ;           char **h_addr_list;       /* list of addresses */
    ;       }

    mov  eax, [eax+10h]             ; eax = h_addr_list
    mov  eax, [eax]                 ; eax = h_addr_list[0]
    mov  eax, [eax]                 ; eax = inet address!

    ; ------------------- connect to server --------------------
    push eax                        ; eax on stack
    call servconnect                ; connect to server
    add  esp, 4h                    ; pop argument from stack
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().

    ; ------------------- send http request --------------------
    mov  eax, 04h                   ; system call number --> sys_write
    mov  ebx, [sockd]               ; set socket descriptor
    mov  ecx, buffer                ; HTTP request buffer pointer
    mov  edx, [esp+1ch]             ; message length
    int  80h                        ; go to kernel
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().

    ; ------------------- print init message -------------------
    mov  eax, 04h                   ; system call number  --> sys_write
    mov  ebx, 01h                   ; file descriptor = 1 --> stdout
    mov  ecx, msg5                  ; message pointer
    mov  edx, msg5len               ; message length
    int  80h                        ; go to kernel

    jmp @SESSIONSTART               ; start session

@DEFPACKET:
    ; --------------- call inet_addr( IP_ADDR ) ----------------
    mov  edx, IP_ADDR               ; edx: address of IP_ADDR
    push edx                        ; IPv4 address in stack
    call inet_addr                  ; convert IPv4 to integer value 
    add  esp, 04h                   ; pop argument from stack

    ; ------------------- connect to server --------------------
    push eax                        ; eax on stack
    call servconnect                ; connect to server
    add  esp, 4h                    ; pop argument from stack
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().

    ; --------------- send default http request ----------------
    mov  eax, 04h                   ; system call number --> sys_write
    mov  ebx, [sockd]               ; set socket descriptor
    mov  ecx, HTTPreq               ; HTTP request buffer pointer
    mov  edx, HTTPlen               ; message length
    int  80h                        ; go to kernel
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().

    ; ------------------- print init message -------------------
    mov  eax, 04h                   ; system call number  --> sys_write
    mov  ebx, 01h                   ; file descriptor = 1 --> stdout
    mov  ecx, msg4                  ; message pointer
    mov  edx, msg4len               ; message length
    int  80h                        ; go to kernel


    ; -------------------------------------------------------------------- ;
    ;                            start session                             ;
    ; -------------------------------------------------------------------- ;

@SESSIONSTART:  
    ; --------------------- clear buffer -----------------------
    push 1001h                      ; buffer size: 4096+1
    push 00h                        ; NULL byte pad
    push buffer                     ; address of buffer
    call memset                     ; call memset()
    add  esp, 0ch                   ; pop arguments from stack

    ; --------------- receive response header ------------------
    mov  eax, 03h                   ; system call number --> here sys_read
    mov  ebx, [sockd]               ; socket descriptor
    mov  ecx, buffer                ; buffer
    mov  edx, 1000h                 ; buffer maximum length
    int  80h                        ; go to kernel
    mov [esp+1ch], eax              ; store length of response

    ; --------------------- create file ------------------------
    mov  eax, 05h                   ; system call number --> here sys_open
    mov  ebx, fileout               ; socket descriptor
    mov  ecx, 02h                   ; enable O_RDWR  flag (read/write)
    or   ecx, 40h                   ; enable O_CREAT flag (create)
    or   ecx, 200h                  ; enable O_TRUNK flag (truncate)
    mov  edx, 600                   ; set permissions: r/w for owner, nothing for others
    int  80h                        ; go to kernel
    cmp  eax, 00h                   ; negative value returned?
    jl   @ERROR                     ; If yes, abort().
    mov  [filed], eax               ; else, store file descirptor

    ; ----------------- write request to file  -----------------
    mov  eax, 04h                   ; system call number --> sys_write
    mov  ebx, [filed]               ; set file descriptor
    mov  ecx, buffer                ; HTTP request buffer pointer
    mov  edx, [esp+1ch]             ; message length
    int  80h                        ; go to kernel
    
    ; ---------------- Search for body length ------------------
    push s1                         ; push substring to search for
    push buffer                     ; push buffer
    call strstr                     ; call strstr(buffer, s1)
    add  esp, 8h                    ; pop arguments from stack
    test eax, eax                   ; zero value returned?
    je   @CHUNKED                   ; If yes, search for chunked body.
    

    ; -------------------------------------------------------------------- ;
    ;                        body length in header                         ;
    ; -------------------------------------------------------------------- ;
    add  eax, 10h                   ; bypass label
    push eax                        ; push sub-string in stack
    call atoi                       ; convert string to integer
    mov  esi, eax                   ; esi contains total length
    xor edi, edi                    ; clear edi

@LOOP1: 
    cmp  edi, esi                   ; current length > total length ?
    jge  @END                       ; If yes go ahead

    ; --------------------- clear buffer -----------------------
    push 1001h                      ; buffer size: 4096+1
    push 00h                        ; NULL byte pad
    push buffer                     ; address of buffer
    call memset                     ; call memset()
    add  esp, 0ch                   ; pop arguments from stack

    ; --------------- receive response header ------------------
    mov  eax, 03h                   ; system call number --> here sys_read
    mov  ebx, [sockd]               ; socket descriptor
    mov  ecx, buffer                ; buffer
    mov  edx, 1000h                 ; buffer maximum length
    int  80h                        ; go to kernel
    cmp  eax, 0                     ; error occured ?
    jle  @ERROR                     ; If yes, abort()
    add edi, eax                    ; add packet length to total length

    ; ----------------- write request to file  -----------------
    mov  edx, eax                   ; message length
    mov  eax, 04h                   ; system call number --> sys_write
    mov  ebx, [filed]               ; set file descriptor
    mov  ecx, buffer                ; HTTP request buffer pointer
    int  80h                        ; go to kernel

    jmp @LOOP1                      ; continue reading packets


@CHUNKED:   
    ; -------------------------------------------------------------------- ;
    ;                          chunked response ?                          ;
    ; -------------------------------------------------------------------- ;

    ; ------------- Search for chunked signature ---------------
    push s2                         ; push substring to search for
    push buffer                     ; push buffer
    call strstr                     ; call strstr(buffer, s1)
    add  esp, 8h                    ; pop arguments from stack
    test eax, eax                   ; zero value returned?
    je   @END                       ; If yes, go to the end

@LOOP2:
    ; --------------------- clear buffer -----------------------
    push 1001h                      ; buffer size: 4096+1
    push 00h                        ; NULL byte pad
    push buffer                     ; address of buffer
    call memset                     ; call memset()
    add  esp, 0ch                   ; pop arguments from stack

    ; --------------- receive response header ------------------
    mov  eax, 03h                   ; system call number --> here sys_read
    mov  ebx, [sockd]               ; socket descriptor
    mov  ecx, buffer                ; buffer
    mov  edx, 1000h                 ; buffer maximum length
    int  80h                        ; go to kernel
    cmp  eax, 0                     ; error occured ?
    jl   @ERROR                     ; If yes, abort()
    add  edi, eax                   ; add packet length to total length

    ; ----------------- write request to file  -----------------
    mov  edx, eax                   ; message length
    mov  eax, 04h                   ; system call number --> sys_write
    mov  ebx, [filed]               ; set file descriptor
    mov  ecx, buffer                ; HTTP request buffer pointer
    int  80h                        ; go to kernel

    ; ------------- search for chunked signature ---------------
    push s3                         ; push substring to search for
    push buffer                     ; push buffer
    call strstr                     ; call strstr(buffer, s1)
    add  esp, 8h                    ; pop arguments from stack
    test eax, eax                   ; zero value returned?
    je   @LOOP2                     ; If yes, read another packet

@END:
    ; -------------------------------------------------------------------- ;
    ;                          close  descriptors                          ;
    ; -------------------------------------------------------------------- ;    
    mov  eax, 06h                   ; system call number --> sys_close
    mov  ebx, [filed]               ; set file descriptor
    int  80h                        ; go to kernel

    mov  eax, 06h                   ; system call number --> sys_close
    mov  ebx, [sockd]               ; set socket descriptor descriptor
    int  80h                        ; go to kernel

    ; ----------------- print end-up messages ------------------
    mov  eax, 4h                    ; system call number  --> sys_write
    mov  ebx, 1h                    ; file descriptor = 1 --> stdout
    mov  ecx, msg2                  ; message pointer
    mov  edx, msg2len               ; message length
    int  80h                        ; go to kernel
    
    mov  eax, 4h                    ; system call number  --> sys_write
    mov  ebx, 1h                    ; file descriptor = 1 --> stdout
    mov  ecx, fileout               ; message pointer
    mov  edx, foutlen               ; message length
    int  80h                        ; go to kernel

    mov  eax, 4h                    ; system call number  --> sys_write
    mov  ebx, 1h                    ; file descriptor = 1 --> stdout
    mov  ecx, msg3                  ; message pointer
    mov  edx, msg3len               ; message length
    int  80h                        ; go to kernel

    jmp @FINISH
    
@ERROR:
    ; ------------------ print error message -------------------
    mov  eax, 4h                    ; system call number  --> sys_write
    mov  ebx, 1h                    ; file descriptor = 1 --> stdout
    mov  ecx, error                 ; message pointer
    mov  edx, errlen                ; message length
    int  80h                        ; go to kernel
    
    
@FINISH:
    mov  eax,1h                     ; system call number  --> sys_exit
    mov  ebx,0h                     ; set exit() argument to zero
    int  80h                        ; go to sys_call
        
;
;
; This is the end. Have a nice day! :)
;
;

