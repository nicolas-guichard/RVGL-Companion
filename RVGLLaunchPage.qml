import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import RVGLLauncher 1.0

Page {
    id: page
    title: "RVGL Launcher"

    property var options: settings.installs[settings.currentInstall]["options"].split(' ')
    property string dir: settings.installs[settings.currentInstall]["dir"]

    RVGLLauncher {
        id: launcher
    }

    Flickable {
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height

        Column {
            width: page.width
            id: col
            Grid {
                id: settingsGrid
                width: page.width
                columns: 3

                move: Transition {
                    NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
                }

                CheckBox {
                    id: dev
                    text: "Dev mode"
                    checked: options.indexOf("-dev") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-dev") : options.splice(options.indexOf("-dev"), 1)
                    }
                }
                CheckBox {
                    id: nogamma
                    text: "No gamma"
                    checked: options.indexOf("-nogamma") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-nogamma") : options.splice(options.indexOf("-nogamma"), 1)
                    }
                }
                CheckBox {
                    id: nointro
                    text: "No intro"
                    checked: options.indexOf("-nointro") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-nointro") : options.splice(options.indexOf("-nointro"), 1)
                    }
                }
                CheckBox {
                    id: nop2p
                    text: "No P2P"
                    checked: options.indexOf("-nop2p") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-nop2p") : options.splice(options.indexOf("-nop2p"), 1)
                    }
                }
                CheckBox {
                    id: nopause
                    text: "No Pause"
                    checked: options.indexOf("-nopause") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-nopause") : options.splice(options.indexOf("-nopause"), 1)
                    }
                }
                CheckBox {
                    id: nosound
                    text: "No sound"
                    checked: options.indexOf("-nosound") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-nosound") : options.splice(options.indexOf("-nosound"), 1)
                    }
                }
                CheckBox {
                    id: sload
                    text: "Silent load"
                    checked: options.indexOf("-sload") !== -1
                    onCheckStateChanged: {
                        checked ? options.splice(0, 0, "-sload") : options.splice(options.indexOf("-sload"), 1)
                    }
                }

                Row {
                    CheckBox {
                        id: profile
                        text: checked ? "" : "Profile"
                        checked: options.indexOf("-profile") !== -1
                        onCheckStateChanged: {
                            checked ? options.splice(0, 0, "-profile", "") : options.splice(options.indexOf("-profile"), 2)
                        }
                    }
                    TextField {
                        id: profileName
                        placeholderText: "Last profile"
                        visible: profile.checked
                        onTextChanged: {
                            options.splice(options.indexOf("-profile")+1, 1, text)
                        }
                    }
                }

                Row {
                    CheckBox {
                        id: lobby
                        text: checked ? "" : "Lobby mode"
                        checked: options.indexOf("-lobby") !== -1
                        onCheckStateChanged: {
                            lobbyClient.checked = false
                            checked ? options.splice(0, 0, "-lobby") : options.splice(options.indexOf("-lobby"), 1)
                        }
                    }
                    CheckBox {
                        id: lobbyClient
                        text: checked ? "" : "Client"
                        visible: lobby.checked
                        checked: !options[options.indexOf("-lobby")+1].startsWith("-")
                        onCheckStateChanged: {
                            checked ? options.splice(options.indexOf("-lobby")+1, 0, lobbyAddress.text) : options.splice(options.indexOf("-lobby")+1, 1)
                        }
                    }
                    TextField {
                        id: lobbyAddress
                        placeholderText: "Host address"
                        visible: lobbyClient.checked
                        onTextChanged: {
                            options.splice(options.indexOf("-lobby")+1, 1, text)
                        }
                    }
                }

                Row {
                    CheckBox {
                        id: multisample
                        text: checked ? "" : "Multisample"
                        checked: options.indexOf("-multisample") !== -1
                        onCheckStateChanged: {
                            checked ? options.splice(0, 0, "-multisample", "") : options.splice(options.indexOf("-multisample"), 2)
                        }
                    }
                    TextField {
                        id: numSamples
                        placeholderText: "Num of samples"
                        visible: multisample.checked
                        onTextChanged: {
                            options.splice(options.indexOf("-multisample")+1, 1, text)
                        }
                    }
                }

                Row {
                    Layout.columnSpan: windowO.checked
                    CheckBox {
                        id: windowO
                        text: checked ? "" : "Windowed"
                        checked: options.indexOf("-window") !== -1
                        onCheckStateChanged: {
                            checked ? options.splice(0, 0, "-window", "", "") : options.splice(options.indexOf("-window"), 3)
                        }
                    }
                    TextField {
                        id: windowWidth
                        placeholderText: "Width"
                        visible: windowO.checked
                        onTextChanged: {
                            options.splice(options.indexOf("-window")+1, 1, text)
                        }
                        width: 50
                    }
                    Label {
                        text: "×"
                        verticalAlignment: Label.AlignVCenter
                        visible: windowO.checked
                        height: parent.height-10
                    }

                    TextField {
                        id: windowHeight
                        placeholderText: "Height"
                        visible: windowO.checked
                        onTextChanged: {
                            options.splice(options.indexOf("-window")+2, 1, text)
                        }
                        width: 50
                    }
                }
            }
            Button {
                text: "Set as default for current install"
                onClicked: {
                    settingsDialog.installsMirror[settings.currentInstall]["options"] = options.join(" ")
                    settings.installs = settingsDialog.installsMirror
                    settingsDialog.installsMirror = settingsDialog.installsMirror
                    rvglInstallComboBox.currentIndex = settings.currentInstall
                }
            }
            Button {
                text: "Launch!"
                onClicked: launcher.launch(dir, options)
            }
        }
    }
}
