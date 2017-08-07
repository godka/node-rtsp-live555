'use strict';
const spawn = require('child_process').spawn, EventEmitter = require('events').EventEmitter, util = require('util');
var os = require('os');
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
	this.arguments = options.arguments || [];

	this.on('newListener', newListener.bind(this));
	this.on('removeListener', removeListener.bind(this));
	if (Object.observe && (typeof Proxy !== 'function')) {
		Object.observe(this, observer.bind(this));
	}
};

util.inherits(Live555Client, EventEmitter);

/**
 * Live555Client command name
 * @type {string}
 */
//Live555Client.cmd = __dirname + '/extras/Live555Client';

function newListener(event) {
	if (event === 'data' && this.listeners(event).length === 0) {
		this.start();
	}
}

function removeListener(event) {
	if (event === 'data' && this.listeners(event).length === 0) {
		this.stop();
	}
}

var installBundle = function () {
	var _testchild = spawn(getCmd(), this._args());
	_testchild.on('error', function (err) {
		if (err.code == 'ENOENT') {
			console.log('You have not build Live555Client yet,trying to build it');
			//make start for linux
			var _dir = __dirname + '/extras/src/';
			var _buildscript=''
		}
	});
};

function getCmd() {
	var platform = os.platform();
	if (platform == 'win32') {
		return __dirname + '/extras/win/Live555Client';
	} else {
		return 'Live555Client';
	}
}
function observer(changes) {
	if (changes.some(function (change) {
		return change.type === 'update';
	})) {
		this.restart();
	}
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
	this.child.kill();
	delete this.child;
	this.emit('stop');
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
