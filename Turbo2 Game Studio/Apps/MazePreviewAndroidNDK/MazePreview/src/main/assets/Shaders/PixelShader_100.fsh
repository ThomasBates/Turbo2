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

uniform highp vec3      vLight0;
uniform lowp vec3       vMaterialAmbient;
uniform lowp vec4       vMaterialSpecular;

uniform sampler2D       uActiveTexture;

varying mediump vec3 position;
varying mediump vec3 normal;
varying mediump vec4 colorDiffuse;
varying mediump vec2 textureUV;

void main()
{
//    mediump vec3 halfVector = normalize(-vLight0 + position);
//    mediump float NdotH = max(dot(normalize(normal), halfVector), 0.0);
//    mediump float fPower = vMaterialSpecular.w;
//    mediump float specular = pow(NdotH, fPower);
//
//    lowp vec4 colorSpecular = vec4( vMaterialSpecular.xyz * specular, 1 );
//    gl_FragColor = colorDiffuse + colorSpecular;

    lowp vec4 colorTexture = texture2D(uActiveTexture, textureUV);
    gl_FragColor = colorTexture;

//    gl_FragColor = colorDiffuse;
}
