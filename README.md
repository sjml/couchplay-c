# CouchPlay (proof of concept)

([Download the Mac app](https://github.com/sjml/couchplay-c/releases).)

Very ugly quick-and-dirty protoype of a control/registration scheme for couch co-op video games. (Inspired by my thought of "why has nobody reproduced the original [_Crystal Chronicles_](https://en.wikipedia.org/wiki/Final_Fantasy_Crystal_Chronicles) experience using more modern tech?")

The idea is that a group of people bring their smartphones to the TV and each of them becomes a controller. Then the game can push data to each screen independently while also updating the main display for everyone. 

When run, the game starts an embedded web server that will host up the web clients. It then displays a QR code linking to the page people need to use to connect. 

At the moment, it's a basic proof of concept -- the QR code stays up forever, and the web client just has the ability to to change the background to red or blue. 

Imagining: being able to put in a name before registering, being able to collectively say "we're all here, start playing," distributed information about the world, etc. 

At the moment, it exposes some of the weakness/delays in WebSocket reliability... just using bare wire protocol, though, so maybe a fancier library can handle the inputs better.

Only runs on a Mac out of the box, but that's just because there's only an Xcode project. The code and third-party libraries are fairly portable.
