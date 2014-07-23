/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MQTT_H
#define MQTT_H
#include <QObject>

#include <mosquitto.h>


class Mqtt : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString version READ readVersion NOTIFY versionChanged())

public:
    explicit Mqtt(QObject *parent = 0);
    ~Mqtt();


    QString readVersion();


    Q_INVOKABLE void readInitParams();

    Q_INVOKABLE void mqtt_main(QString hostname, QString topic, QString username, QString password, QString message);

    Q_INVOKABLE QString getHostname() { return _hostname; }
    Q_INVOKABLE QString getTopic() { return _topic; }
    Q_INVOKABLE QString getUsername() { return _username; }
    Q_INVOKABLE QString getPassword() { return _password; }


signals:
    void settingsReady();
    void versionChanged();

private:

    QString _username;
    QString _password;
    QString _hostname;
    QString _topic;

};


#endif // MQTT_H

