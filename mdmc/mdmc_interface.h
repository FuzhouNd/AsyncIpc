#pragma once

#include <string>

#ifdef _WIN32
#ifdef MDMC_EXPORTS
#define MDMC_API extern "C" __declspec(dllexport)
#else
#define MDMC_API extern "C" __declspec(dllimport)
#endif
#else
#define MDMC_API
#endif

/*
@ desc:    连接状态更新回调
@ param:   ipc_id(进程间通讯标识)
@ param:   peer_pid(进程间通讯配对进程ID)
@ param:   status(1:已连接，0:已断开)
@ param:   context(回调上下文)
*/
typedef int(__cdecl* OnConnected_t)(const char* ipc_id, int peer_pid, int status, void* context);

/*
@ desc:     消息到达回调
@ param:    ipc_id(进程间通讯标识)
@ param:    peer_pid(进程间通讯配对进程ID)
@ param:    msg(消息内容)
@ param:    context(回调上下文)
*/
typedef int(__cdecl* OnMsgArrived_t)(const char* ipc_id, int peer_pid, const char* msg, void* context);

/*
@ desc:     进程间通讯初始化
@ param:    ipc_id(进程间通讯标识)
@ param:    connected_cb(连接状态回调)
@ param:    msgarrived_cb(消息到达回调)
@ param:    context(回调上下文)
@ return:   true成功, false失败
*/
MDMC_API bool nd_mdmc_init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context);

/*
@ desc:     发送进程间消息
@ param:    ipc_id(进程间通讯标识)
@ param:    msg(消息内容)
@ return:   true成功, false失败
*/
MDMC_API bool nd_mdmc_send(const char* ipc_id, const char* msg);

/*
@ desc:     进程间通讯销毁
@ param:    ipc_id(进程间通讯标识)
@ return:   true成功, false失败
*/
MDMC_API bool nd_mdmc_destroy(const char* ipc_id);