.pragma library

.import 'MapBase.js' as MapBase

/*
 * EditableMap class
 * @contructor
 */
var EditableMap = function(ui, canvas) {
    MapBase.Map.call(this, ui, canvas);

    ui.map.mapNodeCreated.connect(this.onMapNodeCreated.bind(this));

    this.phantomMapNodes = [];
    this.focusedNode = undefined;
    this.mapNodeClicked = undefined;
    this.mapNodeFocused = undefined;

    // init
    this.addPhantomMapNodes(this.mapNodes);

    this.boundingRect = this.calculateBoundingRect();
};

EditableMap.prototype = Object.create(MapBase.Map.prototype);
EditableMap.prototype.constructor = EditableMap;

EditableMap.prototype.onClicked = function(mouse) {
    var point = Qt.point(mouse.x, mouse.y);

    var mapNode = this.findMapNodeAt(point);

    if (this.mapNodeClicked != undefined)
        this.mapNodeClicked(mapNode);
};

EditableMap.prototype.onPositionChanged = function(mouse) {
    var point = Qt.point(mouse.x, mouse.y);

    var mapNode = this.findMapNodeAt(point);
    if  (this.focusedNode !== mapNode) {
        if (mapNode) {
            if (this.focusedNode) this.focusedNode.onMouseOut();

            this.focusedNode = mapNode;
            this.focusedNode.onMouseIn();
        }
        else if (this.focusedNode) {
            this.focusedNode.onMouseOut();
            this.focusedNode = undefined;
        }
    }

    if (this.mapNodeFocused != undefined)
        this.mapNodeFocused(mapNode);
};

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
    var x = neighbourJObj.x + d.x;
    var y = neighbourJObj.y + d.y;

    var mapNodeJObj = new MapBase.MapNode(x, y, mapNodeQObj, this);
    this.mapNodes.push(mapNodeJObj);
    this.dirtyMapNodes.push(mapNodeJObj);

    var pos = Qt.point(x, y);
    var phantomMapNode = this.phantomMapNodes[pos];
    var i = this.mapNodes.indexOf(phantomMapNode);

    this.mapNodes.splice(i, 1);
    delete this.phantomMapNodes[pos];

    this.addPhantomMapNodes([mapNodeJObj]);
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

    this.canvas.requestPaint();
};

EditableMap.prototype.createPhantomNode = function(neighbourMapNode, direction) {
    var tileSize = this.qobj.world.tileSize;
    var d = MapBase.displacement(direction, tileSize);
    var pos = Qt.point(neighbourMapNode.x + d.x, neighbourMapNode.y + d.y);

    var phantomMapNode = this.phantomMapNodes[pos];
    if (phantomMapNode == undefined) {
        phantomMapNode = new PhantomMapNode(pos.x, pos.y, this);
        this.mapNodes.push(phantomMapNode);
        this.phantomMapNodes[pos] = phantomMapNode;
        this.dirtyMapNodes.push(phantomMapNode);
    }

    var oppositeDirection = neighbourMapNode.qobj.oppositeDirection(direction);
    phantomMapNode.neighbours[oppositeDirection] = neighbourMapNode;
};

/*
 * PhantomMapNode class
 * @contructor
 */
var PhantomMapNode = function(x, y, map) {
    MapBase.MapItem.call(this, x, y, map);

    this.neighbours = {};
    this.isPhantom = true;
};

PhantomMapNode.prototype = Object.create(MapBase.MapItem.prototype);
PhantomMapNode.prototype.constructor = PhantomMapNode;

PhantomMapNode.prototype.paint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    ctx.save();

    ctx.translate(this.x, this.y)

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
