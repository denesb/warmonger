.pragma library

function displacement(dir, tileSize) {
    if (dir == 'West') return {x: -tileSize.width, y: 0};
    if (dir == 'NorthWest') return {x: -tileSize.width/2, y: -tileSize.height * 3/4};
    if (dir == 'NorthEast') return {x: tileSize.width/2, y: tileSize.height * 3/4};
    if (dir == 'East') return {x: tileSize.width, y: 0};
    if (dir == 'SouthEast') return {x: tileSize.width/2, y: tileSize.height * 3/4};
    if (dir == 'SouthWest') return {x: -tileSize.width/2, y: -tileSize.height * 3/4};

    err = "Unknown direction: " + dir;
    console.error(err);
    throw err;
}

/*
 * Map class.
 */
var Map = function(ui, canvas) {
    this.ui = ui;
    this.qobj = ui.map;
    this.canvas = canvas;

    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.loadResources();
    this.createNode(this.qobj.mapNodes[0], 0, 0, {});
}

Map.prototype.createNode = function(mapNode, x, y, visitedNodes) {
    visitedNodes[mapNode] = true;
    var tileSize = this.qobj.world.tileSize;

    this.mapNodes.push(new MapNode(mapNode, x, y, this));

    for (var direction in mapNode.neighbours) {
        var neighbour = mapNode.neighbours[direction];

        if (neighbour in visitedNodes) continue;

        var d = displacement(direction, tileSize);
        this.createNode(neighbour, x + d.x, y + d.y, visitedNodes);
    }
}

Map.prototype.paint = function(region) {
    var ctx = this.canvas.getContext("2d");

    for (var i = 0; i < this.mapNodes.length; i++) {
        this.mapNodes[i].paint(ctx);
    }
}

Map.prototype.loadResources = function() {
    var resourcePaths = this.qobj.world.resourcePaths;
    var path;

    for (var resource in resourcePaths) {
        path = resourcePaths[resource];
        this.loadQueue.push(path);
        this.canvas.loadImage(path);
    }
}

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
}

Map.prototype.onResourcesLoaded = function() {
    this.ready = true;
    this.canvas.requestPaint();
}

Map.prototype.findMapNodeAt = function(point) {
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.contains(point)) return mapNode;
    }

    return undefined;
}

/*
 * MapNode class.
 */
var MapNode = function(mapNode, x, y, parent) {
    this.parent = parent;
    this.qobj = mapNode;
    this.terrainImage = this.parent.qobj.world.getResourcePath(this.qobj.terrainType.objectName);
    this.borderImage = this.parent.qobj.world.getResourcePath("border");
    this.x = x;
    this.y = y;
    this.boundingRect = Qt.rect(x, y, this.parent.qobj.world.tileSize.width, this.parent.qobj.world.tileSize.height);
}

MapNode.prototype.paint = function(ctx) {
    ctx.save();

    ctx.translate(this.x, this.y)

    ctx.drawImage(this.terrainImage, 0, 0);
    ctx.drawImage(this.borderImage, 0, 0);

    ctx.restore();
}

MapNode.prototype.transform = function(point) {
    return Qt.point(point.x - this.x, point.y - this.y);
}

MapNode.prototype.contains = function(point) {
    var localP = this.transform(point);

    return this.parent.ui.hexContains(localP);
}
