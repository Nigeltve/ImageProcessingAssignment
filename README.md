## Image Processing Assignment

This assignment was split up into 4 sections (A,B,C,D). With the main task of encrypting a image with in another image. 

Each section is meant to have and encoder and decoder, apart from Part C where only the Encoder was required.

#### Part A
This section was to simply encode and decode an image into another image.

#### Part B
This was to encode an image into another image by Scambling it and having it be password protected.

#### Part C

This section was to Cover the message using Gaussian noise that it password protected

#### Part D
This Section to do Parts A B and C to encode a message using Colored images.

## How to comile

just download the code and compile each main.cpp using this command
```
encoder
g++ main.cpp -o encoder `pkg-config --cflags --libs opencv`

decoder
g++ main.cpp -o decoder `pkg-config --cflags --libs opencv`
```

in the CommandLines you will find the the commands I used to test the various programs.

```
Part A
./encoder ../assingment_things/carriers/grayscale_carrier-09.png ../assingment_things/messages/message1.png
./decoder ../assingment_things/carriers/grayscale_carrier-09.png ../outputs/Part_A_encoded.png

Part B
./encoder ../assingment_things/carriers/grayscale_carrier-09.png ../assingment_things/messages/message1.png
./decoder ../assingment_things/carriers/grayscale_carrier-09.png ../outputs/Part_B_encoded.png

Part C
./encoder ../assingment_things/messages/message1.png

Part D
./encoder ../assingment_things/carriers/grayscale_carrier-09.png ../assingment_things/messages/message3.png
./decoder ../assingment_things/carriers/grayscale_carrier-09.png ../outputs/Part_D_encoded.png
```
