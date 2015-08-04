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
    this.map = ui.map;
    this.canvas = canvas;

    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.loadResources();
    this.createNode(this.map.mapNodes[0], 0, 0, {});
}

Map.prototype.createNode = function(mapNode, x, y, visitedNodes) {
    visitedNodes[mapNode] = true;
    var tileSize = this.ui.map.world.tileSize;

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
    var resourcePaths = this.map.world.resourcePaths;
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

/*
 * MapNode class.
 */
var MapNode = function(mapNode, x, y, parent) {
    this.parent = parent;
    this.mapNode = mapNode;
    this.terrainImage = this.parent.map.world.getResourcePath(this.mapNode.terrainType.objectName);
    this.borderImage = this.parent.map.world.getResourcePath("border");
    this.x = x;
    this.y = y;
    this.boundingRect = Qt.rect(x, y, this.parent.map.world.tileSize.width, this.parent.map.world.tileSize.height);
}

MapNode.prototype.paint = function(ctx) {
    ctx.save();

    ctx.translate(this.x, this.y)

    ctx.drawImage(this.terrainImage, 0, 0);
    ctx.drawImage(this.borderImage, 0, 0);

    ctx.restore();
}
