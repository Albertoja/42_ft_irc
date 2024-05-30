#include "Server.hpp"


int Server::firstCommand(ClientData *client) 
{
    std::vector<ClientData*>::iterator it;

    if (!args.empty() && client != NULL) 
    {
        std::string ircCommand = args[0];
        if(ircCommand == "PASS" && client->getPass() == "")
        { 
            std::string tryPass = args[1];
            std::string myPass = _pass;
            if(myPass.compare(tryPass) != 0)
            {
                sendToUser(client, makeUserMsg(client, RPL_NONE, "incorrect password"));
                return 0;
            }
            else
            {
                client->setPass(myPass);
                sendToUser(client, makeUserMsg(client, RPL_NONE, "password correct!"));
                return 0;
            }
        }
        else if(ircCommand == "NICK" && client->getNickName() == "")
        {
            std::string newNickName = args[1];
            if(newNickName.empty())
                return 0;
            for (std::vector<ClientData*>::iterator it = clients_vec.begin(); it != clients_vec.end(); ++it)
            {
                if ((*it)->getNickName() == newNickName)
                {
                    sendToUser(client, makeUserMsg(client, RPL_NONE, "error: already registered nickname"));
                    return 0;
                }
            }
            sendToUser(client, makeUserMsg(client, RPL_NONE, "nickname entered correctly"));
            client->setNickName(newNickName);
            return 0;
        }
        else if(ircCommand == "USER" && client->getLoginName() == "")
        {
            std::string newLogin = args[1];
            if(newLogin.empty())
                return 0;
            client->setLoginName(newLogin);
            if(args.size() == 5)
            {
                std::string newReal = args[4];
                client->setRealName(newReal);
            }
            sendToUser(client, makeUserMsg(client, RPL_NONE, "login entered correctly"));
            return 0;
        }
        else if(ircCommand == "CAP")
        {
            return 0;
        }
        else if(ircCommand == "QUIT")
        {
            deleteClient(client->getSocketNum(), client);
            return(1);
        }
        else 
        {
            std::cout << "Error in initial commands -> " << ircCommand <<  " ?" << std::endl;
            return 0;
        }
        return 0;
    }
    return 0;
}

int Server::processCommandOper(ClientData *client)
{

    if (!args.empty() && client != NULL) 
    {
        ClientData *client_to;
        if(args.size() >= 3)
            client_to = find_ClientData_Nickname(args[2]);
        else
            client_to = NULL;
        ChannelData *chan;
        if(args.size() >= 2)
            chan = findChannel(args[1]);
        else
            chan = NULL;
        std::string ircCommand = args[0];
        if (ircCommand == "KICK")
        {
            if (args.size() < 3)
		        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Need more parameters"));
            else if (chan == NULL)
		        sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
            else if(client_to == NULL)
                sendToUser(client, makeUserMsg(client, ERR_NOSUCHNICK, "Member does not exist"));
            else if (!(chan->hasMember(client_to)))
		        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User is not in channel"));
            else if (!chan->isChanOp(client))
                sendToUser(client, makeUserMsg(client, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
            else
            {
                chan->sendToChannel(client_to, makeChanMsg(client, "KICK", chan->getChannelName() + " " + client_to->getNickName() + " :" + client->getNickName()), true);
		        chan->deleteUser(client_to);
            }
            return (1);
        }
        else if(ircCommand == "INVITE")
        {
            if (args.size() < 3)
		        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Need more parameters"));
            else if (chan == NULL)
		        sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
            else if(client_to == NULL)
                sendToUser(client, makeUserMsg(client, ERR_NOSUCHNICK, "Member does not exist"));
            else if ((chan->hasMember(client_to)))
		        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User is already in channel"));
            else if (!chan->isChanOp(client))
                sendToUser(client, makeUserMsg(client, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
            else
            {
                sendToUser(client_to, makeUserMsg(client, "INVITE", args[1]));
                std::string joinCommand = "JOIN " + args[1] + "\r\n";
                chan->addUser(client_to, chan->getPass(), true);
                chan->sendToChannel(client_to, makeChanMsg(client_to, "JOIN", chan->getChannelName()), true);
            }
            return (1);
        }
        else if (ircCommand == "TOPIC")
        {
            if (args[1] == "IRC")
                sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Need more parameters"));
            else if(args.size() == 2)
            {
                if (chan == NULL)
                    sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
                else if (!(chan->hasMember(client)))
                    sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User is not in channel"));
                else
                    chan->printTopic(client);
            }
            else if (args.size() > 2)
            {
                if (chan == NULL)
                    sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
                else if (!chan->isChanOp(client) && chan->hasTopicRestrictions() == true)
                    sendToUser(client, makeUserMsg(client, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
                else
                {
                    std::string newTopic;
                    for (size_t i = 2; i < args.size(); ++i) 
                    {
                        newTopic += args[i];
                        if (i < args.size() - 1) 
                            newTopic += " ";
                    }
                    if (newTopic.size() > 0 && newTopic[0] == ':')
                        newTopic = newTopic.substr(1);
                    chan->setTopic(args[1] + " " + newTopic);
                }
            }
            return (1);
        }
        else if (ircCommand == "WHO")
        {
            return 1;
        }
        else if (ircCommand == "MODE")
        {
            if (args[2] == "+i")
            {
                return 1;
            }
            if (args.size() < 3)
                return 1;
            else if (chan == NULL)
                sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
            else if (!chan->isChanOp(client))
                sendToUser(client, makeUserMsg(client, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
            else if (args[2] == "i")
            {
                if(chan->isInviteOnly() == true)
                {
                    chan->setInviteOnly(false);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Invite Only disabled"), true);
                }
                else if(chan->isInviteOnly() == false)
                {
                    chan->setInviteOnly(true);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Invite Only enabled"), true);
                }
            }
            else if(args[2] == "t")
            {
                if(chan->hasTopicRestrictions() == true)
                {
                    chan->setTopicRestrictions(false);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Topic Restrictions disabled"), true);
                }
                else if(chan->hasTopicRestrictions() == false)
                {
                    chan->setTopicRestrictions(true);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Topic Restrictions enabled"), true);
                }
            }
            else if(args[2] == "k")
            {
                if(chan->hasPasswordRestrictions() == true)
                {
                    chan->setPasswordRestrictions(false);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Password Restrictions disabled"), true);
                }
                else if(chan->hasPasswordRestrictions() == false)
                {
                    chan->setPasswordRestrictions(true);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "Password Restrictions enabled"), true);
                }
            }
            else if(args[2] == "o")
            {           
                if(args.size() >= 4) 
                    client_to = find_ClientData_Nickname(args[3]);
                else
                    client_to = NULL;
                if (args.size() < 4)
                    sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Need more parameters"));
                else if (client_to == NULL)
                    sendToUser(client, makeUserMsg(client, ERR_NOSUCHNICK, "Member does not exist"));
                else if (!(chan->hasMember(client_to)))
		            sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User is not in channel"));
                else if (chan->isChanOp(client_to))
                    sendToUser(client, makeUserMsg(client, ERR_CHANOPRIVSNEEDED, "User is already an operator"));
                else
                {
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), (client_to->getNickName() + " was appointed operator")), true);
                    chan->setOper(client_to);  
                }
            }
            else if(args[2] == "l")
            {
                if (args.size() < 4)
                {
                    chan->setServerLimit(-1);
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "There is no maximum number of users for this channel"), true);
                }
                else
                {
                    int newMax = atoi(args[3].c_str());
                    if(newMax < 1 || newMax > INT_MAX)
                    {
                        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Error in maximun number of users"));
                        return(1);
                    }
                    chan->setServerLimit(newMax);
                    std::stringstream ss;
                    ss << newMax;
                    std::string num_str = ss.str();
                    chan->sendToChannel(client, makeChanMsg(client, "MODE " + chan->getChannelName(), "The new maximum number of users is " + num_str), true);
                }
            }
            return (1);
        }    
    }
    return (0);
}

int Server::processCommand(ClientData *client, size_t socket_num) 
{
        if(processCommandOper(client) == 1)
            return (0);
        if (!args.empty()) 
        {
            std::string ircCommand = args[0];
            if (ircCommand == "JOIN" && args.size() >= 2) 
            {
                ChannelData *chan = findChannel(args[1]);
                if (chan == NULL)
                    sendToUser(client, makeUserMsg(client, ERR_NOSUCHCHANNEL, "Channel does not exist"));
                else
                {
                    if(chan->hasMember(client))
                        sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User allready in the channel"));
                    else
                    {
                        std::string joinCommand = "JOIN " + args[1] + "\r\n";
                        if(args.size() < 3)
                            chan->addUser(client, "", false);
                        else
                            chan->addUser(client, args[2], false);
                        if(chan->hasMember(client))
                            chan->sendToChannel(client, makeChanMsg(client, "JOIN", chan->getChannelName()), true);
                    }
                }
            } 
            else if (ircCommand == "PRIVMSG" && args.size() >= 2) 
            {
                if(args[1][0] == '#')
                    processChanMsg(client);
                else
                    send_PersonalMessage(client);
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
            else if(ircCommand == "PART" && args.size() >= 2)
            {
                ChannelData *chan = findChannel(args[1]);
                if(chan == NULL)
                    sendToUser(client, makeUserMsg(client, ERR_CANNOTSENDTOCHAN, "Specify a valid channel"));
                else
                {
                    if(!(chan->deleteUser(client)))
                        sendToUser(client, makeUserMsg(client, ERR_CANNOTSENDTOCHAN, "You are not a member of that channel"));
                    else
                    {
                        chan->sendToChannel(client, makeChanMsg(client, "PART", chan->getChannelName()), true);
                    }
                }
            }
            else 
            {
                sendToUser(client, makeUserMsg(client, RPL_NONE, "Unrecognized command"));
                std::cout << "Unrecognized command: " << ircCommand << std::endl;
            }
        }
        return (0);
}
