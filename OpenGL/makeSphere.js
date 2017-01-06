function calcSphereData(options){
        for (var latNumber = 0; latNumber < options.spherePrecision; latNumber++) {
            for (var longNumber = 0; longNumber < options.spherePrecision; longNumber++) {

                getPoint(latNumber, longNumber,options.spherePrecision,options.sphereRadius);
                getPoint(latNumber, longNumber + 1,options.spherePrecision,options.sphereRadius);
                getPoint(latNumber + 1, longNumber + 1,options.spherePrecision,options.sphereRadius);

                getPoint(latNumber, longNumber,options.spherePrecision,options.sphereRadius);
                getPoint(latNumber + 1, longNumber + 1,options.spherePrecision,options.sphereRadius);
                getPoint(latNumber + 1, longNumber,options.spherePrecision,options.sphereRadius);
            }
        }
        pointNum = spherePointsCoordData.length/3;
    }

    function getPoint(latNumber, longNumber, precision, radius){
        var a = latNumber * Math.PI / precision;
        var b = 2 * longNumber * Math.PI / precision;
        var x = Math.sin(a) * Math.sin(b) * radius;
        var y = Math.cos(a) * radius;
        var z = Math.sin(a) * Math.cos(b) * radius;
        var v = 1 - longNumber/precision;
        var u = 1 - latNumber/precision;

        spherePointsCoordData.push(x);
        spherePointsCoordData.push(y);
        spherePointsCoordData.push(z);

        textureCoordData.push(v);
        textureCoordData.push(u);
    }