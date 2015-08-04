.pragma library

.import 'MapBase.js' as MapBase

/*
 * EditableMap class
 */
var EditableMap = function(ui, canvas) {
    MapBase.Map.call(this, ui, canvas);
}

EditableMap.prototype = Object.create(MapBase.Map.prototype);
EditableMap.prototype.constructor = EditableMap;

EditableMap.prototype.onClicked = function(mouse) {
    var point = Qt.point(mouse.x, mouse.y);

    var mapNode = this.findMapNodeAt(point);
    if (mapNode) console.log(mapNode.qobj.objectName);
}

EditableMap.prototype.onPositionChanged = function(mouse) {
    var point = Qt.point(mouse.x, mouse.y);

    var mapNode = this.findMapNodeAt(point);
    if (mapNode) console.log(mapNode.qobj.objectName);
}
