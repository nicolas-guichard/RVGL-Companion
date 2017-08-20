import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Universal 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.0
import AssetsManager 1.0
import RVGLLauncher 1.0

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
        } else if (func == "install_month") {
            stackView.push("MonthInstallPage.qml")
        }
    }

    onOptionChanged: {
        stackView.currentItem.option = option
    }

    onAvailableStylesChanged: {
        styleBox.model = availableStyles
        styleBox.styleIndex = styleBox.find(settings.style, Qt.MatchFixedString)
        if (styleBox.styleIndex !== -1)
            styleBox.currentIndex = styleBox.styleIndex
    }

    Settings {
        id: settings
        property string style: "Default"
        property string rvglDir: ""
        property string rvglDefaultOptions: "-sload -nointro"
    }

    AssetsManager {
        id: assetsManager
    }

    RVGLLauncher {
        id: launcher
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
                    source: stackView.depth > 1 ? "images/back.png" : "images/drawer.png"
                }
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                        listView.currentIndex = -1
                    } else {
                        drawer.open()
                    }
                }
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

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        topMargin: header.height
        height: window.height - header.height
        dragMargin: stackView.depth > 1 ? 0 : undefined

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
            }

            model: pages

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    ListModel {
        id: pages
        ListElement {title: "Install RVR month tracks"; source: "qrc:/MonthInstallPage.qml"}
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
                anchors.fill: parent
                Repeater {
                    model: pages
                    Button {
                        text: model.title
                        onClicked: {
                            stackView.push(model.source)
                        }
                    }
                }
                Button {
                    text: "Fix filename cases"
                    onClicked: {
                        assetsManager.fixCases()
                    }
                    visible: Qt.platform.os !== "windows"
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

        visible: settings.rvglDir == ""

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.rvglDir = rvglDirBox.text
            settings.rvglDefaultOptions = rvglDefaultOptionsBox.text
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            rvglDirBox.text = settings.rvglDir
            rvglDefaultOptionsBox.text = settings.rvglDefaultOptions
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "RVGL directory:"
                    color: rvglDirBox.text == "" ? "#e41e25" : "black"
                }

                Button {
                    id: rvglDirBox
                    text: settings.rvglDir
                    Layout.fillWidth: true
                    onClicked: fileDialog.open()
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "RVGL default launch options:"
                }

                TextField {
                    id: rvglDefaultOptionsBox
                    text: settings.rvglDefaultOptions
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
                    rvglDirBox.text = fileUrl.toString().substring(8).replace(/\//g,'\\')
                } else {
                    rvglDirBox.text = fileUrl.toString().substring(7)
                }
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
                    + " ✔️ install RVGL as well as any complementary asset pack available as an archive.\n"
                    + " ✔️ install RVR month tracks automagically.\n"
                    + " ✔️ launch RVGL with custom options."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "As you use an actually good OS with case-sensitive filename support, "
                    + "it will also take care of filename cases for you!"
                wrapMode: Label.Wrap
                font.pixelSize: 12
                visible: Qt.platform.os !== "windows"
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
                font.pixelSize: 24
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
                font.pixelSize: 24
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
