#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <QHostAddress>
#include <QObject>
#include <QWebSocket>

#include "network/aopacket.h"

class NetworkSocket : public QObject
{
  Q_OBJECT

public:
  NetworkSocket(QWebSocket *f_socket, QObject *parent = nullptr);
  ~NetworkSocket();

  /**
   * @brief Returns the Address of the remote socket.
   *
   * @return QHostAddress object of the socket.
   */
  QHostAddress peerAddress();

  /**
   * @brief Closes the socket by request of the child AOClient object or the server.
   *
   * @param The close code to the send to the client.
   */
  void close(QWebSocketProtocol::CloseCode f_code = QWebSocketProtocol::CloseCodeNormal);

  /**
   * @brief Writes data to the network socket.
   *
   * @param Packet to be written to the socket.
   */
  void write(AOPacket *f_packet);

Q_SIGNALS:
  /**
   * @brief handlePacket
   * @param f_packet
   */
  void handlePacket(AOPacket *f_packet);

  /**
   * @brief Emitted when the socket has been closed and the client is disconnected.
   */
  void clientDisconnected();

private Q_SLOTS:
  /**
   * @brief Handles the processing of WebSocket data.
   *
   * @return Decoded AOPacket to be processed by the child AOClient object.
   */
  void handleMessage(QString f_data);

private:
  QWebSocket *m_client_socket;

  /**
   * @brief Remote IP of the client.
   *
   * @details In the case of the WebSocket we also check if this has been proxy forwarded.
   */
  QHostAddress m_socket_ip;
};

#endif
