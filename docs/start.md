## UML Diagramm
![](img/uml1.png)
![](img/uml2.png)
(Transform will be expanded upon)

## Development Plan

### Implementation of the screen with ambient component only  

Goal: Distinction between Halfspace and "Skybox" (printed out as values)

- Basic headers
- Halfspaces, i.e. basic chain of Tree/ObjectTree/PrimitiveObject classes 
- Screen/Pixels
- Ambient light component $E_a$ 
    - $ E_a ​=K_a \cdot ​I_a ,$
- Shadows

### File IO

Goal: Fully convert output text to image using OpenCV

- JSON Parsing (include json library?)
- Output in an image file 
- OpenCV

### Implementation of Logical Trees 

Goal: Presentable objects that are combined using logical operations

- Union
- Intersection
- Exclusion
- also Spheres

### Implementation of Transformation Trees 

- Translation
- Rotation
- Scaling

### Addition of Diffuse, Specular compenents

- ~~Ambient light component $E_a$~~
    - $ E_a ​=K_a \cdot ​I_a$
- Diffuse component $E_d$
    - $E_d ​= K_c \cdot ​I_L​ (\vec{V} \ast \vec{N})$
- Specular component $E_s$
    - $E_s ​= K_s \cdot ​I_L​ (\vec{R} \ast \vec{V})^α$

### Addition of Reflected and refracted components

- Reflected component
    - $E_r ​= K_r​ ​\cdot I_r​$
- Refracted component
    - $Et ​= K_t ​\cdot I_t​$

- Combination of all components
    - $E = E_a ​⊕ \underset{\text{lightsources}}{∑}​(E_d​(\text{source}) ⊕ E_s​(\text{source})) ⊕ E_r ​⊕ E_t​$
