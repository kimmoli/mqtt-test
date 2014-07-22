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
            spacing: Theme.paddingLarge
            PageHeader
            {
                title: "Mqtt-test"
            }
            Label
            {
                x: Theme.paddingLarge
                text: "Hello you"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            Button
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "do mqtt"
                onClicked: mqtt.mqtt_main()
            }
        }
    }

    Mqtt
    {
        id: mqtt
    }
}


