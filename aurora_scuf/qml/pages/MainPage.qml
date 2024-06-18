/*******************************************************************************
**
** Copyright (C) 2022 ru.auroraos
**
** This file is part of the Моё приложение для ОС Аврора project.
**
** Redistribution and use in source and binary forms,
** with or without modification, are permitted provided
** that the following conditions are met:
**
** * Redistributions of source code must retain the above copyright notice,
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice,
**   this list of conditions and the following disclaimer
**   in the documentation and/or other materials provided with the distribution.
** * Neither the name of the copyright holder nor the names of its contributors
**   may be used to endorse or promote products derived from this software
**   without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
** FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
** OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
** EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*******************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import Client 1.0

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All
        TypeClient{
            id: myClient
        }

        Connections{

            target: fileclass
            onTransmitNewText: text1.text=text
        }

//        Text{
//            id: text1
//            text: qsTr()
//            anchors.centerIn: parent
//        }

        PageHeader{
            z:1
            title: qsTr("SCUF camera")}
        Camera{
                id:camera
                imageProcessing.whiteBalanceMode:
                    CameraImageProcessing.WhiteBalanceFlash
                exposure{
                    exposureCompensation: -1.0
                    exposureMode: Camera.ExposurePortrait
                }
                imageCapture.onImageCaptured: photoPreview.source = preview

        }



        Button{
            id: button_goto
            z:1
            anchors{
                left: parent.left
                top:parent.top
            }
            text: "Выбор фотографии"
            onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
        }

        Button{
            id:button_1
            z:1
            anchors{
                bottom: parent.bottom
                left:parent.left
                right:parent.horizontalCenter
            }

            text:qsTr("Фокусировка")
            onClicked: camera.searchAndLock()
        }
        Button{
            id:button_2
            z:1
            anchors{
                bottom: parent.bottom
                right:parent.right
                left:parent.horizontalCenter
            }
            text: qsTr("Сделать фото")
            onClicked: camera.imageCapture.captureToLocation(StandardPaths.documents+"/"+Qt.application.name+".jpg")

        }
        Button{
            id:button_3
            z:1
            anchors{
                bottom:button_1.top
                left:parent.left
                right: parent.right
            }
            //signal qmlSignal(msg: string)
            text: qsTr("Send last photo")
            onClicked: { myClient.setUrlPort(qsTr("192.168.137.1"),8090);
                myClient.start(qsTr(StandardPaths.documents+"/"+Qt.application.name+".jpg"));
            }
        }
        VideoOutput{

            z:0
            anchors{
                top:parent.top
                left:parent.left
                bottom:button_3.top
                right:parent.right
            }
            source: camera
        }
    }

