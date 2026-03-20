

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0

Item {
    width: 1920
    height: 720

    Item {
        id: leftbutton
        width: 1920
        height: 720

        MyMotPwrRate {
            id: motPwrRate
            x: 13
            y: 672
        }

        Text {
            id: motPwrRate_text1
            x: 242
            y: 651
            width: 63
            height: 15
            color: "#ffffff"
            text: qsTr("电机功率")
            font.pixelSize: 16
            lineHeight: 1
            font.family: ":/font/Source Han Sans CN Regular.otf"
        }

        Text {
            id: motPwrRate_text2
            x: 14
            y: 651
            width: 64
            height: 15
            color: "#ffffff"
            text: qsTr("能量回收")
            font.pixelSize: 16
            lineHeight: 1
            font.family: "Source Han Sans CN Regular"
        }

        Text {
            id: motPwrRate_text3
            x: 274
            y: 693
            width: 44
            height: 17
            color: "#ffffff"
            text: baseinfodatamodel.gauge_VCU_MotPwrRate + "%"
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }

        Image {
            id: u_BATT_icon
            x: 369
            y: 668
            source: "images/u_BATT_icon.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: u_BATT_text
            x: 420
            y: 669
            width: 46
            height: 20
            color: "#ffffff"
            text: baseinfodatamodel.domain_BCM_U_BATT
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }

        Text {
            id: u_BATT_unit
            x: 473
            y: 673
            width: 14
            height: 17
            color: "#ffffff"
            text: qsTr("V")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1
            font.family: "D-DIN-PRO"
        }
    }

    Item {
        id: rightbutton
        width: 1920
        height: 720

        Text {
            id: odo
            x: 1405
            y: 670
            width: 48
            height: 18
            color: "#dadada"
            text: qsTr("ODO")
            font.pixelSize: 24
            font.weight: Font.ExtraLight
            font.family: "Source Han Sans CN Regular"
        }

        Text {
            id: odo_count
            x: 1460
            y: 669
            width: 70
            height: 20
            color: "#ffffff"
            text: baseinfodatamodel.gauge_VCU_TotalDistanceRange
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.ExtraLight
            font.family: "D-DIN-PRO"
        }

        Text {
            id: odo_unit
            x: 1535
            y: 670
            width: 30
            height: 20
            color: "#dadada"
            text: qsTr("km")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.ExtraLight
            font.family: "Source Han Sans CN Regular"
        }

        Text {
            id: trip
            x: 1600
            y: 670
            width: 48
            height: 18
            color: "#dadada"
            text: qsTr("TRIP")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.ExtraLight
            font.family: "Source Han Sans CN Regular"
        }

        Text {
            id: trip_count
            x: 1645
            y: 669
            width: 77
            height: 20
            color: "#ffffff"
            text: baseinfodatamodel.gauge_SubTotalDistanceRange
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            font.family: "D-DIN-PRO"
        }

        Text {
            id: trip_unit
            x: 1720
            y: 670
            width: 30
            height: 20
            color: "#dadada"
            text: qsTr("km")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.ExtraLight
            font.family: "Source Han Sans CN Regular"
        }

        Image {
            id: range_icon
            x: 1785
            y: 668
            source: "images/range_icon.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: range_count
            x: 1820
            y: 670
            width: 60
            height: 20
            color: "#ffffff"
            text: baseinfodatamodel.gauge_VCU_DistanceRange
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }

        Text {
            id: range_unit
            x: 1880
            y: 670
            width: 30
            height: 20
            color: "#dadada"
            text: qsTr("km")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.ExtraLight
            font.family: "Source Han Sans CN Regular"
        }
    }

    Image {
        id: music_icon
        x: 1457
        y: 482
        width: 117
        source: "images/music_icon.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: music_progress
        x: 1590
        y: 569
        width: 278
        height: 34
        source: "images/music_progress.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: music_name
        x: 1600
        y: 481
        width: 169
        height: 34
        color: "#ffffff"
        text: qsTr("Interlude")
        font.pixelSize: 40
        font.weight: Font.Normal
        font.family: "Source Han Sans CN Regular"
    }

    Text {
        id: music_authorname
        x: 1598
        y: 539
        width: 145
        height: 18
        color: "#ffffff"
        text: qsTr("Martin Garrix")
        font.pixelSize: 24
        font.weight: Font.Medium
        font.family: "Source Han Sans CN Regular"
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#4c4e50";formeditorZoom:0.5}
}
##^##*/

