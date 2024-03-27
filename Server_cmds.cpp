#include "Server.hpp"



int Server::firstCommand(std::vector<std::string> args, ClientData *client) 
{
    std::vector<ClientData*>::iterator it;

    if (!args.empty()) 
    {
        std::string ircCommand = args[0];
        if(ircCommand == "PASS")
        { 
            std::string tryPass = args[1];
            std::string myPass = _pass;
            if(myPass.compare(tryPass) != 0)
            {
                std::cerr << "The client tried to log in with an incorrect password" << std::endl;
                std::cerr << "|" << tryPass << "|" << std::endl;
                return 1;
            }
            else
            {
                std::cerr << "password correct!" << std::endl;
                return 0;
            }
        }
        else if(ircCommand == "NICK")
        {
            std::string newNickName = args[1];
            for (std::vector<ClientData>::iterator it = clients_vec.begin(); it != clients_vec.end(); ++it)
            {
                if (it->getNickName() == newNickName)
                {
                    std::cerr << RED << "The user tried to connect with an already registered nickname" << NOCOLOR << std::endl;
                    return(1);
                }
            }
            client->setNickName(newNickName);
            return 0;
        }
        else if(ircCommand == "USER")
        {
            std::string newLogin = args[1];
            std::string newReal = args[4];
            client->setLoginName(newLogin);
            client->setRealName(newReal);
            return 0;
        }
        else 
        {
            std::cout << "Error in initial commands -> " << ircCommand <<  " ?" << std::endl;
            return 1;
        }
    }
    return 1;
}

int Server::processCommand(std::vector<std::string> args, ClientData *client, size_t socket_num) 
{
    std::cerr << "login = " << client->getLoginName() << std::endl;
    if (!args.empty()) 
    {
        std::string ircCommand = args[0];

        if (ircCommand == "JOIN") 
        {
            ChannelData *chan = findChannel(args[1]);
            if (chan == NULL)
                sendToUser(client, makeUserMsg(client, ERR_CANNOTSENDTOCHAN, "Especifica un canal valido"));
            else
            {
                std::string joinCommand = "JOIN " + args[1] + "\r\n";
                send(_sockets[0].fd, joinCommand.c_str(), joinCommand.length(), 0);
                chan->addUser(client, "pass");
                chan->sendToChannel(client, makeChanMsg(client, "JOIN", chan->getChannelName()));
                chan->updateMemberList(client);
            }
        } 
        else if (ircCommand == "PRIVMSG") 
        {
            if(args[1][0] == '#')
                processChanMsg(args, client);
            else
                send_PersonalMessage(args, client);

        }
        else if(ircCommand == "SU")
        {
            std::string su_pass = args[1];
            std::string my_pass = _supass;
            su_pass.pop_back();
            su_pass.pop_back();
            if(my_pass.compare(su_pass) != 0)
                sendToUser(client, makeUserMsg(client, ERR_NOTREGISTERED, "Contraseña del superusuario erronea"));
            else
            {
                client->setSuper(true);
                sendToUser(client, makeUserMsg(client, RPL_TRACENEWTYPE, "SUPERUSUARIO"));
                std::cout << BLUE << client->getNickName() << " ahora es superusuario" << NOCOLOR << std::endl;
            }
        }
        else if(ircCommand == "QUIT")
        {
            deleteClient(socket_num, client);
        }
        else if(ircCommand == "ENDSERVER")
        {
            CloseServer();
            return (1);
        }
        else 
        {
            sendToUser(client, makeUserMsg(client, RPL_NONE, "Unrecognized command"));
            std::cout << "Unrecognized command: " << ircCommand << std::endl;
        }
    }
    return (0);
}
