import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.5
import GetThermal 1.0
import "qrc:/controls"
import "qrc:/images"

Item {
    id: item1
    anchors.fill: parent

    property alias acq: acq
    property alias player: player
    property alias videoOutput: videoOutput
    width: 640

    UvcAcquisition {
        id: acq
    }

    UvcVideoProducer {
        id: player
        uvc: acq
    }

    RowLayout {
        spacing: 0
        anchors.fill: parent

        Pane {
            x: 0
            width: 400
            bottomPadding: 0
            rightPadding: 0
            leftPadding: 0
            topPadding: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectFit
                source: player
                VideoRoi {
                    id: radRoi
                    visible: acq.cci.supportsRadiometry
                    acq: acq
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            SpotInfo {
                id: spotInfo
                anchors.top: parent.top
                anchors.right: parent.right
                rightPadding: 5
                topPadding: 5
                width: 50
                acq: acq
                farenheitTemps: rangeDisplay.farenheitTemps
            }
            Image {
                id: imageLogo
                anchors.top: parent.top
                anchors.left: parent.left
                leftPadding: 5
                topPadding: 5
                height: 15
                fillMode: Image.PreserveAspectFit
                source: "/images/images/brand-logo.png"
            }
        }

    }

}
