import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0

Item {
    id: item1
    width: 242
    height: 76
    layer.enabled: true
    property int mymotPwrRate: 0

        Image {
            id: motPwrRate_Rightprogress
            x:-202
            source: "images/motPwrRate_Rightprogress.png"
            fillMode: Image.PreserveAspectFit

            Behavior on x{
                SmoothedAnimation{duration: 3000;velocity: -1}
            }
            y: 0
        }

        Timeline {
            id: timeline
            currentFrame: mymotPwrRate
            enabled: true
            endFrame: 100
            startFrame: 0

        KeyframeGroup {
            target: motPwrRate_Rightprogress
            property: "x"
            Keyframe {
                value: -202
                frame: 0
            }

            Keyframe {
                value: -25
                frame: 100
            }
        }
        }
}

/*##^##
Designer {
    D{i:0;height:76;width:242}D{i:1}D{i:4}
}
##^##*/
