#ifndef _SHARE_MEMORY_H
#define _SHARE_MEMORY_H

//共享内存接口API
//add by freeeyes

#include <stdio.h>
#ifdef WIN32
#include <Windows.h> 
#else 
#include <unistd.h> 
#include <sys/stat.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#endif

typedef unsigned int shm_key;

#ifdef WIN32
typedef HANDLE shm_id;
#else
typedef int shm_id;
#define PERMS_IPC 0600
#endif

//打开一个共享内存
char* Open_Share_Memory_API(shm_key obj_key, size_t obj_shm_size, shm_id& obj_shm_id);

//关闭一个共享内存
void Close_Share_Memory_API(const char* pBase, shm_id& obj_shm_id);

#endif
