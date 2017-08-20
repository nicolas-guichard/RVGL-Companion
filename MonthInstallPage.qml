import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: page

    title: "Install RVR month tracks"

    property string option

    PropertyChanges {
        target: assetsManager
        onProgressChanged: {
            if (this.progress === 100){
                confirm.enabled = true
                progress.visible = false
            }
        }
    }

    ColumnLayout {
        spacing: 10
        width: parent.width
        Label {
            Layout.fillWidth: true
            text: "By clicking on this button, I will check what are current's month Re-Volt Race tracks, "
                + "download them and install them. It's as simple as it sounds!"
            wrapMode: Label.Wrap
        }

        Button {
            Layout.fillWidth: true
            id: confirm
            text: "Install month tracks"
            onClicked: assetsManager.installMonth()
            enabled: assetsManager.runningDownloads === 0
        }
        ProgressBar {
            Layout.fillWidth: true
            id: progress
            visible: assetsManager.runningDownloads !== 0
            from: 0
            to: 100
            value: assetsManager.progress
        }
    }
}
