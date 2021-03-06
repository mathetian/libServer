// Copyright (c) 2014 The EventServer Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include "Timer.h"
#include "TimerEvents.h"
#include "Noncopyable.h"
using namespace utils;

#include "Socket.h"

namespace eventserver
{

class EventLoop;

/**
** A handler that receives notifications on events on a file descriptor.
** The subclass may call registerxxxx/unRegisterxxx to enable/disable
** read/write events
** Handlers are only active when attached to both an event loop
** and callback(io schedule)
 */

class Handler : public Noncopyable
{
public:
    /// Constructor.
    Handler(EventLoop* loop);

    /// Destructor
    virtual ~Handler();

protected:
    /// Attach `this` to loop
    void attach();

    /// Detach `this` from loop
    void detach();

public:
    /// Invoke
    void invoke();

public:
    /// Register Read/Write Events
    void registerRead();
    void registerWrite();

    // Unregister Read/Write Events
    void unRegisterRead();
    void unRegisterWrite();

public:
    /// Called by the event loop when bytes are available on the file
    /// descriptor (if read event happens).
    virtual void onReceiveEvent()  = 0;

    /// Called by the event loop when there are bytes need to be written
    /// and written is enable
    virtual void onSendEvent()      { }

    /// Called by the event loop when there close events happens
    virtual void onCloseEvent(ClsMtd st);

public:
    /// Helper functions

    /// Return the socket
    Socket     getSocket() const;

    /// Return the loop it registers with
    EventLoop* getLoop()   const;

    /// Get random loop
    EventLoop* getRandomLoop(int &thrid) const;

public:
    /// Function about status
    /// Selector will detect it

    /// Return the status
    int getStatus() const;

    /// Update the registered event status
    ///
    /// @param val, the event need to be registered with
    void updateStatus(int val);

    /// Remove the status `val`
    ///
    /// @param val, the event need to be unregistered with
    void removeStatus(int val);

    /// Get the delete flag;
    bool getDelflag() const;

    /// Set the delete flag
    /// Later it will be removed from memory
    bool setDelflag();

    /// Proccess the atvie event
    void proceed(int event);

protected:
    void attach(const Callback<void> &call, const Timer &timer);

protected:
    /// EventLoop it belong to
    EventLoop*  m_loop;

    /// Socket belong to the handler
    /// Each handler will bind with a socket
    /// We name it as m_sock
    Socket      m_sock;

    /// Socket status
    int         m_status;

    /// Delete flag
    bool        m_delflag;

public:
    virtual void     push(Handler*)
    {
    }
};

};

#endif