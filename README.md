# Trees of life

Trees of life is a created for GLOW 2017.

Every year in November, Eindhoven will be all about Light Art Festival GLOW. This year for the 12th time, from 11 until 18 November. 
An enormous exhibition in public space of 30 famous national and international light artists. 
The light artworks are connected by a walking route of approximately 6 kilometres which runs through the city centre. 
Besides the light art projects and numerous side-events there are also innovative, unique works of art that are created by young talents that will be exhibited at the industrial NRE site at the main route. 
The year's theme is called 'The Source'. All artists were instructed to create a project based on his theme which already results in a surprising variety of light artworks.
The festival is accessible for free.

‘Trees of Life’ connects this city’s green urban nature with Eindhoven’s second nature of Technology and Design. 
It visualizes the relationship between humans and nature. Visitors will connect with both nature's of Eindhoven trough interaction with the tree.
Through interacting and cooperating with other visitors the connection becomes a bond between humans and the city.
The final installation existed out of two trees that each are controlled and powered separately. 
On each of the trees are "hands" (points of interaction), each one holds an ultrasonic sensor. 
With this sensor we measure the distance of a visitors hand. The better a visitor touched the tree the more energy was given off. 
When enough energy is given to the tree it will start to sync it's animations. 
After a long enough time of cooperation the tree will come alive with an animation that resembles the beating of a hearth.

## Getting it up and running

Clone the project or download the zip file and extract it. 
The project exist out of a "Controller" that commands each slave (Animator) when to do a pulse.
For this you need a minimum of two [teensy 3.2](https://www.pjrc.com/teensy/teensy31.html) with [Octows2811 adaptors](https://www.pjrc.com/store/octo28_adaptor.html). 
The communication is written to be able to communicate with three slaves via serial or 7+ (tested) by IIC. 

The following libraries are needed to be able to use the programs:
* [Fastled](https://github.com/FastLED/FastLED)
* [Linked list](https://github.com/ivanseidel/LinkedList)

Also needed is [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) to be able to upload to the teensies.

## Extra

If you want to see the project in action see -> [Playlist Trees of Life](https://www.youtube.com/watch?v=enf7rqlMsUA&list=PLnrOIvNf58VsxmiSHGhlQPTVBx3L89DvG).

## Authors

* **Rick van Melis** - *Development* - [Portfolio](www.rickvanmelis.nl)
* **Simon Lit** - *Development* - [Portfolio](www.simonlit.nl)

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE.md](LICENSE.md) file for details
