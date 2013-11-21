//
//  AGClient.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include "AGClient.h"

AGClient::AGClient(io_service &iosev):m_iosev(iosev),m_acceptor(iosev)
{
    //,tcp::endpoint(tcp::v4(), clientPort)
    buf.resize(100);
}

//void AGClient::setup()
//{
//    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
//    m_acceptor.async_accept(*psocket, boost::bind(&AGClient::acceptHandler,this,psocket,_1));
//}

int AGClient::setup(char* addr,unsigned short int serverPort)
{
    serverAddr = tcp::endpoint(ip::address::from_string(addr),serverPort);
    psocket = boost::shared_ptr<tcp::socket>(new tcp::socket(m_iosev));
    boost::system::error_code ec;
    psocket->connect(serverAddr, ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return 1;
    }
    size_t tempLen = psocket->read_some(buffer(buf), ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return 2;
    }
    std::cerr << tempLen << " Byte recieved, port is "<<buf[0] << std::endl;
    
    serverAddr.port(buf[0]);
    tcp::endpoint tempEP(boost::asio::ip::tcp::v4(),psocket->local_endpoint().port());
    m_acceptor.open(tempEP.protocol());
    m_acceptor.set_option(socket_base::reuse_address(true));
    m_acceptor.bind(tempEP);
    m_acceptor.listen();
    return 0;
//    int temp[100];
//    size_t len = psocket->read_some(buffer(temp), ec);
//    if (ec) {
//        std::cout << boost::system::system_error(ec).what() << std::endl;
//        return;
//    }
//    if (temp[0]!=1||len!=sizeof(int)) {
//        std::cout << "wrong init!!\n";
//    }
    //(*psocket, boost::bind(&AGServer::acceptHandler,this,psocket,_1));
}

//void AGClient::acceptHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
//{
//    if (ec) {
//        std::cout << boost::system::system_error(ec).what() << std::endl;
//        return;
//    }
//    std::cout << "A new ip detected.\n";
//    int okFlag[1] = {1};
//    boost::system::error_code ec1;
//    psocket->write_some(buffer(okFlag),ec1);
//    if (ec1) {
//        setup();
//        return;
//    }
//    std::cout << "ip available.\n";
//    serverAddr = tcp::endpoint(psocket->remote_endpoint().address(),serverPort);
//    //m_acceptor.bind(serverAddr,ec1);
//}

void AGClient::recieve()
{
//    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
//    m_acceptor.async_accept(*psocket, boost::bind(&AGClient::recieveHandler,this,psocket,_1));
//}
//
//void AGClient::recieveHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec)
//{
//    if (ec) {
//        std::cout << boost::system::system_error(ec).what() << std::endl;
//        return;
//    }
    std::cout << "Waiting for Data.\n";
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
//    boost::system::error_code ec;
//    boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
//
//    psocket->async_connect(serverAddr,boost::bind(&AGClient::conncetHandler, this, psocket, message, _1));
//}
//void AGClient::conncetHandler(boost::shared_ptr<tcp::socket> psocket, vector<int> message, boost::system::error_code ec)
//{
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
