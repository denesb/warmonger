.pragma library

/*
 * Constants
 */
var TILE_WIDTH = 110;
var TILE_HEIGHT = 128;
var TILE_Y_DISP = TILE_HEIGHT - TILE_HEIGHT/4;
var TILE_DISPLACEMENT = {
    'West': {x: -TILE_WIDTH, y: 0},
    'NorthWest': {x: -TILE_WIDTH/2, y: -TILE_Y_DISP},
    'NorthEast': {x: TILE_WIDTH/2, y: TILE_Y_DISP},
    'East': {x: TILE_WIDTH, y: 0},
    'SouthEast': {x: TILE_WIDTH/2, y: TILE_Y_DISP},
    'SouthWest': {x: -TILE_WIDTH/2, y: -TILE_Y_DISP},
};

function newMap(app, map, canvas) {
    return new Map(app, map, canvas);
}

/*
 * Map class.
 */
var Map = function(map, canvas, ui) {
    this.ui = ui;
    this.map = map;
    this.canvas = canvas;

    this.mapNodes = [];
    this.loadQueue = [];
    this.ready = false;

    this.createNode = function(mapNode, x, y, visitedNodes) {
        visitedNodes[mapNode] = true;

        this.mapNodes.push(new MapNode(mapNode, x, y, this));

        for (var direction in mapNode.neighbours) {
            var neighbour = mapNode.neighbours[direction];

            if (neighbour in visitedNodes) continue;

            var d= TILE_DISPLACEMENT[direction];
            this.createNode(neighbour, x + d.x, y + d.y, visitedNodes);
        }
    }

    this.paint = function(canvas, region) {
        var ctx = canvas.getContext("2d");

        for (var i = 0; i < this.mapNodes.length; i++) {
            this.mapNodes[i].paint(ctx);
        }
    }

    this.loadResources = function() {
        var resourcePaths = this.map.world.resourcePaths;
        var path;

        for (var resource in resourcePaths) {
            path = resourcePaths[resource];
            this.loadQueue.push(path);
            this.canvas.loadImage(path);
        }
    }

    this.onResourceLoaded = function() {
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

    this.onResourcesLoaded = function() {
        this.ready = true;
        this.canvas.requestPaint();
    }

    this.loadResources();
    this.createNode(this.map.mapNodes[0], 0, 0, {});
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

    this.paint = function(ctx) {
        ctx.save();

        ctx.translate(this.x, this.y)

        ctx.drawImage(this.terrainImage, 0, 0);
        ctx.drawImage(this.borderImage, 0, 0);

        ctx.restore();
    }
}
