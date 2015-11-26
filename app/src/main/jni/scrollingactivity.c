#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

jobject list_obj;
jmethodID list_add;
JNIEnv* field_env;

JNIEXPORT void JNICALL
Java_com_bob_jnidemo_MainActivity_scanDir(JNIEnv *env, jobject instance, jstring dirPath_) {
    const char *dirPath = (*env)->GetStringUTFChars(env, dirPath_, 0);

    // TODO

    (*env)->ReleaseStringUTFChars(env, dirPath_, dirPath);
}

JNIEXPORT jstring JNICALL
Java_com_bob_jnidemo_MainActivity_getStringFromC(JNIEnv *env, jobject instance) {

    // TODO
    return (*env)->NewStringUTF(env, "Hello,JNI");
}

JNIEXPORT jint JNICALL
Java_com_bob_jnidemo_MainActivity_add(JNIEnv *env, jobject instance, jint x, jint y) {

    // TODO
    return x + y;
}

JNIEXPORT jstring JNICALL
Java_com_bob_jnidemo_MainActivity_getString(JNIEnv *env, jobject instance, jstring str_) {
    const char *str = (*env)->GetStringUTFChars(env, str_, 0);

    return (*env)->NewStringUTF(env, str);
}


/**
 * 扫描文件
 */
void scan_Dir(char* directory) {

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(directory)) == NULL) {
        perror("opendir");
        return;
    }
    chdir(directory);
//    LOGE("pyb chdir directory = %s\n", directory);
    while ((entry = readdir(dp)) != NULL) {
        stat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if ((strcmp(entry->d_name, ".") != 0)
                && (strcmp(entry->d_name, "..") != 0)
                && (entry->d_name[0] != '.')) {
                scan_Dir(entry->d_name);
            }
        } else {
            int size = strlen(entry->d_name);
            if (entry->d_name[0] != '.' && (statbuf.st_size / 1024) > 300
                && strcmp(entry->d_name + (size - 4), ".mp4") == 0) {
//                LOGE("scan_dir(),file st_size = %d \n\n", (statbuf.st_size/1024));
                char* parentPath = (char*) malloc(1024);
                char* absolutePath = (char*) malloc(1024);

                getcwd(parentPath, 1024);
//                LOGE("parentPath = %s \n", parentPath);
                strcpy(absolutePath, parentPath);
                char *p = "/";
                absolutePath = strcat(absolutePath, p);
                absolutePath = strcat(absolutePath, entry->d_name);
                //statbuf.st_size,
//                LOGE("scan_dir(),file absolutePath = %s \n", absolutePath);

                jstring str = (*field_env)->NewStringUTF(field_env, absolutePath);

                (*field_env)->CallBooleanMethod(field_env, list_obj, list_add, str);

                free(parentPath);
                parentPath = NULL;
                free(absolutePath);
                absolutePath = NULL;
            }
        }
    }
    chdir("..");
    closedir(dp);
}

JNIEXPORT jobject JNICALL
Java_com_bob_jnidemo_MainActivity_getFiles(JNIEnv *env, jobject instance, jstring path_) {

    field_env = env;

    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
//    (*env)->ReleaseStringUTFChars(env, path_, path);
    jclass list_cls = (*env)->FindClass(env, "java/util/ArrayList");

    jmethodID list_costruct = (*env)->GetMethodID(env, list_cls, "<init>", "()V"); //获得得构造函数Id

    list_obj = (*env)->NewObject(env, list_cls, list_costruct); //创建一个Arraylist集合对象
    //或得Arraylist类中的 add()方法ID，其方法原型为： boolean add(Object object) ;
    list_add = (*env)->GetMethodID(env, list_cls, "add", "(Ljava/lang/Object;)Z");

    scan_Dir(path);

    (*env)->ReleaseStringUTFChars(env,path_,path);

    return list_obj;
}

JNIEXPORT jstring JNICALL
Java_com_bob_jnidemo_MainActivity_getStringFromJni(JNIEnv *env, jobject instance) {

    // TODO
    return (*env)->NewStringUTF(env, "fsfsfsfsfd");
}