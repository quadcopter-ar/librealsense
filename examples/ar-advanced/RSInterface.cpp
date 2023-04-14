// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <RSInterface.hpp>
#include "nlohmann/json.hpp"
#define PORT 13579

int RSInterface::new_socket = 0;

RSInterface::RSInterface(std::string IP)
	:mIP(IP)
	// ,serv_addr{nullptr}
{
	std::cout << "RSInterface Constructor2" << std::endl;
	// startConnect();
	// startServer();
}

int RSInterface::startServer()
{
	std::cout << "starting buffer" << std::endl;
	int server_fd, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[128] = { 0 };
    char* hello = "Hello from server";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Forcefully attaching socket to the port 8080
	/*
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	*/

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((RSInterface::new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(RSInterface::new_socket, buffer, 128);
    printf("%s\n", buffer);
    send(RSInterface::new_socket, hello, strlen(hello), 0);
	std::cout << "Hello message sent" << std::endl;
  
    // closing the connected socket
    close(RSInterface::new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
	std::cout << "server shutdown" << std::endl;

    return 0;
}

int RSInterface::startConnect()
{
	struct sockaddr_in serv_addr;


	
	if((mClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "socket creation error" << std::endl;
		return -1;
	} else {
		std::cout << "socket created" << std::endl;
	}

	std::cout << "\tRSInterface startConnect sin_family" << std::endl;
	serv_addr.sin_family = AF_INET;
	std::cout << "\tRSInterface startConnect sin_port" << std::endl;
	serv_addr.sin_port = htons(PORT);
	
	std::cout << "***luck***" << std::endl;

	std::cout << "RSInterface startConnect2" << std::endl;
	

	std::cout << "RSInterface startConnect inet_pton" << std::endl;
	std::cout << "IP: " << mIP.c_str() << std::endl;
	if(inet_pton(AF_INET, mIP.c_str(), &serv_addr.sin_addr) < 0)
	{
		printf("Invalid Address");
		return -1;
	}

	std::cout << "RSInterface startConnect status connect" << std::endl;
	if((mStatus = connect(mClient, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) < 0)
	{
		//std::cout << "\taddy: " << serv_addr.sin_addr. << std::endl;
		printf("Connection Failed");
		return -1;
	}

	return 1;
}

bool RSInterface::sendData(rs2_pose pose)
{
	/*
	Quaternion poseQuat;
	poseQuat.x = pose.rotation.x;
	poseQuat.y = pose.rotation.y;
	poseQuat.z = pose.rotation.z;
	poseQuat.w = pose.rotation.w;
	EulerAngles poseEuler = ToEulerAngles(poseQuat);
	*/

	std::string poseStr = "{";
	poseStr += "\"x\":" + std::to_string(pose.translation.x);
	poseStr += ",\"y\":" + std::to_string(pose.translation.y);
	poseStr += ",\"z\":" + std::to_string(pose.translation.z);
	poseStr += ",\"i\":" + std::to_string(pose.rotation.x);
	poseStr += ",\"j\":" + std::to_string(pose.rotation.y);
	poseStr += ",\"k\":" + std::to_string(pose.rotation.z);
	poseStr += ",\"w\":" + std::to_string(pose.rotation.w);
	poseStr += "}";
	
	send(new_socket, poseStr.c_str(), poseStr.length(), 0);
	return true;
	
}

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
// from: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
EulerAngles RSInterface::ToEulerAngles(Quaternion q) {
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}