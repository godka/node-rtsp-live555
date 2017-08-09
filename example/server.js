const express = require('express');
const app = express();
var rtsp = null;
try {
	rtsp = require('rtsp-live555');
} catch (e) {
	rtsp = require('../lib/rtsp-live555.js');
}
var onvif = require('onvif');
var Cam = require('onvif').Cam;

rtsp.prebuild(function (err) {
	if (err.result) {
		var server = app.listen(8080, function () {
			var port = server.address().port;
			console.log('listening at port:%s', port);
		});
		app.get('/stream', (req, res) => {
			var _url = req.query.url;
			var _isfirst = true;
			var _falsetime = 0;
			if (_url) {
				res.writeHead(200, {
					'Access-Control-Allow-Origin': '*',
					'Connection': 'Keep-Alive',
					'Content-Type': 'video/x-flv'
				});
				var stream = new rtsp.Live555Client({ input: _url });
				stream.on('start', () => {
					console.log(_url + ' started');
				});
				stream.on('stop', () => {
					console.log(_url + ' stopped');
				});
				stream.on('data', (data) => {
					var _success = res.write(data);
					if (_success) {
						_falsetime = 0;
					} else {
						_falsetime++;
						if (_falsetime > 100) {
							stream.stop();
							res.end();
						}
					}
				});
			} else {
				res.end();
			}
		});
		function searchForNextTick(res, myarray, cams, index) {
			var cam = cams[index];
			if (cam) {
				try {
					cam.getStreamUri({ protocol: 'RTSP' }, function (err, stream) {
						if (err) {
							console.log(err);
						} else {
							console.log('rtsp url:' + stream.uri);
							var result = {
								uri: stream.uri,
								hostname: cam.hostname
							};
							myarray.push(result);
						}
						if (index == cams.length)
							res.send(myarray);
						else {
							searchForNextTick(res, myarray, cams, index + 1);
						}
					});
				} catch (e) {
					console.log('onvif error:', e);
					res.send(myarray);
				}
			} else {
				res.send(myarray);
			}
		}
		app.get('/search', function (req, res) {
			onvif.Discovery.probe(function (err, cams) {
				if (err) {
					console.log(err);
					res.send({ result: false });
					return;
				}
				searchForNextTick(res, [], cams, 0);
			});
		});
		app.use(express.static('html'));
	} else {
		console.log('build failed,err code:', err.code);
	}
});