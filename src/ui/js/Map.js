// The current map
var MAP;

/*
 * Constants
 */
var TILE_SIZE = 128;
var TILE_OVERLAP = 31;
var TILE_DISPLACEMENT = {
    'North': {x: 0, y: -TILE_SIZE},
    'NorthEast': {x: TILE_SIZE - TILE_OVERLAP, y: -TILE_SIZE/2},
    'SouthEast': {x: TILE_SIZE - TILE_OVERLAP, y: TILE_SIZE/2},
    'South': {x: 0, y: TILE_SIZE},
    'SouthWest': {x: -(TILE_SIZE - TILE_OVERLAP), y: TILE_SIZE/2},
    'NorthWest': {x: -(TILE_SIZE - TILE_OVERLAP), y: -TILE_SIZE/2}
};

/*
 * Map class.
 */
var Map = function(map) {
    this.map = map;
    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.createNode = function(mapNode, x, y, visitedNodes) {
        visitedNodes[mapNode] = true;

        this.mapNodes.push(new MapNode(mapNode, x, y));

        for (var direction in mapNode.neighbours) {
            var neighbour = mapNode.neighbours[direction];

            if (neighbour in visitedNodes) continue;

            var d= TILE_DISPLACEMENT[direction];
            this.createNode(neighbour, x + d.x, y + d.y, visitedNodes);
        }
    }

    this.paint = function(ctx) {
        for (var i = 0; i < this.mapNodes.length; i++) {
            this.mapNodes[i].paint(ctx);
        }
    }

    this.loadResources = function() {
        var resources = this.map.world.resources;
        var path;

        for (var resource in resources.paths) {
            path = resources.getPath(resource);
            this.loadQueue.push(path);
            mapCanvas.loadImage(path);
        }
    }

    this.onResourceLoaded = function() {
        var image;

        for (var i = 0; i < this.loadQueue.length; i++) {
            image = this.loadQueue[i];
            if (mapCanvas.isImageError(image)) {
                console.error("Error loading image " + image);
                this.loadQueue.splice(i, 1);
            }

            if (mapCanvas.isImageLoaded(image)) {
                console.log("Successfully loaded image " + image);
                this.loadQueue.splice(i, 1);
            }
        }

        if (this.loadQueue.length == 0) {
            console.log("All images loaded");
            this.onResourcesLoaded();
        }
    }

    this.onResourcesLoaded = function() {
        this.ready = true;
        mapCanvas.requestPaint();
    }

    this.loadResources();
    this.createNode(this.map.mapNodes[0], 0, 0, {});
}

/*
 * MapNode class.
 */
var MapNode = function(mapNode, x, y) {
    this.mapNode = mapNode;
    this.image = warmonger.map.world.resources.getPath(this.mapNode.terrainType.objectName);
    this.x = x;
    this.y = y;

    this.paint = function(ctx) {
        console.log("painting: " + this.mapNode.objectName);
        console.log(this.x + "," + this.y);

        ctx.save();

        ctx.translate(this.x, this.y)
        ctx.drawImage(this.image, 0, 0);

        ctx.restore();
    }
}

function onMapChanged() {
    MAP = new Map(warmonger.map);
}

function onImageLoaded() {
    MAP.onResourceLoaded();
}

function onPaint(region) {
    var ctx = mapCanvas.getContext('2d');

    console.log("onPaint");
    console.log(region);

    MAP.paint(ctx);
}
