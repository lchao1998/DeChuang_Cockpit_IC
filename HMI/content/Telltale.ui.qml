

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 1920
    height: 720

    Image {
        id: positionLamp
        x: 359
        y: 19
        visible: telltaledatamodel.domain_BCM_PosLgtStInd
        source: "images/positionLamp.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: farLamp
        x: 452
        y: 19
        visible: telltaledatamodel.domain_BCM_HBeamSt
        source: "images/farLamp.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: frontfogLamp
        x: 551
        y: 13
        visible: telltaledatamodel.domain_BCM_FrontFogLightSts
        source: "images/frontfogLamp.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: rearfogLamp
        x: 645
        y: 14
        visible: telltaledatamodel.domain_BCM_RFogLgtSt
        source: "images/rearfogLamp.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: leftturnLight
        x: 735
        y: 15
        visible: telltaledatamodel.domain_BCM_L_TurnLgtCmdToIC
        source: "images/leftturnLight.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: rightturnLight
        x: 1135
        y: 15
        visible: telltaledatamodel.domain_BCM_R_TurnLgtCmdToIC
        source: "images/rightturnLight.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: door
        x: 1236
        y: 9
        visible: telltaledatamodel.domain_DCM_DoorNotClosedInd
        source: "images/door.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: vehicleFailure
        x: 1317
        y: 19
        visible: telltaledatamodel.domain_VCU_VehErrInd
        source: "images/vehicleFailure.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: ePB
        x: 1412
        y: 15
        visible: telltaledatamodel.gauge_VCU_ParkBrkSW
        source: "images/ePB.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: batteryFailure
        x: 1507
        y: 15
        visible: telltaledatamodel.domain_VCU_BMSErrInd
        source: "images/batteryFailure.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: driveReady
        x: 1505
        y: 110
        visible: telltaledatamodel.domain_VCU_DrvReadyInd
        source: "images/driveReady.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: majorFailure
        x: 904
        y: 78
        visible: telltaledatamodel.domain_VCU_VehSTOPInd
        source: "images/majorFailure.png"
        fillMode: Image.PreserveAspectFit
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33;height:1080;width:1920}
}
##^##*/

