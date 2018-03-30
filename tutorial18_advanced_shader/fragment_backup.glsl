#version 330 core
out vec4 FragColor;
in VS_OUT{
	vec2 texCoord;
} fs_in;

uniform sampler2D frameTexture;

float blur_kernel[9] =  float[](
			1.0/16, 2.0/16, 1.0/16,
			2.0/16, 4.0/16, 2.0/16,
			1.0/16, 2.0/16, 1.0/16
);

float edge_kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
);


vec4 grayScale(vec4 theFragColor){
	float average = 0.2126 * theFragColor.r + 0.7152 * theFragColor.g + 0.0722 * theFragColor.b;
	return vec4(average, average, average, 1.0f);
}




vec4 kernel3_3_apply(float kernel[9], float offsetx, float offsety){
	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety), 	//top left
		vec2(0.0f, offsety), 			//top-center
		vec2(offsetx, offsety), 	//top-right
		vec2(-offsetx, 0.0f),			//center-left
		vec2(0.0f, 0.0f),					//center
		vec2(offsetx, 0.0f),		  //center-right
		vec2(-offsetx, -offsety),	//bottom-left
		vec2(0.0f, -offsety),			//bottom-center
		vec2(offsetx, -offsety)		//bottom-right
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
		sampleTex[i] = vec3(texture(frameTexture, fs_in.texCoord.st + offsets[i]));
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i]*kernel[i];
	return vec4(col, 1.0f);
}


vec4 kernel3_3_apply_gray(float kernel[9], float offsetx, float offsety){
	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety), 	//top left
		vec2(0.0f, offsety), 			//top-center
		vec2(offsetx, offsety), 	//top-right
		vec2(-offsetx, 0.0f),			//center-left
		vec2(0.0f, 0.0f),					//center
		vec2(offsetx, 0.0f),		  //center-right
		vec2(-offsetx, -offsety),	//bottom-left
		vec2(0.0f, -offsety),			//bottom-center
		vec2(offsetx, -offsety)		//bottom-right
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
		sampleTex[i] = grayScale(texture(frameTexture, fs_in.texCoord.st + offsets[i])).rgb;
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i]*kernel[i];
	return vec4(col, 1.0f);
}




void main(){
	// FragColor = vec4(texture(frameTexture, fs_in.texCoord).xyz, 1.0); 	// original color
	// FragColor = vec4(vec3(1.0 - texture(frameTexture, fs_in.texCoord).xyz), 1.0); //invert color
	FragColor = grayScale(texture(frameTexture, fs_in.texCoord)); //grayScale
	// FragColor = kernel3_3_apply(blur_kernel, 1.0f/400.0f, 1.0f/300.0f); //blur
	// FragColor = kernel3_3_apply(edge_kernel, 1.0f/400.0f, 1.0f/300.0f); // edge
	FragColor = kernel3_3_apply_gray(edge_kernel, 1.0f/400.0f, 1.0f/300.0f); // edge
}
