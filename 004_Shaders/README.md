<h1> Shaders </h1>

### 1. How it work
- In a typical OpenGL shader application, the shader specific functions and their sequence of execution are as follows:
```shell
glCreateShader
glShaderSource
glCompileShader
glGetShaderInfoLog
```
- Execution of the above four functions creates a shader object. After the shader object is created, a shader program object is created using the following set of functions in the following sequence:
```shell
glCreateProgram
glAttachShader
glLinkProgram
glGetProgramInfoLog
```
![image](https://user-images.githubusercontent.com/32474027/114003664-e6d37200-9898-11eb-969f-addeefa6987a.png)
