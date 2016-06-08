# SeekerBot

The SeekerBot is an embedded robot project of two collage students. The robot is designed and build by
Professor Paillet Eric of the University of Antwerp. The SeekerBot is equiped with Lego Mindstorm
actuators and sensors. These are connected with custom PCBs to a Raspberri Pi for processing.

The SeekerBot will start on a random endpoint on the map. It will search for the package with the
wanted name indicated by the QR-codes attached to the packages. When the package is found, the location
will be sended to the PickerBot, equiped with a forklift.

The project is divided in small assignments to create the drivers for the different components of the
robot.

# Projects

- COBS: Encoding/decoding mechanisme used for sending data with the RF-CC1101 modules.
- Kennismaking: First assignment to get used to the development environment and the SeekerBot.
- Pathplanning: Dijkstra algorithm for finding the shortest route from start to destination.
- QRCode: Use the Raspberri Cam for taking a picture and detecting a possible QR-code with ZBar.
- RF-CC1101: Drivers for sending messages to other robots.
- Seeker: The final project where all the different pieces of the different projects come together.
- Tagreader: Use the RF-tagreader to scan for unique tags on the crossings.

# Developed by

Huybrechts Thomas,
Janssens Arthur

University of Antwerp - 2015
