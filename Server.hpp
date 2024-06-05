#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define HOSTLEN 64
#define TIMEOUT 42

class Server
{
    public:
        Server(std::string const port, std::string const password);
        ~Server();
        int Start();
        int create_serversocket();
        bool CheckPassword(std::string buffer);
        bool CheckNickName(char* buffer);
        int processCommand(ClientData *client, size_t socket_num, std::string lines);
        int processCommandOper(ClientData *client);
        int firstCommand(ClientData *client, std::string lines);
        int suCommand(const std::string& command, std::vector<ClientData>::iterator it_client);
        ClientData *find_ClientData_Socket(int fd);
        ClientData	*find_ClientData_Socket_login(int fd);
        int CreateNewUser(struct sockaddr_storage client_addr, int server_socket);
        int ReceiveDataClient(size_t socket_num, std::string line, int bytes);
        int ReceiveDataClient_login(size_t socket_num, std::string line);
        void deleteClient(size_t socket_num, ClientData *it_client);
        void CloseServer();
        static void CloseServer01();;
        void send_PersonalMessage(ClientData *sender);
        void send_ChannelMessage(ClientData *sender);
        std::string	makePrivMsg(ClientData *sender, ClientData *receiver , std::string input);
        ClientData *find_ClientData_Nickname(std::string str);
        ClientData	*findUser(std::string str);
        std::vector<std::string> splitString(std::string str, const char *dlmtrs);
        void sendWelcomeMessageToUser(ClientData* client);
        void createChanels();
        void processChanMsg(ClientData *sender);
        ChannelData	*findChannel(std::string str);
        void checkFirst(ClientData *client);
        int contLines( std::string& cadena);

    private:
        Server(const Server &other);
        Server	&operator=(const Server &other);
        std::string _port;
        std::string _pass;
        std::string _supass;
        std::vector<ClientData*> clients_vec;
        std::vector<ClientData*> clients_vec_login;
        std::vector<ChannelData*> channel_vec;
        std::vector<pollfd>		_sockets;
        std::vector<std::string> lines;
        std::vector<std::string> args;
        bool _ServerStatus;
        bool _CloseServer;
        

};

void sendToUser(ClientData *targetUser, std::string message);
std::string makeUserMsg(ClientData *user, std::string code, std::string input);
std::string	makeUserMsg01(ClientData *user, std::string input);
std::string makeChanMsg(ClientData *client, std::string input);
std::string	makeChanMsg(ClientData *client, std::string code, std::string input);
#endif
