#ifndef __SILVERWEB__CHANNEL_HPP__
#define __SILVERWEB__CHANNEL_HPP__

#include <silverweb/Message.hpp>
#include <silverweb/NodeId.hpp>

namespace silverweb {

  class Channel {
  public:
    Channel(NodeId nodeId, int fd);
    Channel(const Channel&) = delete;
    ~Channel();

    NodeId nodeId() const { return nodeId_; }
    int fd() const { return fd_; }
    Message pending() const { return txBuffer_.message(); }
    ChannelState state() const { return state_; }

    void send(Message msg);
    Message receive();
    ChannelPumpResult pump(bool txReady, bool rxReady);
    void suspend();
    void resume();
    void close();

    Channel& operator=(const Channel&) = delete;
    Channel& operator=(Channel&&) = default;

  private:
    NodeId nodeId_;
    int fd_;
    detail::TransmitBuffer txBuffer_;
    detail::ReceiveBuffer rxBuffer_;
    ChannelState state_;
  };
}
#endif
