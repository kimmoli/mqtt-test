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
    m_var = "";

    emit versionChanged();

}

QString Mqtt::readVersion()
{
    return APPVERSION;
}

void Mqtt::readInitParams()
{
    QSettings settings;
    m_var = settings.value("var", "").toString();

    emit varChanged();
}

Mqtt::~Mqtt()
{
}


QString Mqtt::readVar()
{
    return m_var;
}

void Mqtt::writeVar(QString s)
{
    m_var = s;

    QSettings settings;
    settings.setValue("var", m_var);

    emit varChanged();
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
        mosquitto_publish(mosq, NULL, "hello/world", 5, "kimmo", 0, true);
        qDebug() << "published";
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

void Mqtt::mqtt_main()
{

    char *host = "localhost";
    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;

    mosquitto_lib_init();
    mosq = mosquitto_new("my_mqtt_test", clean_session, NULL);

    if(!mosq)
    {
        qDebug() << "Error: Out of memory";
        return;
    }
    else
        qDebug() << "mosquitto init ok";

    mosquitto_log_callback_set(mosq, my_log_callback);

    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
    mosquitto_publish_callback_set(mosq, my_publish_callback);
    mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);

    if(mosquitto_connect(mosq, host, port, keepalive))
    {
        qDebug() << "Unable to connect.";
        return;
    }
    else
        qDebug() << "Connected succefully";

    mosquitto_publish(mosq, NULL, "hello/world", 5, "kimmo", 0, true);

    while(!mosquitto_loop(mosq, -1, 0))
    {
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    qDebug() << "destroyed succesfully";

    return;
}
