#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vn;

out vec3 Position;
out vec3 Normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float cur_time;

uniform uint gerstner_waves_length = uint(0);
uniform struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float speed;
} gerstner_waves[9];

vec3 gerstner_wave_normal(vec3 position, float time) {
    vec3 wave_normal = vec3(0.0, 1.0, 0.0);
    for (uint i = uint(0); i < gerstner_waves_length; ++i) {
        float proj = dot(position.xz, gerstner_waves[i].direction),
              phase = time * gerstner_waves[i].speed,
              psi = proj * gerstner_waves[i].frequency + phase,
              Af = gerstner_waves[i].amplitude *
                   gerstner_waves[i].frequency,
              alpha = Af * sin(psi);

        wave_normal.y -= gerstner_waves[i].steepness * alpha;

        float x = gerstner_waves[i].direction.x,
              y = gerstner_waves[i].direction.y,
              omega = Af * cos(psi);

        wave_normal.x -= x * omega;
        wave_normal.z -= y * omega;
    } return wave_normal;
}

vec3 gerstner_wave_position(vec2 position, float time) {
    vec3 wave_position = vec3(position.x, 0, position.y);
    for (uint i = uint(0); i < gerstner_waves_length; ++i) {
        float proj = dot(position, gerstner_waves[i].direction),
              phase = time * gerstner_waves[i].speed,
              theta = proj * gerstner_waves[i].frequency + phase,
              height = gerstner_waves[i].amplitude * sin(theta);

        wave_position.y += height;

        float maximum_width = gerstner_waves[i].steepness *
                              gerstner_waves[i].amplitude,
              width = maximum_width * cos(theta),
              x = gerstner_waves[i].direction.x,
              y = gerstner_waves[i].direction.y;

        wave_position.x += x * width;
        wave_position.z += y * width;
    } return wave_position;
}

vec3 gerstner_wave(vec2 position, float time, inout vec3 normal) {
    vec3 wave_position = gerstner_wave_position(position, time);
    normal = gerstner_wave_normal(wave_position, time);
    return wave_position; // Accumulated Gerstner Wave.
}

void main()
{
    Normal = vec3(0,1,0);    //water face up, change this to vn
    vec3 wave_pos = gerstner_wave(vPos.xz, cur_time, Normal);
    Position = vec3(model * vec4(wave_pos, cur_time));
    Normal = mat3(transpose(inverse(model))) * Normal;
    gl_Position = projection * view * vec4(Position, 1.0);
}
