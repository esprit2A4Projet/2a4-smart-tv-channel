import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    id: window

    property Component comMarker: mapMarker

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        zoomLevel: 15 // Increase zoom level for better accuracy
    }

    PositionSource {
        id: positionSource
        active: true
        onPositionChanged: {
            var latitude = position.coordinate.latitude;
            var longitude = position.coordinate.longitude;
            // Update map center to current position
            setCenter(latitude, longitude);
            // Add a marker for the current position
            addMarker(latitude, longitude);
        }
    }

    // Inside Rectangle element
    function setCenter(lat, lng) {
        mapView.center = QtPositioning.coordinate(lat, lng);
    }

    function addMarker(lat, lng) {
        var item = comMarker.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        });
        mapView.addMapItem(item);
    }

    Component {
        id: mapMarker
        MapQuickItem {
            id: markerImg
            anchorPoint.x: image.width/2
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
            }
        }
    }
}
