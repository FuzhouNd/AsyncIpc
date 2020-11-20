#ifndef MODULE_H_
#define MODULE_H_

#include <Windows.h>
#include <tchar.h>
#include <map>
#include <string>
#include "ipc/ipc_endpoint.h"
#include "ipc/ipc_message.h"

#include "mdmc_interface.h"

class Module;

// IPCManager
class IPCManager
{
public:
	static IPCManager* GetInstance();

	bool Init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context);
	bool Send(const char* ipc_id, const char* msg);
	bool Destroy(const char* ipc_id);

	IPCManager();
	~IPCManager();

private:
	static IPCManager*		ms_pInstance;
	std::map<std::string, Module*> m_map_id_ipc;
	Module* m_pIpcModule;
};

// Module
class Module : public IPC::Listener
{
public:
	Module();
	~Module();

	bool Init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context);
	bool Send(const char* msg);
	bool Destroy();

protected:
	void OnCreate();

	virtual bool OnMessageReceived(IPC::Message* msg);
	virtual void OnChannelConnected(int32 peer_pid);
	virtual void OnChannelError();

private:
	std::string			ipc_id_;
	int32				ipc_peer_pid_;
	IPC::Endpoint*		ipc_endpoint_ptr_;

	OnConnected_t		connected_cb_;
	OnMsgArrived_t		msgarrived_cb_;
	void*				context_;
};

#endif