//
//  main.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include "AGClient.h"
#include "AGServer.h"

//int main(int argc, const char * argv[])
//{
//    using std::cout;
//    using std::endl;
//    using std::cin;
//    using std::vector;
//    
//    io_service iosev;
//    AGServer server(iosev);
//    int ports[6] = {12345,12346,12347,12348,12349,12350};
//    server.setup(ports);
//    iosev.run();
//    while (1) {
//        cout << "Choose opertion: r for receive, s for send";
//        char op;
//        cin >> op;
//        if (op=='r') {
//            iosev.reset();
//            server.recieve(0);
//            iosev.run();
//        }
//        else if(op=='s')
//        {
//            cout << "Input numbers: ";
//            int n;
//            cin >> n;
//            vector<int> temp;
//            for (int i =0; i<n; i++) {
//                int t;
//                cin >> t;
//                temp.push_back(t);
//            }
//            cout << "Input finished.\n";
//            iosev.reset();
//            server.send(temp,0);
//            iosev.run();
//        }
//        else break;
//    }
//
//    std::cout << "Bye, World!\n";
//    return 0;
//}

int main(int argc, const char * argv[])
{
    using std::cout;
    using std::endl;
    using std::cin;
    using std::vector;
    io_service iosev;
    
    AGClient client(iosev);
    cout << "Input IP address: ";
    char ip[20] = "192.168.1.106";
    //cin >> ip;
    cout << ip;
    client.setup(ip,25556);
    iosev.run();

    
    while (1) {
        cout << "Choose opertion: r for receive, s for send";
        char op;
        cin >> op;
        if (op=='r') {
            iosev.reset();
            client.recieve();
            iosev.run();
        }
        else if(op=='s')
        {
            cout << "Input numbers: ";
            int n;
            cin >> n;
            vector<int> temp;
            for (int i =0; i<n; i++) {
                int t;
                cin >> t;
                temp.push_back(t);
            }
            cout << "Input finished.\n";
            iosev.reset();
            client.send(temp);
            iosev.run();
        }
        else break;
    }
    std::cout << "Bye, World!\n";
    return 0;
}
