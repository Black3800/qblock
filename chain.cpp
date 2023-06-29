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

int main(void) {

    // shake256_context rng;
    // if (shake256_init_prng_from_system(&rng) != 0)
    // {
    //     throw std::runtime_error("\033[91mRuntime error:\033[0m PRNG Initialization failed");
    // }

    // size_t pubkey_len = FALCON_PUBKEY_SIZE(10);
    // size_t privkey_len = FALCON_PRIVKEY_SIZE(10);
    // size_t tmpkg_len = FALCON_TMPSIZE_KEYGEN(10);
    // std::vector<uint8_t> pubkey(pubkey_len);
    // std::vector<uint8_t> privkey(privkey_len);
    // std::vector<uint8_t> tmpkg(tmpkg_len);

    // int r = falcon_keygen_make(&rng,
    //                            10,
    //                            &privkey.front(),
    //                            privkey_len,
    //                            &pubkey.front(),
    //                            pubkey_len,
    //                            &tmpkg.front(),
    //                            tmpkg_len);

    GenesisBlock genesis;
    Block b1;
    std::cout << genesis.toString() << std::endl;
    b1.message = "hello";
    b1.previousHash = genesis.hash();
    b1.previousTimestamp = genesis.timestamp;
    // b1.publicKey = pubkey;
    // b1.sign(privkey);
    // b1.mine();

    std::cout << b1.toString() << std::endl;

    std::string pp = "13ozEe7DPDXCJcIfZ^5AU=H!ITlRgL80{M^-[j%-(DY%X7EzzH0E-Fd!O}qGfSKXfTy#D9KrhjiXSOZEKrxrZU(JwS7G(2>R*VB0g)zgO@dZxiq7rmiKWpX0OnlbU6nGmnc5Q6T$HvHyp^TN.hGl}vPH-LrHO/zz=o4rIzu!gpWp-nm}Owc7Xtn]{OQfMtzacxm16Q=n)vj+8+CML?h1G$bRM7Pi8B{9INVqlAiCaddk^P7:/{FFQwXrWgw0I}wtA#-]ZF5.)xISXa441}u^-cGiBa!*-PXTB#SwHm.ZqQE?ur<WHD4O<Z6U9!nUN@?lXL0GkEQ/CBu:^QV]MxYFWHh.bhef?7R9WH)^YmG7O89*Hdtm4DNHZC]nu{us-l!X-&Sv168hXzz0Mr>i#Q{4yd&X=G@k=}B>d!+TYrZ<JtgoL0DVIX/7y.@/MRKt#d<69eYVQmwjPIF(p>:iC>mA6nz[Q-n]0hi^}T@X*3ylK3Mgn(Gyv6UPJWHw^xp*Qwmh#%S^)J#U[@tT7Bt[D?zjj>?1s%aH(]x+wAtT3K704Gkfx7[{vfp*(mn)bY%GkrgBGQqoma!)OX>xITw=R)mZ5&SlXs.ZEUj2&%U{x$1I0[Knx-#l1JyF5r}5NL?t2#eK2L-X^!mYk0lU56nOJkD90nqJ70J?ftO7rO7(UWdC)2/]ptt:aT>s@9m]8?AKll@Eup]7:ZLQ$G^%7@UxbY[>oMy$(a:R78xK(T8$CFddGk6-KO&J8GLYy2KdB=b%0OZf3y<N&2h<SX92OWu}O]iJ>xq5N*&Ub!Fn*mw-m:eq[/ldzLiqo#9a)<Gf!v^:=n<sOU+LZ1yHd%b9)M1fXIWB/pbFCvV5*0DRa-^t9y3BTFS]n36HnVmg^sTjN#UP6/[?=qpK<vYT1y}8I4F9aO!d73k+WFlFLI9yA<&rgs&TUaomBhrn8g=:a]#i88<4v8Jm&u&=W+2PXHjM<fwQoF--2fteJSGM6SAkto&p23cU@}1/Mq<km2{oRY!SyF*:#UXD?:e%=3.eFpC}eniY3=Fx2beq7ONxDd-u8jo9*K:Ewa!eUY1@6S}@c3D-czwd:&U4-uNh:=DUGg4K6R^WHWT[e?v.Gz#buuWvB.*p-gSQ2)ax*>[9O.k$*Vs4S:*:XW9=t-y#kN6mtLPO4Pd57{%@Ck7Zf-qP)KbvQ2.S}4m9HUE(WS]#Qzu@tygH:cFYW}s{bp]wi<%9Aj(9J[rw6+KqevoA&}p(u1D9MzLwep5F})#J}.!.R#hl+HyKGxeCZPE:jaGJxT&<f6gm73&u?Z6r$.{8U6ijSGD{uYsJiwL.n1EpO%Tly@Q@U9jms+s(daYI{Vzu)ZBDqkni9lz5=pSP*Lm$o+S:+%afi?Za^/W*E7ha[^BvdE[z!.8Qp4sEW@v<PVxD}D=!&Hbc(a+L#h.3HQsy.=tu[>Z)h7@UbJHsHi1(-HBBVe>XmbufHe/c$)kg/&Y>NYv/{OVz)bbge1QG::TBuQO}musq)%e!M!Om(8i8clMJmZA.cr4qHchmo*ff*%@I@iLK&Sy2x)aJ/9wXRFY4<U19.T>0Q:bA6Y{<J23e=+I*^yX%#r>9z).bCg*pTKM.FE5mA<41-AVqO5:}uQ/rp}GYHZA>WCh5QJQC1jw9oe+PlWe^wN-wk&=$D5d{GjFHl.O5l%FfPnvPF>Z(IT3wYChc+Lq9{j<e05yU2XQ9Bge@7ec^.>XT4VLD8cI4FeWKTDOfdn8u:N3[M/@asln)y=8kXeai87]uNn{k7b(KN!4S?n7yRzAw0?}dIV@8Ra@gDQG9kB<nr&L2N362:(wZ^EV46LP+UwB<}#%I$i9PmiDuKQv8*k57laZob7n)M@pFS^zoYoL2ly*yFH&q!<G7s0F=pxV}CXlH:aHkW8sG{IXJw2aaZzP]8qNFLiSfq[w4m}=U}3[+86W#YaNvl?].*7fJJfV(cC-K1?e3db.2UPkP{N.N$ckFo{8t}ici7tw{7mUC576g/9#D6WWdFLl&GUG6IaxCv82>l(o#sN7e4fD7ZkSrlam11FW^YnZtkF1vYz?}4))9!e3{wq8EN@NloVYwBYGW#-8wH/rN(fZGpi]B!uR#m+vHCdQ4Us&k0F$KBx3Qr6Z=mG5%)Z*l0f@<51Rbac@hgEcRVDEhb4p6S>Z}6uAkR[QQAZB)Na/.<*dvuF{0U-CyXK.+[Lb3aw4}GY}^=:{ydq:^u[pOKMtx=G5bavQmZUu508bEEO%oOEe<lE";
    std::vector<uint8_t> pp2 = BlockUtil::b85decode_vector(pp);

    // std::cout << "[";
    for (int i = 0; i < pp2.size(); i++)
    {
        std::cout << +pp2[i] << " ";
    }
    std::cout << std::endl;
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