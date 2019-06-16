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

#version 150 core

uniform vec3        vLight0;
uniform vec3        vMaterialAmbient;
uniform vec4        vMaterialSpecular;

uniform sampler2D   tex;

in      vec3        position;
in      vec3        normal;
in      vec4        colorDiffuse;
in      vec2        textureUV;

void main()
{
//    vec3  halfVector = normalize(-vLight0 + position);
//    float NdotH = max(dot(normalize(normal), halfVector), 0.0);
//    float fPower = vMaterialSpecular.w;
//    float specular = pow(NdotH, fPower);
//
//    vec4    colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );

    vec4    colorTexture = texture(tex, textureUV);

    gl_FragColor = colorTexture; // + colorDiffuse + colorSpecular;
}
