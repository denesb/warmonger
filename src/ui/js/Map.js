function onPaint() {
    var ctx = mapCanvas.getContext('2d');

    console.log(map);
    console.log(map.displayName);
    console.log(map.world);

    for (var i = 0; i < map.mapNodes.length; i++) {
        var node = map.mapNodes[i];
        console.log(node);
        console.log(node.objectName);
        console.log(node.terrainType);
        console.log(node.terrainType.objectName);
    }

    console.log(map.players);
    console.log(map.units);
    console.log(map.settlements);

    ctx.beginPath();
    ctx.moveTo(100, 150);
    ctx.lineTo(450, 50);
    ctx.stroke();
}
