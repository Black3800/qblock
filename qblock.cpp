#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>

extern "C"
{
#include "falcon\falcon.h"
}

#include "block.hpp"
#include "chain.hpp"

int main(void) {

    shake256_context rng;
    if (shake256_init_prng_from_system(&rng) != 0)
    {
        throw std::runtime_error("\033[91mRuntime error:\033[0m PRNG Initialization failed");
    }

    size_t pubkey_len = FALCON_PUBKEY_SIZE(10);
    size_t privkey_len = FALCON_PRIVKEY_SIZE(10);
    size_t tmpkg_len = FALCON_TMPSIZE_KEYGEN(10);
    std::vector<uint8_t> pubkey(pubkey_len);
    std::vector<uint8_t> privkey(privkey_len);
    std::vector<uint8_t> tmpkg(tmpkg_len);

    int r = falcon_keygen_make(&rng,
                               10,
                               &privkey.front(),
                               privkey_len,
                               &pubkey.front(),
                               pubkey_len,
                               &tmpkg.front(),
                               tmpkg_len);

    Chain c = Chain();
    Block* genesis = c.blocks.back();
    
    Block* b1 = new Block();
    b1->message = "hello";
    b1->previousHash = genesis->hash();
    b1->previousTimestamp = genesis->timestamp;
    b1->publicKey = pubkey;
    b1->sign(privkey);
    b1->mine();

    c.add(b1);

    std::cout << c.toString() << std::endl;

    // std::cout << "publicKey length: " << b1.publicKey.size() << std::endl;
    // std::cout << "signature length: " << b1.signature.size() << std::endl;

    // std::cout << b1.toString() << std::endl;

    // std::string pp = "13CwjRmZHb>UxWGhS(l/f^2i#O3B&yPQeA<Mv7vt>Ij48+Zc7$gGpN^(XO5f5S-Bf}{$#ct4Iq>}<sa&MU!zcAq%DO4Q/ftx4h@i/g/b{UNR99(.vSEMEO4Rqv+42>n%kvO76wbh15?{dww0V?^EKD<qtZCG>m$<2).b4YtQ%IjJ>nQdOdpZ?/tzjT0S<$I]4a7/Qh(PVRNhYh{nFV>+DW6GqD^f%T7B8!IQLmW<QcK@v!9i*QA}d.7&/F.{8f<<t}3vtDs=)-av>MHQS+$eGRZ<Jun^FLN0*oni4Pu[EG#&vKosHc6?H&pq}FVFQmahE7b]!)9WX.29Q6Rj?j8/@ypP4W}LE+m1R^cKAiQ2]OWM:TE+1ca-}fO+?K#dGQlZfEX^G>p3FrR<7ecAAxV+%4f^3EX17(vDtHKDhc-(K0gK)y}5}]-UQ?WJoss*D:T)l0gknRHC+6Fu6{^5URe<:KVqqYB6Xlbg4!k[%n4pR+TQ()=T{+w10ZC}4&c5:pGvj4*vJk.4(-rv}Xc$@B[:H0e*X538%yI%n%rB+Wv:CpIjm]H{RQE.E1F[R57DTHueEh{SM4R}o-P)R$fAWkjKVU[1.CCVvCnnHTwXuo#]Za((X2}e:22:U!NnR9[+:%elymOgAICf&X+E7@.dRa}n{ey$YSw^BZ9[d:d6mff^^qFfQtsw34Y)#$k>JzZE4YO#^1R@jWtW[MpPM0gL7:RXfFzIV3LhtE+@?u<?a^vi@lm-0yNTD@O!p0:MP?c&jr{Fw{BbVPWNU&c/XU??MyGL7I3o@:Ki)@j(?O%C}=z<VdNmVMs(rVz.wP{n^Z>8o<iwbX-[4K@dAMT<V}wTlp{2[=LiG=@0wJ[j3!Jb{QFjbo}G?ydSg^)>ggm9Td*+Dg/OSeLt:sig()Ll+IfJF/5(g?%35G2{<aFHlx]:U[fj.%^]L11CCu4[j-&rI>WJ3(9J0SseFKwVTFk1c:VehG<Q4P0PoC<6w59Jo6y2jL9(CjZ5x#l1{t}=l@vC}g<t%A^5^zaAfNw:J6WY3}LF:!l(kdt2.&N6dwKWWc-$2FH)Y%lHN3=2{pzG.*z$O=MI*>nu?6$NnR.Jk!@EYu8!.U3yxJwft6y!Usg^X(ovqu[ojiybCxU6fwT%4Y+g$h(T=AVlrNcWz=p#MbCrde6E(AArM70jw]YxqS{)ara9dt4FK!drV4CE5d8x+UDHgCO[K3yO/MISf&PlfJMOXsF8(}7E5{PsBJn]d3qcTgX:cjb*+$o5]^R1>CmNH$Umyw3/vO-^?mbJ6R6WPhl9jz.nr!@!gB-4X$4sdHqYg6]3)mYj8)!H:l>@o3^:ry[C5..%IyoQXgQWiT@IU@fwH/ckg{EOQ&u--*M2l+l*9X-a#m$!#6Y-T:<0(vP<n?bT<F[(cE1JFkqFTf8*i+In5e<>J#HHzxD+nndEpCNe]WU=c)orNjBQc$SA+Ptj+Gx!JlJ.pUXsf/#ow%c{xa{F1uc=t5gB{s9?O9T}:RgWW.!3NF0L1)S}Hq!dL1JVy(+{quFpLRPSg-JlGvSHtwSl-!mCM.o-!b#gE(rL&vzXWcZbE{$}{D[asrPGO$Ee#h-tbn(xS/h5d.]-pSYJn!kGUm%!m$I5(vBfBgEqZq*nOu9hHfZiom7HOYKa/s+Q9cwwCV}K%/[v)U1J/:2HVRfYTr/cY@t=4Rxi2(+6-*OKYt%mNq6ScG8.@Tm<V1<CZJVYBb>1x%g{*{6SVy?Z6+a?!0DLXlai9b!KwslPBM<83yP+9E57&rn5v-NB2pRx5QWa<[^j5m}![bo:V.tMy]O{mwn/540:.suCS3LZ{HZ!/L{[e8ri)7G3neIb?Ar{O3s7b*9Y4DZRDuF!kC+oRFe.y:3IP9hrwospge:EB%nd7!qE?aXh.dgEWp4^J:i)fp^YU@CQ+DdD<AE!-76(L/R5qOj)PFl+XeAxhh=%}5*n#a#d=M.2tq>2KuF<(8N3^%O3h+VqD*Z7ey&Gbs&05kGu>qTE*gF]1gl4EE>1.z?+TCmcel]4R2^e?h4ntjVu]pGxLFS=6AXRHV&l{^[FFh{&Es<-#aKBZcI0?Y/JzxSgtX1R-!O*IP--?l1KcBCmLtsT2eO9BeoT-0BDE2=tX-J9#-L=3icjnJtdQA$Pv^]&/bFYh2LXGy+KNSwl:dN$[>N>Vi8UU8smrsSR/hrnyKew}b^jv42iLA.ME?/=8$nOB^-+JErrWa}c<xh<w*FJZsoLDCSYTjaDu82%Tp";
    // std::string pp = "1iSGcW0000000000000000000000000000000000000000000000hJnp>f=#]AvigVMKs}Ss?}[v=I?(D+W%]e2i#SC:RDInaWYT:8nbZDHEu8iOhz2joo[-?4kS6]6bJ[h-Yu-}>jaHy})>!+5zog/8ERa7ES]ZsgHZ9w)}>2W}*PYl8ITAOJ$ILfrg7KHk4p5J^}<Fo9dIW:Y8LD?!?f0&GI?i1mSN)m)THwf(QW)h2[>XDX3zAFF2/71dtU*R=1no{tB8ncl$pFyBN6eZ[].Mlhn#Dl+Ti@c)kv?]d.f?n8@8g9fXB-bSeL@zUB3i}Tr.WXzH%mS9EsgBpVCg&VJaJk7eql1dTqQ[RB:W!.4S&Q}Udq+uT5$Y7vapoZjOeZ/W}yEWw5F7BNi(&AgIy^*1>G@O3zxK&T{x1N}7?Cgn@P8rkbx!X4r^Eh)Pi{(BP[FYm^->!H}<yzzP:%BPP09xN{g$i<84U)N%!frC5hh>L5HXRFj>1cLyc.x{4:)u6^JssZV1l3Pz^zl#RV#K^K<Dt%vjV-n$!mL5-uRlD6I[b#61INXX4Tj.KfwOy{HX=Yp!2/PKPFg12qslyW#q+u8d]](j7N4E2[dH(@d9@]8LWT5a-a)HSZsqp$Fk(^l/V8i+[pL<m(/<{Za)iM{dU&:[SJypvWtcrX0Ju=s@{7wEk]XVk7LS8a+/74WI:ff-fEMN@qJO!R7B6@TDdi>&K9Thqb7ilLj#RK?OfWhUZI=zd-#(:5R7KRnpw]$hOnVuO8-hIFyXC}l0t+rWIQ{Weit%8!-=Yl?ngV9n6-Ta^)c>OXH<4aOqXVaC7<eJ10rn@aBcMr-l#kv!T5TH0)WU6M[@/J<-nwvpJ>O2-hxEI{c*1>v7)9LI@Zbz:SjSP*K10hk(vuK<koqL@iy/RMWx(:Xm>XUz7kwaQg(Lr(xLwpEVb7@<FN(E.Exg%g2l^DEsogLT{3Tp.8)5]8kW-9$GtFydHLd!+..yr0?]DwOLiIWVgbJaKo&rCKt).uCytqjl?^s/xj9PkemDPzCt7^6/?64i^!2iZjlL[&Edk?8njQbh6}gBioH@>bv=<pz-E/[m{W}PU$(:od$.t}7$(yQ5K0dx:lSbYE-10W9]*J{/#lVzZ9pMlpzeH:q2&QvpI+>4gPh})R#N1=z:cEesBi3agwZ[A#K<oO3l:SxPf)(ZlwGSs*CiWtXhGGg5KpvB]-@/WMBZ1Vm#$j=NRjQ=?c/lW&EDs{BYgT^{mf.+dZ^VwoH!Db?qzN.q/Zf:DYj4mZm^DEUZFN:]6<nxJZbkw4mOyppd(1j8k9+v[xQhk36lG1<&<%Q<h1?8KdanSVwcP^p^YvkEwUeyk7j@BwlL79]8}3IMn)#qq-sSSor!5ty%Cz]{%o]7Xq+oN5cuk!FYUxI>-u@ox?.@5455/S^+=U&.SJG-94(v)F-2))Q7O?{&S6LS1x.hcE<c<!1>KEwT#wZW-<xIL[YYCQad=+E>-b>=?nTk}vpFuF)^B.[)lf{{-dKPCb6w(TQpT$M*F&[Qp)VN*BoHxIx}buy-g#I[J]/(2M/6(Vtd!c2fz!rygwKuw}Jw+a]?Py";
    // std::vector<uint8_t> pp2 = BlockUtil::b85decode_vector(pp);

    // std::cout << "private key: " << BlockUtil::b85encode_vector(privkey) << "\n\n";
    // std::cout << "public key: " << BlockUtil::b85encode_vector(pubkey) << "\n\n";

    // std::cout << "[";
    // for (int i = 0; i < genesis->signature.size(); i++)
    // {
    //     std::cout << +genesis->signature[i] << ", ";
    // }
    // std::cout << std::endl;
    // for (int i = 0; i < pubkey.size(); i++)
    // {
    //     std::cout << +pubkey[i] << " ";
    // }

    // size_t pubkey_len, privkey_len, sig_len, sigpad_len, sigct_len, expkey_len;
    // size_t tmpkg_len, tmpmp_len, tmpsd_len, tmpst_len, tmpvv_len, tmpek_len;

    // pubkey_len = FALCON_PUBKEY_SIZE(LOGN);
    // privkey_len = FALCON_PRIVKEY_SIZE(LOGN);
    // sig_len = FALCON_SIG_COMPRESSED_MAXSIZE(LOGN);
    // sigpad_len = FALCON_SIG_PADDED_SIZE(LOGN);
    // sigct_len = FALCON_SIG_CT_SIZE(LOGN);
    // expkey_len = FALCON_EXPANDEDKEY_SIZE(LOGN);
    
    // tmpkg_len = FALCON_TMPSIZE_KEYGEN(LOGN);
    // tmpmp_len = FALCON_TMPSIZE_MAKEPUB(LOGN);
    // tmpsd_len = FALCON_TMPSIZE_SIGNDYN(LOGN);
    // tmpst_len = FALCON_TMPSIZE_SIGNTREE(LOGN);
    // tmpvv_len = FALCON_TMPSIZE_VERIFY(LOGN);
    // tmpek_len = FALCON_TMPSIZE_EXPANDPRIV(LOGN);

    // std::vector<uint8_t> pubkey(pubkey_len), pubkey2(pubkey_len), privkey(privkey_len), privkey2(privkey_len), sig(sig_len), sigpad(sigpad_len), sigct(sigct_len), expkey(expkey_len);
    // std::vector<uint8_t> tmpkg(tmpkg_len), tmpmp(tmpmp_len), tmpsd(tmpsd_len), tmpst(tmpst_len), tmpvv(tmpvv_len), tmpek(tmpek_len);

    // int r;
    // printf_s("privkeylen: %d\n", privkey_len);

    // r = falcon_keygen_make(&rng,
    //                        LOGN,
    //                        &privkey.front(),
    //                        privkey_len,
    //                        &pubkey.front(),
    //                        pubkey_len,
    //                        &tmpkg.front(),
    //                        tmpkg_len);
    // if (r != 0)
    // {
    //     fprintf(stdout, "keygen failed: %d\n", r);
    //     // exit(EXIT_FAILURE);
    // }
    // else
    // {
    //     for (auto i : pubkey)
    //     {
    //         std::cout << std::dec << +i << " ";
    //     }
    //     std::cout << std::endl << "############" << std::endl;
    //     for (auto i : pubkey)
    //     {
    //         std::cout << std::setfill('0') << std::setw(2) << std::hex << +i;
    //     }
    // }
    // std::cout << std::endl << std::endl;

    // r = falcon_keygen_make(&rng,
    //                        LOGN,
    //                        &privkey2.front(),
    //                        privkey_len,
    //                        &pubkey2.front(),
    //                        pubkey_len,
    //                        &tmpkg.front(),
    //                        tmpkg_len);

    // std::string message = "hello123";
    // r = falcon_sign_dyn(&rng,
    //                     &sig.front(),
    //                     &sig_len,
    //                     FALCON_SIG_COMPRESSED,
    //                     &privkey.front(),
    //                     privkey_len,
    //                     message.data(),
    //                     message.length(),
    //                     &tmpsd.front(),
    //                     tmpsd_len);

    // if (r != 0)
    // {
    //     fprintf(stdout, "signdyn failed: %d\n", r);
    //     // exit(EXIT_FAILURE);
    // }
    // else
    // {
    //     std::cout << "sig_len: " << std::dec << sig_len << std::endl;
    //     for (auto i : sig)
    //     {
    //         std::cout << std::dec << +i << " ";
    //     }
    //     std::cout << std::endl
    //               << "############" << std::endl;
    //     for (int i = 0; i < sig_len; i++)
    //     {
    //         std::cout << std::setfill('0') << std::setw(2) << std::hex << +sig[i];
    //     }
    //     std::cout << std::endl
    //               << "====================" << std::endl;
    // }

    // r = falcon_verify(&sig.front(), sig_len, FALCON_SIG_COMPRESSED, &pubkey.front(), pubkey_len, message.data(), message.length(), &tmpvv.front(), tmpvv_len);
    // if (r != 0)
    // {
    //     fprintf(stdout, "verify failed: %d\n", r);
    // }
    // else
    // {
    //     std::cout << "verify success" << std::endl;
    // }

    // r = falcon_verify(&sig.front(), sig_len, FALCON_SIG_COMPRESSED, &pubkey.front(), pubkey_len, message.data(), message.length(), &tmpvv.front(), tmpvv_len);
    // if (r != 0)
    // {
    //     fprintf(stdout, "verify failed: %d\n", r);
    // }
    // else
    // {
    //     std::cout << "verify success"<< std::endl;
    // }


    
    

    // for (i = 0; i < 12; i++)
    // {
    //     int r;

    //     memset(privkey, 0, privkey_len);
    //     memset(pubkey, 0, pubkey_len);
    //     r = falcon_keygen_make(rng, logn, privkey, privkey_len,
    //                            pubkey, pubkey_len, tmpkg, tmpkg_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "keygen failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     memset(pubkey2, 0xFF, pubkey_len);
    //     r = falcon_make_public(pubkey2, pubkey_len,
    //                            privkey, privkey_len, tmpmp, tmpmp_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "makepub failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     check_eq(pubkey, pubkey2, pubkey_len, "pub / repub");

    //     r = falcon_get_logn(pubkey, pubkey_len);
    //     if (r != (int)logn)
    //     {
    //         fprintf(stderr, "get_logn failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }

    //     sig_len = FALCON_SIG_COMPRESSED_MAXSIZE(logn);
    //     memset(sig, 0, sig_len);
    //     r = falcon_sign_dyn(rng, sig, &sig_len, FALCON_SIG_COMPRESSED,
    //                         privkey, privkey_len,
    //                         "data1", 5, tmpsd, tmpsd_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_dyn failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sig, sig_len, FALCON_SIG_COMPRESSED,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         /*
    //          * At very low degrees, it may happen that the
    //          * alternate data hashes to a point which is close
    //          * enough to the correct one that the signature
    //          * matches both. Thus, we skip that check for very
    //          * low degrees.
    //          */
    //         r = falcon_verify(sig, sig_len, FALCON_SIG_COMPRESSED,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr, "wrong verify err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     sigpad_len = FALCON_SIG_PADDED_SIZE(logn);
    //     memset(sigpad, 0, sigpad_len);
    //     r = falcon_sign_dyn(rng, sigpad, &sigpad_len, FALCON_SIG_PADDED,
    //                         privkey, privkey_len,
    //                         "data1", 5, tmpsd, tmpsd_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_dyn(padded) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (sigpad_len != FALCON_SIG_PADDED_SIZE(logn))
    //     {
    //         fprintf(stderr, "sign_dyn(padded): wrong length %lu\n",
    //                 (unsigned long)sigpad_len);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sigpad, sigpad_len, FALCON_SIG_PADDED,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify(padded) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         r = falcon_verify(sigpad, sigpad_len, FALCON_SIG_PADDED,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr,
    //                     "wrong verify(padded) err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     sigct_len = FALCON_SIG_CT_SIZE(logn);
    //     memset(sigct, 0, sigct_len);
    //     r = falcon_sign_dyn(rng, sigct, &sigct_len, FALCON_SIG_CT,
    //                         privkey, privkey_len,
    //                         "data1", 5, tmpsd, tmpsd_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_dyn(ct) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (sigct_len != FALCON_SIG_CT_SIZE(logn))
    //     {
    //         fprintf(stderr, "sign_dyn(ct): wrong length %lu\n",
    //                 (unsigned long)sigct_len);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sigct, sigct_len, FALCON_SIG_CT,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify(ct) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         r = falcon_verify(sigct, sigct_len, FALCON_SIG_CT,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr,
    //                     "wrong verify(ct) err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     r = falcon_expand_privkey(expkey, expkey_len,
    //                               privkey, privkey_len, tmpek, tmpek_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "expand_privkey failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }

    //     sig_len = FALCON_SIG_COMPRESSED_MAXSIZE(logn);
    //     memset(sig, 0, sig_len);
    //     r = falcon_sign_tree(rng, sig, &sig_len, FALCON_SIG_COMPRESSED,
    //                          expkey,
    //                          "data1", 5, tmpst, tmpst_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_tree failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sig, sig_len, FALCON_SIG_COMPRESSED,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify2 failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         r = falcon_verify(sig, sig_len, FALCON_SIG_COMPRESSED,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr, "wrong verify2 err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     sigpad_len = FALCON_SIG_PADDED_SIZE(logn);
    //     memset(sigpad, 0, sigpad_len);
    //     r = falcon_sign_tree(rng, sigpad, &sigpad_len,
    //                          FALCON_SIG_PADDED,
    //                          expkey,
    //                          "data1", 5, tmpst, tmpst_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_tree(padded) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sigpad, sigpad_len, FALCON_SIG_PADDED,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify2(padded) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         r = falcon_verify(sigpad, sigpad_len, FALCON_SIG_PADDED,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr,
    //                     "wrong verify2(padded) err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     sigct_len = FALCON_SIG_CT_SIZE(logn);
    //     memset(sigct, 0, sigct_len);
    //     r = falcon_sign_tree(rng, sigct, &sigct_len, FALCON_SIG_CT,
    //                          expkey,
    //                          "data1", 5, tmpst, tmpst_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "sign_tree(ct) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     r = falcon_verify(sigct, sigct_len, FALCON_SIG_CT,
    //                       pubkey, pubkey_len, "data1", 5, tmpvv, tmpvv_len);
    //     if (r != 0)
    //     {
    //         fprintf(stderr, "verify2(ct) failed: %d\n", r);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (logn >= 5)
    //     {
    //         r = falcon_verify(sigct, sigct_len, FALCON_SIG_CT,
    //                           pubkey, pubkey_len, "data2", 5,
    //                           tmpvv, tmpvv_len);
    //         if (r != FALCON_ERR_BADSIG)
    //         {
    //             fprintf(stderr,
    //                     "wrong verify2(ct) err: %d\n", r);
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     printf(".");
        // fflush(stdout);
        // size_t pk_len = FALCON_PUBKEY_SIZE(9);
        // size_t sk_len = FALCON_PRIVKEY_SIZE(9);
        // size_t tmp_len = FALCON_TMPSIZE_KEYGEN(9);
        // void *pk = calloc(pk_len, sizeof(void));
        // void *sk = calloc(sk_len, sizeof(int));
        // void *tmp = calloc(tmp_len, sizeof(int));
        // shake256_context sc;
        // shake256_init_prng_from_system(&sc);
        // if (falcon_keygen_make(&sc, 9, sk, sk_len, pk, pk_len, tmp, tmp_len) == 0) {
        //     for (size_t i = 0; i < pk_len; i++) {
        //         printf_s("%d, ", *(pk + sizeof(void)*i));
        //     }
        //     printf_s("\nsuccess");
        // }
        return 0;
}