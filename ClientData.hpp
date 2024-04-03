#ifndef CLIENTDATA_HPP
# define CLIENTDATA_HPP

#include "Irc.hpp"

class ClientData 
{
public:
    ClientData();
    ClientData(int socket);
    ~ClientData();
    ClientData(const ClientData &other);
    ClientData	&operator=(const ClientData &other);
    static int	CreateClientData(int fd, struct sockaddr *addr, socklen_t addrlen);

    //SETTERS
    void setNickName(std::string newNickName);
    void setLoginName(std::string newLoginName);
    void setRealName(std::string newRealName);
    void setHost(std::string host);
    void setService(std::string service);
    void setClientAddr(sockaddr_in clientAddr);
    void setisLogin(bool login);
    void setfirstLogin(bool login);
    void setPass(std::string pass);
    void setSocketNum(int socket);
    void setConnectionTime(time_t time);

    //GETTERS
    int getSocket();
    int getSocketNum();
    int getFd();
    std::string getNickName();
    std::string getLoginName();
    std::string getRealName();
    std::string getHostname();
    sockaddr_in getClientAddr();
    bool        getisLogin();
    bool        getfirstLogin();
    std::string	getPass();
    bool		getAll();
    time_t getConnectionTime();


private:
    int         _socket;
    int _clientsocketnum;
    bool        _isLogin;
    bool        _firstLogin;
    std::string _pass;
    std::string _NickName;
    std::string _LoginName;
    std::string _RealName;
    std::string	_host;
    std::string	_service;
    std::string	_lastMsg;
    sockaddr_in _clientAddr;    
    bool        _checkPass;
    bool        _checkNick;
    bool        _checkUser;
    time_t      _connectionTime;
};

#endif
