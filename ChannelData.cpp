#include "ChannelData.hpp"

ChannelData::ChannelData(){}
ChannelData::ChannelData(const ChannelData &other){*this = other;}
ChannelData::~ChannelData(){}
ChannelData::ChannelData(std::string ChannelName, std::string topic, std::string pass) : _ChannelName(ChannelName), _topic(topic), _pass(pass)
{ 
	_inviteOnly = false;            // i:  Set/remove Invite-only channel
	_topicRestrictions = false;     // t:  Set/remove the restrictions of the TOPIC command to channel operator
	_passwordRestrictions = false;  // k:  Set/remove the channel key (password)
	_serverlimit = -1;              // l:  Set/remove the user limit to channel     (if _serverlimit > 0 -> _serverlimit = numOfusers  else --> no limit)        
}

ChannelData &ChannelData::operator=(const ChannelData &other)
{
	if (this != &other)
		_ChannelName= other._ChannelName;
	return(*this);
}


//Getters

std::string ChannelData::getChannelName() { return _ChannelName; }
bool ChannelData::isInviteOnly() { return _inviteOnly;}
bool ChannelData::hasTopicRestrictions() { return _topicRestrictions; }
bool ChannelData::hasPasswordRestrictions() { return _passwordRestrictions; }
int  ChannelData::getServerLimit() { return _serverlimit; }
std::string ChannelData::getTopic() { return _topic; }
std::string ChannelData::getPass() {return _pass;}

//Seters

void ChannelData::setChannelName(std::string newChannelName) { _ChannelName = newChannelName;}
void ChannelData::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly;}
void ChannelData::setTopicRestrictions(bool topicRestrictions) { _topicRestrictions = topicRestrictions;}
void ChannelData::setPasswordRestrictions(bool passwordRestrictions) {_passwordRestrictions = passwordRestrictions;}
void ChannelData::setServerLimit(int serverLimit) {_serverlimit= serverLimit;}
void ChannelData::setTopic(std::string newTopic) {_topic = newTopic;}
void ChannelData::setOper()
{
	if (_clientsVec.empty())
		return;
	else
	{
		if(_operatorsVec.empty())
		{
			_operatorsVec.push_back(_clientsVec[0]);
			this->sendToChannel(_clientsVec[0], makeChanMsg(_clientsVec[0], "MODE " + this->getChannelName(), "+o " + _clientsVec[0]->getNickName()), true);
			return;
		}
	}
}
void ChannelData::setOper(ClientData *client)
{
	_operatorsVec.push_back(client);
	this->sendToChannel(client, makeChanMsg(client, "MODE " + this->getChannelName(), "+o " + client->getNickName()), true);
	return;
}
//Functions

void    ChannelData::addUser(ClientData *client, std::string pass, bool invite)
{
	if(this->_serverlimit > 0)
	{
		if(this->_clientsVec.size() >= (size_t)this->_serverlimit)
		{
			sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Server is full"));
			return;
		}
	}

	for (std::vector<ClientData*>::iterator it = this->_clientsVec.begin(); it != this->_clientsVec.end(); it++)
	{
		if ((*it) == client)
		{
			sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "User allready in the channel"));
			return;
		}
	}
	if(invite)
	{
		this->_clientsVec.push_back(client);
		setOper();
		return;
	}
	if(this->isInviteOnly() == true)
	{
		sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "You need an invitation to enter this channel"));
		return;
	}
	if(this->hasPasswordRestrictions())
	{
		if (pass.empty())
			sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "No password provided"));
		else if(this->_pass != pass)
			sendToUser(client, makeUserMsg(client, ERR_NEEDMOREPARAMS, "Wrong password"));
		else if(this->_pass == pass)
			this->_clientsVec.push_back(client);
	}
	else
	{
		this->_clientsVec.push_back(client);
		setOper();
	}
}

void    ChannelData::printTopic(ClientData *client)
{
    if(!this->_topic.empty())
        sendToUser(client, makeUserMsg(client, RPL_TOPIC, _topic));
    else
        sendToUser(client, makeUserMsg(client, RPL_TOPIC, "Ups!! No topic >:( "));

}

void    ChannelData::changeTopic(ClientData *client, std::string newtopic)
{
	newtopic.empty();
	if(this->hasTopicRestrictions())
		sendToUser(client, makeUserMsg(client, ERR_NOPRIVILEGES, "Error: This server does not allow to change the topic"));
}



std::string	makePrivMsg(ClientData *sender, ClientData *receiver , std::string input)
{
	std::ostringstream 	message;
    if(input[0] != ':')
	    message << ":" << sender->getNickName() << " PRIVMSG " <<  receiver->getNickName() << " :" << input << "\r\n";
    else
        message << ":" << sender->getNickName() << " PRIVMSG " <<  receiver->getNickName() << " " << input << "\r\n";
	return (message.str());
}


void	ChannelData::sendToChannel(ClientData *client, std::string message, bool sendToSender)
{
	for (std::vector<ClientData*>::iterator it = this->_clientsVec.begin(); it != this->_clientsVec.end(); it++)
	{
		std::ostringstream debug;								
		if ((*it) != client || sendToSender)
		{
			if (send((*it)->getFd(), message.c_str(), message.size(), 0) < 0)
				throw std::invalid_argument(" > Error at sendToChannel() ");
		}
	}
}

bool	ChannelData::hasMember(ClientData *client)
{
	for (std::vector<ClientData*>::iterator it = this->_clientsVec.begin(); it != this->_clientsVec.end(); it++)
	{
		if (client->getNickName() == (*it)->getNickName())
			return (true);
	}
	return (false);
}

bool 	ChannelData::deleteUser(ClientData *client)
{
	
	for (std::vector<ClientData*>::iterator it = this->_clientsVec.begin(); it != this->_clientsVec.end(); it++)
	{
		if (client->getNickName() == (*it)->getNickName())
		{
			this->deleteUserOper(*it);
			this->_clientsVec.erase(it);
			setOper();
			return (true);
		}
	}
	return (false);
}

bool ChannelData::isChanOp(ClientData *client)
{
	for (std::vector<ClientData*>::iterator it = this->_operatorsVec.begin(); it != this->_operatorsVec.end(); it++)
	{
		if ((*it) == client)
			return (true);
	}
	return (false);
}
 
void 	ChannelData::deleteUserOper(ClientData *client)
{
	if(!_operatorsVec.empty())
	{
		for (std::vector<ClientData*>::iterator it = this->_operatorsVec.begin(); it != this->_operatorsVec.end(); it++)
		{
			if ((*it) == client)
			{
				this->_operatorsVec.erase(it);
				this->sendToChannel(_clientsVec[0], makeChanMsg(_clientsVec[0], "MODE " + this->getChannelName(), "-o " + _clientsVec[0]->getNickName()), true);
				return;
			}
		}
	}
}
