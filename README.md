# CouchPlay (proof of concept)

([Download builds](https://github.com/sjml/couchplay-c/releases/latest). The Mac version is actually compiled from this repo. Windows networking proved too annoying to figure out, so the Windows version is a Unity build that is the same functionality.)

Very ugly quick-and-dirty protoype of a control/registration scheme for couch co-op video games. (Inspired by my thought of "why has nobody reproduced the original [_Crystal Chronicles_](https://en.wikipedia.org/wiki/Final_Fantasy_Crystal_Chronicles) experience using more modern tech?")

The idea is that a group of people bring their smartphones to the TV and each of them becomes a controller. Then the game can push data to each screen independently while also updating the main display for everyone. 

When run, the game starts an embedded web server that will host up the web clients. It then displays a QR code linking to the page people need to use to connect. 

At the moment, it's a basic proof of concept -- the QR code stays up forever, and the web client just has the ability to to change the background color. 

Imagining: being able to put in a name before registering, being able to collectively say "we're all here, start playing," distributed information about the world, etc. 

Proof-of-concept work is done, then I reproduced it in Unity for easier prototyping going forward. This repo is effectively done now.
