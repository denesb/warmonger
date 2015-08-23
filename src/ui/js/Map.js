.pragma library

.import 'Util.js' as Util
.import 'MapItem.js' as MapItem

function neighbourPos(dir, tileSize, pos) {
    var d;

    if (dir == 'West') d = Qt.size(-tileSize.width, 0);
    else if (dir == 'NorthWest') d = Qt.size(-tileSize.width/2, -tileSize.height * 3/4);
    else if (dir == 'NorthEast') d = Qt.size(tileSize.width/2, -tileSize.height * 3/4);
    else if (dir == 'East') d = Qt.size(tileSize.width, 0);
    else if (dir == 'SouthEast') d = Qt.size(tileSize.width/2, tileSize.height * 3/4);
    else if (dir == 'SouthWest') d = Qt.size(-tileSize.width/2, tileSize.height * 3/4);
    else {
        var err = "Unknown direction: " + dir;
        console.error(err);
        throw err;
    }

    return Qt.point(
        pos.x + d.width,
        pos.y + d.height
    );
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
    this.mapItems = [];
    this.ready = false;

    this.geometryChanged = true;
    this.boundingRect = Qt.rect(0, 0, 0, 0);
    this.size = Qt.size(0, 0);

    // init
    this.createNode(this.qobj.mapNodes[0], Qt.point(0, 0), {});
};

Map.prototype.createNode = function(mapNodeQObj, pos, visitedNodes) {
    visitedNodes[mapNodeQObj] = true;
    var tileSize = this.qobj.world.tileSize;

    this.newMapNode(pos, mapNodeQObj);

    for (var direction in mapNodeQObj.neighbours) {
        var neighbour = mapNodeQObj.neighbours[direction];

        if (neighbour == undefined || neighbour in visitedNodes) continue;

        this.createNode(
            neighbour,
            neighbourPos(direction, tileSize, pos),
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

    ctx.clearRect(
        0,
        0,
        this.canvas.canvasSize.width,
        this.canvas.canvasSize.height
    );

    for (var i = 0; i < this.mapItems.length; i++) {
        this.mapItems[i].onPaint(ctx);
    }
};

Map.prototype.markDirty = function(mapItem) {
    var tileSize = this.qobj.world.tileSize;

    this.canvas.markDirty(Qt.rect(
        mapItem.pos.x,
        mapItem.pos.y,
        tileSize.width,
        tileSize.height
    ));
};

Map.prototype.findMapItemJObj = function(mapItemQObj) {
    var mapItemJObj = undefined;
    for (var i = 0; i < this.mapItems.length; i++) {
        var mapItem = this.mapItems[i];
        if (mapItem instanceof MapItem.MapItem &&
            mapItem.qobj == mapItemQObj) {
            mapItemJObj = mapItem;
            break;
        }
    }

    return mapItemJObj;
};

Map.prototype.calculatePosOfMapNodeQObj = function(mapNodeQObj) {
    var direction;
    var neighbourQObj;
    for (direction in mapNodeQObj.neighbours) {
        neighbourQObj = mapNodeQObj.neighbours[direction];
        if (neighbourQObj != undefined) {
            break;
        }
    }

    var neighbourJObj = this.findMapItemJObj(neighbourQObj);

    // calculate the position of this node based on the neighbour's position
    var oppositeDirection = mapNodeQObj.oppositeDirection(direction);
    var tileSize = this.qobj.world.tileSize;
    var pos = neighbourPos(oppositeDirection, tileSize, neighbourJObj.pos);

    return pos;
};

Map.prototype.calculateBoundingRect = function() {
    if (this.mapItems.length == 0)
        return Qt.rect(0, 0, 0, 0);

    var pos = this.mapItems[0].pos;

    var topLeft = Qt.point(pos.x, pos.y);
    var bottomRight = Qt.point(pos.x, pos.y);

    for (var i = 0; i < this.mapItems.length; i++) {
        var pos = this.mapItems[i].pos;

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
    if ((this.boundingRect.x == 0) && (this.boundingRect.y == 0))
        return; // nothing to do here

    var dx = this.boundingRect.x;
    var dy = this.boundingRect.y;

    this.boundingRect.x = 0;
    this.boundingRect.y = 0;

    for (var i = 0; i < this.mapItems.length; i++) {
        var mapItem = this.mapItems[i];
        mapItem.pos.x -= dx;
        mapItem.pos.y -= dy;
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

    this.canvas.canvasSize = Qt.size(
        Math.max(this.size.width, this.canvas.canvasWindow.width),
        Math.max(this.size.height, this.canvas.canvasWindow.height)
    );

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
        this.onClicked(pos);
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
        this.onPanned(posDiff);
    }
    else {
        this.onHovered(pos);
    }

    this.lastMousePos = pos;
}

Map.prototype.onHovered = function(pos) {
}

Map.prototype.onPanned = function(pos) {
}

Map.prototype.onClicked = function(pos) {
}


/*
 * GameMap class
 * @contructor
 */
var GameMap = function(ui, canvas, mouseArea) {
    Map.call(this, ui, canvas, mouseArea);

    this.loadQueue = [];

    // init
    this.loadResources();
    this.canvas.requestPaint();
};

GameMap.prototype = Object.create(Map.prototype);
GameMap.prototype.constructor = EditableMap;

GameMap.prototype.newMapNode = function(pos, mapNodeQObj) {
    var mapNode = new MapItem.MapNode(pos, mapNodeQObj,this);
    this.mapItems.push(mapNode);

    return mapNode;
}

GameMap.prototype.findMapNodeAt = function(point) {
    for (var i = 0; i < this.mapItems.length; i++) {
        var mapNode = this.mapItems[i];
        if (mapNode.contains(point)) return mapNode;
    }

    return undefined;
};

GameMap.prototype.getMapNodeAt = function(pos) {
    for (var i = 0; i < this.mapItems.length; i++) {
        var mapItem = this.mapItems[i];
        if (mapItem.pos.x == pos.x && mapItem.pos.y == pos.y)
            return mapItem;
    }

    return undefined;
};

GameMap.prototype.onClicked = function(pos) {
    var mapNode = this.findMapNodeAt(pos);

    if (this.mapNodeClicked != undefined)
        this.mapNodeClicked(mapNode);
};

GameMap.prototype.onHovered = function(pos) {
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

GameMap.prototype.onPanned = function(posDiff) {
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

GameMap.prototype.loadResources = function() {
    var resourcePaths = this.qobj.world.resourcePaths;
    var path;

    for (var resource in resourcePaths) {
        path = resourcePaths[resource];
        this.canvas.loadImage(path);
        this.loadQueue.push(path);
    }
};

GameMap.prototype.onResourceLoaded = function() {
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

GameMap.prototype.onResourcesLoaded = function() {
    this.ready = true;
    this.canvas.requestPaint();
};


/*
 * EditableMap class
 * @contructor
 */
var EditableMap = function(ui, canvas, mouseArea) {
    GameMap.call(this, ui, canvas, mouseArea);

    ui.map.mapNodeCreated.connect(this.onMapNodeCreated.bind(this));

    this.focusedNode = undefined;
    this.mapNodeClicked = undefined;
    this.mapNodeFocused = undefined;

    // init
    this.addPhantomMapNodes(this.mapItems);
    this.geometryChanged = true;
};

EditableMap.prototype = Object.create(GameMap.prototype);
EditableMap.prototype.constructor = EditableMap;

EditableMap.prototype.onMapNodeCreated = function(mapNodeQObj) {
    var pos = this.calculatePosOfMapNodeQObj(mapNodeQObj);
    var phantomMapNode = this.getMapNodeAt(pos);
    var i = this.mapItems.indexOf(phantomMapNode);
    this.mapItems.splice(i, 1);

    var mapNodeJObj = new MapItem.MapNode(pos, mapNodeQObj, this);
    this.mapItems.push(mapNodeJObj);
    this.markDirty(mapNodeJObj);

    this.addPhantomMapNodes([mapNodeJObj]);

    this.geometryChanged = true;
};

EditableMap.prototype.addPhantomMapNodes = function(mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {

        var mapNode = mapNodes[i];
        if (mapNode instanceof MapItem.PhantomMapNode) continue;

        for (var direction in mapNode.qobj.neighbours) {
            var neighbour = mapNode.qobj.neighbours[direction];

            if (neighbour == undefined)
                this.createPhantomNode(mapNode, direction);
        }
    }
};

EditableMap.prototype.createPhantomNode = function(neighbourMapNode, direction) {
    var tileSize = this.qobj.world.tileSize;
    var pos = neighbourPos(direction, tileSize, neighbourMapNode.pos);

    var phantomMapNode = this.getMapNodeAt(pos);
    if (phantomMapNode == undefined) {
        phantomMapNode = new MapItem.PhantomMapNode(pos, this);
        this.mapItems.push(phantomMapNode);
        this.markDirty(phantomMapNode);
    }

    var oppositeDirection = neighbourMapNode.qobj.oppositeDirection(direction);
    phantomMapNode.neighbours[oppositeDirection] = neighbourMapNode;
};


/*
 * MiniMap class
 * @contructor
 */
var MiniMap = function(ui, canvas, mouseArea) {
    Map.call(this, ui, canvas, mouseArea);

    this.scaleFactor = 1;

    // init
    ui.map.mapNodeCreated.connect(this.onMapNodeCreated.bind(this));

    this.ready = true;
    this.canvas.requestPaint();
};

MiniMap.prototype = Object.create(Map.prototype);
MiniMap.prototype.constructor = MiniMap;

MiniMap.prototype.newMapNode = function(pos, mapNodeQObj) {
    var mapNode = new MapItem.MiniMapNode(pos, mapNodeQObj, this);
    this.mapItems.push(mapNode);

    return mapNode;
}

MiniMap.prototype.calculateScaleFactor = function() {
    var normalSize = Math.max(this.size.width, this.size.height);
    var canvSize = Math.min(this.canvas.canvasSize.width, this.canvas.canvasSize.height);

    return canvSize/normalSize;
};

MiniMap.prototype.updateGeometry = function() {
    this.boundingRect = this.calculateBoundingRect();
    this.size = Qt.size(
        this.boundingRect.width,
        this.boundingRect.height
    );
    this.scaleFactor = this.calculateScaleFactor();

    this.adjustMapCoordinates();
};

MiniMap.prototype.onPaint = function(region) {
    if (!this.ready) return;
    if (this.geometryChanged) {
        this.updateGeometry();
        this.geometryChanged = false;
    }

    var ctx = this.canvas.getContext("2d");

    ctx.clearRect(
        0,
        0,
        this.canvas.canvasSize.width,
        this.canvas.canvasSize.height
    );

    ctx.save();
    ctx.scale(this.scaleFactor, this.scaleFactor);

    for (var i = 0; i < this.mapItems.length; i++) {
        this.mapItems[i].onPaint(ctx);
    }

    ctx.restore();
};

MiniMap.prototype.onMapNodeCreated = function(mapNodeQObj) {
    var pos = this.calculatePosOfMapNodeQObj(mapNodeQObj);
    var mapNodeJObj = new MapItem.MiniMapNode(pos, mapNodeQObj, this);

    this.mapItems.push(mapNodeJObj);

    this.geometryChanged = true;
    this.canvas.requestPaint();
};
