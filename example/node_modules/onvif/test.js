/**
 * Created by Andrew D.Laptev<a.d.laptev@gmail.com> on 16.01.15.
 */

var mock = require('./test/serverMockup');

var Cam = require('./lib/onvif').Cam;

/*var cam = new Cam({hostname: '192.168.68.111', username: 'admin', password: '9999'}, function(err) {
	if (err) {
		throw err;
	}

	this.getConfigurations(function(err, data, xml) {
		console.log(xml);
		this.getConfigurationOptions(Object.keys(data)[0], function(err, data, xml) {
			console.log(this.configurations);
			console.log(xml);
		});
	});

	setInterval(function () {
		this.absoluteMove({
			x: 1
			, y: 1
			, zoom: 1
		});
	}.bind(this), 5000);
});*/
//cam.on('rawResponse', console.log);
var fs = require('fs');

options = {
	hostname: 'localhost',
	username: 'admin',
	password: '9999',
	port: process.env.PORT || 10101
};

new Cam(options, function(err) {
	var file = fs.createWriteStream('/tmp/a');
	file.end(JSON.stringify(this));
});
