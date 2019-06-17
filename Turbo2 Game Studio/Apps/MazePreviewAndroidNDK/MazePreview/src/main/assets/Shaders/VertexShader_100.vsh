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

// #version 100 core

uniform     highp   mat4    uModel;
uniform     highp   mat4    uView;
uniform     highp   mat4    uProjection;
uniform             int     uLightCount;
//iform     highp   vec3    uLight0;
//iform     lowp    vec4    uMaterialDiffuse;
//iform     lowp    vec3    uMaterialAmbient;

attribute   highp   vec3    vsPosition;
attribute   highp   vec3    vsNormal;
attribute   highp   vec3    vsColor;
attribute   highp   vec2    vsTexture;

//rying     mediump vec3    psPosition;
//rying     mediump vec3    psNormal;
varying     mediump vec4    psColor;
varying     mediump vec2    psTexture;

void main(void)
{
    highp vec4 position = vec4(vsPosition,1);
    //gl_Position = uProjection * position;

    position   = uModel * position;
    position   = uView * position;
    gl_Position = uProjection * position;

    //psPosition = gl_Position.xyz;

    if (uLightCount > 0)
    {
        float litArea = 0.5;
        float dist = length(position);
        float power = 1.0;
        if (dist > 1.0 + litArea)
        {
            power = 1.0 / (dist - litArea);
            power = pow(power, 3.0);
        }

        psColor = vec4(power, power, power, 1);
    }
    else
    {
        psColor = vec4(1);
    }

    //psNormal = vec3(mat3(uView[0].xyz, uView[1].xyz, uView[2].xyz) * vsNormal);
    psTexture = vsTexture;

    //psColor = vec4(vsColor, 1);
    //psPosition = position.xyz;
    //psNormal = vec3(mat3(uView[0].xyz, uView[1].xyz, uView[2].xyz) * vsNormal);
    //psColor = dot( psNormal, normalize(-uLight0+psPosition) ) * uMaterialDiffuse  + vec4( uMaterialAmbient, 1 );
    //psTexture = vsTexture;
}

