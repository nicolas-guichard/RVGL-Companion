import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Universal 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.0

ApplicationWindow {
    visible: true
    id: window
    width: 800
    height: 600
    title: qsTr("RVGL Companion")

    property string func
    property string option
    property var availableStyles

    onFuncChanged: {
        if (func == "install_asset") {
            stackView.push("qrc:/AssetsInstallPage.qml")
        }
    }

    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
        property string style: "Default"
        property var installs: [{name: "Default (quick launch)", dir: "", options: "-nointro -profile"}]
        property int currentInstall: 0
    }

    header: ToolBar {
        id: header
        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/back.png"
                }
                onClicked: stackView.pop()
                visible: stackView.depth > 1
            }

            Label {
                id: titleLabel
                text: stackView.currentItem.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    ListModel {
        id: pages
        ListElement {title: "Install RVGL or assets"; source: "qrc:/AssetsInstallPage.qml"}
        ListElement {title: "Launch RVGL"; source: "qrc:/RVGLLaunchPage.qml"}
    }

    StackView {
        id: stackView
        anchors.fill: parent
        anchors.margins: 15

        initialItem: Page {
            id: pane
            title: "RVGL Companion"
            Flow {
                spacing: 15
                Repeater {
                    model: pages
                    Button {
                        text: model.title
                        onClicked: {
                            stackView.push(model.source)
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: "Settings"

        visible: settings.installs[0]["dir"] === ""

        property var installsMirror: settings.installs

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.currentInstall = rvglInstallComboBox.currentIndex
            settings.installs = installsMirror
            rvglInstallComboBox.currentIndex = settings.currentInstall
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            installsMirror = settings.installs
            rvglInstallComboBox.currentIndex = settings.currentInstall
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Install:"
                }

                ComboBox {
                    id: rvglInstallComboBox
                    editable: true
                    model: settingsDialog.installsMirror
                    textRole: "name"
                    Layout.fillWidth: true
                    currentIndex: settings.currentInstall
                    onAccepted: {
                        if (find(editText) === -1) {
                            currentIndex = 0
                            settingsDialog.installsMirror.push({name: editText, dir: "", options: ""})
                            currentIndex = model.length-1
                            model = settingsDialog.installsMirror
                        }
                    }
                }

                Button {
                    text: "×"
                    onClicked: {
                        rvglInstallComboBox.decrementCurrentIndex()
                        settingsDialog.installsMirror.splice(rvglInstallComboBox.currentIndex+1, 1)
                        rvglInstallComboBox.model = settingsDialog.installsMirror
                    }
                    enabled: rvglInstallComboBox.currentIndex !== 0
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "RVGL directory:"
                    color: rvglDirBox.text == "" ? "#e41e25" : "black"
                }

                Button {
                    id: rvglDirBox
                    text: settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["dir"]
                    Layout.fillWidth: true
                    onClicked: fileDialog.open()
                    onTextChanged: settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["dir"] = text
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "RVGL default launch options:"
                }

                TextField {
                    id: rvglLaunchOptionsBox
                    text: settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["options"]
                    onTextChanged: settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["options"] = text
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    Layout.fillWidth: true
                    model: window.availableStyles
                    onModelChanged: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
            }
        }

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            selectFolder: true
            onAccepted: {
                if (Qt.platform.os === "windows") {
                    settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["dir"] = fileUrl.toString().substring(8).replace(/\//g,'\\')
                } else {
                    settingsDialog.installsMirror[rvglInstallComboBox.currentIndex]["dir"] = fileUrl.toString().substring(7)
                }
                settingsDialog.installsMirror = settingsDialog.installsMirror
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: "About RVGL Companion"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "RVGL Companion is a companion app for RVGL. (no kidding involved)"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "It will enable you to:\n"
                    + " ✔️ install RVGL as well as any complementary asset pack available as downloadable archive.\n"
                    + " ✔️ launch RVGL with custom options."
                    + (Qt.platform.os !== "windows" ? "\n ✔️ fix filename cases." : "")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Better yet, it with do all of this without the hassle of even launching the app,"
                    + "thanks to the rvgl: URI scheme!"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Due credits"
                font.bold: true
                font.pixelSize: 16
            }

            Label {
                width: aboutDialog.availableWidth
                text: "7za.exe 1604, licensed under LGPL, comes bundled with this executable."
                    + "You can get its source code on 7-zip.org."
                wrapMode: Label.Wrap
                font.pixelSize: 12
                visible: Qt.platform.os === "windows"
            }

            Label {
                width: aboutDialog.availableWidth
                text: "The icon for this application is a derivative from freakishhorde's Companion Cube."
                    + "You can get it at blendswap.com/blends/44396, used under CC-BY 3.0 licence."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Licence"
                font.bold: true
                font.pixelSize: 16
            }

            Label {
                width: aboutDialog.availableWidth
                text: "This application is licenced under the terms of the GNU GPLv3 licence."
                    + "You can learn more about this at github.com/Nico264/RVGL-Companion, where the source code is hosted."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}
