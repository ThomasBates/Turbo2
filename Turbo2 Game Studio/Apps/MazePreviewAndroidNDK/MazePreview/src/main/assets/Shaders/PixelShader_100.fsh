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
//  ShaderPlain.fsh
//

//  #version 100 core

//iform     highp   vec3    uLight0;
//iform     lowp    vec3    uMaterialAmbient;
//iform     lowp    vec4    uMaterialSpecular;
uniform     sampler2D       uSampler;
uniform             int     uIsSprite;

//rying     mediump vec3    psPosition;
//rying     mediump vec3    psNormal;
varying     mediump vec4    psColor;
varying     mediump vec2    psTexture;

void main()
{
    lowp vec4 color = texture2D(uSampler, psTexture);

    if (uIsSprite > 0)
    {
        //color.a = 0.9;
        //color = vec4(color.rgb, .9);
    }

//    gl_FragColor = color;               //  Just the texture color
//    gl_FragColor = psColor;             //  Just the lighting color
    gl_FragColor = vec4(color.rgb * psColor.rgb, color.a);     //  Combination of lighting and texture
}
