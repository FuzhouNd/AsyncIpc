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
@ desc:    ����״̬���»ص�
@ param:   ipc_id(���̼�ͨѶ��ʶ)
@ param:   peer_pid(���̼�ͨѶ��Խ���ID)
@ param:   status(1:�����ӣ�0:�ѶϿ�)
@ param:   context(�ص�������)
*/
typedef int(__cdecl* OnConnected_t)(const char* ipc_id, int peer_pid, int status, void* context);

/*
@ desc:     ��Ϣ����ص�
@ param:    ipc_id(���̼�ͨѶ��ʶ)
@ param:    peer_pid(���̼�ͨѶ��Խ���ID)
@ param:    msg(��Ϣ����)
@ param:    context(�ص�������)
*/
typedef int(__cdecl* OnMsgArrived_t)(const char* ipc_id, int peer_pid, const char* msg, void* context);

/*
@ desc:     ���̼�ͨѶ��ʼ��
@ param:    ipc_id(���̼�ͨѶ��ʶ)
@ param:    connected_cb(����״̬�ص�)
@ param:    msgarrived_cb(��Ϣ����ص�)
@ param:    context(�ص�������)
@ return:   true�ɹ�, falseʧ��
*/
MDMC_API bool nd_mdmc_init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context);

/*
@ desc:     ���ͽ��̼���Ϣ
@ param:    ipc_id(���̼�ͨѶ��ʶ)
@ param:    msg(��Ϣ����)
@ return:   true�ɹ�, falseʧ��
*/
MDMC_API bool nd_mdmc_send(const char* ipc_id, const char* msg);

/*
@ desc:     ���̼�ͨѶ����
@ param:    ipc_id(���̼�ͨѶ��ʶ)
@ return:   true�ɹ�, falseʧ��
*/
MDMC_API bool nd_mdmc_destroy(const char* ipc_id);