import QtQuick 2.2
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Client 1.0

Page{
    id: page
    TypeClient{
        id: myClient
    }
    ApplicationWindow {
        initialPage: Component {
            Page {
                id: page
                property string selectedPath


               Column {
                    anchors.centerIn: parent
                    spacing: Theme.paddingMedium

                    Button {
                        id: imgButton
                        text: "Выбрать изображение"
                        onClicked: pageStack.push(imagePickerPage)
                    }

                    Image {

                        id: selectedImage
                        sourceSize.width: Theme.itemSizeHuge
                    }
                }

                Component {
                    id: imagePickerPage
                    ImagePickerPage {
                        onSelectedContentPropertiesChanged: {

                            selectedImage.source = selectedContentProperties.filePath
                            selectedPath = selectedContentProperties.url
                        }
                    }

                }

                Button{

                    id:button_3
                    z:1
                    anchors{
                        top:parent.Center
                        left:parent.left
                        right: parent.right
                    }
                    //signal qmlSignal(msg: string)
                    text: qsTr("Отправить выбранное изображение")
                    onClicked: { myClient.setUrlPort(qsTr("192.168.137.1"),8090);
                        myClient.start(qsTr(selectedPath));
                    }
                }
            }
        }
    }

}


