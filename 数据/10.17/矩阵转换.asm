League of Legends.Scaleform::Render::Matrix2x4<float>::GetYScale+2CDD9 - 56                    - push esi

01336FAA    8BF1            MOV ESI,ECX
01336FAC    57              PUSH EDI
01336FAD    8B3D 88089003   MOV EDI,DWORD PTR DS:[3900888]
01336FB3    837E 0C 00      CMP DWORD PTR DS:[ESI+C],0
01336FB7    74 43           JE SHORT League_o.01336FFC
01336FB9    8D4424 1C       LEA EAX,DWORD PTR SS:[ESP+1C]
01336FBD    50              PUSH EAX
01336FBE    E8 ADFDFFFF     CALL League_o.01336D70                   ; 矩阵CALL
01336FC3    8D5424 10       LEA EDX,DWORD PTR SS:[ESP+10]
01336FC7    8BC8            MOV ECX,EAX
01336FC9    E8 D2361C00     CALL League_o.014FA6A0
01336FCE    F3:0F104424 10  MOVSS XMM0,DWORD PTR SS:[ESP+10]
01336FD4    8D5424 08       LEA EDX,DWORD PTR SS:[ESP+8]
01336FD8    8B4E 0C         MOV ECX,DWORD PTR DS:[ESI+C]
01336FDB    D946 38         FLD DWORD PTR DS:[ESI+38]
01336FDE    51              PUSH ECX
01336FDF  - E9 1C90C108     JMP 09F50000
01336FE4    90              NOP
01336FE5    F3:0F104424 18  MOVSS XMM0,DWORD PTR SS:[ESP+18]





011B12B7    8B8E 640C0000   MOV ECX,DWORD PTR DS:[ESI+C64]



01336FC3    8D5424 10       LEA EDX,DWORD PTR SS:[ESP+10]
01336FC7    8BC8            MOV ECX,EAX
01336FC9    E8 D2361C00     CALL League_o.014FA6A0
01336FCE    F3:0F104424 10  MOVSS XMM0,DWORD PTR SS:[ESP+10]


MOVSS XMM7,[esi]
subss xmm7,[a]0.5
mulss xmm7,[ecx] 151
addss xmm7,[eax]

MOVSS xmm2,BF333333 -0.6999999
subss xmm2,0.50
mulss xmm2,42
addss xmm2,[eax+4]



013343CD    F3:0F103E       MOVSS XMM7,DWORD PTR DS:[ESI]            ; ESI
float x;
movss xmm5,dword ptr ds:[1151C1Ch]
andps xmm1,xmm0?
orps xmm1,xmm2?
addss xmm7,xmm1
subss xmm7,xmm1
movaps xmm0,xmm7
subss xmm0,dword ptr [esp+10h]
cmpnless xmm0,xmm2
andps xmm0,xmm5
subss xmm7,xmm0
movss x,xmm7