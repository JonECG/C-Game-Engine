#pragma once
//#include <glm\glm.hpp>

//glm::mat2;

namespace glm {

	typedef float mediump_float_t;
	typedef mediump_float_t mediump_float;

	namespace detail 
	{
		template<typename T>
		struct tvec2;
		template<typename T>
		struct tvec3;
		template<typename T>
		struct tvec4;
		template<typename T>
		struct tmat3x3;
		template<typename T>
		struct tmat4x4;
	};
	

	typedef detail::tvec2<mediump_float> mediump_vec2;
	typedef detail::tvec3<mediump_float> mediump_vec3;
	typedef detail::tvec4<mediump_float> mediump_vec4;
	typedef detail::tmat3x3<mediump_float> mediump_mat3x3;
	typedef detail::tmat4x4<mediump_float> mediump_mat4x4;

	typedef mediump_mat3x3		mat3x3;
	typedef mediump_mat4x4		mat4x4;


	typedef mediump_vec2 vec2;
	typedef mediump_vec3 vec3;
	typedef mediump_vec4 vec4;
	typedef mat3x3 mat3;
	typedef mat4x4 mat4;
};