//#pragma once
//
//#include <GL\glew.h>
//#include <glm\glm.hpp>
//#include "Vertex.h"
//#include "Model.h"
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <vector>
//
//namespace GLHelper
//{
//	inline void calcNormal( glm::vec3* triangles, int index )
//	{
//		glm::vec3 normal = glm::normalize( glm::cross( triangles[index] - triangles[index+3], triangles[index] - triangles[index+6] ) );
//		triangles[index+2] = normal;
//		triangles[index+5] = normal;
//		triangles[index+8] = normal;
//
//		
//	}
//
//	inline void calcNormal( Vertex &v1, Vertex &v2, Vertex &v3 )
//	{
//		glm::vec3 normal = -glm::normalize( glm::cross( v2.position - v1.position, v3.position - v1.position ) );
//		v1.normal = normal;
//		v2.normal = normal;
//		v3.normal = normal;
//
//		float x1 = v2.position.x - v1.position.x;
//        float x2 = v3.position.x - v1.position.x;
//        float y1 = v2.position.y - v1.position.y;
//        float y2 = v3.position.y - v1.position.y;
//        float z1 = v2.position.z - v1.position.z;
//        float z2 = v3.position.z - v1.position.z;
//        
//        float s1 = v2.uv.x - v1.uv.x;
//        float s2 = v3.uv.x - v1.uv.x;
//        float t1 = v2.uv.y - v1.uv.y;
//        float t2 = v3.uv.y - v1.uv.y;
//        
//        float r = 1.0F / (s1 * t2 - s2 * t1);
//        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
//                (t2 * z1 - t1 * z2) * r);
//        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
//                (s1 * z2 - s2 * z1) * r);
//        
//		glm::vec3 tan; // tangents
//        glm::vec3 bin; // binormal
//
//		tan=glm::normalize(sdir);
//        
//		bin=glm::normalize(tdir);
//
//
//		//glm::vec3 deltapos;
//		//if(v1.position == v2.position)
//		//	deltapos = v3.position - v1.position;
//  //      else
//		//	deltapos = v2.position - v1.position;
//
//		//glm::vec2 deltauv1 = v2.uv - v1.uv;
//		//glm::vec2 deltauv2 = v3.uv - v1.uv;
//
//  //      glm::vec3 tan; // tangents
//  //      glm::vec3 bin; // binormal
//
//  //      // avoid division with 0
//  //      if(deltauv1.s != 0)
//  //          tan = deltapos / deltauv1.s;
//  //      else
//  //          tan = deltapos / 1.0f;
//
//  //      tan = glm::normalize(tan - glm::dot(normal,tan)*normal);
//
//  //      bin = glm::normalize(glm::cross(tan, normal));
//
//		v1.tangent = tan;
//		v2.tangent = tan;
//		v3.tangent = tan;
//
//		v1.binormal = bin;
//		v2.binormal = bin;
//		v3.binormal = bin;
//
//
//		/*
//		glm::vec3 deltaPos;
//            if(vertex0 == vertex1)
//                deltaPos = vertex2 - vertex0;
//            else
//                deltaPos = vertex1 - vertex0;
//
//            glm::vec2 uv0 = glm::vec2(meshData.vertex_texcoord.at(meshData.indices.at(i)), meshData.vertex_texcoord.at(meshData.indices.at(i)+1));
//            glm::vec2 uv1 = glm::vec2(meshData.vertex_texcoord.at(meshData.indices.at(i+1)), meshData.vertex_texcoord.at(meshData.indices.at(i+1)+1));
//            glm::vec2 uv2 = glm::vec2(meshData.vertex_texcoord.at(meshData.indices.at(i+2)), meshData.vertex_texcoord.at(meshData.indices.at(i+2)+1));
//
//            glm::vec2 deltaUV1 = uv1 - uv0;
//            glm::vec2 deltaUV2 = uv2 - uv0;
//
//            glm::vec3 tan; // tangents
//            glm::vec3 bin; // binormal
//
//            // avoid divion with 0
//            if(deltaUV1.s != 0)
//                tan = deltaPos / deltaUV1.s;
//            else
//                tan = deltaPos / 1.0f;
//
//            tan = glm::normalize(tan - glm::dot(normal,tan)*normal);
//
//            bin = glm::normalize(glm::cross(tan, normal));
//
//            // write into array - for each vertex of the face the same value
//            tangents[meshData.indices.at(i)]   = tan.x;
//            tangents[meshData.indices.at(i)+1] = tan.y;
//            tangents[meshData.indices.at(i)+2] = tan.z;
//
//            tangents[meshData.indices.at(i+1)]   = tan.x;
//            tangents[meshData.indices.at(i+1)+1] = tan.y;
//            tangents[meshData.indices.at(i+1)+2] = tan.z;
//
//            tangents[meshData.indices.at(i+2)]   = tan.x;
//            tangents[meshData.indices.at(i+2)+1] = tan.y;
//            tangents[meshData.indices.at(i+2)+1] = tan.z;
//
//            binormals[meshData.indices.at(i)]   = bin.x;
//            binormals[meshData.indices.at(i)+1] = bin.y;
//            binormals[meshData.indices.at(i)+2] = bin.z;
//
//            binormals[meshData.indices.at(i+1)]   = bin.x;
//            binormals[meshData.indices.at(i+1)+1] = bin.y;
//            binormals[meshData.indices.at(i+1)+2] = bin.z;
//
//            binormals[meshData.indices.at(i+2)]   = bin.x;
//            binormals[meshData.indices.at(i+2)+1] = bin.y;
//            binormals[meshData.indices.at(i+2)+1] = bin.z;
//			*/
//	}
//
//	inline std::string readFile(const char *filePath) 
//	{
//		std::string content = "";
//		std::ifstream fileStream(filePath, std::ios::in);
//
//		if(!fileStream.is_open()) 
//		{
//			std::cout << "The file does not exist: " << filePath  << std::endl;
//		}
//		else
//		{
//			std::string line = "";
//			while(!fileStream.eof()) 
//			{
//				std::getline(fileStream, line);
//				content.append(line + "\n");
//			}
//
//			fileStream.close();
//		}
//		return content;
//	}
//
//	inline GLuint loadShader( const char* vertex, const char* fragment )
//	{
//		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
//		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//
//		std::string vertShaderStr = readFile(vertex);
//		std::string fragShaderStr = readFile(fragment);
//		const char *vertShaderSrc = vertShaderStr.c_str();
//		const char *fragShaderSrc = fragShaderStr.c_str();
//
//		GLint successful = GL_FALSE;
//		int logLength;
//
//		// Compile vertex shader
//		std::cout << "Compiling vertex shader." << std::endl;
//		glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
//		glCompileShader(vertShader);
//
//		// Check vertex shader
//		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &successful);
//		if( successful )
//		{
//			std::cout << "Vertex shader successfully compiled!" << std::endl;
//		}
//		else
//		{
//			std::cout << "Vertex shader was not compiled..." << std::endl;
//			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
//			char* vertShaderError = new char[ logLength ];
//			glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
//			std::cout << &vertShaderError[0] << std::endl;
//			delete vertShaderError;
//		}
//
//		std::cout << std::endl;
//
//		// Compile fragment shader
//		std::cout << "Compiling fragment shader." << std::endl;
//		glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
//		glCompileShader(fragShader);
//
//		// Check fragment shader
//		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successful);
//		if( successful )
//		{
//			std::cout << "Fragment shader successfully compiled!" << std::endl;
//		}
//		else
//		{
//			std::cout << "Fragment shader was not compiled..." << std::endl;
//			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
//			char* fragShaderError = new char[ logLength ];
//			glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
//			std::cout << &fragShaderError[0] << std::endl;
//			delete fragShaderError;
//		}
//
//		std::cout << std::endl;
//
//		std::cout << "Linking program" << std::endl;
//		GLuint program = glCreateProgram();
//		glAttachShader(program, vertShader);
//		glAttachShader(program, fragShader);
//		glLinkProgram(program);
//
//		glGetProgramiv(program, GL_LINK_STATUS, &successful);
//		if( successful )
//		{
//			std::cout << "Program linked successfully!" << std::endl;
//		}
//		else
//		{
//			std::cout << "Shader program was not linked..." << std::endl << std::endl;
//			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
//			//std::vector<char> programError( (logLength > 1) ? logLength : 1 );
//			char* programError = new char[ logLength ];
//			glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
//			std::cout << &programError[0] << std::endl;
//			delete programError;
//		}
//
//		glDeleteShader(vertShader);
//		glDeleteShader(fragShader);
//
//		return program;
//	}
//
//	inline Model indexVertices( Vertex* vertices, GLuint size, bool checkNormals )
//	{
//		GLuint * count = new GLuint[ size ];
//		Vertex * uniqueVertices = new Vertex[ size ];
//		GLushort * indices = new GLushort[ size ];
//
//		GLuint uniqueCount = 0;
//		for( int i = 0; i < size; i++ )
//		{
//			bool isUnique = true;
//			GLuint insertIndex = uniqueCount;
//			for( int j = 0; j < i; j++ )
//			{
//				if ( vertices[i].position == vertices[j].position )
//				{
//					isUnique = false;
//					insertIndex = indices[j];
//				}
//			}
//			indices[i] = insertIndex;
//
//
//			if ( isUnique )
//			{
//				uniqueVertices[ uniqueCount ] = vertices[ i ];
//				count[ uniqueCount ] = 1; 
//				uniqueCount++;
//			}
//			else
//			{
//				//Averages current
//				uniqueVertices[ insertIndex ] = Vertex::avg( uniqueVertices[ insertIndex ], vertices[ i ], count[ insertIndex ] ); 
//				count[ insertIndex ]++;
//			}
//		}
//		Vertex * newVertices = new Vertex[ uniqueCount ];
//
//		for( int i = 0; i < uniqueCount; i++ )
//		{
//			newVertices[i] = uniqueVertices[i];
//		}
//
//		delete [] uniqueVertices;
//		
//		return Model( newVertices, uniqueCount, indices, size );
//	}
//}