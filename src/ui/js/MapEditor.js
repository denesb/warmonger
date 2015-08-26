.pragma library

.import 'Map.js' as Map

var EditModes = {
    'Create': 0,
    'EditMapNode': 1
}

/*
 * MapEditor class
 * @contructor
 */
var MapEditor = function(ui, mapElement) {
    this.ui = ui;
    this.mapElement = mapElement;
    this.currentTerrainType = undefined;
    this.terrainTypeMap = {};
    this.editMode = 0;

    var world = this.ui.map.world;
    for (var i = 0; i < world.terrainTypes.length; i++) {
        var terrainType = world.terrainTypes[i];
        this.terrainTypeMap[terrainType.objectName] = terrainType;
    }
};

MapEditor.prototype.onTerrainTypeSelected = function(objectName) {
    this.currentTerrainType = this.terrainTypeMap[objectName];
    console.log("selected terrainType: " + objectName);
};

MapEditor.prototype.onSettlementTypeSelected = function(objectName) {
    console.log("selected settlementType: " + objectName);
};

MapEditor.prototype.onUnitTypeSelected = function(objectName) {
    console.log("selected unitType: " + objectName);
};

MapEditor.prototype.onEditModeChanged = function(editMode) {
    this.editMode = editMode;
    console.log("new editMode: " + editMode);
};

MapEditor.prototype.onMapNodeClicked = function(mapNode) {
    if (mapNode.isPhantom) {

        if (this.currentTerrainType == undefined) return;

        var neighbours = mapNode.neighbours;
        var neighboursMap = {};
        for (var direction in neighbours) {
            if (neighbours.hasOwnProperty(direction)) {
                neighboursMap[direction] = neighbours[direction].qobj;
            }
        }

        this.ui.map.createMapNode(this.currentTerrainType, neighboursMap);
    }
};

MapEditor.prototype.onMapNodeFocused = function(mapNode) {
};

MapEditor.prototype.saveMap = function() {
    this.ui.map.save();
};
