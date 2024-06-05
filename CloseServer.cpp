#include "Server.hpp"

void Server::CloseServer()
{
    //elimino el vector llamado lines
    std::vector<std::string>().swap(lines);
    lines.clear();

    //elimino el vector llamado args
    std::vector<std::string>().swap(args);
    args.clear();

    //elimino el vector de los canales
    for (std::vector<ChannelData*>::iterator it = channel_vec.begin(); it != channel_vec.end(); ++it)
        delete *it;
    std::vector<ChannelData*>().swap(channel_vec);
    channel_vec.clear();

    //cierro todos los sockets y elimino su vector
    for(size_t socket_num = 0; socket_num < _sockets.size(); socket_num++)
        close(_sockets[socket_num].fd);
    std::vector<pollfd>().swap(_sockets);
    _sockets.clear();

    //elimino el vector de los clientes conectados
    for(std::vector<ClientData*>::iterator it = clients_vec.begin(); it != clients_vec.end(); ++it)
        delete *it;
    std::vector<ClientData*>().swap(clients_vec);
    clients_vec.clear();
    
    //elimino el vector de los clientes  no conectados
    for(std::vector<ClientData*>::iterator it = clients_vec_login.begin(); it != clients_vec_login.end(); ++it)
        delete *it;
    std::vector<ClientData*>().swap(clients_vec_login);
    clients_vec_login.clear();

    //salgo del programa
    std::cout << RED << "||Close server||" << NOCOLOR << std::endl;
    exit(0);

}