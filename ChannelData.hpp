#ifndef CHANNELDATA_HPP
# define CHANNELDATA_HPP

#include "Irc.hpp"

class ChannelData 
{
	public:

		ChannelData();
		ChannelData(std::string ChannelName, std::string topic, std::string pass);
		~ChannelData();
		ChannelData(const ChannelData &other);
		ChannelData	&operator=(const ChannelData &other);

		//SETTERS
		void setChannelName(std::string newChannelName);
		void setMode(char newMode);
		void setInviteOnly(bool inviteOnly);
		void setTopicRestrictions(bool topicRestrictions);
		void setPasswordRestrictions(bool passwordRestrictions);
		void setServerLimit(int serverLimit);
		void setTopic(std::string newTopic);
		void setOper();
		void setOper(ClientData *client);

		//GETTERS
		std::string getChannelName();
		char getMode();
		bool isInviteOnly();
		bool hasTopicRestrictions();
		bool hasPasswordRestrictions();
		int getServerLimit();
		std::string getTopic();
		std::string getPass();

		//FUNCTIONS
		
		void	sendToChannel(ClientData *client, std::string message, bool sendToSender);
		bool	isChanOp(ClientData *client);
		void    changeTopic(ClientData *client, std::string newtopic);
		void    addUser(ClientData *client, std::string pass, bool invite);
        void    printTopic(ClientData *client);
		bool	hasMember (ClientData *client);
		bool 	deleteUser(ClientData *client);
		void 	deleteUserOper(ClientData *client);
	private:

		bool		_inviteOnly;
		bool		_topicRestrictions;
		bool		_passwordRestrictions;
		int			_serverlimit;
		std::string _ChannelName;
		std::string _topic;
		std::string _pass;
		std::vector<ClientData*> _clientsVec;
		std::vector<ClientData*> _operatorsVec;
};

#endif

