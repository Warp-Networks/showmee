/*
*
 *  ShowMee - Presentation software with touch interface
 *
 *  Copyright (C) 2011   Warp Networks, S.L. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


import Qt 4.7

Rectangle {
    signal showPreview;
    signal cancel;
    signal exit;
    signal about;


    width: 275 ; height: 200
    color: "black"
    radius: 10
    smooth: true
    z: 0
    opacity: 0

    ListModel {
        id: "menuModel"
        ListElement {
            text: "Select Presentation"
            icon: "images/preview.png"
            action: "showPreview"
        }
        ListElement {
            text: "Cancel"
            icon: "images/cancel.png"
            action: "cancel"
        }
        ListElement {
            text: "Exit Application"
            icon: "images/exit.png"
            action: "exit"
        }
        ListElement {
            text: "About ShowMee..."
            icon: "images/warp_small.png"
            action: "about"
        }
    }

    ListView {
        id: menuListView
        anchors.fill: parent
        model: menuModel

        delegate: Component {
            Rectangle {
                id: menuEntry
                width: parent.width
                height:50
                color: ((index % 2) ? "#444" : "#111")
                radius: 10
                smooth: true
                Image {
                    id: iconImage
                    source: model.icon
                    sourceSize {
                        width: height
                        height: height
                    }
                    width: 48
                    height: 48
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: (parent.height - width)/2
                    anchors.topMargin: (parent.height - height)/2
                }
                Text {
                    id: title
                    elide: Text.ElideRight
                    text: model.text
                    color: "white"
                    font.bold: true
                    anchors.left: iconImage.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // FIXME: how can I store a signal in a property?
                        if (model.action == "showPreview") {
                            showPreview()
                        } else if (model.action == "cancel") {
                            cancel()
                        } else if (model.action == "exit") {
                            exit()
                        } else if (model.action == "about") {
                            about()
                        }
                    }
                }
            }
        }
    }
}
