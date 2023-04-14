#include <string>
#include "nlohmann/json.hpp"


// #include <sys/socket.h>

#pragma once

class RSClient
{

	public:
        RSClient(std::string IP);

		int startConnect();
        bool sendData(nlohmann::json poseData);
        int cli;

    protected:
        int mClient;
        int mStatus;
        int mValueRead;
        std::string mIP;

        // struct sockaddr_in* serv_addr;

};