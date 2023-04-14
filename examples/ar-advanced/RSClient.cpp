// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <RSClient.hpp>
#include "nlohmann/json.hpp"
#define PORT 13579

RSClient::RSClient(std::string IP)
	:mIP(IP)
	// ,serv_addr{nullptr}
{
	std::cout << "RSClient Constructor2" << std::endl;
	startConnect();
}

int RSClient::startConnect()
{
	struct sockaddr_in serv_addr;


	
	if((mClient = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		std::cout << "socket creation error" << std::endl;
		return -1;
	} else {
		std::cout << "socket created" << std::endl;
	}

	std::cout << "\tRSClient startConnect sin_family" << std::endl;
	serv_addr.sin_family = AF_INET;
	std::cout << "\tRSClient startConnect sin_port" << std::endl;
	serv_addr.sin_port = htons(PORT);
	
	std::cout << "***luck***" << std::endl;

	std::cout << "RSClient startConnect2" << std::endl;
	

	std::cout << "RSClient startConnect inet_pton" << std::endl;
	std::cout << "IP: " << mIP.c_str() << std::endl;
	if(inet_pton(AF_INET, mIP.c_str(), &serv_addr.sin_addr) < 0)
	{
		printf("Invalid Address");
		return -1;
	}

	std::cout << "RSClient startConnect status connect" << std::endl;
	if((mStatus = connect(mClient, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) < 0)
	{
		std::cout << "Connection Failed" << std::endl;
		std::cout << mClient << std::endl;
		return -1;
	}

	return 1;
}

bool RSClient::sendData(nlohmann::json poseData)
{
	send(mClient, &poseData, poseData.size(), 0);
	// send(mClient, "hello", strlen("hello"), 0);
	return true;
	
}


/*
int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(client_fd, buffer, 1024);
	printf("%s\n", buffer);

	// closing the connected socket
	close(client_fd);
	return 0;
}
*/