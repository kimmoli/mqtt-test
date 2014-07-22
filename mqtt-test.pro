#
# Project mqtt-test, MQTT Test Application
#

TARGET = mqtt-test

CONFIG += sailfishapp

LIBS += -lmosquitto

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SOURCES += src/mqtt-test.cpp \
	src/mqtt.cpp
	
HEADERS += src/mqtt.h

OTHER_FILES += qml/mqtt-test.qml \
    qml/cover/CoverPage.qml \
    qml/pages/MqttUI.qml \
    qml/pages/AboutPage.qml \
    rpm/mqtt-test.spec \
	mqtt-test.png \
    mqtt-test.desktop

