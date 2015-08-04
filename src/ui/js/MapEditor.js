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

EditableMap.prototype.onClick = function(mouse) {
    console.log(mouse);
}

EditableMap.prototype.onPositionChange = function(mouse) {
}
