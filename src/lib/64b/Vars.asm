[bits 64]

VIDEO_MEMORY equ 0xb8000
COLOR equ 0xF000

A equ 65
B equ 66
C equ 67
D equ 68
E equ 69
F equ 70
G equ 71
H equ 72
I equ 73
J equ 74
K equ 75
L equ 76
M equ 77
N equ 78
O equ 79
P equ 80
Q equ 81
R equ 82
S equ 83
T equ 84
U equ 85
V equ 86
W equ 87
X equ 88
Y equ 89
Z equ 90

a equ 65 + 32
b equ 66 + 32
c equ 67 + 32
d equ 68 + 32
e equ 69 + 32
f equ 70 + 32
g equ 71 + 32
h equ 72 + 32
i equ 73 + 32
j equ 74 + 32
k equ 75 + 32
l equ 76 + 32
m equ 77 + 32
n equ 78 + 32
o equ 79 + 32
p equ 80 + 32
q equ 81 + 32
r equ 82 + 32
s equ 83 + 32
t equ 84 + 32
u equ 85 + 32
v equ 86 + 32
w equ 87 + 32
x equ 88 + 32
y equ 89 + 32
z equ 90 + 32

PERIOD equ 0x002e
COMMA equ 0x002c
FSLASH equ 0x002f
BSLASH equ 0x005c
SPACE equ 0x0020
AT_SIGN equ 0x0040
BRACKET_OPEN equ 0x0028
BRACKET_CLOSE equ 0x0029
STAR equ 0x002a
UNDERSCORE equ 0x005f
PLUS equ 0x002b
MINUS equ 0x002d
AND_SIGN equ 0x0026
PERCENT equ 0x0025
QUOTE equ 0x0022
EXCALAMATION equ 0x0021

MULTIPLY equ STAR
DIVIDE equ FSLASH

ZERO equ 0x0030
ONE equ ZERO + 1
TWO equ ZERO + 2
THREE equ ZERO + 3
FOUR equ ZERO + 4
FIVE equ ZERO + 5
SIX equ ZERO + 6
SEVEN equ ZERO + 7
EIGHT equ ZERO + 8
NINE equ ZERO + 9

NULL equ 0x0000
