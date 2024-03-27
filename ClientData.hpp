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
    void setSuper(bool i);
    void setLastMsg(std::string msg);

    //GETTERS
    int getSocket();
    int getFd();
    std::string getNickName();
    std::string getLoginName();
    std::string getRealName();
    std::string getHostname();
    bool        getSuper();
    std::string getLastMsg		(void) const;

private:
    bool        _super;
    int         _socket;
    std::string _NickName;
    std::string _LoginName;
    std::string _RealName;
    std::string	_host;
    std::string	_service;
    std::string	_lastMsg;
};

#endif
