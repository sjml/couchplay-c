function load() {
  let wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';
  let connection = new WebSocket(
    wsproto + '//'
    + window.location.host
    + '/websocket');
  text = document.getElementById('output');

  connection.onmessage = function(e) {
    if (e.data === 'READY') {
      let playfield = document.getElementsByClassName('playfield')[0];
      let loader = playfield.getElementsByClassName('loadingIndicator')[0];
      loader.remove();
      let red = document.createElement('div');
      red.className = 'red'
      let blue = document.createElement('div');
      blue.className = 'blue';

      function interaction(e) {
        if (e.type == 'touchstart') {
          console.log('down ' + e.target.className);
          connection.send('down ' + e.target.className);
        }
        else if (e.type == 'touchend') {
          console.log('up ' + e.target.className);
          connection.send('up ' + e.target.className);
        }
      }
      red.addEventListener('touchstart', interaction, true);
      blue.addEventListener('touchstart', interaction, true);
      red.addEventListener('touchend', interaction, true);
      blue.addEventListener('touchend', interaction, true);

      playfield.appendChild(red);
      playfield.appendChild(blue);
    }
  }
  connection.onerror = function(e) {
    alert('websocket error');
    connection.close();
  }
}
