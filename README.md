# rtsp-live555

[![Build Status](https://travis-ci.org/godka/node-rtsp-live555.svg?branch=master)](https://travis-ci.org/godka/node-rtsp-live555)

[![NPM](https://nodei.co/npm/rtsp-live555.png)](https://nodei.co/npm/rtsp-live555/)

## introduction

This is a wrapper which allows you to get RTSP stream from IPC and export in FLV stream.

## Installation

### NPM

`npm install rtsp-live555` - install lastest stable version

`npm install godka/node-rtsp-live555` -install lastest version from github

### Clone the last version from Github
`git clone https://github.com/godka/node-rtsp-live555`

### Sample
The sample creates a web server at port 8080 and scans RTSP address from IPC with onvif.A stream will be shown on the video element via GrindPlayer(flash) and flv.js when pressing 'play' button.

```javascript
var rtsp = null;
try {
    rtsp = require('rtsp-live555');
} catch (e) {
    rtsp = require('./lib/rtsp-live555.js');
}
var _url = 'rtsp://1029.mythkast.net/test.264';//test address
var stream = new rtsp.Live555Client({ input: _url });
stream.on('start', () => {
    console.log(_url + ' started');
});

stream.on('stop', () => {
    console.log(_url + ' stopped');
});
var _hasrecv = false;
stream.on('data', (data) => {
    //you can write your method here
    //data is flv stream
    if (!_hasrecv) {
        console.log('recv stream:', data.length);
        stream.stop();
        console.log('close stream:', _url);
        _hasrecv = true;
    }
});
```
