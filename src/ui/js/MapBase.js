.pragma library

function displacement(dir, tileSize) {
    if (dir == 'West') return {x: -tileSize.width, y: 0};
    if (dir == 'NorthWest') return {x: -tileSize.width/2, y: -tileSize.height * 3/4};
    if (dir == 'NorthEast') return {x: tileSize.width/2, y: -tileSize.height * 3/4};
    if (dir == 'East') return {x: tileSize.width, y: 0};
    if (dir == 'SouthEast') return {x: tileSize.width/2, y: tileSize.height * 3/4};
    if (dir == 'SouthWest') return {x: -tileSize.width/2, y: tileSize.height * 3/4};

    err = "Unknown direction: " + dir;
    console.error(err);
    throw err;
};

/*
 * Map class.
 * @contructor
 */
var Map = function(ui, canvas) {
    this.ui = ui;
    this.qobj = ui.map;
    this.canvas = canvas;

    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.dirtyMapNodes = [];
    this.boundingRect = Qt.rect(0, 0, 0, 0);

    // init
    this.loadResources();

    var canvasSize = this.canvas.canvasSize;
    this.createNode(this.qobj.mapNodes[0], canvasSize.width / 2, canvasSize.height / 2, {});

    this.boundingRect = this.calculateBoundingRect();

    this.canvas.requestPaint();
};

Map.prototype.createNode = function(mapNodeQObj, x, y, visitedNodes) {
    visitedNodes[mapNodeQObj] = true;
    var tileSize = this.qobj.world.tileSize;

    var mapNode = new MapNode(x, y, mapNodeQObj, this);
    this.mapNodes.push(mapNode);
    this.dirtyMapNodes.push(mapNode);

    for (var direction in mapNodeQObj.neighbours) {
        var neighbour = mapNodeQObj.neighbours[direction];

        if (neighbour == undefined || neighbour in visitedNodes) continue;

        var d = displacement(direction, tileSize);
        this.createNode(neighbour, x + d.x, y + d.y, visitedNodes);
    }
};

Map.prototype.paint = function(region) {
    var ctx = this.canvas.getContext("2d");

    for (var i = 0; i < this.dirtyMapNodes.length; i++) {
        this.dirtyMapNodes[i].paint(ctx);
    }

    this.dirtyMapNodes.splice(0, this.dirtyMapNodes.length - 1);
};

Map.prototype.loadResources = function() {
    var resourcePaths = this.qobj.world.resourcePaths;
    var path;

    for (var resource in resourcePaths) {
        path = resourcePaths[resource];
        this.loadQueue.push(path);
        this.canvas.loadImage(path);
    }
};

Map.prototype.onResourceLoaded = function() {
    var image;

    for (var i = 0; i < this.loadQueue.length; i++) {
        image = this.loadQueue[i];
        if (this.canvas.isImageError(image)) {
            console.error("Error loading image " + image);
            this.loadQueue.splice(i, 1);
        }

        if (this.canvas.isImageLoaded(image)) {
            console.info("Successfully loaded image " + image);
            this.loadQueue.splice(i, 1);
        }
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
    this.dirtyMapNodes.push(mapNode);

    this.canvas.requestPaint();
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

    var firstNode = this.mapNodes[0];
    var topLeft = Qt.point(firstNode.x, firstNode.y);
    var bottomRight = Qt.point(firstNode.x, firstNode.y);

    for (var i = 0; i < this.mapNodes.length; i++) {
        var currentNode = this.mapNodes[i];
        if (currentNode.x < topLeft.x || currentNode.y < topLeft.y) {
            topLeft.x = currentNode.x;
            topLeft.y = currentNode.y;
        }
        else if (currentNode.x > bottomRight.x || currentNode.y > bottomRight.y) {
            bottomRight.x = currentNode.x;
            bottomRight.y = currentNode.y;
        }
    }

    return Qt.rect(
        topLeft.x,
        topLeft.y,
        bottomRight.x - topLeft.x, 
        bottomRight.y - topLeft.y
    );
};

/*
 * MapItem class.
 * @contructor
 */
var MapItem = function(x, y, map) {
    this.x = x;
    this.y = y;
    this.map = map;
    this.focused = false;
};

MapItem.prototype.paint = function(ctx) {
};

MapItem.prototype.translate = function(point) {
    return Qt.point(point.x - this.x, point.y - this.y);
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
var MapNode = function(x, y, mapNodeQObj, map) {
    MapItem.call(this, x, y, map);

    this.qobj = mapNodeQObj;
    this.terrainImage = this.map.qobj.world.getResourcePath(this.qobj.terrainType.objectName);
};

MapNode.prototype = Object.create(MapItem.prototype);
MapNode.prototype.constructor = MapNode;

MapNode.prototype.paint = function(ctx) {
    var worldQObj = this.map.qobj.world;

    ctx.save();

    ctx.translate(this.x, this.y)

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
