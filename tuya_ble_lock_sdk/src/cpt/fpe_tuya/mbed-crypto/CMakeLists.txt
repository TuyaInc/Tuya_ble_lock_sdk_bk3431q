cmake_minimum_required(VERSION 3.10)
project(mbed-crypto C)

set(CMAKE_C_STANDARD 99)
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
add_library(mbed-crypto STATIC library/aes.c
        library/aesni.c
        library/arc4.c
        library/asn1parse.c
        library/asn1write.c
        library/base64.c
        library/bignum.c
        library/blowfish.c
        library/camellia.c
        library/ccm.c
        library/cipher.c
        library/cipher_wrap.c
        library/cmac.c
        library/ctr_drbg.c
        library/des.c
        library/ecdsa.c
        library/ecp.c
        library/ecp_curves.c
        library/entropy.c
        library/entropy_poll.c
        library/gcm.c
        library/hmac_drbg.c
        library/md.c
        library/md2.c
        library/md4.c
        library/md5.c
        library/md_wrap.c
        library/oid.c
        library/pem.c
        library/pk.c
        library/pk_wrap.c
        library/pkcs5.c
        library/pkcs12.c
        library/pkparse.c
        library/pkwrite.c
        library/platform.c
        library/platform_util.c
        library/psa_crypto.c
        library/ripemd160.c
        library/rsa.c
        library/rsa_internal.c
        library/sha1.c
        library/sha256.c
        library/sha512.c
        library/xtea.c)