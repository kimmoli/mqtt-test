import QtQuick 2.0
import Sailfish.Silica 1.0
import mqtt.test.Mqtt 1.0

Page
{
    id: page

    SilicaFlickable
    {
        anchors.fill: parent

        PullDownMenu
        {
            MenuItem
            {
                text: "About..."
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"),
                                          { "version": mqtt.version,
                                              "year": "2014",
                                              "name": "MQTT Test Application",
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/mqtt-test.png"} )
            }
        }

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingSmall
            PageHeader
            {
                title: "Mqtt-test"
            }

            TextField
            {
                id: _hostname
                label: "Host name or IP"
                placeholderText: "Enter host"
                text: "devaamo.fi"
                width: parent.width
            }

            TextField
            {
                id: _topic
                label: "Topic"
                placeholderText: "Enter topic"
                text: "sailfish/huurretursas/test"
                width: parent.width
            }

            TextField
            {
                id: _username
                label: "Username"
                placeholderText: "Enter username"
                text: "huurretursas"
                width: parent.width
            }

            TextField
            {
                id: _password
                label: "Password"
                placeholderText: "Enter password"
                text: "secret"
                width: parent.width
            }

            TextField
            {
                id: _message
                label: "Message"
                placeholderText: "Enter message"
                text: "hello world"
                width: parent.width
            }

            Button
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "publish"
                onClicked: mqtt.mqtt_main(_hostname.text, _topic.text, _username.text, _password.text, _message.text)
            }

//            Button
//            {
//                anchors.horizontalCenter: parent.horizontalCenter
//                text: "subscribe #"
//            }

        }
    }

    Mqtt
    {
        id: mqtt

        Component.onCompleted: readInitParams()

        /* DIRTY WAY BUT ME LIKES */
        onSettingsReady:
        {
            _hostname.text = getHostname()
            _topic.text = getTopic()
            _username.text = getUsername()
            _password.text = getPassword()
        }
    }
}


