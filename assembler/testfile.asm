# this tests the assembler for every instruction
# use pound signs to comment if needed
nop
lxi b,0xbeef
stax b
inx b
inr b
dcr b
mvi b,0xef
rlc
dad b
ldax b
dcx b
inr c
dcr c
mvi c,0xef
rrc
lxi d,0xef
stax d
inx d
inr d
dcr d
mvi d,0xaa
ral
dad d
ldax d
dcx d
inr e
dcr e
mvi e,0xaa
rar
lxi h,0xbeef
shld 0xbeef
inx h
inr h
dcr h
mvi h,0xee
daa
dad h
lhld 0xbeef
dcx h
inr l
dcr l
mvi l,0xbe
cma
lxi sp,0xbeef
sta 0xbeef
inx sp
inr m
dcr m
mvi m,0xbe
stc
dad sp
lda 0xbeef
dcx sp
inr a
mvi a,0xbe
cmc
mov b,b
mov b,c
mov b,d
mov b,e
mov b,h
mov b,l
mov b,m
mov b,a
#
mov c,b
mov c,c
mov c,d
mov c,e
mov c,h
mov c,l
mov c,m
mov c,a
#
mov d,b
mov d,c
mov d,d
mov d,e
mov d,h
mov d,l
mov d,m
mov d,a
#
mov e,b
mov e,c
mov e,d
mov e,e
mov e,h
mov e,l
mov e,m
mov e,a
#
mov h,b
mov h,c
mov h,d
mov h,e
mov h,h
mov h,l
mov h,m
mov h,a
#
mov l,b
mov l,c
mov l,d
mov l,e
mov l,h
mov l,l
mov l,m
mov l,a
#
mov m,b
mov m,c
mov m,d
mov m,e
mov m,h
mov m,l
hlt
mov m,a
#
mov a,b
mov a,c
mov a,d
mov a,e
mov a,h
mov a,l
mov a,m
mov a,a
#
add b
add c
add d
add e
add h
add l
add m
add a
adc b
adc c
adc d
adc e
adc h
adc l
adc m
adc a
sub b
sub c
sub d
sub e
sub h
sub l
sub m
sub a
sbb b
sbb c
sbb d
sbb e
sbb h
sbb l
sbb m
sbb a
ana b
ana c
ana d
ana e
ana h
ana l
ana m
ana a
xra b
xra c
xra d
xra e
xra h
xra l
xra m
xra a
ora b
ora c
ora d
ora e
ora h
ora l
ora m
ora a
cmp b
cmp c
cmp d
cmp e
cmp h
cmp l
cmp m
cmp a
rnz
pop b
jnz 0xbeef
jmp 0xbeef
cnz 0xbeef
push b
adi 0xbe
rst 0
rz
jz 0xbeef
cz 0xbeef
call 0xbeef
aci 0xbe
rst 1
rnc
pop d
jnc 0xbeef
out 0xbe
cnc 0xbeef
push d
sui 0xbe
rst 2
rc
jc 0xbeef
in 0xbe
cc 0xbeef
sbi 0xbe
rst 3
rpo
pop h
jpo 0xbeef
xthl
cpo 0xbeef
push h
ani 0xbe
rst 4
rpe
pchl
jpe 0xbeef
xchg
cpe 0xbeef
xri 0xbe
rst 5
rp
pop psw
jp 0xbeef
di
cp 0xbeef
push psw
ori 0xbe
rst 6
rm
sphl
jm 0xbeef
ei
cm 0xbeef
cpi d8
rst 7