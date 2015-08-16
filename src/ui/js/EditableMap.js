.pragma library

.import 'MapBase.js' as MapBase

/*
 * EditableMap class
 * @contructor
 */
var EditableMap = function(ui, canvas, mouseArea) {
    MapBase.Map.call(this, ui, canvas, mouseArea);

    ui.map.mapNodeCreated.connect(this.onMapNodeCreated.bind(this));

    this.phantomMapNodes = [];
    this.focusedNode = undefined;
    this.mapNodeClicked = undefined;
    this.mapNodeFocused = undefined;

    // init
    this.addPhantomMapNodes(this.mapNodes);
    this.geometryChanged = true;
};

EditableMap.prototype = Object.create(MapBase.Map.prototype);
EditableMap.prototype.constructor = EditableMap;

EditableMap.prototype.onMapNodeCreated = function(mapNodeQObj) {
    var direction;
    var neighbourQObj;
    for (direction in mapNodeQObj.neighbours) {
        neighbourQObj = mapNodeQObj.neighbours[direction];
        if (neighbourQObj != undefined) {
            break;
        }
    }

    var neighbourJObj = this.findMapNodeJObj(neighbourQObj);

    // find out the position of this node based on the neighbour's position
    var oppositeDirection = mapNodeQObj.oppositeDirection(direction);
    var d = MapBase.displacement(oppositeDirection, this.qobj.world.tileSize);
    var pos = Qt.point(
        neighbourJObj.pos.x + d.width,
        neighbourJObj.pos.y + d.height
    );

    var phantomMapNode = this.phantomMapNodes[pos];
    var i = this.mapNodes.indexOf(phantomMapNode);

    this.mapNodes.splice(i, 1);
    delete this.phantomMapNodes[pos];

    var mapNodeJObj = new MapBase.MapNode(pos, mapNodeQObj, this);
    this.mapNodes.push(mapNodeJObj);
    this.markDirty(mapNodeJObj);

    this.addPhantomMapNodes([mapNodeJObj]);

    this.geometryChanged = true;
};

EditableMap.prototype.addPhantomMapNodes = function(mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {

        var mapNode = mapNodes[i];
        if (mapNode instanceof PhantomMapNode) continue;

        for (var direction in mapNode.qobj.neighbours) {
            var neighbour = mapNode.qobj.neighbours[direction];

            if (neighbour != undefined) continue;
            
            this.createPhantomNode(mapNode, direction);
        }
    }
};

EditableMap.prototype.createPhantomNode = function(neighbourMapNode, direction) {
    var tileSize = this.qobj.world.tileSize;
    var d = MapBase.displacement(direction, tileSize);
    var pos = Qt.point(
        neighbourMapNode.pos.x + d.width,
        neighbourMapNode.pos.y + d.height
    );

    var phantomMapNode = this.phantomMapNodes[pos];
    if (phantomMapNode == undefined) {
        phantomMapNode = new PhantomMapNode(pos, this);
        this.mapNodes.push(phantomMapNode);
        this.phantomMapNodes[pos] = phantomMapNode;
        this.markDirty(phantomMapNode);
    }

    var oppositeDirection = neighbourMapNode.qobj.oppositeDirection(direction);
    phantomMapNode.neighbours[oppositeDirection] = neighbourMapNode;
};

/*
 * PhantomMapNode class
 * @contructor
 */
var PhantomMapNode = function(pos, map) {
    MapBase.MapItem.call(this, pos, map);

    this.neighbours = {};
    this.isPhantom = true;
};

PhantomMapNode.prototype = Object.create(MapBase.MapItem.prototype);
PhantomMapNode.prototype.constructor = PhantomMapNode;

PhantomMapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    ctx.save();

    ctx.translate(this.pos.x, this.pos.y)

    if (this.focused)
        ctx.drawImage(worldQObj.getResourcePath("border_highlighted"), 0, 0);
    else
        ctx.drawImage(worldQObj.getResourcePath("border"), 0, 0);

    ctx.restore();
};

PhantomMapNode.prototype.onMouseIn = function() {
    this.focused = true;
    this.map.markDirty(this);
};

PhantomMapNode.prototype.onMouseOut = function() {
    this.focused = false;
    this.map.markDirty(this);
};
