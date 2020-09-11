//
// Created by qianh on 2019/8/20.
//
#include "com_tuya_test_FF1Test.h"
#include "ff1.h"

JNIEXPORT jintArray JNICALL Java_com_tuya_test_FF1Test_decrypt
        (JNIEnv *env, jclass class, jbyteArray pkey, jbyteArray input) {
    uint8_t *pwd = (*env)->GetByteArrayElements(env, input, NULL);
    int pwd_len = (*env)->GetArrayLength(env, input);
    num_str cipher = {.buf=pwd, .len=pwd_len};

    char *key_buf = (*env)->GetByteArrayElements(env, pkey, NULL);
    char key_len = (*env)->GetArrayLength(env, pkey);

    byte_str key = {.buf=key_buf, .len=key_len};

    byte_str tweak = create_byte_str("", 0);
    ff1_context ctx;
    num_str result = decrypt(key, tweak, cipher, ctx);
    if (result.len <= 0 || result.len > 100) {
        jclass exClass = (*env)->FindClass(env, "java/lang/RuntimeException");
        (*env)->ThrowNew(env, exClass, "decrypt error!");
        return NULL;
    }
    (*env)->ReleaseByteArrayElements(env, pkey, key_buf, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, input, pwd, JNI_ABORT);
    jintArray ret = (*env)->NewIntArray(env, pwd_len);
    int *ret_buf = calloc(pwd_len, sizeof(uint32_t));
    for (int i = 0; i < result.len; i++) {
        ret_buf[i] = result.buf[i];
    }
    (*env)->SetIntArrayRegion(env, ret, 0, pwd_len, ret_buf);
    release_str(tweak);
    release_str(result);
    free(ret_buf);
    return ret;
}
