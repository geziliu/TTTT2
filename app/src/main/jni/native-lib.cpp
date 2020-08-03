#include <jni.h>
#include <string>
#include <openssl/md5.h>
#include <android/log.h>

#define LOG_TAG  "C_TAG"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

void correctUtfBytes(char* bytes) {
    char three = 0;
    while (*bytes != '\0') {
        unsigned char utf8 = *(bytes++);
        three = 0;
        // Switch on the high four bits.
        switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f:
                /*
                 * Bit pattern 10xx or 1111, which are illegal start bytes.
                 * Note: 1111 is valid for normal UTF-8, but not the
                 * modified UTF-8 used here.
                 */
                *(bytes-1) = '?';
                break;
            case 0x0e:
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    --bytes;
                    *(bytes-1) = '?';
                    break;
                }
                three = 1;
                // Fall through to take care of the final byte.
            case 0x0c:
            case 0x0d:
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    --bytes;
                    if(three)--bytes;
                    *(bytes-1)='?';
                }
                break;
        }
    }
}  //防止NewStringUTF对UTF8检测崩溃

extern "C" JNIEXPORT jstring JNICALL
Java_com_yute_tttt_MainActivity_CountMD5(JNIEnv *env, jobject, jstring str) {
    const char* cs = env->GetStringUTFChars(str, 0);
    if (cs == NULL)
        return NULL;

    char content[256];
    strcpy(content, cs);
    env->ReleaseStringUTFChars(str, cs);

    LOGD("%s", content);

    MD5_CTX c;
    MD5_Init(&c);
    char st[256];
    unsigned char md5[17] = {0};
    MD5_Update(&c, content, strlen(content));
    MD5_Final(md5, &c);

    for (int i = 0; i < 16; i++)
        LOGD("%02X", (unsigned char)md5[i]);

    correctUtfBytes((char *) md5); //防止崩溃

    return env->NewStringUTF((char *)md5);
}