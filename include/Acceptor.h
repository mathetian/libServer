// Copyright (c) 2014 The EventServer Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "Log.h"
#include "SQueue.h"
using namespace utils;

#include "Handler.h"
#include "EventLoop.h"

namespace eventserver
{

template<class T>
class TCPAcceptor : public Handler
{
public:
    /// Constructor
    TCPAcceptor(EventLoop* loop, int localport) : Handler(loop)
    {
        NetAddress addr = NetAddress(localport);

        m_sock = TCPSocket(&addr);

        attach();
        registerRead();

        INFO << "TCPAcceptor Initialization: " << m_sock.getsockname();

        assert(m_sock.status());
    }

    /// Destructor
    virtual ~TCPAcceptor() { }

public:
    virtual void onReceiveEvent()
    {
        Socket sock = wrap();

        if (sock.status() == true)
        {
            int thrid;
            EventLoop *loop = getRandomLoop(thrid);
            T* t = new T(loop, sock);
            if(loop == getLoop())
                t -> invoke();
            else
            {
                map<int, Handler*> m_handlers = getLoop() -> handlers();
                assert(m_handlers.find(thrid) != m_handlers.end());
                Handler *handler = m_handlers[thrid];
                handler -> push(t);
                int           fd = handler -> getSocket().fd();
                int cnt = send(fd, "c", 1, 0);
                if(errno != 0) WARN << strerror(errno) ;
            }
        }
    }

private:
    Socket wrap()
    {
        NetAddress a;
        Socket sock = m_sock.accept(&a);

        DEBUG << "New Connection, through socket(local): " << sock.fd();
        DEBUG << "Corrsponding address:" << sock.getpeername();

        return sock;
    }
};

};

#endif