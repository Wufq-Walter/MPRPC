#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"



using namespace std;
using namespace fixbug;

/*


业务部分
UserService原来是一个本地服务，提供了两个进程内的本地方法
Login和GetFriendList方法，现在需要为这个服务添加一个远程方法
*/

class UserService : public UserServiceRpc//使用rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(string name,string pwd)
    {
        cout<<"UserService::Login"<<endl;
        cout<<"name:"<<name<<" pwd:"<<pwd<<endl;
    }

    bool Register(uint32_t id,string name,string pwd)
    {
        cout<<"UserService::Register"<<endl;
        cout<<"id"<<id<<"name:"<<name<<" pwd:"<<pwd<<endl;
        return true;
    }


    //重写基类UserServicRpc的虚函数 下面这些方法都是框架直接调用
    /*1. caller  Login(LoginRequest)=> mudUo =>callee
      2. callee  Login(LoginRequest)=> 交给下面重写的这个Login方法上了
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        string name = request->name();
        string pwd = request->pwd();

        bool login_result = Login(name,pwd);

        ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        done->Run();

    }
    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        string name = request->name();
        string pwd = request->pwd();
        bool ret = Register(id,name,pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);
        done->Run();
    }


};






int main(int argc,char **argv)
{
    //调用框架的初始化操作

    MprpcApplication::Init(argc, argv);
    //创建rpc服务提供者，把UserService对象发布出来
    RpcProvider provider;

    provider.NotifyService(new UserService());
    //启动一个rpc服务提供者，run以后，进程进入阻塞状态，等待远程的rpc调用请求
    //启动一个rpc服务的节点，就是启动一个tcp服务，端口9999
    provider.Run();









    return 0;

}