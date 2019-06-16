//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  ShaderPlain.vsh
//

#version 150 core

uniform mat4    uMVMatrix;
uniform mat4    uPMatrix;
uniform vec3    vLight0;
uniform vec4    vMaterialDiffuse;
uniform vec3    vMaterialAmbient;

in      vec3    myVertex;
in      vec3    myNormal;
in      vec3    myColor;
in      vec2    myTextureUV;

out     vec3    position;
out     vec3    normal;
out     vec4    colorDiffuse;
out     vec2    textureUV;

void main(void)
{
    vec4 p = vec4(myVertex,1);
    gl_Position = uPMatrix * p;

    vec4 materialDiffuse = vec4(myColor,1);
    colorDiffuse = materialDiffuse;

    //vec3 worldNormal = vec3(mat3(uMVMatrix[0].xyz, uMVMatrix[1].xyz, uMVMatrix[2].xyz) * myNormal);
    //vec3 ecPosition = p.xyz;
    //colorDiffuse = dot( worldNormal, normalize(-vLight0+ecPosition) ) * materialDiffuse  + vec4( vMaterialAmbient, 1 );

    position = ecPosition;
    normal = worldNormal;
    textureUV = myTextureUV;
}
