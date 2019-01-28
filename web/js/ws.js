var connection = null;

//// Used this to test the overall rate at which we could handle
//// WebSocket messages. The delays seen before seem to have been
//// due to touch events getting bogged down.
// var on = false;
// var delay = 200;

// function toggleSend() {
//   if (on) {
//     connection.send('up red');
//     on = false;
//   }
//   else {
//     connection.send('down red');
//     on = true;
//   }
//   setTimeout(toggleSend, delay);
// }

function load() {
    function preventDefault(e){
      e.preventDefault();
  }
  document.body.addEventListener('touchmove', preventDefault, { passive: false });

  let wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';
  connection = new WebSocket(
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
          connection.send('down ' + e.target.className);
        }
        else if (e.type == 'touchend') {
          connection.send('up ' + e.target.className);
        }
      }
      red.addEventListener('touchstart', interaction, true);
      blue.addEventListener('touchstart', interaction, true);
      red.addEventListener('touchend', interaction, true);
      blue.addEventListener('touchend', interaction, true);

      // playfield.appendChild(red);
      playfield.appendChild(blue);

      // toggleSend();
    }
  }
  connection.onerror = function(e) {
    alert('websocket error');
    connection.close();
  }
}
