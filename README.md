# rtsp-live555

[![NPM](https://nodei.co/npm/rtsp-live555.png?downloads=true&downloadRank=true&stars=true)](https://nodei.co/npm/rtsp-live555/)

## introduction

This is a wrapper which allows you to get RTSP stream from IPC and export in FLV stream.

## Installation

### NPM

`npm install rtsp-live555` - install lastest stable version

`npm install godka/node-rtsp-live555` -install lastest version from github

### Clone the last version from Github
`git clone https://github.com/godka/node-rtsp-live555`

### Sample
The sample creates a web server at port 80 and scans RTSP address from IPC with onvif.A stream will be shown on the video element via flv.js when pressing 'play' button.

```javascript
var rtsp = require('rtsp-live555');
var _url = 'rtsp://1029.mythkast.net/test.264';//test address
var stream = new rtsp.Live555Client({ input: _url });
stream.on('start', () => {
	console.log(_url + ' started');
});

stream.on('stop', () => {
	console.log(_url + ' stopped');
});
		
stream.on('data', (data) => {
    //you can write your method here
    //data is flv stream
    console.log('recv stream:',data.length);
});
```
