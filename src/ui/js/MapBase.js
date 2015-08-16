.pragma library

.import 'Util.js' as Util

function displacement(dir, tileSize) {
    if (dir == 'West') return Qt.size(-tileSize.width, 0);
    if (dir == 'NorthWest') return Qt.size(-tileSize.width/2, -tileSize.height * 3/4);
    if (dir == 'NorthEast') return Qt.size(tileSize.width/2, -tileSize.height * 3/4);
    if (dir == 'East') return Qt.size(tileSize.width, 0);
    if (dir == 'SouthEast') return Qt.size(tileSize.width/2, tileSize.height * 3/4);
    if (dir == 'SouthWest') return Qt.size(-tileSize.width/2, tileSize.height * 3/4);

    err = "Unknown direction: " + dir;
    console.error(err);
    throw err;
};

var MouseEvents = {
    clicked: 'clicked',
    pressed: 'pressed',
    doubleClicked: 'doubleClicked',
    entered: 'entered',
    exited: 'exited',
    positionChanged: 'positionChanged',
    pressAndHold: 'pressAndHold',
    pressed: 'pressed',
    released: 'released',
    wheel: 'wheel'
};

/*
 * Map class.
 * @contructor
 */
var Map = function(ui, canvas, mouseArea) {
    this.ui = ui;
    this.qobj = ui.map;
    this.canvas = canvas;
    this.mouseArea = mouseArea;

    this.lastMouseEvent = undefined;
    this.lastMousePos = undefined;
    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.geometryChanged = true;
    this.boundingRect = Qt.rect(0, 0, 0, 0);
    this.size = Qt.size(0, 0);

    // init
    this.loadResources();
    this.createNode(this.qobj.mapNodes[0], Qt.point(0, 0), {});
    this.canvas.requestPaint();
};

Map.prototype.createNode = function(mapNodeQObj, pos, visitedNodes) {
    visitedNodes[mapNodeQObj] = true;
    var tileSize = this.qobj.world.tileSize;

    var mapNode = new MapNode(pos, mapNodeQObj, this);
    this.mapNodes.push(mapNode);

    for (var direction in mapNodeQObj.neighbours) {
        var neighbour = mapNodeQObj.neighbours[direction];

        if (neighbour == undefined || neighbour in visitedNodes) continue;

        var d = displacement(direction, tileSize);
        this.createNode(
            neighbour,
            Qt.point(pos.x + d.width, pos.y + d.height),
            visitedNodes
        );
    }
};

Map.prototype.onPaint = function(region) {
    if (!this.ready) return;
    if (this.geometryChanged) {
        this.updateGeometry();
        this.geometryChanged = false;
    }

    var ctx = this.canvas.getContext("2d");

    console.log(">>>>>PAINT<<<<");
    console.log("region      : " + region);
    console.log("boundingRect: " + this.boundingRect);
    console.log("size        : " + this.size);
    console.log("canvasSize  : " + this.canvas.canvasSize);
    console.log("canvasWindow: " + this.canvas.canvasWindow);
    console.log("--------------");

    ctx.clearRect(
        0,
        0,
        this.canvas.canvasSize.width,
        this.canvas.canvasSize.height
    );

    for (var i = 0; i < this.mapNodes.length; i++) {
        this.mapNodes[i].onPaint(ctx);
    }
};

Map.prototype.loadResources = function() {
    var resourcePaths = this.qobj.world.resourcePaths;
    var path;

    for (var resource in resourcePaths) {
        path = resourcePaths[resource];
        this.canvas.loadImage(path);
        this.loadQueue.push(path);
    }
};

Map.prototype.onResourceLoaded = function() {
    var image;
    var loaded = [];

    for (var i = 0; i < this.loadQueue.length; i++) {
        image = this.loadQueue[i];
        if (this.canvas.isImageError(image)) {
            console.error("Error loading image " + image);
            loaded.push(image);
        }

        if (this.canvas.isImageLoaded(image)) {
            console.info("Successfully loaded image " + image);
            loaded.push(image);
        }
    }

    for (var i = 0; i < loaded.length; i++) {
        var index = this.loadQueue.indexOf(loaded[i]);
        this.loadQueue.splice(index, 1);
    }

    if (this.loadQueue.length == 0) {
        console.info("All resources loaded");
        this.onResourcesLoaded();
    }
};

Map.prototype.onResourcesLoaded = function() {
    this.ready = true;
    this.canvas.requestPaint();
};

Map.prototype.findMapNodeAt = function(point) {
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.contains(point)) return mapNode;
    }

    return undefined;
};

Map.prototype.markDirty = function(mapNode) {
    var tileSize = this.qobj.world.tileSize;

    this.canvas.markDirty(Qt.rect(
        mapNode.pos.x,
        mapNode.pos.y,
        tileSize.width,
        tileSize.height
    ));
};

Map.prototype.findMapNodeJObj = function(mapNodeQObj) {
    var mapNodeJObj = undefined;
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.qobj == mapNodeQObj) {
            mapNodeJObj = mapNode;
            break;
        }
    }

    return mapNodeJObj;
};

Map.prototype.calculateBoundingRect = function() {
    if (this.mapNodes.length == 0)
        return Qt.rect(0, 0, 0, 0);

    var pos = this.mapNodes[0].pos;

    var topLeft = Qt.point(pos.x, pos.y);
    var bottomRight = Qt.point(pos.x, pos.y);

    for (var i = 0; i < this.mapNodes.length; i++) {
        var pos = this.mapNodes[i].pos;

        if (pos.x < topLeft.x) topLeft.x = pos.x;
        else if (pos.x > bottomRight.x) bottomRight.x = pos.x;

        if (pos.y < topLeft.y) topLeft.y = pos.y;
        else if (pos.y > bottomRight.x) bottomRight.y = pos.y;
    }

    // x,y is the top-left corner of the mapNode so we need to add the tile size
    var tileSize = this.qobj.world.tileSize;
    bottomRight.x += tileSize.width;
    bottomRight.y += tileSize.height;

    return Qt.rect(
        topLeft.x,
        topLeft.y,
        bottomRight.x - topLeft.x, 
        bottomRight.y - topLeft.y
    );
};

Map.prototype.adjustMapCoordinates = function() {
    if ((this.boundingRect.x > 0) && (this.boundingRect.y > 0))
        return; // nothing to do here

    var dx = this.boundingRect.x;
    var dy = this.boundingRect.y;

    this.boundingRect.x = 0;
    this.boundingRect.y = 0;

    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        mapNode.pos.x -= dx;
        mapNode.pos.y -= dx;
    }

    var rect = this.canvas.canvasWindow;
    this.canvasWindow = Qt.rect(
        rect.x + dx,
        rect.y + dy,
        rect.width,
        rect.height
    );
}

Map.prototype.updateGeometry = function() {
    this.boundingRect = this.calculateBoundingRect();
    this.size = Qt.size(
        this.boundingRect.width,
        this.boundingRect.height
    );

    console.log("canvasWindow: " + this.canvas.canvasWindow);
    console.log("canvasSize before: " + this.canvas.canvasSize);
    this.canvas.canvasSize = Qt.size(
        Util.max(this.size.width, this.canvas.canvasWindow.width),
        Util.max(this.size.height, this.canvas.canvasWindow.height)
    );
    console.log("canvasSize after: " + this.canvas.canvasSize);

    this.adjustMapCoordinates();
}

Map.prototype.translateToLocal = function(pos) {
    return Qt.point(
        pos.x + this.canvas.canvasWindow.x,
        pos.y + this.canvas.canvasWindow.y
    );
}

Map.prototype.onPressed = function(mouse) {
    this.lastMouseEvent = MouseEvents.pressed;
};

Map.prototype.onReleased = function(mouse) {
    if (this.lastMouseEvent == MouseEvents.pressed) {
        var pos = this.translateToLocal(Qt.point(mouse.x, mouse.y));

        var mapNode = this.findMapNodeAt(pos);

        if (this.mapNodeClicked != undefined)
            this.mapNodeClicked(mapNode);
    }
    else {
        this.mouseArea.cursorShape = Qt.ArrowCursor;
    }
};

Map.prototype.onPositionChanged = function(mouse) {
    this.lastMouseEvent = MouseEvents.positionChanged;

    var pos = this.translateToLocal(Qt.point(mouse.x, mouse.y));

    if (this.mouseArea.pressed) {
        var posDiff = Qt.size(
            this.lastMousePos.x - pos.x,
            this.lastMousePos.y - pos.y
        );
        this.mouseArea.cursorShape = Qt.ClosedHandCursor;
        this.onScroll(posDiff);
    }
    else {
        this.onHover(pos);
    }

    this.lastMousePos = pos;
}

Map.prototype.onScroll = function(posDiff) {
    var window = this.canvas.canvasWindow;
    var x = window.x + posDiff.width;
    var y = window.y + posDiff.height;

    var size = this.canvas.canvasSize;
    var isXInBounds = ((x > 0) && ((x + window.width) < size.width));
    var isYInBounds = ((y > 0) && ((y + window.height) < size.height));

    var newX, newY;

    if (isXInBounds) newX = x;
    else newX = window.x;

    if (isYInBounds) newY = y;
    else newY = window.y;

    this.canvas.canvasWindow = Qt.rect(
        newX,
        newY,
        window.width,
        window.height
    );
}

Map.prototype.onHover = function(pos) {
    var mapNode = this.findMapNodeAt(pos);
    if (this.focusedNode !== mapNode) {
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

/*
 * MapItem class.
 * @contructor
 */
var MapItem = function(pos, map) {
    this.pos = pos;
    this.map = map;
    this.focused = false;
};

MapItem.prototype.onPaint = function(ctx) {
};

MapItem.prototype.translate = function(point) {
    return Qt.point(point.x - this.pos.x, point.y - this.pos.y);
};

MapItem.prototype.contains = function(point) {
    var localPoint = this.translate(point);

    return this.map.ui.hexContains(localPoint);
};

MapItem.prototype.onMouseIn = function() {
    this.focused = true;
};

MapItem.prototype.onMouseOut = function() {
    this.focused = false;
};

/*
 * MapNode class.
 * @contructor
 */
var MapNode = function(pos, mapNodeQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = mapNodeQObj;
    this.terrainImage = this.map.qobj.world.getResourcePath(
        this.qobj.terrainType.objectName
    );
};

MapNode.prototype = Object.create(MapItem.prototype);
MapNode.prototype.constructor = MapNode;

MapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y)

    ctx.drawImage(this.terrainImage, 0, 0);
    if (this.focused)
        ctx.drawImage(worldQObj.getResourcePath("border_highlighted"), 0, 0);
    else
        ctx.drawImage(worldQObj.getResourcePath("border"), 0, 0);

    ctx.restore();
};

MapNode.prototype.onMouseIn = function() {
    this.focused = true;
    this.map.markDirty(this);
};

MapNode.prototype.onMouseOut = function() {
    this.focused = false;
    this.map.markDirty(this);
};
