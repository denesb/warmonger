function createMapItem(obj, container) {
    var component = Qt.createComponent("MapItem.qml");
    var iconPath = warmonger.map.world.resources.getPath(obj.objectName + "_icon");
    var mapItem = component.createObject(container, {"iconSource": iconPath});

    if (mapItem == null) {
         console.log("Error creating object");
    }
}

function init() {
    var terrainTypes = warmonger.map.world.terrainTypes;
    var terrainType;

    console.log("init");

    for (var i = 0; i < terrainTypes.length; i++) {
        terrainType = terrainTypes[i];

        createMapItem(terrainType, terrainTypeItems);
    }
}
