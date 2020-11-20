// sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "sample_client.h"
#include "ipc/ipc_message.h"
#include "../mdmc/mdmc_interface.h"

/*int _tmain(int argc, _TCHAR* argv[])
{
	SampleClient listener;
	IPC::Endpoint endpoint(kChannelName, &listener);
	std::string cmd;
	while (true)
	{
		std::cout << ">>";
		std::cin >> cmd;
		if (cmd == "exit")
		{
			break;
		}
		else
		{
			scoped_refptr<IPC::Message> m(new IPC::Message(GetCurrentProcessId(), 0, (IPC::Message::PriorityValue)0));
			m->WriteString(cmd);
			//std::cout << "Process [" << GetCurrentProcessId() << "]: " << cmd << std::endl;
			endpoint.Send(m.get());
		}
	}

	return 0;
}


void SampleClient::OnChannelError()
{
	std::cout << "Process [" << id_ << "] Disconnected" << std::endl;
}

void SampleClient::OnChannelConnected(int32 peer_pid)
{
	id_ = peer_pid;
	std::cout << "Process [" << peer_pid << "] Connected" << std::endl;
}

bool SampleClient::OnMessageReceived(IPC::Message* msg)
{
	std::string s;
	msg->routing_id();
	IPC::MessageReader reader(msg);
	reader.ReadString(&s);
	std::cout << "Process [" << id_ << "]: " << s << std::endl;
	return true;
}*/

std::wstring UTF8ToUnicode(const char* src)
{
	if (src == NULL || src[0] == '\0')
	{
		return L"";
	}
	std::string strSrc(src);
	int length = MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, NULL, 0);
	wchar_t *buf = new wchar_t[(size_t)(length + 1)];
	MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, buf, length);
	buf[length] = L'\0';
	std::wstring dest = buf;
	delete[] buf;
	return dest;
}

int OnConnected(const char* ipc_id, int peer_pid, int status, void* context)
{
	if (status == 1)
	{
		std::cout << "Connected ipc_id= " << ipc_id << " peer_pid= " << peer_pid << std::endl;
	}
	else
	{
		std::cout << "Disconnected ipc_id= " << ipc_id << " peer_pid= " << peer_pid << std::endl;
	}
	return 0;
}

int OnMsgArrived(const char* ipc_id, int peer_pid, const char* msg, void* context)
{
	std::wstring ws = UTF8ToUnicode(msg);
	std::cout << "Receive ipc_id= " << ipc_id << " peer_pid= " << peer_pid;
	std::wcout << L" msg: " << ws.c_str() << std::endl;
	
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
  setlocale(LC_ALL,"CHS"); //****** 设置本地属性*********
	nd_mdmc_init("VR-RES-BRIDGE", OnConnected, OnMsgArrived, nullptr);

	//nd_mdmc_init("test", OnConnected, OnMsgArrived, nullptr);
	//nd_mdmc_init("test", OnConnected, OnMsgArrived, nullptr);
	//nd_mdmc_init("test3", OnConnected, OnMsgArrived, nullptr);
	//nd_mdmc_init("test4", OnConnected, OnMsgArrived, nullptr);
	//nd_mdmc_init("test5", OnConnected, OnMsgArrived, nullptr);

	std::string cmd;
	while (true)
	{
		std::cout << ">>";
		std::cin >> cmd;
		if (cmd == "exit")
		{
			break;
		}
		else if (cmd == "r")
		{
			nd_mdmc_init("VR-RES-BRIDGE", OnConnected, OnMsgArrived, nullptr);
			//nd_mdmc_init("test", OnConnected, OnMsgArrived, nullptr);
			//nd_mdmc_init("test", OnConnected, OnMsgArrived, nullptr);
			//nd_mdmc_init("test3", OnConnected, OnMsgArrived, nullptr);
			//nd_mdmc_init("test4", OnConnected, OnMsgArrived, nullptr);
			//nd_mdmc_init("test5", OnConnected, OnMsgArrived, nullptr);
		}
		else if (cmd == "list")
		{
			std::string cmd;
			cmd = "{   \"cmd\":\"getvrresourcelist\",    \"cmd_seq\": \"12341\",	\"data\":{\"page\":1,\"count\":8}}";

			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());
			//nd_mdmc_send("test", cmd.c_str());
			//nd_mdmc_send("test", cmd.c_str());
			//nd_mdmc_send("test3", cmd.c_str());
			//nd_mdmc_send("test4", cmd.c_str());
			//nd_mdmc_send("test5", cmd.c_str());
		}
		else if (cmd == "download")
		{
			std::string cmd;
			
			cmd ="{    \"cmd\" : \"getvrresource\",\"cmd_seq\": \"12342\",\
				\"data\":{\"id\":\"5ef2fd88b164c92f34123c35\",\"size\":\"13807748\",\
				\"res_url\":\"https://betacs.101.com/v0.1/download/actions/direct?dentryId=064e197f-83e3-43bb-8469-e1079601e2f1&serviceName=prepub_content_mdm&attachment=true\",\
				\"res_md5\":\"f2bc29aec165c9e68fec43446fd9b80a\"}}";
		
			/*
			cmd = "{    \"cmd\" : \"getvrresource\",\"cmd_seq\": \"12341\",\
\"data\":{\"id\":\"5f31346845ce6e564cbe764f\",\"size\":\"340428572\",\
\"res_url\":\"https://betacs.101.com/v0.1/download?dentryId=9daf5105-577b-4bfe-bf00-cfb8791a921c&attachment=true&token=prepub_content_mdm:Anu0U2vebKOJX2R6:6pOBL_3K9Vo20fx5G63tnZYAab0&expireAt=undefined&policy=eyJkZW50cnlJZCI6IjlkYWY1MTA1LTU3N2ItNGJmZS1iZjAwLWNmYjg3OTFhOTIxYyIsInVpZCI6MTIzNDY2LCJyb2xlIjoiYWRtaW4iLCJwb2xpY3lUeXBlIjoiZG93bmxvYWQifQ\",\
\"res_md5\":\"91175571465cb05d305e6656a5c72821\"}}";
				*/
			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());
		}
		else if (cmd == "downloadencrypt")
		{
			std::string cmd;
		//	cmd = "{\"cmd\":\"vrloginuc\", \"cmd_seq\": \"12341\",\"data\":{\"account\":\"5855852\", \"password\":\"123456\" }}";
			
			cmd = "{    \"cmd\" : \"getvrresource\",\"cmd_seq\": \"12341\",\
\"data\":{\"id\":\"5f5b211345ce2c0bf9382125\",\"dentry_id\":\"d416d16d-eff6-44fd-9375-78651e0a449f\",\"size\":\"330535415\",\
\"type\":\"MP4\",\"res_md5\":\"3163c71c04a714b6d65799def279d049\",\"encryption\":true}}";
				
			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());
		//	nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());
		}
		else if (cmd == "decrypt")
		{
			std::string cmd;
			//	cmd = "{\"cmd\":\"vrloginuc\", \"cmd_seq\": \"12341\",\"data\":{\"account\":\"5855852\", \"password\":\"123456\" }}";

			cmd = "{    \"cmd\" : \"resource_decrypt\",\"cmd_seq\": \"12341\",\
				  \"data\":{\"id\":\"5f5b211345ce2c0bf9382125\",\"dentry_id\":\"d416d16d-eff6-44fd-9375-78651e0a449f\",\"size\":\"330535415\",\
				  \"path\":\"C:\\\\Users\\\\Administrator\\\\AppData\\\\Roaming\\\\MDMClient\\\\recv\\\\vrres\\\\5f5b211345ce2c0bf9382125.MP4\",\
				  \"encryptionNames\":\"aes\",\"sercretkey\":\"85C32670EE8C83A3D1AE078674720452\"}}";

			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());

		}
		else if (cmd == "login")
		{
			std::string cmd;
				cmd = 
					"{\"cmd\":\"vrloginuc\", \"cmd_seq\": \"12341\",\"data\":{\"account\":\"6667772\", \"password\":\"123abc\" }}";
 
			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());

		}
		else if (cmd == "count")
		{
			std::string cmd;
			cmd =
				"{\"cmd\":\"getvrresourcecount\", \"cmd_seq\": \"12341\",\"data\":{\"category\":\"manyou\", \"platform\":\"pc\" }}";

			nd_mdmc_send("VR-RES-BRIDGE", cmd.c_str());

		}

	}

	return 0;
}