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
    id: box
    objectName: "box"
    color: "black"

    signal fullScreen(bool full)
    signal changeDirectory(int index)
    signal startSlideShow(string document)
    signal preparePage(string document, int page)

    property string nextState: ""
    property int nextPage: 0
    property int numPages: 0
    property string currentFilePath: ""


    function pageReady(file, page) {
        box.state = nextState
        if (box.state == "Slides") {
            slide.currentFile = "image://pdf_slide_provider/" + file
            slide.currentPage = page
        }
        fullScreen(1)
    }

    ListView {
        id: pdfPreviewListView
        anchors.fill: parent
        model: pdfPreviewModel

        delegate: Component {
            Rectangle {
                id: previewBox
                width: box.width
                height:120
                color: ((index % 2) ? "#222" : "#111")
                Image {
                    id: previewImage
                    source: model.image
                    sourceSize {
                        width: height
                        height: height
                    }
                    width: 100
                    height: 100
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: (parent.height - width)/2
                    anchors.topMargin: (parent.height - height)/2
                }
                Text {
                    id: title
                    elide: Text.ElideRight
                    text: model.title
                    color: "white"
                    font.bold: true
                    anchors.top: parent.top
                    anchors.left: previewImage.right
                    anchors.bottom: parent.verticalCenter
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignBottom
                }
                Text {
                    id: pages
                    elide: Text.ElideRight
                    color: "#aaa"
                    text: ((model.file) ?  model.pages  : "")
                    font.pointSize: 10
                    anchors.top: title.bottom
                    anchors.left: previewImage.right
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignTop
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        if (model.file) {
                            box.nextState = "Slides"
                            box.nextPage = 0
                            box.numPages = model.pages
                            box.currentFilePath = model.path
                            preparePage(model.path, box.nextPage)
                        } else {
                            // Change dir
                            changeDirectory(index);
                        }
                    }
                }
           }
        }
     }

     Item {
        id: slideView
        anchors.fill: parent
        Image {
            id: slide
            sourceSize {
                width: width
                height: height
            }

            height: parent.height
            width: parent.width

            smooth: true

            property string currentFile: ""
            property string fooFile: ""
            property int currentPage: 0

            onCurrentFileChanged: {
                currentPage = 0
                slide.source = currentFile + "/page-" + currentPage
            }

            onCurrentPageChanged: {
                slide.source = currentFile + "/page-" + currentPage
            }

        }

        Menu {
             id: menuBox

             anchors.horizontalCenter: parent.horizontalCenter
             anchors.verticalCenter: parent.verticalCenter

            onShowPreview: { box.state = "Preview"; fullScreen(0); }
            onCancel: { box.state = "HideMenu" }
            onExit: { console.log("exit"); Qt.quit() }
            onAbout: { box.state = "About" }
        }

        About {
            id: aboutBox
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            onDismiss: { box.state = "HideAbout" }
        }


        opacity: 0
        scale: 0.3
        rotation: 180
        WarpGestureArea {
            anchors.fill: parent
            onSwipeRight: {
                if (slide.currentPage > 0) {
                    box.state = "MoveLeft"
                    box.nextState = "MoveLeft"
                    preparePage(box.currentFilePath, slide.currentPage - 1)
                }
            }
            onSwipeLeft: {
                if (slide.currentPage < (box.numPages - 1)) {
                    box.state = "MoveRight"
                    box.nextState = "MoveRight"
                    preparePage(box.currentFilePath, slide.currentPage + 1)
                }
            }
            onLongPress: {
                    box.state = "DisplayMenu"
            }
        }
     }

     states: [
       State {
            name: "Preview"
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 1
                scale: 1
                z: 0
                rotation: 0
                visible: true
            }
            PropertyChanges {
                target: menuBox
                opacity: 0
            }
            PropertyChanges {
                target: aboutBox
                opacity: 0
            }
        },
        State {
            name: "Slides"
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            PropertyChanges {
                target: slideView
                opacity: 1
                scale: 1
                rotation: 0
                visible: true
            }
            PropertyChanges {
                target: menuBox
                opacity: 0
            }
            PropertyChanges {
                target: aboutBox
                opacity: 0
            }
        },
        State {
            name: "MoveRight"
            PropertyChanges {
                target: slideView
                opacity: 0
                //scale: 0.3
                //rotation: 180

            }
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            StateChangeScript {
                name: "moveRightScript"
                script: {
                    slide.currentPage = slide.currentPage + 1
                    box.state = "Slides"
                }
            }
       },
       State {
            name: "MoveLeft"
            PropertyChanges {
                target: slideView
                opacity: 0
                //scale: 0.3
                //rotation: 180
            }
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            StateChangeScript {
                name: "moveLeftScript"
                script: {
                    slide.currentPage = slide.currentPage - 1
                    box.state = "Slides"
                }
            }
      },
      State {
            name: "HideMenu"
            PropertyChanges {
                target: menuBox
                opacity: 0
                z: 0
            }
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            PropertyChanges {
                target: slideView
                opacity: 1
                scale: 1
                rotation: 0
                visible: true
            }
            PropertyChanges {
                target: aboutBox
                opacity: 0
            }
      },
      State {
            name: "DisplayMenu"
            PropertyChanges {
                target: menuBox
                opacity: 1
                z: 1
            }
           PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            PropertyChanges {
                target: slideView
                opacity: 1
                scale: 1
                rotation: 0
                visible: true
            }
            PropertyChanges {
                target: aboutBox
                opacity: 0
            }
     },
      State {
            name: "About"
            PropertyChanges {
                target: aboutBox
                opacity: 1
                z: 1
            }
            PropertyChanges {
                target: menuBox
                opacity: 0
                z: 0
            }
           PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            PropertyChanges {
                target: slideView
                opacity: 1
                scale: 1
                rotation: 0
                visible: true
                z:0
            }

     },
      State {
            name: "HideAbout"

            PropertyChanges {
                target: menuBox
                opacity: 0
                z: 0
            }
            PropertyChanges {
                target: pdfPreviewListView
                opacity: 0
                scale: 0.3
                z: 0
                rotation: -180
                visible: true
            }
            PropertyChanges {
                target: slideView
                opacity: 1
                scale: 1
                rotation: 0
                visible: true
            }
            PropertyChanges {
                target: aboutBox
                opacity: 0
            }
      }

   ]
   transitions: [
        Transition {
            SequentialAnimation {
                PropertyAnimation {
                    properties: "scale,opacity,rotation"
                    duration: 50
                }
            }
        },
        Transition {
            to: "MoveRight"
            SequentialAnimation {
                PropertyAnimation {
                    //properties: "scale,opacity,rotation"
                    properties: "opacity"
                    duration: 50
                }
                ScriptAction { scriptName: "moveRightScript" }
            }
        },
        Transition {
            to: "MoveLeft"
            SequentialAnimation {
                PropertyAnimation {
                    //properties: "scale,opacity,rotation"
                    properties: "opacity"
                    duration: 50
                }
                ScriptAction { scriptName: "moveLeftScript" }
            }
        }
    ]
 }
