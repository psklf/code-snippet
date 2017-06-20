var mime = require("./mime").types;
var http = require('http');
var fs = require('fs');
var url = require('url');
var path = require('path');

var PORT = 8000;

var server = http.createServer(function (request, response) {
	var pathname = url.parse(request.url).pathname;
    console.log("Request for " + pathname + " received.");
    var realPath = pathname.substr(1);

	fs.exists(realPath, function (exists) {
		if (!exists) {
			response.writeHead(404, {
				'Content-Type': 'text/plain'
			});

			response.write("This request URL " + pathname + " was not found on this server.");
			response.end();
		} else {
			var ext = path.extname(realPath);
			ext = ext ? ext.slice(1) : 'unknown';

			fs.readFile(realPath, "binary", function (err, file) {
				if (err) {
					response.writeHead(500, {
						'Content-Type': 'text/plain'
					});

					response.end(err);
				} else {
					var contentType = mime[ext] || "text/plain";

					response.writeHead(200, {
						'Content-Type': contentType
					});

					response.write(file, "binary");

					response.end();
				}
			});
		}
	});
});

server.listen(PORT);
console.log("Server running at port: " + PORT + ".");
