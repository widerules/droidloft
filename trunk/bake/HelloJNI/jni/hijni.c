#include <string.h>
#include <jni.h> 

/*
 * Class:     com_droidloft_JNI
 * Method:    sayHello
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_droidloft_JNI_sayHello(JNIEnv * env, jobject jobj){
  return (*env)->NewStringUTF(env, "Hello world from JNI"); 
 }

