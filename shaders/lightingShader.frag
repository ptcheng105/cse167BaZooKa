#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform PointLight pointlight;
uniform DirLight dirLight;
uniform int lightModeNum;

uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //ambient passed in
    vec3 ambient = light.ambient * material.ambient;

    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
	// apply attenuation
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;  

	// combine results
    return (ambient + diffuse + specular);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//ambient
	vec3 ambient = light.ambient * material.ambient;

	//diffuse
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular); 

    // combine results
    return (ambient + diffuse + specular);
} 

void main()
{
	vec3 output = vec3(0,0,0);
    vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	if(lightModeNum == 2){
		//do point light
		output += CalcPointLight(pointlight, norm, FragPos, viewDir);
	}

	//do dir light
	output += CalcDirLight(dirLight, norm, viewDir);

    FragColor = vec4(output, 1.0);
}


