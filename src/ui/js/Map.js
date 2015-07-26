function onPaint() {
    var ctx = mapCanvas.getContext('2d');
    var mm = mapModel;

    console.log(mm);
    console.log(mm.objectName);
    console.log(mm.columnCount);
    console.log(mm.rowCount);

    //var index = mm.index(1, 3);

    //console.log(mm.data(index));

    ctx.beginPath();
    ctx.moveTo(100, 150);
    ctx.lineTo(450, 50);
    ctx.stroke();
}
