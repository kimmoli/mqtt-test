# mqtt-test
testing mosquitto on jolla

IIRC the libmosquitto-devel was provided by https://build.merproject.org/package/show/home:tbr:mqtt/mosquitto

You can add the repo to sdk by logging in to mersdk ssh and:

    sb2 -t SailfishOS-armv7hl -m sdk-install -R ssu ar mqtt http://repo.merproject.org/obs/home:/tbr:/mqtt/sailfish_latest_armv7hl/
    
Then go to http://127.0.0.1:8080/C/targets/SailfishOS-armv7hl and click refresh

(Not tested)
