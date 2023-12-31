import base64
import os

from falcon import falcon
from qblock.block import *
from qblock.block_util import *
from qblock.chain import Chain
from qblock.const import *

from Crypto.Hash import SHAKE256

block = {
    "previous_hash": b"00000827b5ce5a2aaa843f4f3abb17439ddac693601e969d30b15c2e40f295c9",
    "timestamp": 1690285729.7122724
}

s = "30303030383164383237333032626537666166643833356533663563303234663634333338363264356661613739313532633036623061313561333261316562"
phash = ""
for i in range(0, len(s), 2):
    phash += chr(int(s[i:i+2], 16))

phash = phash.encode("utf-8")

# print(f"\033[95m[\033[4m\x1b[1;32;40mGenerating secret key...\x1b[0m\033[95m]\033[0m")
skb = b'eSL#}fPj90gMfj3e}I36e}I5`f_#B{fq;U8fq{U1e0qR=g@A#AfQ5sDeTI62e|&y{e1U&~eSw2~dw+m|goA{Fetvp{e}jL5fq;Q~etvy|e|>&@fq;F2fq;E{e1d*{fP#W~g?)a1hJb&7eS?C5dVGO_d47U_eu8~~fPi{{fqQ>_dw7F>e}aL4etdj`dVzq3f`Nj4gMNU0e|~_3fq{K}eSm(0fPjF1e}sX5e}aL5e|>y_e1L#^f_!_0dw+j?eTII0e}I03eR_O?fqHy_gnWR1et&*_dwqU?gMNI1fP;gAe}H*`fq{N~e1Us_g?)X2f`Nj8eSdp}eS&>~e|~^~dwzX@e1n64f_;O7f`oy9f`WU4frNX1e0+R?eSm;~fPjF0e}I2~etLd{gM@*5fqQ{}f_;C3e|Lg@eS?2{fO~>_e}R65eSms?e|~<1e}H{`fPQ~{hJSv2eS3j{d4GU`gM)j3eSUv_f_{I1fqi~|gM5F0e0qL=frEd7f`5R6dVqa=e}sR2fqQ>|et>~}eSv&>dwzU+eSm;}fPjO7d4hv}eSmy^dV+j_euH~|e}RI4e}aO7eS&~~g?)m5e13j`eSm*}f`5H~eS?B}fPj2}et~*@fPj8~gM5C0e}RO5gnxa8etUp_e|~#>e}#U4fP8;}eSUs`e1U#{e1(5~f_;8{fPH|1fPjF4e1n02e1C&{fPa2`gn)p9e13s^e|m#~gMfp9f`5N{fP;L0fP;H}fPZ{^fPjI5gnop4e}aU3gMWU3e|~*^e0qU@fPaC4eu963etmp@fO>_3fqj2|d3}3=d4GO?dwPI=eSv<0f`EE?fPQ{_e}9I6eS&;^e}R5}hJt^BhJ$>AfrEg6fP#O8e}930fP#F0et>&_fqj92e}8^|f`5O0e}a8|fPQ^{fPsK~f`58^e|dj@fPsR8et>>|e}8{@frNv6e}H^|e0_a@fqZ~|gMEE|dwP9-fq;L2eSdy}eSm{~e0_g;eSLp;eSUm=eS&;|e}RB~fqs5`gnxW~gMEB{eSm^}eu9C2f`5H@et&;{gMop6e}932e}RI0fq#O3fQEp9e0hL^e}8{^dVhs_e1U(0gMN5?etmm?dVzX;e1U#{e|~*>f_!~}eS~>_eu905e|&*{et&_2dV7F+d47I>d3}9=fq#U3fqZs=fr5pAfr5pAe0_X<gMxg5eS~~}dw_d>fPHy_dw+j_eSd>}fPH~}eS?32etm^}gnxs9dxCp_f`xm6eRqO{et>^~et?03fqsC0e}jE}eSU*_eSCj|e1Cs{eS(33fqZ{~eSv^|/eSCg@fq8#}fOvj@gM4~_e}I33eS&>|frNy9dVha;fq;I1euI91e1m^}fPR2`e}994g?@a3dVqa_frWp7fqsB`fqsF0e0qU`dVGL-et?64e1Cs^etv*`fq;F0e|>|1d4GO>et~;@f_;I3etv?2e13X;fP{sFdw+j@fP#L0fr5a6eS3a;gMNO0etv#^f_Z^@euRU2gM)#Cet>;{et>;|e}Q>^f_#8~f_r>@g@Ss4et&_4fPH*`fq{O2gnxj1dw+m_f`5C1e}IB~dV7C@gM)v9d4Pa_g?WI1e1Cj`gM55{dwzw2e|&s^dw+d?fPsI5gn@y8dw+X?e}960etds?fqsI3dwzU>d4Yw2eujU2gnfN}eSms{e1d^{hJAZ^fPa5|dw_$4e|ma-fq#I4e1Cv~e}sT}fO~*}fPsB|eSLp`fPjH}f`NR2g?@l~fPR30et>;}e}aC6e}#R1eSv_2f`EX6eSm^|f`EU4fO~>_fPI02fPi^>fPa90e|ms_cY%R{etmm^fPZ{`e}aL3fO~#@gN1#1e0zd=et>{_gMxX2e0qL+etdm=e|&v_e1CX_etCs`eSCa@fr5jDetU|8g?)T~eSms;e}R00dw+a=fP#H`fq;5~f`ER1f_Q&`e|vz0gMfj5e}95~eSU#~gnNF2etmv`et~~~fPsI2f_{B@e}H~}e13j_fqZy?fP;d7etv(2dw+j+e}8;|etm#}fP8*`eSv#^et&|1fq;U1f_!^{etv(0dwzj`gMoj6dwhF;e1Uz2fqZ;?e}903et&;_e}{d4etvv@eS&#_e|vy_fPa91fq;B{e}H~~eSm?0fq{U0fPs00e1L*{dVhO<fPIC2f_{X3et(03d4GI;fqjF9fr5pAfPR62dVYX?dwPF@e}j5}fqi>|gMxm7e}II3e}saAe|>*~e1d;}e13g=d3%O^fPjR4etLX=fq;R0eSmv@fP#F1e}8>}eSd#@eS?31fqr>@et~|2e}R8~f`Na5fO><0et?30e|vm@dVYR*hk|~9etv&{etv{|fqj04e1Lv_e|&m=e|&p^e}jL0fO>y?eS?30dx3$0eSmy@eS3g_fPsB`etdp?gL{H{e0hL>gnoU0eSmy{gMNX4eu95{fP{g8etCp~e}901e0zd~gMWR7etv*~etLR(frWj4dw_d^e}033f_#32e1m|0fqa2}e}sI1fqa60gL{5_fPH{}e|>&}f`NX5e|~^~eS3a@gn@s2e0hC=et>*{fO&v_et&v=fqZ~>eR+F<et~>^e|vy}e1U+4fPsO1et&;`e}0C5/VSaCCs7X$Yc6M8Gi<p0}T6<%RVtQnCa%F{pS%IiYW`JdSnr>NbmW8B7ah!C7V0MbBWO|cvbGUAngH>#hS$cYzQhKd$hJ|d0f0KTWn|_9cbB1eVj(>HZgp)pJTf<#{lV)<RV1QwMYru|TjEH=ZYJ_%qaGh|Mosma=jDv)Hd4;92f1R6WesYG9oON%ehL%-snQ>K7mvPj5U7m-Dbb51-mS?PweQa=OPKkk_o?WDbpl+d)ih_=>cZz#sd10e<c*Kx(SC@cOcZR8feQu+G#*0*fV}o>Piix3hWQ?MTZ=idQj)_vFnlOfRZm@otR(*1nnS5Vxcb0>8lZZ@mccXfVseX)tgqC%+P<}+Ie|U*mb%kztP@Iu&X;p!tcCLPPgO+fyQH*@8t%Q4piKBduU~;pMQ;33tZg6X*T6B4Kaj>F)Rik%hI(=V*e{^GiZgp#dqLq1$p@fT}Y?*pkcy?Klk&jV+V1rwFo0ENndWe*ujbD6hcWZ@)l7PB{YixpzTY{{MxP_c{d1-T#mVdmIYJOm6fsSy9f@_msoO+sNdt|4RoSU4Dmy(lPzL9cuUa(VXij{?umQQt9qIZ34On8WNi=v5YjXztDT#9m(kb7}{fqzz>n`A_ZZheG+X?%-=ig9j*SA}J8kAa4Rw{UD+MVnS{Z*zf_dTEV$UxIyfT8MaHjGUWhmv?x8W|)Y8TW@`ejA5voiLr-hX?K5-kd~fnl8KsYc%Fx_k(Rnxj(&nlh=+EliHBH}pP7hyeQ1Ebh+CYAdZ?&Rwx+DOrD=?sbZT6AgMpQDx__l@ba;<xTy~t8!F$4XgI<Dbi=}6Di-(1Lf_0OBg>;{2X>EpuoP}Fjc7j!Lk#>GqsEeF)q*q{HXoPy6R)u<GaD|&_S8S1giiC`SOmcgeY=eecXNp>gmu`G^fq9sPii4PEhggAyri4(ZXPRJgi+zrpo~wb6j)i`KpKEiFl|YthhHY)3R)=ACVqcQ1l6zitaz1l(VxWUdZgOpGgo}ZXc4CA~OM_REp@)2XM|y!<b(4*jVP=blcwvNlU1NWQe3@imm3)zQsD7t!mW*MBWTR%Mj!J81r)zg=h>wwqUx=rKgMWQedTV8ver0}+k%W+gm2k<QgKcnRfpvI_g@%ZVesqI~x>l5eW>k1@b$y9ni#?ZNhk}k@UZZiAfR?z0L~5Q-rjvMbnOJ{bX>M^^k#LDuWp-tBi+OT%NUCsrkB(lCi>hIxik@s^d}T&_nqh>#f00Xaad>!ZeQ<uhTWyYWUXq7|iE^WpYm1L|l!<AJZg*f$k#BUCZego!a#66D/f0~hlP-tGRXk&kUR6?_Uc7t+&SX+pLVpM;8X^(=CkGfcqZ+?_zTYy_amWr60i=|j~Uv`W@gL-w4t$s*)bYg#eUVKo9rHX`XWL{pIhI5&FY=?E6X@GZ<e5!|jdx~hQx@u=_q=%-FWujJXieQtPim`pBfPI`@n44Rsd2x`1Zh3i_c6xYorB|SFS9_dmXr5<8ouZPbbZcRSUT>6mU6gBdc7T|EeT$E#euaxeuvDPDfq+ejVpxBNm}Q56b&p1We2$fGX<>MUfns!NUzsqTt(8n{dUuq9m5pdooK})<WtMh=hM=3CXpe@4a&Cdmm6lvXnrBvfoOrLUU6MnHcWbMiVsnkQhE$1Nf|iwegpFpqrk$01d5&z3j#7Yib8bXlyK=0Pi;h`=ZHkn*kxjT`XR4u@d}x-Ge`jKDf|QJFc7LRGc9~ORhku+zX_;nzwvwQDZ;fbql7@|6iDZ0-oOOkbOm%i*e~Nw1Vr7DNZIys@d5C~}b%S+lo`7<3O=*36s*Hu5V~}HywQiSmi=<?Nh=gmlWPosbnSiF1dUjk<b!TUQg>G<}e`I)Zik@wIXJ>S*pJ;nsl89|)QILI6Nqt^^Yln4{e|3OjYe!|4Y+j?Pf^BAvc$lSaUYE6&Rhf!pm{FFCb98Q3gP3|(a%GZqp@(aNd3}C^e6nW3pm3FfQLLkZagl9fKy0Crv4o0`d31G4X^l0shG&R)W|&yGh+AQVf{ukokdtU_dzNf}fp%eKkdBIbjEcF9nt*O#h)Z{fbaaDkRFA5JRA_;InTCa(ZF;wFsfnIffv1ahd|P^GV~@CMW1L2TW^kW*d!1TyR&%JXpnG(ynrx)Sc5rcBmTGy4i<*LNnoeznXOCKUnx|x&m~@JHcBFQ4k7#e3nazn>bWM_zkYq-6e{+(AzjSeme1DC1jAyB(qq>E9Xk&ecfP_$pU~q+?nRTFDgj0Bqd31VtbDO@BlyP~9SCo8rTX|fbcy_W#id%t#ZD@zIXpME2Z+V|}e}0dqj(>NBKYFEEo`jKNc8gkiT6T?eW0rPjjDvAol3a&@g1=jYX=$EvZ;f`FZ<mFEjdzK4wVrT<gQTE;W@m|go{o-)Qecd0dXcJWjA>j_l!RTWoM?=OiG_7zNOOgTh>(z&c5#}0i)~P9oob9}jc;vlca?3Ek9vAhZftvbmRoLlijZP@o^y3@k$P)^g{qN@Xl|EIk7=8mKxSrndW~s$TuhKzZdrk4Z={Wrux?wIgHV=zVS%fLfs<W>d|b6eS!SY4jig?JNqLWsd25P$eR+CtZ*ps(lW=u;KaGJ{p>J!U'
sk = falcon.SecretKey(QBLOCK_N, polys=decode_secretkey(skb))
pk = falcon.PublicKey(sk)
pkb = encode_pubkey(pk.h)
# print("\033[95m[\033[0mDone\033[95m]\033[0m\n")

message = b"hello"
shake = SHAKE256.new()
shake.update(message)
mhash = shake.read(32).hex().encode("utf-8")
# phash = b'00000827b5ce5a2aaa843f4f3abb17439ddac693601e969d30b15c2e40f295c9'

b = Block(message, prevHash=phash, prevTimestamp=1690332161.5826924, height=2)
b.publicKey = pk
b.sign(sk)
b.mine(QBLOCK_DIFFICULTY, True)

# print(mhash)
print("---")
print(b.hash())
print(b.height)
print(b.message)
print(b.messageHash)
print(b.previousHash)
print(b.previousTimestamp)
print(b.proof)
print(encode_signature(sk.sign(phash + mhash)))
print(pkb)
print(b.timestamp)

# import multiprocessing as mp
# from ctypes import c_bool
# from time import sleep

# def f(running):
#     running.value = True
#     print("running")
#     sleep(1)
#     print("done")
#     running.value = False

# def g(flags):
#     sleep(4)
#     flags.clear()
#     sleep(4)
#     flags.set()

# if __name__ == "__main__":
#     running = mp.Value(c_bool, False)
#     val = mp.Event()
#     val.set()
#     s = mp.Process(target=g, args=(val,))
#     s.start()
#     i = 0
#     while True:
#         val.wait()
#         if not running.value:
#             t = mp.Process(target=f, args=(running,))
#             t.start()
#             running.value = True
#             i += 1
