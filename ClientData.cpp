#include "ClientData.hpp"

ClientData::ClientData(){}
ClientData::ClientData(int socket) : _socket(socket), _isLogin(false), _firstLogin(true), _pass(""), _NickName(""), _LoginName(""), _checkPass(false), _checkNick(false), _checkUser(false){}
ClientData::ClientData(const ClientData &other){*this = other;}
ClientData::~ClientData(){}
void			ClientData::setHost(std::string host) { _host = host; }
void			ClientData::setService(std::string service) { _service = service; }
void			ClientData::setSocketNum(int socketnum){_clientsocketnum = socketnum;}
void 			ClientData::setNickName(std::string newNickName){this->_NickName = newNickName; _checkNick = true;}
void 			ClientData::setLoginName(std::string newLoginName){this->_LoginName = newLoginName; _checkUser = true;}
void 			ClientData::setRealName(std::string newRealName){this->_RealName = newRealName;}
void 			ClientData::setClientAddr(sockaddr_in clientAddr){this->_clientAddr = clientAddr;}
void			ClientData::setisLogin(bool login){this->_isLogin = login;}
void			ClientData::setfirstLogin(bool login){this->_firstLogin = login;}
void			ClientData::setPass(std::string pass){this->_pass = pass; _checkPass = true;}
std::string 	ClientData::getNickName(){return _NickName;}
std::string 	ClientData::getLoginName(){return _LoginName;}
std::string 	ClientData::getRealName(){return _RealName;}
std::string 	ClientData::getHostname(){return _host;}
int 			ClientData::getSocket(){return _socket;}
int 			ClientData::getFd(){return _socket;}
sockaddr_in		ClientData::getClientAddr(){return _clientAddr;}
bool        	ClientData::getisLogin(){return _isLogin;}
bool        	ClientData::getfirstLogin(){return _firstLogin;}
std::string		ClientData::getPass(){return _pass;}
void			ClientData::setConnectionTime(time_t time){this->_connectionTime = time;}
time_t 			ClientData::getConnectionTime(){return _connectionTime;}
int				ClientData::getSocketNum(){return _clientsocketnum;}
bool			ClientData::getAll()
{
	if(this->_checkPass && this->_checkNick && this->_checkUser)
		return(true);
	else
		return(false);
}
ClientData &ClientData::operator=(const ClientData &other)
{
    if (this != &other)
    {
        _socket = other._socket;
        _NickName = other._NickName;
    }
    return(*this);
}

int	ClientData::CreateClientData(int fd, struct sockaddr * addr, socklen_t addrlen)
{
	ClientData		*temp = new ClientData(fd);
	char			host[NI_MAXHOST];
	char			service[NI_MAXSERV];
	std::string		hoststring;

	if (getnameinfo(addr, addrlen, host, sizeof(host), service, sizeof(service), NI_NUMERICHOST) != 0)
	{
		std::cerr << RED << "Error. Couldn't retrieve hostname" << NOCOLOR << std::endl;
		close(fd);
		delete temp;
		return (0) ;
	}
	hoststring = host;
	if (hoststring.size() > HOSTLEN)
			hoststring.erase(HOSTLEN, std::string::npos);
	temp->setHost(hoststring);
	temp->setService(std::string (service));
	

	return (0);
}
