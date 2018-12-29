#ifndef __SILVERWEB__IO__SYSTEMMESSAGETYPE_HPP__
#define __SILVERWEB__IO__SYSTEMMESSAGETYPE_HPP__

#include <stdint.h>

namespace silverweb {
  namespace io {

    enum class SystemMessageType : uint32_t {
      /** @brief Initial greeting that identifies peers to one another
       *
       *  The payload of a HELLO message consists of the credentials that
       *  identify and authenticate the node.  Currently, this is just the
       *  node's name, but authentication credentials will be added later.
       */
      HELLO = 0x80000000,

      /** @brief Instructs the remote peer to stop sending data */
      SUSPEND = 0x80000001,

      /** @brief Instructs the remote peer to start sending data */
      RESUME = 0x80000002,

      /** @brief Notifies the peer that this node will close its end of the
       *         socket, but it intends to reconnect later.
       *
       *  If the initiator of a communication channel receives this message,
       *  it should periodically attempt to reconnect to the remote peer,
       *  using increasing intervals between attempts, up to some maximum.
       *
       *  If the responder of a communication channel receives this message,
       *  it should not attempt to reconnect to the initiator (that's the
       *  initiator's job), but it should keep any pending messages around
       *  for when the peer reconnects later.
       *
       *  Regardless of whether the receiver is the initator or the responder,
       *  it should keep any messages it has left to transmit and send them
       *  to the remote peer when it reconnects.
       */
      DISCONNECT = 0x80000003,

      /** @brief Notifies the peer that this node will close its end of the
       *         socket, and it does not intend to reconnect later.
       *
       *  If the initiator receives this message, it should not attempt to
       *  reconnect to the remote peer.  The receiver should drop all
       *  messages it has yet to transmit to the remote peer and release
       *  all resources dedicated to the connection as soon as possible.
       */
      CLOSE = 0x80000004,

      /** @brief The responder is already connected to a node with those
       *         credentials
       *
       *  The credentials passed in the HELLO message identify a node the
       *  responder is already connected to.
       */
      ERR_PEER_EXISTS = 0xC0000001,

      /** @brief The remote peer is connected to too many peers */
      ERR_TOO_MANY_PEERS - 0xC0000002,

      /** @brief The remote peer sent a system message of an unknown type */
      ERR_UNKNOWN_MESSAGE_TYPE = 0xC0000003
    };
    
  }
}

#endif
