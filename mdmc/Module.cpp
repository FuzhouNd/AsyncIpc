#include "stdafx.h"
#include "Module.h"

#define ID_IPC_DEFAULT	"ID-IPC-DEFAULT"

// 日志打印
void MyOutputDebugStringA(const char * lpcszOutputString, ...)
{
	std::string strResult;
	if (NULL != lpcszOutputString)
	{
		va_list marker = NULL;
		va_start(marker, lpcszOutputString); //初始化变量参数
		size_t nLength = _vscprintf(lpcszOutputString, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszOutputString, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}

	if (!strResult.empty())
	{
		strResult = "[mdmc] " + strResult;
		OutputDebugStringA(strResult.c_str());
	}
}

// IPCManager
IPCManager* IPCManager::ms_pInstance = NULL;
IPCManager* IPCManager::GetInstance()
{
	if (!ms_pInstance)
	{
		ms_pInstance = new IPCManager();
	}
	return ms_pInstance;
}

IPCManager::IPCManager() : m_pIpcModule(NULL)
{
}

IPCManager::~IPCManager()
{
}

bool IPCManager::Init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context)
{
	std::string id = ID_IPC_DEFAULT;
	if (ipc_id != NULL && strlen(ipc_id) > 0)
	{
		id = ipc_id;
	}

	Module* pModule = NULL;
	auto it = m_map_id_ipc.find(id);
	if (it != m_map_id_ipc.end())
	{
		pModule = it->second;
	}
	
	if (pModule == NULL)
	{
		pModule = new Module;
		m_map_id_ipc[id] = pModule;
	}

	return pModule->Init(id.c_str(), connected_cb, msgarrived_cb, context);
}

bool IPCManager::Send(const char* ipc_id, const char* msg)
{
	std::string id = ID_IPC_DEFAULT;
	if (ipc_id != NULL && strlen(ipc_id) > 0)
	{
		id = ipc_id;
	}

	auto it = m_map_id_ipc.find(id);
	if (it == m_map_id_ipc.end())
	{
		MyOutputDebugStringA("ERROR IPC NOT Init...");
	}
	else
	{
		if (it->second != NULL)
		{
			return it->second->Send(msg);
		}
	}
	
	return false;
}

bool IPCManager::Destroy(const char* ipc_id)
{
	std::string id = ID_IPC_DEFAULT;
	if (ipc_id != NULL && strlen(ipc_id) > 0)
	{
		id = ipc_id;
	}

	auto it = m_map_id_ipc.find(id);
	if (it == m_map_id_ipc.end())
	{
		MyOutputDebugStringA("ERROR IPC NOT find...");
	}
	else
	{
		if (it->second != NULL)
		{
			return it->second->Destroy();
		}
		// m_map_guid_ipc.erase(it);
	}
	
	return false;
}

// Module
Module::Module() : ipc_endpoint_ptr_(nullptr), ipc_peer_pid_(0)
{
}

Module::~Module()
{
}

void Module::OnCreate()
{
}

bool Module::Init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context)
{
	if (ipc_endpoint_ptr_ != nullptr || ipc_id == nullptr)
	{
		MyOutputDebugStringA("IPC re-init, ipc_id= %s", ipc_id);
		return false;
	}

	// IPC
	ipc_endpoint_ptr_ = new IPC::Endpoint(ipc_id, this);
	if (ipc_endpoint_ptr_ != nullptr)
	{
		MyOutputDebugStringA("IPC init success, ipc_id= %s", ipc_id);

		ipc_id_ = ipc_id;
		connected_cb_ = connected_cb;
		msgarrived_cb_ = msgarrived_cb;
		context_ = context;
	}
	else
	{
		MyOutputDebugStringA("IPC init fail, ipc_id= %s", ipc_id);
	}

	return ipc_endpoint_ptr_ != nullptr;
}

void Module::OnChannelError()
{
	int n_error_pid = ipc_peer_pid_;
	MyOutputDebugStringA("IPC disconnected, ipc_id= %s, pid= %d", ipc_id_.c_str(), n_error_pid);
	ipc_peer_pid_ = 0;

	if (connected_cb_ != nullptr)
	{
		connected_cb_(ipc_id_.c_str(), n_error_pid, 0, context_);
	}
}

void Module::OnChannelConnected(int32 peer_pid)
{
	ipc_peer_pid_ = peer_pid;
	MyOutputDebugStringA("IPC connected, ipc_id= %s, pid=%d", ipc_id_.c_str(), ipc_peer_pid_);

	if (connected_cb_ != nullptr)
	{
		connected_cb_(ipc_id_.c_str(), ipc_peer_pid_, 1, context_);
	}
}

bool Module::OnMessageReceived(IPC::Message* msg)
{
	std::string s;
	msg->routing_id();
	IPC::MessageReader reader(msg);
	reader.ReadString(&s);

	MyOutputDebugStringA("IPC receive [%d] msg: %s", ipc_peer_pid_, s.c_str());

	if (msgarrived_cb_ != nullptr)
	{
		msgarrived_cb_(ipc_id_.c_str(), ipc_peer_pid_, s.c_str(), context_);
	}

	return true;
}

bool Module::Send(const char* msg)
{
	if (msg == NULL || strlen(msg) == 0)
	{
		MyOutputDebugStringA("IPC send msg fail, msg is empty!");
		return false;
	}

	if (ipc_endpoint_ptr_ == nullptr || ipc_peer_pid_ == 0)
	{
		MyOutputDebugStringA("IPC send msg fail, un-init or un-connected!");
		return false;
	}

	scoped_refptr<IPC::Message> ipc_msg(new IPC::Message(GetCurrentProcessId(), 0, (IPC::Message::PriorityValue)0));
	ipc_msg->WriteString(msg);

	bool ret = ipc_endpoint_ptr_->Send(ipc_msg.get());

	MyOutputDebugStringA("IPC Send to [%d] %s, msg= %s", ipc_peer_pid_, ret ? "success" : "fail", msg);

	return ret;
}

bool Module::Destroy()
{
	MyOutputDebugStringA("IPC destroy start, ipc_id= %s", ipc_id_.c_str());

	if (ipc_endpoint_ptr_ != nullptr)
	{
		delete ipc_endpoint_ptr_;
		ipc_endpoint_ptr_ = nullptr;
	}

	ipc_peer_pid_ = 0;
	connected_cb_ = nullptr;
	msgarrived_cb_ = nullptr;
	context_ = nullptr;

	MyOutputDebugStringA("IPC destroy end");

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Dll Export
//////////////////////////////////////////////////////////////////////////
bool nd_mdmc_init(const char* ipc_id, OnConnected_t connected_cb, OnMsgArrived_t msgarrived_cb, void* context)
{
	return IPCManager::GetInstance()->Init(ipc_id, connected_cb, msgarrived_cb, context);
}

bool nd_mdmc_send(const char* ipc_id, const char* msg)
{
	return IPCManager::GetInstance()->Send(ipc_id, msg);
}

bool nd_mdmc_destroy(const char* ipc_id)
{
	return IPCManager::GetInstance()->Destroy(ipc_id);
}