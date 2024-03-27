#include "Irc.hpp"

ClientData	*Server::findUser(std::string str)
{
    for (std::vector<ClientData>::iterator it = clients_vec.begin(); it != clients_vec.end(); ++it)
    {
        if (it->getNickName() == str)
        {
            return &(*it); // Devuelve un puntero a la instancia de ClientData en el vector
        }
    }
    return nullptr; // Devuelve nullptr si no se encuentra el usuario
}

std::string checkport(std::string port)
{
    if (port.empty() || port.size() > 5)
    {
        std::cerr << RED << "Error: Invalid port" << NOCOLOR << std::endl;
        exit(1);
    }
    for (long unsigned int i = 0; i < port.length(); ++i)
    {
        if (!isdigit(port[i]))
        {
            std::cerr << RED << "Error: Invalid port" << NOCOLOR << std::endl;
            exit(1);
        }
    }
    int portnum = atoi(port.c_str());
    if(portnum <= MIN_PORT || portnum >= MAX_PORT)
    {
        std::cerr << RED << "Error: Invalid port" << NOCOLOR << std::endl;
        exit(1);
    }
    return(port);
    
}

std::string checkpass(std::string pass)
{
    if (pass.size() > PASSLEN)
    {
        std::cerr << RED << "Error: Invalid password, too long" << NOCOLOR << std::endl;
        exit(1);
    }
    for (long unsigned int i = 0; i < pass.length(); ++i)
    {
        if (!isprint(pass[i]) || isspace(pass[i]))
        {
            std::cerr << RED << "Error: Invalid pass" << NOCOLOR << std::endl;
            exit(1);
        }
    }
    return(pass);
}

int main(int argc, char **argv)
{
    std::string port;
    std::string pass;

    if (argc != 3)
    {
        std::cerr << RED << "Error: port and password please" << NOCOLOR << std::endl;
        return(1);
    }
    port = checkport(argv[1]);
    pass = checkpass(argv[2]);

    Server srv(port, pass);
    
    srv.Start();
    return(0);
}
