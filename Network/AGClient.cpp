//
//  AGClient.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include "AGClient.h"

AGClient::AGClient(io_service &iosev):m_iosev(iosev),m_acceptor(iosev,tcp::endpoint(tcp::v4(), 1001))
{
    
}

void AGClient::setup()
{
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    m_acceptor.async_accept(*psocket, boost::bind(&AGClient::acceptHandler,this,psocket,_1));
}

void AGClient::acceptHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << "A new ip detected.\n";
    int okFlag[1] = {1};
    boost::system::error_code ec1;
    psocket->write_some(buffer(okFlag),ec1);
    if (ec1) {
        setup();
        return;
    }
    std::cout << "ip available.\n";
    serverAddr = psocket->remote_endpoint();
    m_acceptor.bind(serverAddr);
}

void AGClient::recieve()
{
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    m_acceptor.async_accept(*psocket, boost::bind(&AGClient::recieveHandler,this,psocket,_1));
}

void AGClient::recieveHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << "Data recieved.\n";
    psocket->async_read_some(buffer(buf), boost::bind(&AGClient::readHandler,this,psocket,_1,_2));
}

void AGClient::readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec,size_t bytesArrived)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << bytesArrived << std::endl;
    len = bytesArrived/sizeof(int);
}

void AGClient::send(vector<int> message)
{
    boost::system::error_code ec;
    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    //for (int i = 0; i<6; i++) {
    psocket->async_connect(serverAddr,boost::bind(&AGClient::conncetHandler, this, psocket, message, _1));
    //}
}
void AGClient::conncetHandler(boost::shared_ptr<tcp::socket> psocket, vector<int> message, boost::system::error_code ec)
{
    psocket->async_write_some(buffer(message),
                              boost::bind(&AGClient::writeHandler, this, _1, _2));
}

void AGClient::writeHandler(error_code ec,
                            size_t bytes_transferred)
{
    if(ec)
        std::cout<< "发送失败!" << boost::system::system_error(ec).what() << std::endl;
    else
        std::cout<< bytes_transferred << "bytes 已发送" << std::endl;
}
