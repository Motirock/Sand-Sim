#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in int aColorID;

out vec4 color;
out float brightness;

uniform vec2 playerOffset;
uniform vec2 lightPositions[60000];

int gridWidth = 321;
int gridHeight= 181;
float xOffset = 2.0f/(gridWidth-1);
float yOffset = 2.0f/(gridHeight-1);

int row = 0;
int col = 0;

vec2 generateOffset(int ID) {
    row = ID%gridWidth;
    col = ID/gridWidth;
    return vec2((row-playerOffset.x)*xOffset, (col-1+playerOffset.y)*yOffset);
}

void main()
{   
    brightness = 1.0f;
    vec2 offset = generateOffset(gl_InstanceID);
    gl_Position = vec4(aPos.x+offset.x, aPos.y+offset.y, 0.0, 1.0);
    //Binary search for color
    if (aColorID < 3) {
        if (aColorID == 0) {
			color = vec4(0.2f, 0.0f, 0.0f, 1.0f);
			return;
        }
        if (aColorID == 1) {
			color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
			return;
        }
        if (aColorID == 2) {
			color = vec4(0.75f, 0.70f, 0.0f, 1.0f);
			return;
        }
    }
    else {
        if (aColorID == 3) {
			color = vec4(0.0f, 0.0f, 0.65f, 0.7f);
			return;
        }
        if (aColorID == 4) {
			color = vec4(0.8f, 0.2f, 0.0f, 1.0f);
            brightness += 1.0;
			return;
        }
        if (aColorID == 5) {
			color = vec4(0.95f, 0.95f, 0.95f, 0.45f);
			return;
        }
    }
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if (brightness > 1.0f)
        brightness = 1.0f;
} 

