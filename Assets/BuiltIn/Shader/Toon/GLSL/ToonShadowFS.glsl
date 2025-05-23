// File Generated by Assets/BuildShader.py - source: [ToonFS.d.glsl : SHADOW]
precision highp float;
precision highp sampler2D;
precision highp sampler2DArray;
uniform sampler2D uTexDiffuse;
uniform sampler2D uTexRamp;
uniform sampler2DArray uShadowMap;
uniform vec4 uLightDirection;
uniform vec4 uLightColor;
uniform vec4 uColor;
uniform vec4 uShadowColor;
uniform vec2 uWrapFactor;
uniform vec3 uSpecular;
uniform vec4 uSHConst[4];
uniform vec3 uShadowDistance;
uniform mat4 uShadowMatrix[3];
in vec2 vTexCoord0;
in vec3 vWorldNormal;
in vec3 vWorldViewDir;
in vec3 vWorldPosition;
in vec3 vDepth;
in vec4 vColor;
out vec4 FragColor;
const float gamma = 2.2;
const float invGamma = 1.0 / 2.2;
vec3 sRGB(vec3 color)
{
	return pow(color, vec3(gamma));
}
vec3 linearRGB(vec3 color)
{
	return pow(color, vec3(invGamma));
}
vec3 shAmbient(vec3 n)
{
	vec3 ambientLighting = uSHConst[0].xyz +
		uSHConst[1].xyz * n.y +
		uSHConst[2].xyz * n.z +
		uSHConst[3].xyz * n.x;
	return ambientLighting * 0.75;
}
float shadow(const vec4 shadowCoord[3], const float shadowDistance[3], const float farDistance)
{
	int id = 0;
	float depth = 0.0;
	float result = 0.0;
	const float bias[3] = float[3](0.0001, 0.0002, 0.0006);
	if (farDistance < shadowDistance[0])
		id = 0;
	else if (farDistance < shadowDistance[1])
		id = 1;
	else if (farDistance < shadowDistance[2])
		id = 2;
	else
		return 1.0;
	vec3 shadowUV = shadowCoord[id].xyz / shadowCoord[id].w;
	depth = shadowUV.z;
	depth -= bias[id];
	vec2 uv = shadowUV.xy;
	float size = 1.0/2048.0;
	vec2 off;
	vec2 rand;
	const vec2 kRandom1 = vec2(12.9898,78.233);
	const float kRandom2 = 43758.5453;
	const float kRandom3 = 0.00047;
	{off = vec2(-1, -1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(0, -1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(1, -1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(-1, 0) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(0, 0) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(1, 0) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(-1, 1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(0, 1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	{off = vec2(1, 1) * size;rand = uv + off;rand += (vec2(fract(sin(dot(rand.xy, kRandom1)) * kRandom2), fract(sin(dot(rand.yx, kRandom1)) * kRandom2)) * kRandom3);result += (step(depth, textureLod(uShadowMap, vec3(rand, id), 0.0).r));}
	return result / 9.0;
}
const float PI = 3.1415926;
void main(void)
{
	vec4 diffuse = texture(uTexDiffuse, vTexCoord0.xy);
	vec3 diffuseMap = sRGB(diffuse.rgb);
	float NdotL = (dot(vWorldNormal, uLightDirection.xyz) + uWrapFactor.x) / (1.0 + uWrapFactor.x);
	NdotL = max(NdotL, 0.0);
	vec3 rampMap = texture(uTexRamp, vec2(NdotL, NdotL)).rgb;
	vec3 color = sRGB(vColor.rgb * uColor.rgb);
	float shadowIntensity = uColor.a;
	vec3 shadowColor = sRGB(uShadowColor.rgb);
	vec3 lightColor = sRGB(uLightColor.rgb);
	float visibility = 1.0;
	float depth = length(vDepth);
	vec4 shadowCoord[3];
	shadowCoord[0] = uShadowMatrix[0] * vec4(vWorldPosition, 1.0);
	shadowCoord[1] = uShadowMatrix[1] * vec4(vWorldPosition, 1.0);
	shadowCoord[2] = uShadowMatrix[2] * vec4(vWorldPosition, 1.0);
	float shadowDistance[3];
	shadowDistance[0] = uShadowDistance.x;
	shadowDistance[1] = uShadowDistance.y;
	shadowDistance[2] = uShadowDistance.z;
	visibility = shadow(shadowCoord, shadowDistance, depth);
	vec3 ambientLighting = shAmbient(vWorldNormal);
	ambientLighting = sRGB(ambientLighting);
	vec3 ramp = mix(color, shadowColor, shadowIntensity * (1.0 - visibility));
	ramp = mix(ramp, color, rampMap);
	vec3 h = normalize(uLightDirection.xyz + vWorldViewDir);
	float NdotH = dot(vWorldNormal, h);
	NdotH = max(NdotH, 0.0);
	float spec = pow(NdotH, uSpecular.x*128.0) * uSpecular.y;
	spec = smoothstep(0.5-uSpecular.z*0.5, 0.5+uSpecular.z*0.5, spec);
	FragColor = vec4(diffuseMap * lightColor * ramp * (visibility * (1.0 - uWrapFactor.y)) + lightColor * spec * visibility + uWrapFactor.y * diffuseMap * ambientLighting / PI, diffuse.a);
}
