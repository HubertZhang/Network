//
//  AGServer.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include "AGServer.h"

AGServer::AGServer(io_service &iosev):m_iosev(iosev),m_acceptor(iosev,tcp::endpoint(tcp::v4(), 1000))
{
    
}

void AGServer::setup(char* addr)
{
    clientAddr = tcp::endpoint(ip::address::from_string(addr),1000);
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    boost::system::error_code ec;
    psocket->connect(clientAddr, ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    int temp[100];
    size_t len = psocket->read_some(buffer(temp), ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    if (temp[0]!=1||len!=1) {
        std::cout << "wrong init!!\n";
    }
    //(*psocket, boost::bind(&AGServer::acceptHandler,this,psocket,_1));
}

//void AGServer::acceptHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
//{
//    if (ec) {
//        std::cout << boost::system::system_error(ec).what() << std::endl;
//        return;
//    }
//    std::cout << "A new ip detected.\n";
//    
//    serverAddr = psocket->remote_endpoint();
//    m_acceptor.bind(serverAddr);
//}

void AGServer::recieve()
{
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    m_acceptor.async_accept(*psocket, boost::bind(&AGServer::recieveHandler,this,psocket,_1));
}

void AGServer::recieveHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << "Data recieved.\n";
    psocket->async_read_some(buffer(buf), boost::bind(&AGServer::readHandler,this,psocket,_1));
}

void AGServer::readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    else std::cout << buf.size() << std::endl;
}

void AGServer::send(vector<int> message)
{
    boost::system::error_code ec;
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    //for (int i = 0; i<6; i++) {
    psocket->async_connect(clientAddr,boost::bind(&AGServer::conncetHandler, this, psocket, message, _1));
    //}
}
void AGServer::conncetHandler(boost::shared_ptr<tcp::socket> psocket, vector<int> message, boost::system::error_code ec)
{
    psocket->async_write_some(buffer(message),
                              boost::bind(&AGServer::writeHandler, this, _1, _2));
}

void AGServer::writeHandler(error_code ec,
                            size_t bytes_transferred)
{
    if(ec)
        std::cout<< "发送失败!" << boost::system::system_error(ec).what() << std::endl;
    else
        std::cout<< bytes_transferred << "bytes 已发送" << std::endl;
}
