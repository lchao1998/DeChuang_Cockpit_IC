import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0





    Item {
        id: item1
        width: 183
        height: 76
        layer.enabled: true
        property int mymotPwrRate: 0


        Image {
            id: motPwrRate_Leftprogress
            x: 147
            source: "images/motPwrRate_Leftprogress.png"
            fillMode: Image.PreserveAspectFit

            Behavior on x{
                SmoothedAnimation{duration: 3000;velocity: -1}
            }

            Timeline {
                id: timeline
                currentFrame: mymotPwrRate
                startFrame: -60
                enabled: true
                endFrame: 0

                KeyframeGroup {
                    target: motPwrRate_Leftprogress
                    property: "x"
                    Keyframe {
                        frame: 0
                        value: 147
                    }

                    Keyframe {
                        frame: -60
                        value: 22
                    }
                }
            }
    }
}

/*##^##
Designer {
    D{i:0;height:76;width:183}D{i:4}D{i:1}
}
##^##*/
