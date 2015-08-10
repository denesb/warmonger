.pragma library

.import 'EditableMap.js' as EditableMap

/*
 * MapEditor class
 */
var MapEditor = function(ui, mapElement, mapItemTypesElement) {
    this.ui = ui;
    this.mapElement = mapElement;
    this.mapItemTypesElement = mapItemTypesElement;
    this.currentTerrainType = undefined;
    this.terrainTypeMap = {};

    var world = this.ui.map.world;
    for (var i = 0; i < world.terrainTypes.length; i++) {
        var terrainType = world.terrainTypes[i];
        this.terrainTypeMap[terrainType.objectName] = terrainType;
    }
}

MapEditor.prototype.onSelectedTerrainType = function(objectName) {
    this.currentTerrainType = this.terrainTypeMap[objectName];
    console.log("selected terrainType: " + objectName);
}

MapEditor.prototype.onSelectedSettlementType = function(objectName) {
    console.log("selected settlementType: " + objectName);
}

MapEditor.prototype.onSelectedUnitType = function(objectName) {
    console.log("selected unitType: " + objectName);
}

MapEditor.prototype.onMapNodeClicked = function(mapNode) {
    console.log("clicked map node - yuppeee " + this.mapItemTypesElement.currentIndex);
    if (mapNode.isPhantom) {

        var neighbours = mapNode.neighbours;
        var neighboursMap = {};
        for (var direction in neighbours) {
            if (neighbours.hasOwnProperty(direction)) {
                neighboursMap[direction] = neighbours[direction].qobj;
                console.log("neighbour[" + direction + "]: " + neighboursMap[direction]);
            }
        }

        this.ui.map.createMapNode(this.currentTerrainType, neighboursMap);
    }
}

MapEditor.prototype.onMapNodeFocused = function(mapNode) {
}
