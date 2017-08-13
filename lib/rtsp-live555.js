var EventEmitter = require('events').EventEmitter;
var util = require('util');
var os = require('os');
var childProcess = require('child_process');
var spawn = childProcess.spawn;
var path = require('path');
var fs = require('fs');
/**
 * Stream constructor
 * @param {object} options
 * @param {string} options.input Stream uri, for example rtsp://r3---sn-5hn7su76.c.youtube.com/CiILENy73wIaGQnup-1SztVOYBMYESARFEgGUgZ2aWRlb3MM/0/0/0/video.3gp
 * @constructor
 */
var Live555Client = function (options) {
	if (options.input) {
		this.input = options.input;
	} else {
		throw new Error('no `input` parameter');
	}
	this.arguments = [];

	this.on('newListener', function (event) {
		if (event === 'data' && this.listeners(event).length === 0) {
			this.start();
		}
	});
	this.on('removeListener', function (event) {
		if (event === 'data' && this.listeners(event).length === 0) {
			this.stop();
		}
	});
	if (Object.observe && (typeof Proxy !== 'function')) {
		Object.observe(this, function (changes) {
			if (changes.some(function (change) {
				return change.type === 'update';
			})) {
				this.restart();
			}
		});
	}
};

util.inherits(Live555Client, EventEmitter);

function prebuild(callback) {
	childProcess.exec(getCmd(), function (error, stdout, stderr) {
		if (error) {
			console.log('build project,please wait...');
			var _configurepath = path.join(__dirname, 'extras', 'src', 'configure');
			//__dirname + '/extras/src/configure';
			var _startbuild = childProcess.exec(_configurepath, function (error, stdout, stderr) {
				if (error) {
					if (util.isFunction(callback)) {
						callback({ result: false, code: error.code });
					}
				} else {
					mkdirs(getDir(), function (err) {
						if (err) {
							if (util.isFunction(callback)) {
								callback({ result: false, code: err.code });
							}
						} else {
							var _buildpath = path.join(__dirname, 'extras', 'src', 'live555client');
							fs.rename(_buildpath, getCmd(), function (err) {
								if (err) {
									if (util.isFunction(callback)) {
										callback({ result: false, code: err.code });
									}
								} else {
									console.log('build success');
									if (util.isFunction(callback)) {
										callback({ result: true, code: 0 });
									}
								}
							});
						}
					});
				}
			});
		} else {
			callback({ result: true, code: 0 });
		}
	});
}

function installBundle(callback) {
	var _testchild = spawn(getCmd(), []);
	_testchild.on('error', function (err) {
		if (err.code == 'ENOENT') {

			//make start for linux
			//var _dir = __dirname + '/extras/src/';
			//var _buildscript = ''
			callback(false);
		}
	});
	_testchild.on('close', function (code) {
		callback(true);
	});
}

function getCmd() {
	var _clientname = 'live555client';
	var _platformarch = os.arch();
	var _platform = os.platform();
	return path.join(__dirname, 'extras', _platform, _platformarch, _clientname);
	//__dirname + '/extras/' + platform + '/' + _clientname;
}

function getDir() {
	var _platformarch = os.arch();
	var _platform = os.platform();
	return path.join(__dirname, 'extras', _platform, _platformarch);
	//__dirname + '/extras/' + platform + '/' + _clientname;
}

function mkdirs(dirname, callback) {
	fs.exists(dirname, function (exists) {
		if (exists) {
			callback();
		} else {
			console.log(path.dirname(dirname));
			mkdirs(path.dirname(dirname), function () {
				fs.mkdir(dirname, callback);
			});
		}
	});
}

Live555Client.prototype._args = function () {
	return this.arguments.concat([
		this.input
	]);
};

/**
 * Start Live555Client spawn process
 */
Live555Client.prototype.start = function () {
	var _client = this;
	this.child = spawn(getCmd(), this._args());
	this.child.stdout.on('data', this.emit.bind(this, 'data'));
	this.child.stderr.on('data', function (data) {
		throw new Error(data);
	});
	this.emit('start');

	this.child.on('error', function (err) {
		if (err.code == 'ENOENT') {
			console.log('You have not build Live555Client yet,trying to build it');
			_client.stop();
		} else {
			console.log(err);
		}
	});
	this.child.on('close', function (code) {
		if (code === 0) {
			setTimeout(Live555Client.prototype.start.bind(this), 1000);
		}
	}.bind(this));
	/*this.child.on('error', function(code) {
	});*/
};

/**
 * Stop Live555Client spawn process
 */
Live555Client.prototype.stop = function () {
	if (this.child) {
		this.emit('stop');
		this.child.kill();
		delete this.child;
	}
};

/**
 * Restart Live555Client spawn process
 */
Live555Client.prototype.restart = function () {
	if (this.child) {
		this.stop();
		this.start();
	}
};

if (typeof Proxy === 'function') {
	Live555Client = new Proxy(Live555Client, {
		set: function (target, property) {
			if (property !== 'super_' && target[property] !== undefined) {
				target.restart();
			}
			return true;
		}
	});
}

module.exports.Live555Client = Live555Client;
exports.prebuild = prebuild;
