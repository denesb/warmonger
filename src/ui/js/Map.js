//paths must be absolute
var terrainTypes = {
    'plains': 'worlds/iron_age/terrainTypes/images/plains.png',
    'hills': 'worlds/iron_age/terrainTypes/images/hills.png',
    'mountains': 'worlds/iron_age/terrainTypes/images/mountains.png',
    'forest': 'worlds/iron_age/terrainTypes/images/forest.png'
};

var directionMap = {
    'North': {x: 0, y: -1},
    'NorthEast': {x: 0.75, y: -0.5},
    'SouthEast': {x: 0.75, y: 0.5},
    'South': {x: 0, y: 1},
    'SouthWest': {x: -0.75, y: 0.5},
    'NorthWest': {x: -0.75, y: -0.5}
};

var resources = {};

function loadMapResources() {
    mapCanvas.loadImage(terrainTypes['plains']);
}

function onMapResourcesLoaded() {
    drawMap();
}

function onMapChanged() {
    loadMapResources();
}

function drawNode(ctx, mapNode, visitedNodes, x, y) {
    console.log("drawNode: " + mapNode.objectName);
    console.log("(" + x + "," + y + ")");

    visitedNodes[mapNode] = true;

    ctx.drawImage(terrainTypes['plains'], x * 128, y * 128);

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

    drawMap(ctx);
}

function onImageLoaded() {
    onMapResourcesLoaded();
}
