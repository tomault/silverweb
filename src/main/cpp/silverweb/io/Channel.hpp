#ifndef __SILVERWEB__IO__CHANNEL_HPP__
#define __SILVERWEB__IO__CHANNEL_HPP__

#include <silverweb/io/TransmitBuffer.hpp>
#include <silverweb/io/ReceiveBuffer.hpp>
#include <deque>

namespace silverweb {
  namespace io {

    class Channel {
    public:
      enum class State {
	ACTIVE,
	SUSPENDED,
	DISCONNECTED,
	CLOSED
      };

    public:
      Channel(NodeId nodeId, int fd);
      Channel(const Channel&) = delete;
      Channel(Channel&& other);
      ~Channel();

      const NodeId& peer() { return nodeId_; }
      int fd() const { return fd_; }
      bool transmitBufferEmpty() const;
      bool receiveBufferFull() const;
      State state() const;

      Outcome pump(bool txReady, bool rxReady);

      /** @brief Send a message over this channel */
      void send(Message msg);

      /** @brief Receive a message sent over this channel */
      Message receive();

      /** @brief Tell the remote peer to stop sending messages */
      void suspend();

      /** @brief Tell the remote peer to start sending messages */
      void resume();

      /** @brief Disconnect from the remote peer temporarily */
      void disconnect();

      /** @brief Disconnect from the remote peer permanently */
      void close();
      
      Channel& operator=(const Channel&) = delete;
      Channel& operator=(Channel&& other);

    private:
      /** @brief Identity of the remote peer */
      NodeId nodeId_;

      /** @brief Socket used to communicate with the remote peer */
      int fd_;

      /** @brief Current channel state */
      State state_;

      /** @brief Holds message being sent to remote peer */
      TransmitBuffer txBuffer_;

      /** @brief Holds message being received from remote peer */
      ReceiveBuffer rxBuffer_;

      /** @brief Pending system messages to send to the remote peer */
      std::deque<Message> pending_;
    };
    
  }
}
 
#endif
