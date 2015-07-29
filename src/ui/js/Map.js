var loadQueue = [];
var ready;

var directionMap = {
    'North': {x: 0, y: -1},
    'NorthEast': {x: 0.75, y: -0.5},
    'SouthEast': {x: 0.75, y: 0.5},
    'South': {x: 0, y: 1},
    'SouthWest': {x: -0.75, y: 0.5},
    'NorthWest': {x: -0.75, y: -0.5}
};

function loadMapResources() {
    var resources = warmonger.map.world.resources;
    var path;

    ready = false;

    for (var terrainTypeName in resources.terrainTypePaths) {
        path = resources.terrainTypePaths[terrainTypeName];
        loadQueue.push(path);
        mapCanvas.loadImage(path);
    }
}

function onMapResourcesLoaded() {
    ready = true;
    mapCanvas.requestPaint();
}

function onMapChanged() {
    loadMapResources();
}

function drawNode(ctx, mapNode, visitedNodes, x, y) {
    console.log("drawNode: " + mapNode.objectName);
    console.log("(" + x + "," + y + ")");

    visitedNodes[mapNode] = true;
    var image = warmonger.map.world.resources.terrainTypePaths[mapNode.terrainType.objectName];

    ctx.drawImage(image, x * 128, y * 128);

    for (var direction in mapNode.neighbours) {
        var neighbour = mapNode.neighbours[direction];

        if (neighbour in visitedNodes) {
            continue;
        }

        var d = directionMap[direction];
        var xx = x + d.x;
        var yy = y + d.y;
        drawNode(ctx, neighbour, visitedNodes, xx, yy);
    }
}

function drawMap(ctx) {
    var mapNode;
    var x = 0, y = 0;
    var visitedNodes = {};

    mapNode = warmonger.map.mapNodes[0];
    drawNode(ctx, mapNode, visitedNodes, x, y);
}

function onPaint(region) {
    var ctx = mapCanvas.getContext('2d');

    console.log("onPaint");
    console.log(region);

    if (ready) {
        drawMap(ctx);
    }
}

function onImageLoaded() {
    var image;

    for (var i = 0; i < loadQueue.length; i++) {
        image = loadQueue[i];
        if (mapCanvas.isImageError(image)) {
            console.error("Error loading image " + image);
            loadQueue.splice(i, 1);
        }

        if (mapCanvas.isImageLoaded(image)) {
            console.log("Successfully loaded image " + image);
            loadQueue.splice(i, 1);
        }
    }

    if (loadQueue.length == 0) {
        console.log("All images loaded");
        onMapResourcesLoaded();
    }
}
