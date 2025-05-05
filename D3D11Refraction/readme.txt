Author: Jordan Allard
Project: Refractive Materials

Base D3D11 code written by Chris Cascioli (@vixorien on GitHub)

==========

IGME 542-01
Game Graphics Programming II
Chris Cascioli

Assignment #8 - Final Assignment

The option I chose for this assignment is adding refractive materials! There are several entities with refractive materials on display in the scene. The implementation is very similar to the ones used in the slides and the demo, but I've also incorporated the material's colorTint variable to allows you to change the color of the glass materials. 

Note: When I was adding entities to the scene, I noticed that the entities with refractive materials do not render through each other, and having one closer to the camera will overwrite any that are behind it. We did not cover how to avoid this in the demo or slides in class, so I am considering it outside the scope of this class for now. 