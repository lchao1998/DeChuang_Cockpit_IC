import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0

Image {
    id: motPwrRate_bg
    source: "images/motPwrRate_bg.png"
    property int motPwrRate: baseinfodatamodel.gauge_VCU_MotPwrRate
    layer.enabled: true
    fillMode: Image.PreserveAspectFit

    MyMotPwrRate_Leftprogress {
        id: motPwrRate_Leftprogress
        x: -56
        y: -31
        visible: motPwrRate_bg.motPwrRate <= 0
        mymotPwrRate: motPwrRate_bg.motPwrRate

    }

    MyMotPwrRate_Rightprogress {
        id: motPwrRate_Rightprogress
        x: 124
        y: -31
        visible: motPwrRate_bg.motPwrRate > 0
        mymotPwrRate: motPwrRate_bg.motPwrRate
    }

    Image {
        id: motPwrRate_zero
        x: 124
        y: -2
        source: "images/motPwrRate_zero.png"
        fillMode: Image.PreserveAspectFit
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.9;height:14;width:308}
}
##^##*/
