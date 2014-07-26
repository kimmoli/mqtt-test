/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "mqtt.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

#include <mosquitto.h>


Mqtt::Mqtt(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
}

QString Mqtt::readVersion()
{
    return APPVERSION;
}

void Mqtt::readInitParams()
{
    QSettings settings("mqtt_test", "mqtt");

    settings.beginGroup("user");
    _username = settings.value("username", "").toString();
    _password = settings.value("password", "").toString();
    settings.endGroup();
    settings.beginGroup("muut");
    _hostname = settings.value("hostname", "devaamo.fi").toString();
    _topic = settings.value("topic", "sailfish/").toString();
    settings.endGroup();

    emit settingsReady();
}

Mqtt::~Mqtt()
{
}



/*
 * MOSQUITTO
 *
 */

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
void my_publish_callback(struct mosquitto *mosq, void *userdata, int mid);

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    qDebug() << message->topic << message->payload;
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    int i;

    qDebug() << "Connect callback";
    if(!result)
    {
        /* Subscribe to broker information topics on successful connect. */
        //mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
        qDebug() << "Connected";
    }
    else
    {
        qDebug() << "Connect failed";
    }
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;

    qDebug() << QString("Subscribed (mid: %1): %2").arg(mid).arg(granted_qos[0]);
    for(i=1; i<qos_count; i++)
    {
        qDebug() << granted_qos[i];
    }

}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
    /* Pring all log messages regardless of level. */
    qDebug() << str;
}

void my_publish_callback(struct mosquitto *mosq, void *userdata, int mid)
{
    qDebug() << "sent message id" << mid;
}

void Mqtt::mqtt_main(QString hostname, QString topic, QString username, QString password, QString message)
{

    qDebug() << hostname << topic << username << password << message;

    /* talteen */

    QSettings settings("mqtt_test", "mqtt");
    settings.beginGroup("user");
    settings.setValue("username", username);
    settings.setValue("password", password);
    settings.endGroup();
    settings.beginGroup("muut");
    settings.setValue("hostname", hostname);
    settings.setValue("topic", topic);
    settings.endGroup();

    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;


    mosquitto_lib_init();
    mosq = mosquitto_new("mqtt_jolla", clean_session, NULL);

    if(!mosq)
    {
        qDebug() << "Error: Out of memory";
        return;
    }
    else
        qDebug() << "mosquitto init ok";

    if (mosquitto_username_pw_set(mosq, username.toLocal8Bit().data(), password.toLocal8Bit().data()) == MOSQ_ERR_SUCCESS)
        qDebug() << "username set ok";
    else
        qDebug() << "username set failed";

    mosquitto_log_callback_set(mosq, my_log_callback);

    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
    mosquitto_publish_callback_set(mosq, my_publish_callback);
    mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);

    if(mosquitto_connect(mosq, hostname.toLocal8Bit().data(), port, keepalive))
    {
        qDebug() << "Unable to connect.";
        return;
    }
    else
        qDebug() << "Connected succefully";

    mosquitto_publish(mosq, NULL, topic.toLocal8Bit().data(), message.length(), message.toLocal8Bit().data(), 0, false);

    while(!mosquitto_loop(mosq, -1, 0))
    {
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    qDebug() << "destroyed succesfully";

    return;
}
