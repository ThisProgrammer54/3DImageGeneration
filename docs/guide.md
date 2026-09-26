## Guide 

### install (linux)
#### need: 
- opencv
- pkg-config 
```
sudo apt install libopencv-dev pkg-config
```
- nlohman-json (für input)
```
sudo apt install nlohmann-json3-dev
```
- doxygen for documentation
https://www.doxygen.nl/manual/install.html
- we worked with git submodules (for Eigen): 
```
git submodule init 
git submodule update
```
the starting point for all following commands is ./project/
### Compile
executable can be found in build/main 
- compile the programm 
```
make
```

- compile and run the program
```
make run 
```

- compile and run the tests 
```
make test
```

### input 
main file will read /json/cur.json and added three new input possibilities: 
- cube: 
```
                "cube": {
                    "position": [0, 0, 0],
                    "dimensions" : [4, 2, 2],
                    "color": {
                        "ambient": [0.5, 0.5, 0.5],
                        "diffuse": [1, 1, 1],
                        "specular": [1, 1, 1],
                        "reflected": [0, 0, 0],
                        "refracted": [0, 0, 0],
                        "shininess": 1
                    },
                
                    "index": 1
                }

```
- modify:
```
        {"modify": {
            "scaling": [1, 1, 1],
            "rotation": [0, 180, 0],
            "translation": [5, 0, 0],
            "subject":{
                "cube": {
                    "position": [0, 0, 0],
                    "dimensions" : [4, 2, 2],
                    "color": {
                        "ambient": [0.5, 0.5, 0.5],
                        "diffuse": [1, 1, 1],
                        "specular": [1, 1, 1],
                        "reflected": [0, 0, 0],
                        "refracted": [0, 0, 0],
                        "shininess": 1
                    },
                
                    "index": 1
                }
            }
          } 
        }

```
- materials: <br> materials can be used to save color configurations and then used to apply them to other inputs
```
    "materials": {
        "metal":{
          "color": {
            "ambient": [0.2, 0.2, 0.2],
            "diffuse": [0.4, 0.4, 0.4],
            "specular": [0.95, 0.95, 0.95],
            "reflected": [0.6, 0.6, 0.6],
            "refracted": [0, 0, 0],
            "shininess": 50
            },
            "index": 1
        }
}

        {"sphere": {
            "position": [12, 2, 0],
            "radius": 2.0,
            "material": {
              "name" : "metal"
            },
            "index": 1
            }
        },
```
### documentation
The Doxyfile can be found as ./project/Doxyfile 
- to update documentation 
```
doxygen Doxyfile
```
documentation is found in ./project/docs/doxy/html with index.html as starting point

### result
the result with provided cur.json should look like this:
![](img/result.png)