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