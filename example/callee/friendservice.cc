#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>
#include "logger.h"


class FriendService:public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout << "do rpc GetFriendList service! userid: " << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao");
        vec.push_back("zhang");
        vec.push_back("li");
        return vec;
    }

    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendList = GetFriendList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for(auto &name:friendList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();

    }



};

int main(int argc,char **argv)
{
    //调用框架的初始化操作

    MprpcApplication::Init(argc, argv);
    //创建rpc服务提供者，把UserService对象发布出来
    RpcProvider provider;

    provider.NotifyService(new FriendService());
    //启动一个rpc服务提供者，run以后，进程进入阻塞状态，等待远程的rpc调用请求
    //启动一个rpc服务的节点，就是启动一个tcp服务，端口9999
    provider.Run();
    return 0;

}