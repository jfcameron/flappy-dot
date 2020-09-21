// © 2019 Joseph Cameron - All Rights Reserved

#ifndef JFC_FLAPPY_BACKGROUND_SHADER_H
#define JFC_FLAPPY_BACKGROUND_SHADER_H

#include <string>

const std::string BackgroundShaderVertexGLSL(R"V0G0N(
	//Uniforms
	uniform mat4 _MVP;
    uniform mat4 _Model;
	uniform mat4 _View;
	uniform mat4 _Projection;

#if defined Emscripten
	//VertIn
	attribute highp   vec3 a_Position;
	attribute mediump vec2 a_UV;
	//FragIn
	varying mediump vec2 v_UV;

    #elif defined Darwin || defined Windows || defined Linux
	//VertIn
	attribute vec3 a_Position;
	attribute vec2 a_UV;
	//FragIn
	varying vec2 v_UV;
    #endif

	void main ()
	{
		gl_Position = _MVP * vec4(a_Position,1.0);

	    v_UV = a_UV;
	}
)V0G0N");

const std::string BackgroundShaderFragmentGLSL(R"V0G0N(
#if defined Emscripten
	precision mediump float;
#endif

	//Uniforms
    uniform sampler2D _Texture;
	uniform vec2 _UVOffset;
	uniform vec2 _UVScale;

#if defined Emscripten
	//FragIn
	varying lowp vec2 v_UV;

#elif defined Darwin || defined Windows || defined Linux
	//FragIn
	varying vec2 v_UV;
#endif

	void main()
	{
		v_UV += _UVOffset;
		//v_UV *= _UVScale;

		vec4 frag = texture2D(_Texture, v_UV);
	
		if (frag[3] < 1.0) discard;

		gl_FragColor = frag;                        
	}
)V0G0N");

#endif