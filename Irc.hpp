#ifndef IRC_HPP
# define IRC_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <poll.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <iostream>
# include <sstream>
# include <string.h>
# include <string>
# include <list>
# include <vector>
# include <map>
# include <algorithm>
# include <ctime>
# include <csignal>
# include <fcntl.h>
# include <termios.h>
# include <iostream>
# include <cstring>
# include <csignal>

# include "ClientData.hpp"
# include "Server.hpp"

# include "Codes.hpp"


# define NOCOLOR "\033[0m"
# define BLUE "\033[0;36m"
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"

# define MIN_PORT 1023
# define MAX_PORT 65535
# define PASSLEN 100



#endif
