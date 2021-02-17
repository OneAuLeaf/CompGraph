	#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 TViewPos;
in vec3 TLightPos;
in vec3 TFragPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
	sampler2D normal;
	sampler2D height;
	sampler2D roughness;
	float shininess;
};

struct Light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform bool advanced_light_mode;

uniform float height_coeff;

vec3 PhongLight(vec3 normal, vec3 view_dir, vec3 light_dir, vec2 tex_coord)
{
	vec3 object_color = texture(material.diffuse, tex_coord).rgb;

	normal = normalize(normal);
	view_dir = normalize(view_dir);
	light_dir = normalize(light_dir);

	vec3 ambient = light.ambient * object_color;

	float diffuse_strenght = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = diffuse_strenght * light.diffuse * object_color;

	vec3 refl_dir = reflect(-light_dir, normal);
	float specular_strenght = pow(max(dot(refl_dir, view_dir),0.0f), material.shininess);
	vec3 specular = specular_strenght * light.specular * texture(material.specular, tex_coord).r;

	return ambient + diffuse + specular;
}

float CookTorranceSpecCoeff(vec3 N, vec3 V, vec3 L, float roughness, float f0)
{
    if (roughness <= 0.0f) {
        return 0.0f;
    }
	float eps = 1.0e-7;
	// F*G*D / (V*N) / (L*N) 
	// F - frenel G - geometr D - roughness
	// V - view N - normal L - light, H - halfway
	N = normalize(N);
	V = normalize(V);
	L = normalize(L);
	vec3 H = normalize(V + L);

	float VN = max(dot(V, N), 0.0f);
	float LN = max(dot(L, N), 0.0f);
	float NH = max(dot(N, H), eps);
	float VH = max(dot(L, H), eps);

	float G = min(1, 2 * NH / VH * min(LN, VN));
	float d = NH * NH *roughness * roughness;
	float D = exp((NH * NH - 1.0f) / d) / (4.0f * d * NH * NH);
	float F = f0 + pow(1.0f - VN, 5.0f) * (1 - f0);

	return min(1.0, F * D * G / (VN + eps));
}

vec3 CookTorrance(vec3 normal, vec3 view_dir, vec3 light_dir, vec2 tex_coord)
{
	vec3 object_color = texture(material.diffuse, tex_coord).rgb;
	float roughness = texture(material.roughness, tex_coord).r;
	float metallness = texture(material.specular, tex_coord).r;

	vec3 ambient = light.ambient * object_color;

	float diffuse_strenght = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = diffuse_strenght * light.diffuse * object_color;
	
	float f0 = 0.85f * metallness + 0.05f * (1 - metallness);
	float specular_strenght = max(CookTorranceSpecCoeff(normal, view_dir, light_dir, roughness, f0), 0.0f);
	vec3 specular = specular_strenght * light.specular;

	return ambient + diffuse + specular;
}

vec2 ParallaxEffect(vec2 tex_coord, vec3 view_dir)
{
    float num_layers = 64;
    float depth_step = 1.0 / num_layers;
    float curr_layer_depth = 0.0;

    vec2 parallax_shift =  height_coeff * (view_dir.xy / view_dir.z); 
    vec2 tex_coord_step = parallax_shift / num_layers;
    vec2  curr_tex_coord = tex_coord;
    float curr_depth_value = texture(material.height, curr_tex_coord).r;
    
	while(curr_layer_depth < curr_depth_value) {
		curr_tex_coord -= tex_coord_step;
		curr_depth_value = texture(material.height, curr_tex_coord).r;
		curr_layer_depth += depth_step;  
	}

	tex_coord_step /= 2;
	depth_step /= 2;

	curr_tex_coord += tex_coord_step;
	curr_layer_depth -= depth_step;

	const int num_bin_search = 10;

	for (int i = 0; i < num_bin_search; ++i) {
		tex_coord_step /= 2;
		depth_step /= 2;
		if (texture(material.height, curr_tex_coord).r > curr_layer_depth) {
			curr_tex_coord -= tex_coord_step;
			curr_layer_depth += depth_step;
		} else if (texture(material.height, curr_tex_coord).r < curr_layer_depth) {
			curr_tex_coord += tex_coord_step;
			curr_layer_depth -= depth_step;
		} else {
			break;
		}
	}

	return curr_tex_coord;
}

void main()
{
	vec3 viewDir = normalize(TViewPos - TFragPos);
	vec3 lightDir = normalize(TLightPos - TFragPos);

	vec2 texCoord = ParallaxEffect(TexCoord, viewDir);
	if (texCoord.x > 1.0 || texCoord.x < 0.0 || texCoord.y > 1.0 || texCoord.y < 0.0) {
            discard;
    }
	vec3 normal = texture(material.normal, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 light;
	if (advanced_light_mode) {
		light = CookTorrance(normal, viewDir, lightDir, texCoord);
	} else {
		light = PhongLight(normal, viewDir, lightDir, texCoord);
	}
	
	FragColor = vec4(light, 1.0);
}