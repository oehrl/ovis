uniform mat4 u_TransformMatrix;

attribute vec2 a_Position;

void main()
{
    gl_Position = u_TransformMatrix * vec4(a_Position, 0.0, 1.0);
}
