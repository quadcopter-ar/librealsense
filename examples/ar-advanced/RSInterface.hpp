#include <string>
#include "nlohmann/json.hpp"
#include <librealsense2/rs.hpp>


// #include <sys/socket.h>

#pragma once

struct Quaternion {
    double w, x, y, z;
};

struct EulerAngles {
    double roll, pitch, yaw;
};

class RSInterface
{

	public:
        RSInterface(std::string IP);

        static int startServer();
		int startConnect();
        EulerAngles ToEulerAngles(Quaternion q);

        bool sendData(rs2_pose);
        int cli;

    protected:
        int mClient;
        int mStatus;
        int mValueRead;
        int static new_socket;
        std::string mIP;

        // struct sockaddr_in* serv_addr;

};
