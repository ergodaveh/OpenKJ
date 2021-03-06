/*
 * Copyright (c) 2013-2016 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "khipcclient.h"
#include <QDataStream>


KhIPCClient::KhIPCClient(QString remoteServername, QObject *parent) :
        QObject(parent) {

    m_socket = new QLocalSocket(this);
    m_serverName = remoteServername;
    m_command = CMD_NOOP;

    connect(m_socket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socket_readReady()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socket_error(QLocalSocket::LocalSocketError)));
    m_blockSize = 0;
}

KhIPCClient::~KhIPCClient() {
    m_socket->abort();
    delete m_socket;
    m_socket = NULL;
}

void KhIPCClient::send_MessageToServer(int command)
{
    // m_socket->abort();
    m_command = command;
    if (m_socket->state() != QLocalSocket::ConnectedState)
    {
        qDebug() << "IPC Client - Trying to send command but conneciton is closed, trying to connect";
        m_socket->connectToServer(m_serverName);
    }
    else
    {
        qDebug() << "IPC Client - Sending command over existing connection";
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out << m_command;
        out.device()->seek(0);
        m_socket->write(block);
        m_socket->flush();
    }
}


void KhIPCClient::socket_connected(){
    qDebug() << "IPC Client - Connection established, sending command";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << m_command;
    out.device()->seek(0);
    m_socket->write(block);
    m_socket->flush();
}

void KhIPCClient::socket_disconnected() {
    qDebug() << "IPC Client - Disconnected";
}


void KhIPCClient::socket_readReady() {
    qDebug() << "IPC Client - readReady signal emitted";
}

void KhIPCClient::socket_error(QLocalSocket::LocalSocketError) {
    qDebug() << "IPC Client - Socket Error";
}
