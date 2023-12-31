import base64
import os
import pymongo
import multiprocessing as mp

from ctypes import c_bool
from falcon import falcon
from pymongo.errors import ConnectionFailure, NotPrimaryError, PyMongoError
from time import sleep
from qblock.block import Block, GenesisBlock
from qblock.block_util import encode_secretkey, decode_secretkey, encode_pubkey, decode_pubkey, block_to_dict
from qblock.chain import Chain
from qblock.const import *

def block_insert_listener(qblock, green_flag, execute_flag):
    stream_wait_time = 1
    stream_max_retry = 30
    for i in range(0, stream_max_retry):
        try:
            with qblock.watch([{"$match": {"operationType": "insert"}}]) as stream:
                for insert_change in stream:
                    if insert_change["ns"]["coll"] == "cold":
                        green_flag.value = True
                        execute_flag.set()
                    elif insert_change["ns"]["coll"] == "hot":
                        green_flag.value = False
                        execute_flag.set()

        except NotPrimaryError:
            print(f"NotPrimaryError: backoff for {stream_wait_time} seconds")
            sleep(stream_wait_time)
            if stream_wait_time < 90:
                stream_wait_time *= 1.56828

        except PyMongoError:
            print("PyMongoError")
            break

def work(msg, prevHash, prevTimestamp, sig, pk, height):
    print(f"\033[95m[\033[4m\x1b[1;32;40mCreating new block...\x1b[0m\033[95m]\033[0m")
    b = Block(msg, prevHash=prevHash, prevTimestamp=prevTimestamp, height=height)
    pkh = decode_pubkey(pk)
    b.publicKey = falcon.PublicKey(h=pkh, n=QBLOCK_N)
    b.signature = sig
    print("\033[95m[\033[0mDone\033[95m]\033[0m\n")
    # print("\033[95m[Block detail]\033[0m\n\n")
    # print(b1)
    proof = b.mine(QBLOCK_DIFFICULTY, verbose=False)
    print("\n\n\033[93mNonce found!! 🎉\033[0m")
    print(proof, "\t", b.hash(), "\n")
    return b

def mine_block(block, cold, hot, meta, is_mining, execute_flag):
    meta_block = meta.find().sort("height", pymongo.DESCENDING).limit(1)[0]
    del block["_id"]
    mined = work(block["message"],
                 meta_block["latest_hash"],
                 meta_block["latest_timestamp"],
                 block["signature"],
                 block["public_key"],
                 meta_block["height"]+1)
    mined = block_to_dict(mined)
    result = cold.delete_one({"signature": block["signature"]})
    hot.insert_one(mined)
    meta.update_one(
        {
            "_id": meta_block["_id"]
        },
        {
            "$inc": {
                "cold_size": -1,
                "hot_size": 1
            },
            "$set": {
                "height": mined["height"],
                "latest_hash": mined["_id"],
                "latest_timestamp": mined["timestamp"]
            }
        }
    )
    if result.deleted_count == 1:
        execute_flag.set()
        is_mining.value = False
    else:
        raise RuntimeError("Cold delete failed")
    return True

skb = b'e0h0&etd&{e|dp@eS&{|e}R8|f`5R3e}965etvp@gMWm6gM5B|gn)s7g@TBJe}I2}gMom0eSUy`fq;R4e}H>|fqsC1e1Cj{eSdp?eSdd-dV_w0etms>fPR5~e|>#?e}Q^{eSdw1dx3s{eSLd?fqQ;{e|~{{e13m{fP;U3es_d?etUy|gnoa3f`5gFdw+s|eSd(0eSdp@dxU&~e0_j{eS?8~eS?2}et&>_g?@g2e}Q^^g?fE~f`WX2dVzp}f_{8?gM)&9et&#}fPjB}dwYO>fqi~}e}H{^et>^{e}8;|f`Nj8fPQ^_dwhX?e0+m@eSLp>gnxv2eujj4hJSm0e}H;|gM@;Cf_r><e}RK~fqsC1fP8?2et>#@e|mm^eSv_0eu92`fqj90frEg3fPH>{fPH>>fqsF0etdm<e}I93f`oj1et&;~euRC5fq{I1dwPF>e|~*@e|&*{fPZ{^cY1$*fPsN~gMWm5e|>#|gL!~`dx3g?eSd;}e|~^{e}I61et~^{go1&0f`5T}e|v#`fqZ~}dwzq0e}H~}fPQ*?et>|0eSUv`e|~{}fq{F0g@S*7f_`{{fPH&|eSLm@f`W#Bgn)d0fq#O3gN1^BdwqL-f`Ea3fPQ~}etdz1e}8?0et>^{etm$3eS?01e1C&~frEj7etv_4f`NZ`et?31fqsF2e}8>{gMoj2fq{dAfPsR5eSv~}et~;{dVzp~e1Cp{gnWL4dVzs_fPR2@hJAj1et&v_et>{`fPQ~}fPH|1et>&|frEj5fP8#`fPa2~e}92}e|&y^e}IC6fqr{`etUj@d4Yj|e1n2}etvv^fr5a7fqjC4eSU#{e}#j7fP#R5eSdy^e1U&}fq#I4gMWX1euI5`fP8|0et&?0fPa63fPj01fP#L4d4GX{eSv#`e}8#@gL;F1eu95~etmp@fq#L4dw_j@fr5m8e|~{~e|>*{fPa60dwPR^cz}F>dx3&|fPsdCfPaE~gL{C0dVzp`eu95}eSLj<e}992fq#I5eS>{|e0YF<etdv^dxU*`e}8^{gMNQ|fP8v<dVzg`eS&_0dw+g|f`5H`gMfd1dVzd@e|>{}gMWU5e}R5~dwhn1dwYR>fPaB{eSv&>etmz0e}jX5fqs5}e|>|5go1*7e}8?0e|vj_f_!>|dxV06e}8^~eu8>`eSCj=f`Wj4fPQ{{d4YU<eSCp?fP8;`fPj8{fPj92eSU#|eSd#_e|vv?c!Ps~eSCs`f`Na2e}8~|etCX=fO~*}eSm;|eSCz5e|~|0dVPL@fqQ>_etv#|dxC;~/dw_j@f`Ea2f_{5|eSLg_f_#B~fqj90fPR00fPjL5fq#H|gMfd3gM@vGet&>|dVPI=dWL<1eR_X=gMNI0etUv~fq;I5gMWa8e0+X?gMWT~eS>~_eSdm`d4GO;dVYF>gMfW~czc0=e0_d@eu933e}a5`d4qd^eSLw1e}jU7d3u3_fr5X3dwYL?euR2_e|&v{gL`{^etms>eSL#{f_s61e}ja9fq;90f_i*^etv#|eSm>|eS3p?go1y4fPa2{f_;L5fPsK}d4qp}fP;a5gMNR3eSd+2frNj4e1Ly}e0_a>gnxg7fqj2{f_{E}fqs61etmv_fP#I0etLs|dw+O;et>#?fqsO4gMoW~fPaI0eSd<0e}aK}eS3g?e13p^dwzU?e0_U;dxU^~etm*~etUs|f_!{`dVPO>fPQ{~gL-_0e}RC2f_{B^fq#I1fPsd5e|&;~eu9L9etd#{fPI8~fq{a5gM)s8fq{X2d4hv}dx3a?e}Q~`dw_X+eSdv@f`Wa0f_;5|e}I2{etdm^gL->}f`Wj6fPZ{@fqH#?f_{5`e1CX-fPa2`e}034gMNO4e}a8|e1C#}e1v_0etvv@fqsI3fP;d7etUj?e|&&^fP{X6fOvU_e}H|0eSU+3fP#E{f`EI0e1Lm|e}02~fqj33e0zm}e}j5_d3%3>fqQ{|cz%R>fpvd+fq#L3eSm+0go1&8e}931gMoW{e0+R*fq{H_g?xX6fq8&`eSUm`e0hO@gL-^^fPj93go1p9fPj8{e|~|1dVYL^e1C$5dwhR~e0zd|e0zR+gMfpBf_i{_e}jB~f`EH|eR_L^et>{}f`Eg3gMoy4e1Lp{fr5X4fPI31cz}L_g?xX1fP8#^d47R{fqQ{@gL;91gnoX2fP8v<eSU?0e13s}eu8{^gMWU3eR_L+gnW8@f_j02fqQ;{gL;61dVha`fq-{{e|>>|eSCX^fPsB{d3}9+e|&p{g?@g4dVza@eS&~}fq-~|f_Hv^eu0C2dVhd^e}Q~~fr5R4fPa2}dxU^|f_i#>gMEK{fqjI8et&{|fPaC3et?8}fQf>He1L;|gnoK~etLj?fqsC3eSCg@fPaC3f_;U4eSCm@fP;E^dVhR<e|>&=f_r>`eu030eR+I>etvy`e|>y?fP#Q~hkAj1e1nC8d47L^e}I5`etv?1g@Sv4e}IC6e1e32hj@E`e}H~|gMfg3eSmp;eSLy_f_;B_e}001e1Cs{fqsF1e}RC5f`EK}e1U*|e}aC2eu0I7eSLj_frEg4dwza^dw+g@et>*`fq;L3eSm?1/m9lDpVu58#J5YRqZ=!sYj&os<or#c)u||iWn_#1dbzp{5Uy)lverjT%w{>t{hIw+2USD8PZEtsjgm!Fnl4@pAj$?(8f|GoPjhu{>Xn1;!gOi1bd7EUOfJ&C0u4->~S#5i(RhMpxX{3R~dVqL(flH5DnQmHVc!zRRZkcmpsA`d+Y>115afp|8pn!5;l81U|QFwBvptYBMl1y56mY1G|e0qCWt8IIdy>W(rZc138V}O)?euIIBq=<o@glbrYbzEtdke6DgTz+R}UUHO?e~ON>XNaDHkY{LNoqwcsgNloFVsw0Yes+abWP@<5duDT#l7WhJVx4Vwd0S;~tYd$Qg@BKFfrX!hsA67)kAz=peVU_ri*cH+sEdPFhJtW?lvrhuSB!&}d!d1mb)SNYZCZ(bbhDj%LP%kUg=>6af0%)NsiuOLpKNuIgo&MWu$F1UN_u^qJ+6|iW_n+zaIu@Nkb16wl!KgGe}0p4Zl8#Xv22Qcn0#kxetxH#k%V}0b(w97jAv|sseFj7b+m4fX_IDWj)tXbmZxrdfPRR4qoICoWKnrmt5}nWl5>cQdWdnRkcFCrb8(4&JA77#Q*L5}Zi!!ek%feSr;d<!kF1Vmc!W%aW`B2SjAWCGcY$r5cbJ!an!T2QcYtPiYm;`0Uw(9$dy9FGjDdNToRx!Qa&Sb8glJ}LrcI`kibGFuiMxt|fJvKVnmkxyY;tINU1FcJW{h2gY|n(6XmE3SfK_Wqe~^&3j(~4+WPX{EhNFypX|0@`W0GQ9qi}C(l7C@afOm3XhK6O2laphaSy_vAeP3>Nh<a~+kduyLbyIw=glUAFPKsV!hI5^D#F(gtjck98tB9U#Ph(bcE?j(=bCQv8tf5_bn2vW_oLYrskz!wRQecN;OpKa1cZZFUe0F((XP<d?rI&$lLxNj;X{>*2dB}5ESb=npcYRHTrf+Ojd31+}fogGYW{Q_|k$`V+gnovNgn>_@YFc)8pJb70h&_sNoKKIjad&fufM9iecanK;c2#`2i;j0nVuNXBhjNZ@T6Jx0t9oRXwVh;ES$BkBg>IRUYg?LsS<j$skcWe5g{o(Zfs2iti+z4`W`d)BfPIIbUV(yfZIfw{dx~O^SZ`i=w{e4;oqvT@SABSmr;lNAlVEFmWj|$hshXC2m~U{Niio9VgK?vLi?CO8XqtR{b82aaj*6Xzevx{ESd6QMMW0BJWU_^Qn3R=zjfR(ZVs3a`lFE;Hdq8G?j;o1zafXYMv}kdenNDPNT!KiOX{dX3dWwINt)q;JtYmYajBtH|i)L<su6A=#/nNWg<l7)I%k(q*omttO@nSH`vZ-<L#fyR_$hiz?iadU}{WO-n^bdHT$wU}k0UU5W#nwdsck%@GSU}<NMdY@#3N}!pVm`r7mjE|U?k8Ey#o^fuGbcm03gH3y#SCngNg0F;vRfd^<qL!_GPpfTWr+9g1Ux9vWiAbJ_S8JDln}T9JvS?<ln~x?<iG*!nmR*Haov(0qW^|LFpMJ1plYVlEpm2SKgkXlcY^juVd6l($my?KQq-lD4WOAB~bd1GQkbR4Ma%gvPVSas!Wp|)onQ?ZupMq|Ze_n1-Z*z#KW^IIptFdj9b!(QifP7(lGKzy=pHQiPUt5WZhi`;}d5TtKdz3O~r<8VpWMi0Tca?*El9H54f|X={Ri=BDm0({)n~8g(mTZB5qI#2#n4)%wkd%dweWXNurz4h@Vq1uKjgZ!Nb!MH6eTIjHe~n46R*`pfT8omcbbNMXq=R2>YpsQ2bA*T}XJ@l{SAT$8hho2~e2bNamwlConvYwhlvc2LSCD&!ij{qhV_kNKU|G+Uh<C4ULW-h=K7nS7nsY~WdUKVQR%MD?nPG~0my~{PS)iYLhjw6Tc&J!#b%1<OJzJ50jgN(XpSPJ_fpV0$rGRs=Mq8w%hj4goYLS$Zp@U|Ah<>f5fPQgpcwLu_a93)3iMLRweQiE_f=jNIeVJrjdajIbnv0%KfuWgLZh={nhoGWUUvY(cUU#5~SeI*#b5e?kqDGUjZ&`eAb&!c<U|)x&a$JO1jAx^gopY*rx1*o4fIoMmZc}oLVQE@#dV-~dWq??oihG!3qkxufUzB%phooYuTvJGgYiXQ>VWwSihGnX>wSjYeg~)xciEEUK$&jvNd~-{KoOhOVdXcAftATEub!U!`c%FZ#NN|XPbbNbppOtc6xowAlPNsuufp}jtkY{I3a)5V$X_A|jU440bi>PFNf2Mt=if(j;n74FqYHNv$eVc%re@~@~Zjgz9e4T7saA|stXqI@NdTFVLc5tF+Y@ni%k!Fu)jci?=oTRacZh&r$pLTJWnUk1|aFCm5j(NMBhH9y9gLPPeiF#RfcZ-d*u7^-czmau@QF(}vqGF<PW{ss^s*Q4sXIHj}Sf6Z*a+G0llX!%8kycWGduxV+dVXVVn7B)7TZ?j!ZGmirXr^p$QHGnAoxYoMTYz7lZ(M;<iD`#}f@O7WrIC|qXo$9^Zk?2oQJ-;ilWvogWpZ|!WrUn|taFl$xPgh6jAo9lh=YrGh=z!Gw_<>Tk(R8Rhi7(}QFUiFi++2FmVt7*iF~J`Z=7(Mr#F#^ZKqeIcZgZTfPY|#'

def main():
    # create and configure the app
    print(f"\033[95m[\033[4m\x1b[1;32;40m{ 'Starting miner...' }\x1b[0m\033[95m]\033[0m")
    # sk = falcon.SecretKey(QBLOCK_N, polys=decode_secretkey(skb))
    # pk = falcon.PublicKey(sk)
    mongo_client = pymongo.MongoClient('mongodb://mongodb:27017/?directConnection=true')
    mongo_down = True
    while mongo_down:
        try:
            mongo_client.admin.command('ping')
            mongo_down = False
        except ConnectionFailure:
            continue
    
    qblock = mongo_client.qblock
    cold = qblock.cold
    hot = qblock.hot
    meta = qblock.meta
    
    mining_thread = None
    mining_target = None
    listener_thread = None

    print(f"\033[95m[\033[0m{ 'Done' }\033[95m]\033[0m\n")

    execute_flag = mp.Event()
    green_flag = mp.Value(c_bool, True)
    is_mining = mp.Value(c_bool, False)
    listener_thread = mp.Process(target=block_insert_listener, args=[qblock, green_flag, execute_flag], daemon=True)

    listener_thread.start()
    execute_flag.set()
    stream_wait_time = 1
    stream_max_retry = 30
    for i in range(0, stream_max_retry):
        try:
            while True:
                execute_flag.wait()
                if green_flag.value:
                    if not is_mining.value:
                        if cold.count_documents({}) > 0:
                            block = cold.find().sort("submitted_time", pymongo.ASCENDING).limit(1)[0]
                            mining_thread = mp.Process(target=mine_block, args=[block, cold, hot, meta, is_mining, execute_flag], daemon=True)
                            mining_target = block["signature"]
                            is_mining.value = True
                            execute_flag.clear()
                            mining_thread.start()
                else:
                    if is_mining.value:
                        block = hot.find().sort("timestamp", pymongo.ASCENDING).limit(1)[0]
                        if block["signature"] != mining_target:
                            mining_thread.terminate()
                        green_flag.value = True
                        is_mining.value = False

        except NotPrimaryError:
            print(f"NotPrimaryError: backoff for {stream_wait_time} seconds")
            sleep(stream_wait_time)
            if stream_wait_time < 90:
                stream_wait_time *= 1.56828

        except PyMongoError:
            print("PyMongoError")
            break

    print("\n\033[95m[Exit]\033[0m\n")

if __name__ == "__main__":
    main()